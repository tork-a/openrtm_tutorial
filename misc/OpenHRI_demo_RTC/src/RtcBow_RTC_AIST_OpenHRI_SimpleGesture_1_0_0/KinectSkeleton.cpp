#include "KinectSkeleton.h"


void copyKinectData(double *dist, RTC::TimedFloatSeq src, int idx)
{
  return TimedDoubleSeq2Double(dist, src,  idx*3, 3);
}



void 
TimedDoubleSeq2Double(double *dist, RTC::TimedFloatSeq src, int idx, int size)
{
  for(int i=0;i<size;i++){
    dist[i] = src.data[i+idx];
  }
  return;
}


double
calcLength3d(double *v)
{
  return sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
}

double 
calcAngle(double *a, double *b, double *c)
{
  double v1[3], v2[3];
  for(int i=0; i<3; i++){
    v1[i] = a[i] - b[i];
    v2[i] = c[i] - b[i];
  }

  return acos((v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2])/calcLength3d(v1)/calcLength3d(v2)) ;
}

double
calcAngle(RTC::TimedFloatSeq val, int idx1, int idx2, int idx3)
{
  double a[3], b[3], c[3];

  TimedDoubleSeq2Double((double *)a, val, idx1, 3);
  TimedDoubleSeq2Double((double *)b, val, idx2, 3);
  TimedDoubleSeq2Double((double *)c, val, idx3, 3);

  return  calcAngle((double *)a, (double *)b, (double *)c);
}

double calcAngle2(RTC::TimedFloatSeq val, int idx1, int idx2, int idx3, int idx4)
{
  double xa1 = val.data[idx2  ] - val.data[idx1];
  double ya1 = val.data[idx2+1] - val.data[idx1+1];
  double za1 = val.data[idx2+2] - val.data[idx1+2];

  double xa2 = val.data[idx3  ] - val.data[idx1];
  double ya2 = val.data[idx3+1] - val.data[idx1+1];
  double za2 = val.data[idx3+2] - val.data[idx1+2];

  double xa3 = val.data[idx4  ] - val.data[idx1];
  double ya3 = val.data[idx4+1] - val.data[idx1+1];
  double za3 = val.data[idx4+2] - val.data[idx1+2];

  double a[3],b[3];

  a[0] = ya1 * za2 - za1 * ya2;
  a[1] = za1 * xa2 - xa1 * za2;
  a[2] = xa1 * ya2 - ya1 * xa2;

  b[0] = ya1 * za3 - za1 * ya3;
  b[1] = za1 * xa3 - xa1 * za3;
  b[2] = xa1 * ya3 - ya1 * xa3;

  return acos((a[0]*b[0]+a[1]*b[1]+a[2]*b[2])/calcLength3d(a)/calcLength3d(b)) ;
}


double
calcJointAngle(RTC::TimedFloatSeq val, int idx)
{
  double ans;

  switch(idx){
    case  ELBOW_LEFT_JOINT:
      ans = M_PI - calcAngle(val, SHOULDER_LEFT, ELBOW_LEFT, HAND_LEFT);
      break;

    case ELBOW_RIGHT_JOINT:
      ans = calcAngle(val, SHOULDER_RIGHT, ELBOW_RIGHT, HAND_RIGHT) - M_PI ;
      break;

    case SHOULDER_R_RIGHT_JOINT:
      {
        ans = M_PI_H - calcAngle(val, SHOULDER_LEFT, SHOULDER_RIGHT, ELBOW_RIGHT) ;
      }
      break;

    case SHOULDER_R_LEFT_JOINT:
      {
        ans = calcAngle(val, SHOULDER_RIGHT, SHOULDER_LEFT, ELBOW_LEFT) - M_PI_H;
      }
      break;

    case SHOULDER_P_LEFT_JOINT:
      {
        ans =  calcAngle2(val, SHOULDER_LEFT,SHOULDER_RIGHT, ELBOW_LEFT, SPINE);
        if( val.data[ELBOW_LEFT+2] > val.data[SHOULDER_LEFT+2] ){
          ans = -ans;
        }
        if(ans > M_PI+M_PI_H/2){
            ans -= M_PI2;
        }else if(ans < -M_PI_H - M_PI_H/2){
            ans += M_PI2;
        }
      }
      break;

    case SHOULDER_P_RIGHT_JOINT:
      {
        ans = -calcAngle2(val, SHOULDER_RIGHT ,SHOULDER_LEFT, ELBOW_RIGHT, SPINE);
        if( val.data[ELBOW_RIGHT + 2] > val.data[SHOULDER_RIGHT+2] ){
          ans = -ans;
        }
        if(ans < -M_PI-M_PI_H/2){
            ans += M_PI2;
        }else if(ans > M_PI_H+ M_PI_H/2){
            ans -= M_PI2;
        }
      }
      break;

    case CHEST_JOINT:
    {
      double a[3],o[3],b[3];
      a[0] = val.data[HEAD  ] - val.data[SPINE];
      a[1] = val.data[HEAD+1] - val.data[SPINE+1];
      a[2] = val.data[HEAD+2] - val.data[SPINE+2];

      b[0] = b[2] = o[0] = o[1] = o[2] = 0;
      b[1] = 1;

      if( val.data[HEAD+2] > val.data[SPINE+2]){
        ans=0;
      }else{
        ans = -1 * calcAngle(a,o,b);
      }
    }
      break;

    case ROT_JOINT:
    {
      double x1 = val.data[SHOULDER_RIGHT  ] - val.data[SHOULDER_LEFT];
      double z1 = val.data[SHOULDER_RIGHT+2] - val.data[SHOULDER_LEFT+2];

      if(val.data[SHOULDER_RIGHT+2] > val.data[SHOULDER_LEFT+2] ){
        ans = acos(x1/sqrt(x1*x1+z1*z1));
      }else{;
        ans  =-acos(x1/sqrt(x1*x1+z1*z1));
      }
    }
      break;

    default:
      ans = 0;
      break;
  }

  return ans;
}

