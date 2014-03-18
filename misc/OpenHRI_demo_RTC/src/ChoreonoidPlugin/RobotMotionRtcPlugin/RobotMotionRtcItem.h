
/**
   @file
   @author Isao Hara
*/

#ifndef CNOID_ROBOT_RTC_ITEM_H_INCLUDED
#define CNOID_ROBOT_RTC_ITEM_H_INCLUDED

#include <cnoid/Item>
#include <cnoid/ConnectionSet>

#include "RobotMotion.h"
#include <RtcManager.h>

namespace cnoid {

    class MessageView;
    class BodyItem;

    class RobotMotionRtcItem : public Item
    {
      public:
        RobotMotionRtcItem();
        RobotMotionRtcItem(const RobotMotionRtcItem& org);
        ~RobotMotionRtcItem();
            
      protected:
        virtual ItemPtr doDuplicate() const;
//        virtual void doPutProperties(PutPropertyFunction& putProperty);
        virtual bool store(Archive& archive);
        virtual bool restore(const Archive& archive);
        virtual void onConnectedToRoot();
        virtual void onDisconnectedFromRoot();

      private:
        RobotMotion* rtc;
        BodyItem* bodyItem;
        RtcManager *manager;
        char *rtc_name;

        MessageView* mv;
    };

    typedef boost::intrusive_ptr<RobotMotionRtcItem> RobotMotionRtcItemPtr;
}

#endif
