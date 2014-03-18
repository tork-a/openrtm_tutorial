#ifndef __KINECTSKELETON_H__
#define __KINECTSKELETON_H__

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

#ifndef M_PI
#define M_PI 3.14159265358979
#endif

#ifndef M_PI2
#define M_PI2 3.14159265358979 * 2.0
#endif

#ifndef M_PI_H
#define M_PI_H 3.14159265358979 / 2.0
#endif

void  TimedDoubleSeq2Double(double *dist, RTC::TimedFloatSeq src, int idx, int size);
double calcLength3d(double *v);
double calcAngle(double *a, double *b, double *c);
double calcAngle2(RTC::TimedFloatSeq val, int idx1, int idx2, int idx3, int idx4);
double calcAngle(RTC::TimedFloatSeq val, int idx1, int idx2, int idx3);
double calcJointAngle(RTC::TimedFloatSeq val, int idx);

#endif