/*
  Gesture...

*/

int 
nearly_equal(double x,  double y, double tolerance)
{
  double v = fabs( x - y );

  if(v < tolerance) return 1;
  return 0;
}

/*

*/

KinectSimpleGesture::KinectSimpleGesture()
{

  motion_thr1=MOTION_TH1;
  motion_thr2=MOTION_TH2;
  min_z = 1.0;
  max_z = 1.6;
  hand_r_pos = new double[10];
  hand_l_pos = new double[10];
  initState();
}

KinectSimpleGesture::~KinectSimpleGesture()
{

}

void KinectSimpleGesture::initState()
{
  gesture_state = GESTURE_DISABLE;
  current_state = INIT_STATE;

  prev_hand_r = prev_hand_l = INIT_POS;
  v_hand_r = v_hand_l = 0;

  prev_hand_r_v = prev_hand_l_v = INIT_POS;
  v_hand_r_v = v_hand_l_v =0;

  state_changed = 0;
  motion_l_h = motion_l_v = motion_r_h = motion_r_v = NO_ACTION;

  for(int i=0;i<10;i++){
    hand_l_pos[i] = 0.0;
    hand_r_pos[i] = 0.0;
  }
  pos_r = pos_l = 0;
}


int KinectSimpleGesture::getHandMotion_H(double *hand, double *v, double *prev)
{
  int res = NO_ACTION;

  if(hand[1] - spine[1] > 0.0 &&  head[1] - hand[1] > 0.0){  
    if(hand[0] - shoulder_l[0] > 0.0 &&  shoulder_r[0] - hand[0] > 0.0){

      if (*prev == INIT_POS){
        *v = 0;
        *prev = hand[0];
      }else{
        *v += hand[0] - *prev;
        *prev= hand[0];
      }
    }else{
      if(*v > motion_thr1) {
        res = SWIPE_RIGHT;

      }else if(*v > motion_thr2) {
        res = SWIPE_RIGHT_FAST;

      }else if(*v < -motion_thr1) {
        res = SWIPE_LEFT;

      }else if(*v < -motion_thr2) {
        res = SWIPE_LEFT_FAST;

      }
        *prev = INIT_POS;
        *v = 0;
    }
  }else{
    if(*v > motion_thr1) {
      res = SWIPE_RIGHT;
    }else if(*v < -motion_thr1) {
      res = SWIPE_LEFT;
    }

    *prev = INIT_POS;
    *v = 0;
  }
  return res;
}


int KinectSimpleGesture::getHandMotion_V(double *hand, double *v, double *prev)
{
  int res = NO_ACTION;

  double motion_thr1_v = motion_thr1 *1.0;
  double motion_thr2_v = motion_thr2 * 1.0;

  if(hand[0] - shoulder_l[0] > 0.0 &&  shoulder_r[0] - hand[0] > 0.0){
    if(hand[1] - spine[1] > 0.0 &&  shoulder_c[1] - hand[1] > 0.0){  
      if (*prev == INIT_POS){
        *v = 0;
        *prev = hand[1];
      }else{
        *v += hand[1] - *prev;
        *prev= hand[1];
      }

    }else{
      if(*v > motion_thr1_v) {
        res = SWIPE_UP;

      }else if(*v > motion_thr2_v) {
        res = SWIPE_UP_FAST;

      }else if(*v < -motion_thr1_v) {
        res = SWIPE_DOWN;

      }else if(*v < -motion_thr2_v) {
        res = SWIPE_DOWN_FAST;
	  }

      *prev = INIT_POS;
      *v = 0;
    }
  }else{
    if(*v > motion_thr1_v) {
      res = SWIPE_UP;

    }else if(*v < -motion_thr1_v) {
      res = SWIPE_DOWN;
	}

    *prev = INIT_POS;
    *v = 0;
  }

  return res;
}

