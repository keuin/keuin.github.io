#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <signal.h> //处理Ctrl-C事件要用的
#include <unistd.h> //使用sleep();要用的
#include <cstdlib> //使用atoi();

#define REC_FREQ 20 //每n秒读取一次状态
#define ERR_FAILED_TO_OPEN_LOG_FILE -1
#define BUILDTIME "151003.135225"
#define VERSION "1.2"

/*****************************************************************************
更新日志：
1.2：
（修复）存档文件有多余空格。
*（更新）增加模式2：将原始数据稍稍加工后保存，可配合Excel等工具快速生成图表。

1.1：
（修复）使用Ctrl-C杀死程序时由于未close()文件流而导致崩溃。
*****************************************************************************/

using namespace std;

string GetHardwareInfo(); //获取硬件信息，保存为一个单行string

volatile sig_atomic_t flag = 0;

static void my_handler(int sig){ // 可以被异步调用
  flag = 1;
}

bool oRaw = false;
int main(int argc, char *argv[]){
    for(char *di : *argv){ //处理命令行参数
        switch(*di){
            "raw":
                oRaw = true;
                break;
        }
    }
    cout << "Welcome to use Cpu&Ram Monitor!\nAuthor: Keuin   BuildTime: " << BUILDTIME << "  Version: "<< VERSION<< endl;
    signal(SIGINT, my_handler);
    cout << "Opening log file: HardwareLog.txt" <<endl;
    ofstream log("HardwareLog.txt");
    if(!log.good()){ //打开日志文件
        cout<<"Failed to open file: HardwareLog.txt !"<<endl;
        return ERR_FAILED_TO_OPEN_LOG_FILE;
    }
    cout << "Done! Now record." <<endl;
    
    int count = REC_FREQ - 1; //计数器，记够REC_FREQ个记录一次。
    int tick = 0; //非raw时用到的模式标志。0：第一个数据，1：第二个数据
    int cpuTime = 0, cpuIdle = 0; //分别为：CPU时间、CPU空闲时间
    int ramTotal = 0, ramFree; //分别为：RAM总量、可用RAM
    int tmp1 = 0 //CPU累加临时变量：CPU时间
        tmp4 = 0 //CPU累加临时变量：空闲时间片
    double tmp2 = 0 //RAM累加临时变量：内存总量
          ,tmp3 = 0;//RAM累加临时变量：可用内存
    
    while(flag != 1){
        ++count;
        if(count == REC_FREQ){
            count = 0; //计数器复位
            string inf = GetHardwareInfo(); //获取原始数据
            if(oRaw){
                //保存原始数据
                log << inf
                    << "\n";
            }else {//保存加工后的数据
                cpuTime=cpuIdle=ramTotal=ramFree=tmp1=tmp2=tmp3=tmp4=0; //清空临时变量
                //将原始数据分割，存入vector
                vector<string> d;
                char *nowPtr = nullptr;
                while((nowPtr = strsep(&&GetHardwareInfo().c_str()," ")) != NULL){ //使用空格分割
                    d.push_back(*nowPtr);
                }
                
                for(auto p = d.cbegin();p != d.cbegin()+4;++p){
                    //读取前5个数字，累加入tmp1
                    if(tick == 0)
                        tmp1+=atoi(*p.c_str());
                    else
                        tmp1-=atoi(*p.c_str());
                    
                    if(p==d.cbegin()+4)
                        tmp4+=atoi(*p.c_str());
                }
                
                tmp2+=atoi(*d[30].c_str());
                tmp3+=atoi(*d[31].c_str());
            if(tick == 1){ //第二个数据
                tick=0;
                tmp2/=2;
                tmp3/=2;
                
                log << tmp1 << " "
                    << tmp4 << " "
                    << tmp2 << " "
                    << tmp3 << " "
                    << "\n";
                    
            }else if(tick == 0){
                tick = 1;
            }

            cout << "Log saved." <<endl;
        }
        sleep(1); //sleep 一秒
    }
    
    cout << "Closing log file...";
    log.close();
    return 0;
}

string GetHardwareInfo(){ //从proc中读取硬件信息
    ifstream f; //文件流
    string infOut; //输出结果
    vector<string> inf_; //以换行符分割的信息
    string inf;
    string ln;
    
    f.open("/proc/stat");
    while(getline(f,ln))
        inf_.push_back(ln);
    f.close();
    
    for(string l : inf_){
        if(l.substr(0,3) == "cpu"){
            inf+=l.substr(5);
            inf+=" ";
        }else{
            infOut+=inf;
            break;
        }
    }
    
    inf_.clear(); //先清空vector
    inf.clear();
    
    f.open("/proc/meminfo");
    while(getline(f,ln))
        inf_.push_back(ln);
    f.close();
    
    for(string l : inf_){
        if(l.substr(0,8) == "MemTotal"){
            auto endp = l.rfind(" "); //查找结尾空格
            auto begp = l.substr(0,endp).rfind(" "); //查找开始空格
            inf+=l.substr(begp + 1,endp - begp - 1);
            inf+=" ";
        }else 
        if(l.substr(0,7) == "MemFree"){
            auto endp = l.rfind(" "); //查找结尾空格
            auto begp = l.substr(0,endp).rfind(" "); //查找开始空格
            inf+=l.substr(begp + 1,endp - begp - 1);
            infOut+=inf;
            break;
        }
    }
    
    return infOut;
}