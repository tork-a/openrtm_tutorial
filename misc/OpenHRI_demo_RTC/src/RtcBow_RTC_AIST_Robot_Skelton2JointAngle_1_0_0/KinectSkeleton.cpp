#include "KinectSkeleton.h"

\
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
#if 1
        ans = M_PI_H - calcAngle(val, SHOULDER_LEFT, SHOULDER_RIGHT, ELBOW_RIGHT) ;
#else
        double v[3];
        v[0] = val.data[ ELBOW_RIGHT ]- val.data[ SHOULDER_RIGHT ];
        v[1] = val.data[ ELBOW_RIGHT+1 ]- val.data[ SHOULDER_RIGHT+1 ];
        v[2] = val.data[ ELBOW_RIGHT+2 ]- val.data[ SHOULDER_RIGHT+2 ];
        double x = v[0] / calcLength3d(v);
        ans = -asin(x) ;
#endif
      }
      break;

    case SHOULDER_R_LEFT_JOINT:
      {
#if 1
        ans = calcAngle(val, SHOULDER_RIGHT, SHOULDER_LEFT, ELBOW_LEFT) - M_PI_H;
#else
        double v[3];
        v[0] = val.data[ ELBOW_LEFT ]- val.data[ SHOULDER_LEFT ];
        v[1] = val.data[ ELBOW_LEFT+1 ]- val.data[ SHOULDER_LEFT+1 ];
        v[2] = val.data[ ELBOW_LEFT+2 ]- val.data[ SHOULDER_LEFT+2 ];
        double x = v[0] / calcLength3d(v);
        ans = asin(-x);
#endif
      }
      break;

    case SHOULDER_P_LEFT_JOINT:
      {
#if 1
        ans =  calcAngle2(val, SHOULDER_LEFT,SHOULDER_RIGHT, ELBOW_LEFT, SPINE);
        if( val.data[ELBOW_LEFT+2] > val.data[SHOULDER_LEFT+2] ){
          ans = -ans;
        }
        if(ans > M_PI+M_PI_H/2){
            ans -= M_PI2;
        }else if(ans < -M_PI_H - M_PI_H/2){
            ans += M_PI2;
        }
#else
        double v[3];
        v[0] = val.data[ ELBOW_LEFT ]- val.data[ SHOULDER_LEFT ];
        v[1] = val.data[ ELBOW_LEFT+1 ]- val.data[ SHOULDER_LEFT+1 ];
        v[2] = val.data[ ELBOW_LEFT+2 ]- val.data[ SHOULDER_LEFT+2 ];
        double l = calcLength3d(v);
        double y = v[1] /l;
        double z = v[2] /l;
        ans = atan2(z, y)+M_PI;
        if(ans < -M_PI){
           ans += M_PI2;
        }else if(ans > M_PI){
          ans -= M_PI2;
        }
#endif

      }
      break;

    case SHOULDER_P_RIGHT_JOINT:
      {
#if 1
        ans = -calcAngle2(val, SHOULDER_RIGHT ,SHOULDER_LEFT, ELBOW_RIGHT, SPINE);
        if( val.data[ELBOW_RIGHT + 2] > val.data[SHOULDER_RIGHT+2] ){
          ans = -ans;
        }
        if(ans < -M_PI-M_PI_H/2){
            ans += M_PI2;
        }else if(ans > M_PI_H+ M_PI_H/2){
            ans -= M_PI2;
        }
#else
        double v[3];
        v[0] = val.data[ ELBOW_RIGHT ]- val.data[ SHOULDER_RIGHT ];
        v[1] = val.data[ ELBOW_RIGHT+1 ]- val.data[ SHOULDER_RIGHT+1 ];
        v[2] = val.data[ ELBOW_RIGHT+2 ]- val.data[ SHOULDER_RIGHT+2 ];
        double l = calcLength3d(v);
        double y = v[1] /l;
        double z = v[2] /l ;
        ans = -atan2(z, y) - M_PI;

        if(ans < -M_PI){
           ans += M_PI2;
        }else if(ans > M_PI){
          ans -= M_PI2;
        }

#endif


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