int KinectSimpleGesture::is_startPose(){
  return (gesture_state == GESTURE_DISABLE &&
          nearly_equal(hand_r[1], head[1], 0.1) &&
          nearly_equal(hand_l[1], head[1], 0.1)  ) ;
}

int KinectSimpleGesture::is_recogArea()
{
  return (nearly_equal(hand_r[1], shoulder_c[1], 0.1) && 
          nearly_equal(hand_l[1], shoulder_c[1], 0.1) )  ;
}

int KinectSimpleGesture::is_gestureArea(double pos_z)
{
   return( pos_z >min_z &&  pos_z <max_z);
}

void KinectSimpleGesture::setGestureArea(double z1, double z2)
{
   min_z = z1;
   max_z = z2;
   return;
}

int KinectSimpleGesture::is_stopPose()
{
  double thr = 0.08;
  return  (nearly_equal(shoulder_r[1], elbow_r[1], thr) && 
           nearly_equal(shoulder_l[1], elbow_l[1], thr) &&
		   nearly_equal(shoulder_l[1], hand_l[1], thr) &&
		   nearly_equal(shoulder_r[1], hand_r[1], thr) 
		   );
}

int KinectSimpleGesture::in_HeadSide(int flag){
  double *hand;
  if(flag == 1){  // right side
    hand = hand_r;
	return (hand[1] > shoulder_c[1] && hand[1] < head[1]+0.1 && hand[0] > shoulder_c[0]);
  }else{         // left side
    hand = hand_l;
	return (hand[1] > shoulder_c[1] && hand[1] < head[1]+0.1 && hand[0] < shoulder_c[0]);
  }
}


int KinectSimpleGesture::isHandUp(double *hand){
 return( hand[1] > head[1]+0.1);
}

int KinectSimpleGesture::isHandDown(double *hand){
  return (hand[1] < shoulder_c[1]);
}

int KinectSimpleGesture::getHandState()
{
  next_state = current_state;

  if( is_startPose() ){
	
    gesture_state = GESTURE_ENABLE;
    next_state = INIT_STATE;
  }

  if(gesture_state){
    if( is_recogArea() ) {
      if( is_stopPose() ) {
		 std::cout << " ]]] Stop Gesture mode  " << std::endl;
        gesture_state = GESTURE_DISABLE;
        next_state = INIT_STATE;
      }

    }else if( isHandUp( hand_r ) && isHandDown( hand_l ) && current_state != RIGHT_HAND_UP ) {
      next_state = RIGHT_HAND_UP;

    }else if( isHandDown( hand_r ) && isHandUp( hand_l ) && current_state != LEFT_HAND_UP ) {
      next_state = LEFT_HAND_UP;

    }else if( isHandUp( hand_r ) && isHandUp( hand_l ) && current_state != BOTH_HANDS_UP ){
      next_state = BOTH_HANDS_UP;

    }else if( isHandDown( hand_r ) && isHandDown( hand_l ) && current_state != BOTH_HANDS_DOWN ){
      next_state = BOTH_HANDS_DOWN;

    }
  }
  return next_state != current_state;
}

int KinectSimpleGesture::getRightHandMotion()
{
	int res=0;
	if(in_HeadSide(1)){
		//std::cout << pos_r << " ";
		hand_r_pos[pos_r] = hand_r[0] - shoulder_r[0];
		
		if(pos_r == 9){
			double sum = 0.0;
			double sumabs = 0.0;
		    double m_pos=0.0;
	
			for(int i=0; i<9 ; i++){
				double d = hand_r_pos[i] -  hand_r_pos[i+1];
				m_pos +=  hand_r_pos[i];
				sum +=  d;
				sumabs += fabs(d);
			}
			m_pos +=  hand_r_pos[9];
			m_pos = m_pos/10.0;

		  if(sumabs < 0.01 && fabs(sum) < 0.01){
			hand_pos_r = m_pos;
			res = 1;

		  }else if (  ( sumabs >  fabs(sum)*3.0 && sumabs > 0.25 )){
			//std::cout << sumabs << ", " << fabs(sum)*3.0 << "   " ;
			res = 2;
		  }else{
			hand_pos_r = m_pos;
			res = 3;
		  }
		}
		pos_r = (pos_r+1) % 10;
	}else{
		pos_r = 0;
	}
	return res;
}

