/*

KinectSkeleton.cpp

Copyright (c) 2012, Isao Hara, ISRI, AIST, Japan
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

  1.Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
  2.Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.



*/

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

double
calcJointAngle(RTC::TimedFloatSeq val, int idx)
{
  double ans;

  switch(idx){
    case  ELBOW_LEFT_JOINT:
      ans = 3.141592 - calcAngle(val, SHOULDER_LEFT, ELBOW_LEFT, HAND_LEFT);
      break;

    case ELBOW_RIGHT_JOINT:
      ans = calcAngle(val, SHOULDER_RIGHT, ELBOW_RIGHT, HAND_RIGHT) - 3.141592 ;
      break;

    case SHOULDER_R_RIGHT_JOINT:
      {	
        double v[3];
        v[0] = val.data[ ELBOW_RIGHT ]- val.data[ SHOULDER_RIGHT ];
        v[1] = val.data[ ELBOW_RIGHT+1 ]- val.data[ SHOULDER_RIGHT+1 ];
        v[2] = val.data[ ELBOW_RIGHT+2 ]- val.data[ SHOULDER_RIGHT+2 ];
        double x = v[0] / calcLength3d(v);
        ans = -asin(x) ;
      }
      break;

    case SHOULDER_R_LEFT_JOINT:
      {
        double v[3];
        v[0] = val.data[ ELBOW_LEFT ]- val.data[ SHOULDER_LEFT ];
        v[1] = val.data[ ELBOW_LEFT+1 ]- val.data[ SHOULDER_LEFT+1 ];
        v[2] = val.data[ ELBOW_LEFT+2 ]- val.data[ SHOULDER_LEFT+2 ];
        double x = v[0] / calcLength3d(v);
        ans = asin(-x);
      }
      break;

    case SHOULDER_P_LEFT_JOINT:
      {
        double v[3];
        v[0] = val.data[ ELBOW_LEFT ]- val.data[ SHOULDER_LEFT ];
        v[1] = val.data[ ELBOW_LEFT+1 ]- val.data[ SHOULDER_LEFT+1 ];
        v[2] = val.data[ ELBOW_LEFT+2 ]- val.data[ SHOULDER_LEFT+2 ];
        double l = calcLength3d(v);
        double y = v[1] /l;
        double z = v[2] /l;
        ans = atan2(z, y)+3.14;
      }
      break;

    case SHOULDER_P_RIGHT_JOINT:
      {
        double v[3];
        v[0] = val.data[ ELBOW_RIGHT ]- val.data[ SHOULDER_RIGHT ];
        v[1] = val.data[ ELBOW_RIGHT+1 ]- val.data[ SHOULDER_RIGHT+1 ];
        v[2] = val.data[ ELBOW_RIGHT+2 ]- val.data[ SHOULDER_RIGHT+2 ];
        double l = calcLength3d(v);
        double y = v[1] /l;
        double z = v[2] /l ;
        ans = -atan2(z, y)+3.14;
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


