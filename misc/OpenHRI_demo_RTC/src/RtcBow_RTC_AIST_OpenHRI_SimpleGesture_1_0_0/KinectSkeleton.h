#ifndef __KINECTSKELETON_H__
#define __KINECTSKELETON_H__

#include <iostream>
#include <math.h>
#include <rtm/idl/BasicDataTypeSkel.h>


/* Values.... */

#define ROT_JOINT               21

#define N_SKEL_DATA             20

enum SkeletonPos {
  NUI__HIP_CENTER=0,
  NUI__SPINE,
  NUI__SHOULDER_CENTER,
  NUI__HEAD,
  NUI__SHOULDER_LEFT,
  NUI__ELBOW_LEFT,
  NUI__WRIST_LEFT,
  NUI__HAND_LEFT,
  NUI__SHOULDER_RIGHT,
  NUI__ELBOW_RIGHT,
  NUI__WRIST_RIGHT,
  NUI__HAND_RIGHT,
  NUI__HIP_LEFT,
  NUI__KNEE_LEFT,
  NUI__ANKLE_LEFT,
  NUI__FOOT_LEFT,
  NUI__HIP_RIGHT,
  NUI__KNEE_RIGHT,
  NUI__ANKLE_RIGHT,
  NUI__FOOT_RIGHT
};

#define HEAD            NUI__HEAD * 3
#define SHOULDER_LEFT   NUI__SHOULDER_LEFT * 3
#define ELBOW_LEFT      NUI__ELBOW_LEFT * 3
#define HAND_LEFT	    NUI__HAND_LEFT * 3
#define SHOULDER_RIGHT	NUI__SHOULDER_RIGHT * 3
#define ELBOW_RIGHT     NUI__ELBOW_RIGHT * 3
#define HAND_RIGHT	    NUI__HAND_RIGHT * 3
#define SPINE		    NUI__SPINE * 3


#define HEIGHT                  20

#define HIP_R_RIGHT             0
#define HIP_R_LEFT              6

#define ELBOW_LEFT_JOINT        19
#define ELBOW_RIGHT_JOINT       16
#define SHOULDER_R_LEFT_JOINT   18
#define SHOULDER_R_RIGHT_JOINT  15
#define SHOULDER_P_LEFT_JOINT   17
#define SHOULDER_P_RIGHT_JOINT  14
#define CHEST_JOINT             12


/*   */
#define GESTURE_DISABLE         0
#define GESTURE_ENABLE          1


#define INIT_STATE				0
#define BOTH_HANDS_DOWN         1
#define BOTH_HANDS_UP           2
#define LEFT_HAND_UP            3
#define RIGHT_HAND_UP           4

#define INIT_POS				-100
#define NO_ACTION				0
#define SWIPE_RIGHT				1
#define SWIPE_RIGHT_FAST		2
#define SWIPE_LEFT				3
#define SWIPE_LEFT_FAST			4
#define SWIPE_DOWN				5
#define SWIPE_DOWN_FAST			6
#define SWIPE_UP				7
#define SWIPE_UP_FAST			8

#define MOTION_TH1				0.2
#define MOTION_TH2				0.1

#ifndef M_PI
#define M_PI 3.14159265358979
#endif

#ifndef M_PI2
#define M_PI2 3.14159265358979 * 2.0
#endif

#ifndef M_PI_H
#define M_PI_H 3.14159265358979 / 2.0
#endif


void copyKinectData(double *dist, RTC::TimedFloatSeq src, int idx);
void  TimedDoubleSeq2Double(double *dist, RTC::TimedFloatSeq src, int idx, int size);
double calcLength3d(double *v);
double calcAngle(double *a, double *b, double *c);
double calcAngle2(RTC::TimedFloatSeq val, int idx1, int idx2, int idx3, int idx4);
double calcAngle(RTC::TimedFloatSeq val, int idx1, int idx2, int idx3);
double calcJointAngle(RTC::TimedFloatSeq val, int idx);
int nearly_equal(double x,  double y, double tolerance);



class KinectSimpleGesture
{

public:
	KinectSimpleGesture();
	~KinectSimpleGesture();

    void initState();

	int is_startPose();
	int is_stopPose();
	int is_recogArea();
	
	int in_HeadSide(int flag);

	int is_gestureArea(double pos_z);
    void setGestureArea(double z1, double z2);

	int isHandUp(double *hand);
	int isHandDown(double *hand);

	int getHandMotion_H(double *hand, double *v, double *prev);
	int getHandMotion_V(double *hand, double *v, double *prev);

	int getRightHandMotion();
	int getLeftHandMotion();

	int getHandState();

	int checkGesture(RTC::TimedFloatSeq m_skeleton);

public:
  double m_JointAngle[ROT_JOINT];
  double body_rot, body_height;
  double spine[3], hand_r[3], hand_l[3], head[3], wrist_r[3], wrist_l[3], elbow_r[3],elbow_l[3];
  double shoulder_c[3], shoulder_l[3], shoulder_r[3];
  double *hand_r_pos, *hand_l_pos;

  double hand_pos_r, hand_pos_l;

  int gesture_state;

  int current_state;
  int next_state;
  int state_changed;

  int pos_r, pos_l;

  double prev_hand_r, v_hand_r;
  double prev_hand_l, v_hand_l;

  double prev_hand_r_v, v_hand_r_v;
  double prev_hand_l_v, v_hand_l_v;

  double motion_thr1, motion_thr2;

  int motion_r_h, motion_l_h, motion_r;
  int motion_r_v, motion_l_v, motion_l;

  double min_z, max_z;
};


#endif