int KinectSimpleGesture::getLeftHandMotion()
{
	int res = 0;
	if(in_HeadSide(0)){
		hand_l_pos[pos_l] = hand_l[0] - shoulder_l[0];
		
		if(pos_l == 9){
			double sum = 0.0;
			double sumabs = 0.0;
		    double m_pos=0.0;
	
			for(int i=0; i<9 ; i++){
				double d = hand_l_pos[i] -  hand_l_pos[i+1];
				m_pos +=  hand_l_pos[i];
				sum +=  d;
				sumabs += fabs(d);
			}
			m_pos +=  hand_l_pos[9];
			m_pos = m_pos/10.0;

		  if(sumabs < 0.01 && fabs(sum) < 0.01){
			hand_pos_l = m_pos;
		    res = 1;

		  }else if (  ( sumabs >  fabs(sum)*3.0 && sumabs > 0.25 )){
			//std::cout << sumabs << ", " << fabs(sum)*3.0 << "   " ;
			res = 2;
		  }else{
			hand_pos_l = m_pos;
			res = 3;
		  }
		}
		pos_l = (pos_l+1) % 10;
	}else{
		pos_l = 0;
	}
	return res;
}

int KinectSimpleGesture::checkGesture(RTC::TimedFloatSeq m_skeleton)
{
  int len = m_skeleton.data.length() / 3;

//  m_JointAngle[ ELBOW_LEFT_JOINT]        = calcJointAngle(m_skeleton, ELBOW_LEFT_JOINT);
//      m_JointAngle[ SHOULDER_R_LEFT_JOINT]   = calcJointAngle(m_skeleton, SHOULDER_R_LEFT_JOINT);
//      m_JointAngle[ SHOULDER_P_LEFT_JOINT]   = calcJointAngle(m_skeleton, SHOULDER_P_LEFT_JOINT);

//  m_JointAngle[ ELBOW_RIGHT_JOINT]       = calcJointAngle(m_skeleton, ELBOW_RIGHT_JOINT);
//      m_JointAngle[ SHOULDER_R_RIGHT_JOINT ] = calcJointAngle(m_skeleton, SHOULDER_R_RIGHT_JOINT);
//      m_JointAngle[ SHOULDER_P_RIGHT_JOINT ] = calcJointAngle(m_skeleton, SHOULDER_P_RIGHT_JOINT);
//      m_JointAngle[ CHEST_JOINT]             = calcJointAngle(m_skeleton, CHEST_JOINT);
      
//      body_rot = calcJointAngle(m_skeleton, ROT_JOINT);

  body_height = m_skeleton.data[SPINE+1];

  copyKinectData(spine,      m_skeleton, NUI__SPINE);
  copyKinectData(hand_r,     m_skeleton, NUI__HAND_RIGHT);
  copyKinectData(hand_l,     m_skeleton, NUI__HAND_LEFT);
  copyKinectData(head,       m_skeleton, NUI__HEAD);
  copyKinectData(wrist_r,    m_skeleton, NUI__WRIST_RIGHT);
  copyKinectData(wrist_l,    m_skeleton, NUI__WRIST_LEFT);
  copyKinectData(shoulder_c, m_skeleton, NUI__SHOULDER_CENTER);
  copyKinectData(shoulder_r, m_skeleton, NUI__SHOULDER_RIGHT);
  copyKinectData(shoulder_l, m_skeleton, NUI__SHOULDER_LEFT);
  copyKinectData(elbow_l, m_skeleton, NUI__ELBOW_LEFT);
  copyKinectData(elbow_r, m_skeleton, NUI__ELBOW_RIGHT);
     
  if( is_gestureArea( m_skeleton.data[SPINE+2] ) ){

    if(state_changed = getHandState()){
      current_state = next_state;
    }

    if(current_state == BOTH_HANDS_DOWN){
      motion_r_h = getHandMotion_H(hand_r, &v_hand_r, &prev_hand_r);
      motion_l_h = getHandMotion_H(hand_l, &v_hand_l, &prev_hand_l);
    }

    if( gesture_state ){
      motion_r_v = getHandMotion_V(hand_r, &v_hand_r_v, &prev_hand_r_v);
      motion_l_v = getHandMotion_V(hand_l, &v_hand_l_v, &prev_hand_l_v);
    }

	motion_r = getRightHandMotion();
    motion_l = getLeftHandMotion();

  }else{
    initState();
  }
  return gesture_state;
}

