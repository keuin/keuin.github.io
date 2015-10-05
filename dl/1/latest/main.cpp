#include <iostream>
#include <windows.h>

#define MP_STRONGER_ATTACK  3
#define MP_SUPER_ATTACK     5
#define MP_MAKE_ME_ANGRY    8
#define MP_NO_END_ATTACK    10

void seeSkillDetails(char func);
void useSkill(char func);
void ceo5686UseSkill();
void deathJudge();

int ceo5686Hp=50,ceo5686HpMax=50;
int myHp=50,myHpMax=50;
int ceo5686Mp=20,ceo5686MpMax=20;
int myMp=20,myMpMax=20;
int ceo5686AttBuf=1,myAttBuf=1;//攻击Buff,实际伤害为攻击力*攻击Buff
int ceo5686Att=4,myAtt=4;

int main(int argc, char** argv) {
	char inp=0;
	do {
		std::cout<<"Beat-ceo5686 (Cpp version) Version: Beta 1.3 BuildTime:20150808.2104"<<std::endl<<std::endl;
		std::cout<<"     ceo5686  You"<<std::endl
		         <<"HP   "<<ceo5686Hp<<"/"<<ceo5686HpMax
		         <<"    "<<myHp<<"/"<<myHpMax<<std::endl
		         <<"MP   "<<ceo5686Mp<<"/"<<ceo5686MpMax
		         <<"    "<<myMp<<"/"<<myMpMax<<std::endl
		         <<"ATT  "<<ceo5686Att<<"*"<<ceo5686AttBuf
		         <<"      "<<myAtt<<"*"<<myAttBuf<<std::endl;
		std::cout<<std::endl<<std::endl;
		std::cout<<"Your Skills:"<<std::endl
		         <<"[q].ATTACK"<<std::endl
		         <<"[Q].STRONGER ATTACK"<<std::endl
		         <<"[w].SUPER ATTACK"<<std::endl
		         <<"[e].MAKE ME ANGRY"<<std::endl
		         <<"[r].NO END ATTACK"<<std::endl;
		std::cout<<std::endl;
		std::cout<<"-------MENU-------"<<std::endl
		         <<"[1].See Details"<<std::endl
		         <<"[2].Use Skill"<<std::endl
		         <<"[3].Bye"<<std::endl
		         <<"------------------"<<std::endl
		         <<"What will you do(123)?"<<std::flush;
		std::cin>>inp;
		switch(inp) {
			case '1':
				std::cout<<"Press Skill Key(qQwer):"<<std::flush;
				while(!(std::cin>>inp));
				seeSkillDetails(inp);
				break;
			case '2':
				std::cout<<"Press Skill Key(qQwer):"<<std::flush;
				while(!(std::cin>>inp));
				useSkill(inp);
				deathJudge();
				ceo5686UseSkill();
				deathJudge();
				break;
			case '3':
				std::cout<<"Really exit(y/n)?"<<std::flush;
				while(!(std::cin>>inp));
				if(inp=='y' | inp=='Y')
					exit(0);
				else std::cout<<"Never Mind."<<std::endl;
				break;
			default:
				std::cout<<"Never Mind."<<std::endl;
				break;
		}
		system("PAUSE");
		system("CLS");
	} while(1);
	return 0;
}

void seeSkillDetails(char func) {
	switch(func) {
		case 'q':
			std::cout<<"ATTACK is a basic skill, it can make little damage."<<std::endl;
			break;
		case 'Q':
			std::cout<<"STRONGER ATTACK is a normal skill, it has double buff with Mp("<<MP_STRONGER_ATTACK<<")"<<std::endl;
			break;
		case 'w':
			std::cout<<"Whth the help of Mp("<<MP_SUPER_ATTACK<<"), SUPER ATTACK can make tripe damage."<<std::endl;
			break;
		case 'e':
			std::cout<<"Use Mp("<<MP_MAKE_ME_ANGRY<<"), this skill makes yourself angry, next attack will be stronger than usual."<<std::endl;
			break;
		case 'r':
			std::cout<<"With the help of more Mp("<<MP_NO_END_ATTACK<<"), NO END ATTACK can let you beat ceo5686 many times once."<<std::endl;
			break;
		default:
			std::cout<<"Wrong Skill Code."<<std::endl;
			break;
	}

}

void useSkill(char func) {
	int damage=0;
	int mpCost=0;//消耗MP
	bool reset=false;//存储是否要重置buff的bool值
	if(myAttBuf==-2) {
		myAttBuf=2;
		reset=true;
	}
	switch(func){//Mp取值
		default:
			std::cout<<"Wrong Skill Code."<<std::endl;
			return;
			break;
	    case 'q':
	    	mpCost=0;
	    	break;
		case 'Q':
			mpCost=MP_STRONGER_ATTACK;
			break;
		case 'w':
			mpCost=MP_SUPER_ATTACK;
			break;
	    case 'e':
	    	mpCost=MP_MAKE_ME_ANGRY;
	    	break;
		case 'r':
			mpCost=MP_NO_END_ATTACK;
			break;
	}
	if(mpCost>myMp){
		std::cout<<"You don't have enough Mp to use this skill!"<<std::endl;
		return;
	}
	switch(func) {
		case 'q':
			damage=myAtt*myAttBuf;
			std::cout<<"You use skill ATTACK, ceo5686 is beat by your right fist("<<damage<<")."<<std::endl;
			break;
		case 'Q':
			damage=myAtt*myAttBuf*myAttBuf;
			std::cout<<"You stamp ceo5686 with double buff on your right leg("<<damage<<")."<<std::endl;
			break;
		case 'w':
			damage=myAtt*myAttBuf*3;
			std::cout<<"You make a great damage("<<damage<<") on ceo5686, it looks very painful."<<std::endl;
			break;
		case 'e':
			myAttBuf=-2;
			std::cout<<"You are angry! 'I will kill ceo5686!', you thought(myAttBuf=-2)."<<std::endl;
			break;
		case 'r':
			int attTimes=rand()%20+1;//攻击次数
			damage=attTimes*myAtt*myAttBuf;
			std::cout<<"You mad! You crazily beat ceo5686 "<<attTimes<<" time"<<(attTimes==1 ? "(" : "s(")<<damage<<")."<<std::endl;
			break;
	}
	myMp-=mpCost;
	ceo5686Hp-=damage;
	if(reset)
		myAttBuf=1;//为临时buff，重置
}

void ceo5686UseSkill(){
	int damage=ceo5686Att*ceo5686AttBuf+rand()%3;
	std::cout<<"ceo5686 bites! You was bitten("<<damage<<") by it."<<std::endl;
	myHp-=damage;
	++ceo5686AttBuf;
}

void deathJudge(){//死亡判断
	bool gameOver=false;
	if(ceo5686Hp<=0){
		std::cout<<"Wow, ceo5686 was killed!"<<std::endl;
		gameOver=true;
	}
	if(myHp<=0){
		std::cout<<"You died!"<<std::endl;
		gameOver=true;
	}
	if(gameOver==true){
		std::cout<<"   ceo5686  You"<<std::endl
				 <<"HP   "<<ceo5686Hp<<"    "<<myHp<<std::endl
				 <<"MP   "<<ceo5686Mp<<"    "<<myMp<<std::endl;
		std::cout<<"PRESS ANY KEY TO EXIT."<<std::flush;
		system("PAUSE>NUL");
		exit(0);
	}
}
