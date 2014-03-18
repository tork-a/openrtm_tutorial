/*
  
  Wrapper class for RTC::Manager

  Copyright(C) Isao Hara

*/

#include "RtcManager.h"

namespace cnoid {

  RtcManager::RtcManager() 
  {
    mv = cnoid::MessageView::mainInstance();
  }

  RtcManager *RtcManager::instance()
  {
    static RtcManager *instance = new RtcManager();
    return instance;
  }


  bool RtcManager::initialize() 
  {
    manager=NULL;
    init_rtc_manager();
    return true;
  }

  void RtcManager::init_rtc_manager()
  {
    if(manager == NULL){
      manager = RTC::Manager::init(0, NULL);
      manager->activateManager();
      manager->runManager(true);
      mv->cout() << "RTC::Manager has initialized." << std::endl;
    }
    return;
  }

  void RtcManager::registerModuleInitProc(const char *name, RTC::ModuleInitProc proc)
  {
	  std::vector<std::string> f = manager->getModulesFactories();
	  int size = f.size();
	  for(int i=0;i < size;i++){
		if(f[i] == name){
			mv->cout() << name << " is already registered" << std::endl;
			return;
		}
	  }
	  proc(manager);
  }

  RTC::RtcBase *RtcManager::create_component(const char *name, RTC::ModuleInitProc proc)
  {
	registerModuleInitProc(name, proc);
    RTC::RtcBase *cmp = manager->createComponent(name);
    mv->cout() << "Create RTC..." << std::endl;
    return cmp;
  }

  RTC::RtcBase *RtcManager::create_component(const char *name)
  {
    RTC::RtcBase *cmp = manager->createComponent(name);
    mv->cout() << "Create RTC..." << std::endl;
    return cmp;
  }

  void RtcManager::delete_component(char *comp_name)
  {
	 RTC::RTObject_impl *rtc = manager->getComponent(comp_name);
	 delete_component(rtc);
	 return;
  }

  void RtcManager::delete_component(RTC::RtcBase *comp)
  {
    if (comp != NULL){
	  RTC::RTObject_impl *rtc = (RTC::RTObject_impl *)comp;

	  try{
		 rtc->exit();
		 Sleep(1000);
	  }catch(void *e){
		  ;
	  }
      manager->deleteComponent(comp);
    }
	return;
  }
}