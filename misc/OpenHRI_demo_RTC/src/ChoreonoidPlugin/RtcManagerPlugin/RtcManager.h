/*

  Wrapper class for RTC::Manager

  Copyright(C) Isao Hara

*/


#ifndef CNOID_RTC_MANAGER_H_INCLUDE
#define CNOID_RTC_MANAGER_H_INCLUDE


#include <cnoid/Plugin>
#include <cnoid/ItemTreeView>
#include <cnoid/MessageView>
#include <boost/bind.hpp>

#include <rtm/Manager.h>
#include <rtm/RTObject.h>

namespace cnoid {
  class CNOID_EXPORT RtcManager
  {
  public:
    RtcManager();
    ~RtcManager();

    static RtcManager *instance();

    bool initialize();
    void init_rtc_manager();
    void registerModuleInitProc(const char *name, RTC::ModuleInitProc proc);
    RTC::RtcBase *create_component(const char *name, RTC::ModuleInitProc proc);
    RTC::RtcBase *create_component(const char *name);
	void delete_component(char *rtc_name);
    void delete_component(RTC::RtcBase *rtc);

  public:
    RTC::Manager* manager;
    cnoid::MessageView *mv;
  };

}

#endif
