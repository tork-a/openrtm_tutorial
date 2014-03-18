/*
   Motion pleyer class:

   Copyrght(C) Isao Hara

*/
#ifndef __MOTION_PLAYER_H__
#define __MOTION_PLAYER_H__

#include <cnoid/Plugin>
#include <cnoid/ItemTreeView>
#include <cnoid/BodyItem>
//#include <cnoid/ToolBar>
#include <cnoid/MessageView>
#include <cnoid/BodyMotion>
#include <boost/bind.hpp>

#include <rtm/idl/BasicDataTypeSkel.h>

#include "cutils.h"

using namespace boost;
using namespace cnoid;

class MotionPlayer
{
public:

	MotionPlayer();
	MotionPlayer(BodyItem *body, std::string name);
	~MotionPlayer();

	void svc();
	int start();
	void stop();
	bool isActive();
	int getTimeout();
	bool loadMotion(std::string name);
	void setMotion(BodyMotion *motion);
	void setMotionPath(std::string path);
	void setBody(BodyItem *body);
	int doAction(std::string name);
	int doAction(std::string dir,std::string name);

	void setPose(RTC::TimedDoubleSeq pose, std::vector<int> mask, int flag);
	void setJointAngle(int id, double angle);
	void updateJoints();

	void setMask(int *mask, int len);
	void clearMask();
	int isMaskedMotion();

public:
	MultiValueSeqPtr motionPtr;
	std::string yamlpath;

	int numJoints;
	int numFrames;
	int frameRate;
	int currentFrame;
	int timeout;
	BodyItem *bodyItem;

	std::vector<int> maskMotion;
	THREAD_HANDLE hThread; 
};


THREAD_FUNC thread_execution(void *args);

#endif
