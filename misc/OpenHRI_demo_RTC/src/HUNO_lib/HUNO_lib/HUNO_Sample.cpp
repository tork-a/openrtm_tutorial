// HUNO_Sample.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//


#include "stdafx.h"
#include "SerialPort.h"
#include "YamlParser.h"
#include "HUNO.h"
#include <iostream>
#include <fstream>

#include <conio.h>

static	unsigned char initPos[N_JOINTS] = { 123, 175, 155, 122, 125, 122, 66, 96, 126, 118, 49, 47, 40, 199, 204, 211};
static	unsigned char currentPos[N_JOINTS] = { 123, 175, 155, 122, 125, 122, 66, 96, 126, 118, 49, 47, 40, 199, 204, 211};
static	unsigned char zeroPos[N_JOINTS] = { 123, 175, 155, 122, 125, 122, 66, 96, 126, 118, 49, 47, 40, 199, 204, 211};
static	unsigned char allZero[N_JOINTS] = { 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127};

static	int flag = 1;

static short huno_init_pos[] ={
	123, 175, 155, 122, 125,
	122, 66, 96, 126, 118,
	49, 47, 40,
	199, 204, 211
};


static short sample[] ={
	0, 0, 0, 0, 0, 
	0, 0, 0 ,0, 0,
	0, 0, 0,
	0, 0, 0
};


void outputPoseToFile(char *name, unsigned char *pose)
{
	std::ofstream ofs;

	ofs.open(name, std::ios::app);

	if(ofs.fail()){
        std::cerr << "Fail to open file: " << name << std::endl;
        ofs.close();
        return;
    }

	ofs << "  - [ " ;
	for(int i=0; i < N_JOINTS; ){
	  ofs <<(int)pose[i] ;
	  i++;
	  if(i  < N_JOINTS){
		  ofs << ", " ;
	  }else{
		   ofs << " ]" << std::endl ;
	  }
	}

	ofs.close();

	return;

}

void print_joints(const char *title, 	HUNO *robot)
{
	printf("\%s: ",title);
	for(int i=0;i<N_JOINTS;i++){
		short p = robot->getAngle(i);
		printf("%d ", p);
		currentPos[i] = p;
	}
	printf("\n");
	return;
}

#define D_TH	30

int _tmain(int argc, _TCHAR* argv[])
{
	HUNO *robot;

	int loop = 1;
	char ch = ' ';
	int count = 0;
	int jointNo = 0;
	char *id="1";

	if(argc > 1){
		id = argv[1];
	}

	robot = new HUNO("COM2");
	robot->loadConfig("huno.config", "2");
	robot->connect();
	robot->startThread();


	 while(loop){
		if(flag == 1){
		   std::cout << "=>" << ch;
		}

		ch = _getch();
		flag = 1;

		switch(ch){
			/* move joint */
		    case 'l': robot->moveJoint(11,  D_TH, 0);  break;
			case 'L': robot->moveJoint(11, -D_TH, 0);  break;

			case 'k': robot->moveJoint(10,  D_TH, 0);  break;
			case 'K': robot->moveJoint(10, -D_TH, 0);  break;

			case 'm': robot->moveJoint(12, -D_TH, 0);  break;
			case 'M': robot->moveJoint(12,  D_TH, 0);  break;

			case 'h': robot->moveJoint( 14, -D_TH, 0);  break;
			case 'H': robot->moveJoint(14,  D_TH, 0);  break;

			case 'j': robot->moveJoint(13, -D_TH, 0);  break;
			case 'J': robot->moveJoint( 13,  D_TH, 0);  break;

			case 'n': robot->moveJoint( 15,  D_TH, 0);  break;
			case 'N': robot->moveJoint( 15, -D_TH, 0);  break;


			case 'x': robot->moveJoint(jointNo, -D_TH, 0);  break;
			case 'z': robot->moveJoint( jointNo,  D_TH, 0);  break;

				/* Servo */
			case 'b': robot->setBreakMode(); break;
			case 'a': robot->servo(0, 0); break;
			case 'A': robot->servo(0, 1); break;

				/* Init Position */
			case 'i': robot->initPosition(); break;

			case 'Z':
				robot->setJointsFromString("0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0");
				robot->startMotion();
				   break;

				   /* load motion file */
			case 'X':
				robot->loadMotion("SampleMotion");
				robot->setMotionCount(1);
				   break;
				/* print state */
			case 's':
				   printf("CPose: ");
				   for(int i=0;i<N_JOINTS;i++){  printf("%d ", currentPos[i]); }
				   printf("\n");
				   break;

			case 'S':
				  robot->clearComBuffer();
				  print_joints("CPose", robot);
				 break;
				 

			case 'P': outputPoseToFile("out.txt", currentPos); break;

			case 'C': robot->clearComBuffer();  break;

			case 'q':  loop=0;  break;

			default:
				if(ch >= '0' && ch <= '9') {  jointNo = ch - '0'; }  break;
		}
	}

	robot->terminate();
	delete robot;

	exit(0);
}

