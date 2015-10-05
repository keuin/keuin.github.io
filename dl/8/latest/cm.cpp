#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <signal.h> //����Ctrl-C�¼�Ҫ�õ�
#include <unistd.h> //ʹ��sleep();Ҫ�õ�
#include <cstdlib> //ʹ��atoi();

#define REC_FREQ 20 //ÿn���ȡһ��״̬
#define ERR_FAILED_TO_OPEN_LOG_FILE -1
#define BUILDTIME "151003.135225"
#define VERSION "1.2"

/*****************************************************************************
������־��
1.2��
���޸����浵�ļ��ж���ո�
*�����£�����ģʽ2����ԭʼ�������Լӹ��󱣴棬�����Excel�ȹ��߿�������ͼ��

1.1��
���޸���ʹ��Ctrl-Cɱ������ʱ����δclose()�ļ��������±�����
*****************************************************************************/

using namespace std;

string GetHardwareInfo(); //��ȡӲ����Ϣ������Ϊһ������string

volatile sig_atomic_t flag = 0;

static void my_handler(int sig){ // ���Ա��첽����
  flag = 1;
}

bool oRaw = false;
int main(int argc, char *argv[]){
    for(char *di : *argv){ //���������в���
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
    if(!log.good()){ //����־�ļ�
        cout<<"Failed to open file: HardwareLog.txt !"<<endl;
        return ERR_FAILED_TO_OPEN_LOG_FILE;
    }
    cout << "Done! Now record." <<endl;
    
    int count = REC_FREQ - 1; //���������ǹ�REC_FREQ����¼һ�Ρ�
    int tick = 0; //��rawʱ�õ���ģʽ��־��0����һ�����ݣ�1���ڶ�������
    int cpuTime = 0, cpuIdle = 0; //�ֱ�Ϊ��CPUʱ�䡢CPU����ʱ��
    int ramTotal = 0, ramFree; //�ֱ�Ϊ��RAM����������RAM
    int tmp1 = 0 //CPU�ۼ���ʱ������CPUʱ��
        tmp4 = 0 //CPU�ۼ���ʱ����������ʱ��Ƭ
    double tmp2 = 0 //RAM�ۼ���ʱ�������ڴ�����
          ,tmp3 = 0;//RAM�ۼ���ʱ�����������ڴ�
    
    while(flag != 1){
        ++count;
        if(count == REC_FREQ){
            count = 0; //��������λ
            string inf = GetHardwareInfo(); //��ȡԭʼ����
            if(oRaw){
                //����ԭʼ����
                log << inf
                    << "\n";
            }else {//����ӹ��������
                cpuTime=cpuIdle=ramTotal=ramFree=tmp1=tmp2=tmp3=tmp4=0; //�����ʱ����
                //��ԭʼ���ݷָ����vector
                vector<string> d;
                char *nowPtr = nullptr;
                while((nowPtr = strsep(&&GetHardwareInfo().c_str()," ")) != NULL){ //ʹ�ÿո�ָ�
                    d.push_back(*nowPtr);
                }
                
                for(auto p = d.cbegin();p != d.cbegin()+4;++p){
                    //��ȡǰ5�����֣��ۼ���tmp1
                    if(tick == 0)
                        tmp1+=atoi(*p.c_str());
                    else
                        tmp1-=atoi(*p.c_str());
                    
                    if(p==d.cbegin()+4)
                        tmp4+=atoi(*p.c_str());
                }
                
                tmp2+=atoi(*d[30].c_str());
                tmp3+=atoi(*d[31].c_str());
            if(tick == 1){ //�ڶ�������
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
        sleep(1); //sleep һ��
    }
    
    cout << "Closing log file...";
    log.close();
    return 0;
}

string GetHardwareInfo(){ //��proc�ж�ȡӲ����Ϣ
    ifstream f; //�ļ���
    string infOut; //������
    vector<string> inf_; //�Ի��з��ָ����Ϣ
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
    
    inf_.clear(); //�����vector
    inf.clear();
    
    f.open("/proc/meminfo");
    while(getline(f,ln))
        inf_.push_back(ln);
    f.close();
    
    for(string l : inf_){
        if(l.substr(0,8) == "MemTotal"){
            auto endp = l.rfind(" "); //���ҽ�β�ո�
            auto begp = l.substr(0,endp).rfind(" "); //���ҿ�ʼ�ո�
            inf+=l.substr(begp + 1,endp - begp - 1);
            inf+=" ";
        }else 
        if(l.substr(0,7) == "MemFree"){
            auto endp = l.rfind(" "); //���ҽ�β�ո�
            auto begp = l.substr(0,endp).rfind(" "); //���ҿ�ʼ�ո�
            inf+=l.substr(begp + 1,endp - begp - 1);
            infOut+=inf;
            break;
        }
    }
    
    return infOut;
}