/*

  Simple RT Component Item to play a motion pattern.
  Copyright(C) Isao Hara

*/

#include "RobotMotionRtcItem.h"
#include <cnoid/ItemTreeView>
#include <cnoid/MessageView>
#include <cnoid/TimeBar>
#include <cnoid/Archive>
#include <cnoid/BodyItem>
#include <cnoid/BodyMotionItem>
#include <boost/bind.hpp>

using namespace std;
using namespace boost;
using namespace cnoid;


RobotMotionRtcItem::RobotMotionRtcItem()
    : mv(MessageView::mainInstance())

{
	rtc=NULL;
	manager = RtcManager::instance();
	mv->cout() << "Create RobotMotionRtcItem..." << std::endl;
}


RobotMotionRtcItem::RobotMotionRtcItem(const RobotMotionRtcItem& org)
    : Item(org),
      mv(MessageView::mainInstance())
{
	manager = RtcManager::instance();
	mv->cout() << "Copy RobotMotionRtcItem..." << std::endl;
}


RobotMotionRtcItem::~RobotMotionRtcItem()
{

}

void RobotMotionRtcItem::onConnectedToRoot()
{
	rtc = (RobotMotion *)manager->create_component("RobotMotion",RobotMotionInit);
	rtc_name = (char *)rtc->getInstanceName();

	MotionPlayer *player = new MotionPlayer();
	rtc->setMotionPlayer(player);

	mv->cout() << "Connect Component...";
    player->setBody(findOwnerItem<BodyItem>());
	mv->cout() << "done" << std::endl;
	
	return;
}

void RobotMotionRtcItem::onDisconnectedFromRoot()
{
	
	mv->cout() << "Delete Component...";
	manager->delete_component(rtc_name);
	mv->cout() << "done" << std::endl;
	
	return;
}

ItemPtr RobotMotionRtcItem::doDuplicate() const
{
	mv->cout() << "doDuplicate RobotMotionRtcItem..." << std::endl;
	return new RobotMotionRtcItem(*this);
}

bool RobotMotionRtcItem::store(Archive& archive)
{
   
    return true;
}


bool RobotMotionRtcItem::restore(const Archive& archive)
{
   
    return true;
}
