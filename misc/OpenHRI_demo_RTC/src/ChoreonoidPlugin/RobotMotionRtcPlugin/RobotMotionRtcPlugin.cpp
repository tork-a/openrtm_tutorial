/*

  Simple RTC Plugin

  Copyright(C) Isao Hara

*/
#include "RobotMotionRtcItem.h"
#include <cnoid/App>
#include <cnoid/Config>
#include <cnoid/Plugin>
#include <cnoid/ItemManager>
#include <boost/format.hpp>

namespace cnoid {

    class RobotMotionRtcPlugin : public Plugin
    {
    public:
        
        RobotMotionRtcPlugin() : Plugin("RobotMotionRtc") {
            require("Body");
			require("RtcManager");
        }

        virtual bool initialize() {
            itemManager().registerClass<RobotMotionRtcItem>("RobotMotionRtcItem");
            itemManager().addCreationPanel<RobotMotionRtcItem>();
            return true;
        }

        virtual const char* description() {
            static std::string text =
                str(boost::format("RobotMotionRtc Plugin Version %1%\n") % CNOID_FULL_VERSION_STRING) +
                "\n" +
                "This plugin has been developed by Choreonoid Development Team, AIST, "
                  "\n" +
                LGPLtext();
            return text.c_str();
        }

	public:

    };
}


CNOID_IMPLEMENT_PLUGIN_ENTRY(RobotMotionRtcPlugin)
