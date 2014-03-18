// -*- C++ -*-
/*!
 * @file  RobotMotion.cpp
 * @brief ModuleDescription
 * @date $Date$
 *
 * $Id$
 */

#include <cnoid/Plugin>
#include <cnoid/ItemTreeView>
#include <cnoid/BodyItem>
#include <cnoid/ToolBar>
#include <cnoid/MessageView>
#include <cnoid/BodyMotion>
#include <boost/bind.hpp>

#include "RobotMotion.h"

using namespace boost;
using namespace cnoid;

// Module specification
// <rtc-template block="module_spec">
static const char* robotsim_spec[] =
  {
    "implementation_id", "RobotMotion",
    "type_name",         "RobotMotion",
    "description",       "ModuleDescription",
    "version",           "1.0.0",
    "vendor",            "VenderName",
    "category",          "Plugin",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.project_dir","share\\motions\\",
    // Widget
    "conf.__widget__.project_dir", "text",
    // Constraints
    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
RobotMotion::RobotMotion(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_commandIn("command", m_command),
	m_targetPoseIn("targetPose", m_targetPose),
	m_skeletonIn("skeleton", m_skeleton)

    // </rtc-template>
{
	mv = MessageView::mainInstance();
}

/*!
 * @brief destructor
 */
RobotMotion::~RobotMotion()
{
	if(m_player){
	  m_player->stop();
	  delete m_player;
	}
}



RTC::ReturnCode_t RobotMotion::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("command", m_commandIn);
  addInPort("targetPose", m_targetPoseIn);
  addInPort("skeleton", m_skeletonIn);
  
  // Set OutPort buffer
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("project_dir", m_project_dir, "share\\motions\\");
  
  // </rtc-template>

  m_player =NULL;
  kinect_ctrl = 1;

  return RTC::RTC_OK;
}


RTC::ReturnCode_t RobotMotion::onFinalize()
{
  if(m_player){
	  m_player->stop();
  }
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t RobotMotion::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RobotMotion::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t RobotMotion::onActivated(RTC::UniqueId ec_id)
{
	if (m_player->bodyItem == NULL){
	  mv->cout() << "no body selected." << std::endl;
	  return RTC::RTC_ERROR;
	}
	mv->cout() << "Component activated...." << std::endl;

	spine_y = 0;
	spine_z = 0;
	return RTC::RTC_OK;
}


RTC::ReturnCode_t RobotMotion::onDeactivated(RTC::UniqueId ec_id)
{
  //m_player->stop();
  return RTC::RTC_OK;
}

/*
  for G-ROBOT GR001
*/
#define HEAD	0
#define SHOULDER_LEFT 3
#define ELBOW_LEFT 6
#define WRIST_LEFT	9
#define SHOULDER_RIGHT	12
#define ELBOW_RIGHT 15
#define WRIST_RIGHT	18
#define SPINE		21
#define HAND_LEFT	24
#define HAND_RIGHT	27


#define HIP_R_RIGHT 0
#define HIP_R_LEFT  6

#define ELBOW_LEFT_JOINT 19
#define ELBOW_RIGHT_JOINT 16
#define SHOULDER_R_LEFT_JOINT 18
#define SHOULDER_R_RIGHT_JOINT 15
#define SHOULDER_P_LEFT_JOINT 17
#define SHOULDER_P_RIGHT_JOINT 14
#define CHEST_JOINT 12

#define ROT_JOINT 20


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
	  ans = 3.141592 - calcAngle(val, SHOULDER_LEFT, ELBOW_LEFT, WRIST_LEFT);
	  break;

	case ELBOW_RIGHT_JOINT:
	  ans = calcAngle(val, SHOULDER_RIGHT, ELBOW_RIGHT, WRIST_RIGHT) - 3.141592 ;
	  break;

	case SHOULDER_R_RIGHT_JOINT:
		ans = 1.570796 - calcAngle(val, SHOULDER_LEFT, SHOULDER_RIGHT, ELBOW_RIGHT) ;
	    break;

	case SHOULDER_R_LEFT_JOINT:
		ans = calcAngle(val, SHOULDER_RIGHT, SHOULDER_LEFT, ELBOW_LEFT) - 1.570796;
	    break;

	case SHOULDER_P_LEFT_JOINT:
		{
		  ans =  calcAngle2(val, SHOULDER_LEFT,SHOULDER_RIGHT, ELBOW_LEFT, SPINE);
		  if( val.data[ELBOW_LEFT+2] > val.data[SHOULDER_LEFT+2] ){
			  if(ans < 1.57){
			    ans = -ans;
			  }
		  }
		}
		break;

	case SHOULDER_P_RIGHT_JOINT:
		{
		  ans = -calcAngle2(val, SHOULDER_RIGHT ,SHOULDER_LEFT, ELBOW_RIGHT, SPINE);
		  if( val.data[ELBOW_RIGHT + 2] > val.data[SHOULDER_RIGHT+2] ){
			  if(ans > -1.57){
			    ans = -ans;
			  }
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


void 
setHeightOfRobot(double height, MotionPlayer *m_player)
{
  if(height < -0.03){
    m_player->setJointAngle(2, 0);
	m_player->setJointAngle(3, 0);
	m_player->setJointAngle(4, 0);

	m_player->setJointAngle(8, 0);
	m_player->setJointAngle(9, 0);
	m_player->setJointAngle(10, 0);

  }else if(height < 0.0){
	m_player->setJointAngle(2, 0.3488 + 3.488* 3 *height );
	m_player->setJointAngle(3, -0.698 -6.98 * 3* height);
	m_player->setJointAngle(4, -0.3488 - 3.488 * 3* height);
			  
	m_player->setJointAngle(8, -0.3488 -3.488 *3 * height);
	m_player->setJointAngle(9, 0.698 + 6.98 * 3 * height);
	m_player->setJointAngle(10, 0.3488 +  3.488 * 3* height);

  }else if(height < 0.01){
	m_player->setJointAngle(2, 0.3488);
	m_player->setJointAngle(3, -0.698);
	m_player->setJointAngle(4, -0.3488);

	m_player->setJointAngle(8, -0.3488);
	m_player->setJointAngle(9, 0.698);
	m_player->setJointAngle(10, 0.3488);

  }else if(height < 0.15){
	m_player->setJointAngle(2, 3.488* height + 0.3488 );
	m_player->setJointAngle(3, -6.98 * height -0.698);
	m_player->setJointAngle(4, -3.488 * height- 0.3488);

	m_player->setJointAngle(8, -3.488 * height- 0.3488);
	m_player->setJointAngle(9,  6.98 * height +0.698);
	m_player->setJointAngle(10, 3.488* height + 0.3488);
  }
  return ;
}

static int legOnly[20] = {0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1};


RTC::ReturnCode_t RobotMotion::onExecute(RTC::UniqueId ec_id)
{
	if(m_commandIn.isNew()){
		m_commandIn.read();

		if(m_player->isActive()){
			mv->cout() << "Robot is moving" << std::endl;
			return RTC::RTC_OK;
		}
		if(strlen(m_command.data) == 0){
			mv->cout() << "No Commend..." << std::endl ;
		}else if(strcmp(m_command.data, "None") == 0){
			mv->cout() << "None" << std::endl;
		}else if(strcmp(m_command.data, "KINECT") == 0){
			if(kinect_ctrl == 1){
			  kinect_ctrl = 0;
			}else{
			  kinect_ctrl = 1;
			}
		}else{
		  mv->cout() << "Motion = "<< m_command.data <<  ",    " ;
		  m_player->clearMask();
		  int res = m_player->doAction(m_project_dir, (std::string)m_command.data);
		  if (res == 1){
			  mv->cout() << "Start Motion."<< std::endl;
		  }else if (res == 0){
			  mv->cout() << "Fail to start Motion."<<  std::endl;
		  }else{
			  mv->cout() << "Fail to load Motion."<<  std::endl;
		  }
		}
	}

	if(m_targetPoseIn.isNew()){
		m_targetPoseIn.read();
		if(m_player->isActive()){
			mv->cout() << "Robot is moving 1" << std::endl;
			return RTC::RTC_OK;
		}
		 m_player->setPose(m_targetPose, m_jointAngleMask, 1);
	}

	if(m_skeletonIn.isNew()){
		while(m_skeletonIn.isNew()) m_skeletonIn.read();


		  double chest_angle = calcJointAngle(m_skeleton, CHEST_JOINT);
		 // m_player->setJointAngle(CHEST_JOINT, chest_angle);

		  if(chest_angle < -0.4){
			spine_y = m_skeleton.data[SPINE+1];
			spine_z = m_skeleton.data[SPINE+2];
			mv->cout() << "Position Reset: " << spine_y << ", " << spine_z << std::endl;
			if(m_player->isActive()){
			   m_player->stop();
			   return RTC::RTC_OK;
			}

	      }
		  
		if(m_player->isActive()){
			if(m_player->isMaskedMotion() == 0){
			  mv->cout() << "Robot is moving 2" << std::endl;
			  return RTC::RTC_OK;
			}
		}

		if(m_skeleton.data[SPINE+2] < 1.1) { 
			kinect_ctrl=0;
			spine_y=0;
			return RTC::RTC_OK; 
		}

		if(m_skeleton.data[SPINE+2] > 1.3) {
			if(kinect_ctrl == 0){
				spine_y = m_skeleton.data[SPINE+1];
				spine_z = m_skeleton.data[SPINE+2];
			}
			kinect_ctrl = 1;
		}

		if(kinect_ctrl == 1){
		  if(spine_y == 0){
			  spine_y = m_skeleton.data[SPINE+1]; 
		      spine_z = m_skeleton.data[SPINE+2];
		  }


		  m_player->setJointAngle(CHEST_JOINT, chest_angle);

		  double dist = spine_z - m_skeleton.data[SPINE+2];
		  if(dist > 0.1){
			 m_player->setMask(legOnly, 20);
			 m_player->doAction(m_project_dir, "f1");
			// return RTC::RTC_OK;
		  }else if(dist < -0.1){
			 m_player->setMask(legOnly, 20);
			 m_player->doAction(m_project_dir, "b1");
			// return RTC::RTC_OK;
		  }

	

	  	  double rot = calcJointAngle(m_skeleton, ROT_JOINT);

		  //if(m_player->maskMotion[HIP_R_LEFT] == 1){
		    m_player->setJointAngle(HIP_R_LEFT, rot);
		  //}
		  //if(m_player->maskMotion[HIP_R_RIGHT] == 1){
		    m_player->setJointAngle(HIP_R_RIGHT, rot);
		  //}


		  m_player->setJointAngle(ELBOW_LEFT_JOINT, calcJointAngle(m_skeleton, ELBOW_LEFT_JOINT));
		  m_player->setJointAngle(SHOULDER_R_LEFT_JOINT, calcJointAngle(m_skeleton, SHOULDER_R_LEFT_JOINT));
          m_player->setJointAngle(SHOULDER_P_LEFT_JOINT, calcJointAngle(m_skeleton, SHOULDER_P_LEFT_JOINT));

		  m_player->setJointAngle(ELBOW_RIGHT_JOINT, calcJointAngle(m_skeleton, ELBOW_RIGHT_JOINT));
		  m_player->setJointAngle(SHOULDER_R_RIGHT_JOINT, calcJointAngle(m_skeleton, SHOULDER_R_RIGHT_JOINT));
		  m_player->setJointAngle(SHOULDER_P_RIGHT_JOINT, calcJointAngle(m_skeleton, SHOULDER_P_RIGHT_JOINT));


		  setHeightOfRobot(spine_y - m_skeleton.data[SPINE+1], m_player);
		 
		  m_player->updateJoints();

		}

	}
  return RTC::RTC_OK;
}



/*
RTC::ReturnCode_t RobotMotion::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RobotMotion::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RobotMotion::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RobotMotion::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RobotMotion::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/* private member functions */
void  RobotMotion::setMotionPlayer(MotionPlayer *mp)
{
	m_player = mp;
	return;
}

MotionPlayer *  RobotMotion::getMotionPlayer()
{
	return m_player;
}


extern "C"
{
 
  void RobotMotionInit(RTC::Manager* manager)
  {
    coil::Properties profile(robotsim_spec);
    manager->registerFactory(profile,
                             RTC::Create<RobotMotion>,
                             RTC::Delete<RobotMotion>);
  }
  
};


