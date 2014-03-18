/*
   Motion pleyer class:

   Copyrght(C) Isao Hara

*/

#include "MotionPlayer.h"

MotionPlayer::MotionPlayer()
{
	setMotionPath(PROJECT_DIR);
	currentFrame = -1;
	maskMotion.reserve(20);
	clearMask();
}


MotionPlayer::MotionPlayer(BodyItem *body, std::string name)
{
	bodyItem = body;
	setMotionPath(PROJECT_DIR);
	loadMotion(name);
}

MotionPlayer::~MotionPlayer()
{

}

void
MotionPlayer::setBody(BodyItem *body)
{
	bodyItem = body;
}

int
MotionPlayer::doAction(std::string name)
{
	if(isActive()) { return -1; }
	if(loadMotion(name)){
		return start();
	}
	return -1;
}

int
MotionPlayer::doAction(std::string dir, std::string name)
{
	setMotionPath(dir);
	return doAction(name);
}


void 
MotionPlayer::svc()
{
	if(currentFrame >= 0 && currentFrame < numFrames)
	{
		BodyPtr body = bodyItem->body();

		for(int i=0; i < body->numJoints() && i < numJoints ; ++i){
			if(maskMotion[i] != 1 ){
			  body->joint(i)->q = motionPtr->frame(currentFrame)[i];
			}
		}
		bodyItem->notifyKinematicStateChange(true);

		currentFrame += 1;
	}else{
		stop();
	}
}

int
MotionPlayer::start()
{
	if(isActive()){   stop();  }

	currentFrame = 0;
	if(Pthread_Create(&hThread, NULL, thread_execution, this) != 0){
		return 0;
	}
	return 1;
}

void 
MotionPlayer::stop()
{
	currentFrame = -1;


#ifdef WIN32
	if(hThread == NULL) return;
#else
	if(hThread == 0) return;
#endif

	Pthread_Join(hThread, NULL);
	Pthread_Exit(hThread);

	hThread = NULL;
	return;
}

bool 
MotionPlayer::isActive()
{
	return  (currentFrame >= 0);
}

int 
MotionPlayer::getTimeout()
{
	return  timeout;
}

bool 
MotionPlayer::loadMotion(std::string name)
{
	bool res;
	std::string fname = yamlpath + name + ".yaml"; 
    BodyMotion *motion = new BodyMotion();

	if( (res = motion->loadStandardYamlFormat(fname)) ){
		MessageView::mainInstance()->cout() << "Load " << fname << std::endl;
		motionPtr = motion->jointPosSeq();
		setMotion(motion);
	}else{
		MessageView::mainInstance()->cout() <<  "Fail to load file:" << fname << std::endl;
		delete motion;
	}
	return res;
}

int getJointIndex(int index, int type)
{
   char GRoo1JointIdToMotorIdMap[] = {
    9 /* R_HIP_Y */, 11 /* R_HIP_R */,  10 /* R_HIP_P */, 12 /* R_KNEE_P */, 13 /* R_ANKLE_P */, 14 /* R_ANKLE_R */,
    15 /* L_HIP_Y */, 17 /* L_HIP_R */, 16 /* L_HIP_P */, 18 /* L_KNEE_P */, 19 /* L_ANKLE_P */, 20 /* L_ANKLE_R */,
    1 /* CHEST_P */,  2 /* NECK_Y */,
    3 /* R_SHOULDER_P */, 4 /* R_SHOULDER_R */, 5 /* R_ELBOW_P */,
    6 /* R_SHOULDER_P */, 7 /* R_SHOULDER_R */, 8 /* R_ELBOW_P */ };


	switch(type){
	case 1:
		return GRoo1JointIdToMotorIdMap[index] -1;
	default:
		break;
	}

	return index;
}

double convertAngle(double angle, int type)
{
	switch(type){
	case 1:
		return angle  / 1800.0 *3.141592;
	default:
		break;
	}

	return angle;
}

void
MotionPlayer::setPose(RTC::TimedDoubleSeq pose, std::vector<int> mask, int flag)

{
	BodyPtr body = bodyItem->body();

	int pose_len = pose.data.length();
	int mask_len = mask.size();

	for(int i=0; i < body->numJoints() && i < pose_len ; ++i){
		if(i >= mask_len || mask[i] == 0){
			body->joint(i)->q = convertAngle(pose.data[getJointIndex(i, flag)], flag);
		}
	}
	bodyItem->notifyKinematicStateChange(true);
	
	return;
}


void
MotionPlayer::setJointAngle(int id, double angle)

{
	if(angle > 10){ return; }
	BodyPtr body = bodyItem->body();

	body->joint(id)->q = angle;
	
	return;
}

void
MotionPlayer::updateJoints()
{
	BodyPtr body = bodyItem->body();
	bodyItem->notifyKinematicStateChange(true);
	return;
}


void
MotionPlayer::setMotion(BodyMotion *motion)

{
	numJoints = motion->getNumParts();
	numFrames = motion->getNumFrames();
	frameRate = motion->getFrameRate();

	currentFrame = -1;
	timeout = 1000/frameRate;
	motionPtr = motion->jointPosSeq();
	return;
}

void
MotionPlayer::setMotionPath(std::string path)
{
	yamlpath = path;
}

void 
MotionPlayer::setMask(int *mask, int len){
    int mlen = maskMotion.size();

	if(mlen < len){
		maskMotion.resize(len);
	}
	for(int i=0;i<len;i++){
		maskMotion[i]=mask[i];
	}
	return;
}

void 
MotionPlayer::clearMask(){
	int len = maskMotion.size();
	for(int i=0;i<len;i++){
		maskMotion[i]=0;
	}
	return;
}

int
MotionPlayer::isMaskedMotion(){
	int len = maskMotion.size();
	for(int i=0;i<len;i++){
		if(maskMotion[i] != 0) return 1;
	}
	return 0;
}
/*



*/
THREAD_FUNC thread_execution(void *args)
{

  int t1;
  int ms;

  MotionPlayer *robot = (MotionPlayer *)args;
  t1 = getCurrentTime();

  while(robot->isActive()){
    robot->svc();

    ms = getCurrentTime() - t1;
	t1 = t1 + ms;
    ms = robot->getTimeout() - ms;

    if(ms > 0){
      Sleep(ms);
	  t1 = t1+ms;
    }else{
      Sleep(0);
    }
	
  }

#ifdef WIN32
  return;
#else
  return NULL;
#endif
}

