/*

  RTCManager Plugin

  Copyright(C) Isao Hara

*/

#include <cnoid/App>
#include <cnoid/Config>
#include <cnoid/Plugin>
#include <cnoid/ItemManager>
#include <boost/format.hpp>

#include "RtcManager.h"

using namespace cnoid;

namespace {

    class RtcManagerPlugin : public Plugin
    {
    public:
        
        RtcManagerPlugin() : Plugin("RtcManager") {
        }

        virtual bool initialize() {
            mgr = RtcManager::instance();
            mgr->initialize();
            return true;
        }

        virtual const char* description() {
            static std::string text =
                str(boost::format("RtcManager Plugin Version %1%\n") % CNOID_FULL_VERSION_STRING) +
                "\n" +
                "This plugin has been developed by Choreonoid Development Team, AIST, " ;

            return text.c_str();
        }

	public:
            RtcManager *mgr;
    };
}


CNOID_IMPLEMENT_PLUGIN_ENTRY(RtcManagerPlugin)