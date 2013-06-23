#include "osc.h"
#include "main.h"
#include "leslie.h"

#ifdef ENABLE_GUI
#include "gui.h"
#endif

#include <lo/lo.h>
#include <stdio.h>
#include <iostream>
using namespace std;
OSC::OSC()
{
    thread = lo_server_thread_new_multicast("224.0.0.1","7770", osc_error);
    lo_server_thread_add_method(thread, "/leslie/rate", "f", rate_cb, NULL);
    lo_server_thread_add_method(thread, "/leslie/amp", "f", amp_cb, NULL);
    lo_server_thread_add_method(thread, "/leslie/mod", "f", mod_cb, NULL);
    lo_server_thread_add_method(thread, "/leslie/phase", "f", phase_cb, NULL);
    lo_server_thread_add_method(thread, "/leslie/enable", "i", enable_cb, NULL);
    lo_server_thread_add_method(thread, "/leslie/enable/toggle", NULL, enable_toggle_cb, NULL);
    lo_server_thread_start(thread);
}

int OSC::rate_cb(const char* path,const char* types,lo_arg** argv,int argc,lo_message msg,void* user_data)
{
	#ifdef ENABLE_GUI
    Valuator* v=(Valuator*)GUI::widget[0];
    v->value(argv[0]->f);
	#endif
    leslie->setRate(argv[0]->f);
}
int OSC::amp_cb(const char* path,const char* types,lo_arg** argv,int argc,lo_message msg,void* user_data)
{
	#ifdef ENABLE_GUI
    Valuator* v=(Valuator*)GUI::widget[1];
    v->value(argv[0]->f);
	#endif
    leslie->setAmp(argv[0]->f);
}
int OSC::mod_cb(const char* path,const char* types,lo_arg** argv,int argc,lo_message msg,void* user_data)
{
	#ifdef ENABLE_GUI
    Valuator* v=(Valuator*)GUI::widget[2];
    v->value(argv[0]->f);
	#endif
    leslie->setMod(argv[0]->f);
}
int OSC::phase_cb(const char* path,const char* types,lo_arg** argv,int argc,lo_message msg,void* user_data)
{
	#ifdef ENABLE_GUI
    Valuator* v=(Valuator*)GUI::widget[3];
    v->value(argv[0]->f);
	#endif
    leslie->setPhase(argv[0]->f);
}
int OSC::enable_cb(const char* path,const char* types,lo_arg** argv,int argc,lo_message msg,void* user_data)
{
    leslie->setEnable(argv[0]->i);
	#ifdef ENABLE_GUI
    ((Switch*)GUI::widget[4])->value(argv[0]->i);
	#endif
}
int OSC::enable_toggle_cb(const char* path,const char* types,lo_arg** argv,int argc,lo_message msg,void* user_data)
{
    leslie->setEnable(!leslie->getEnable());
	#ifdef ENABLE_GUI
    ((Switch*)GUI::widget[4])->value(leslie->getEnable());
	#endif
}
int OSC::message(const char* path,const char* types,lo_arg** argv,int argc,lo_message msg)
{
    cout<<path<<endl;
    for (int i=0; i<argc; i++) {
        cout<<"arg "<<i<<" '"<<types[i]<<"' ";
        lo_arg_pp((lo_type)types[i], argv[i]);
        cout<<endl;

    }
    printf("\n");
    fflush(stdout);
    return 1;
}

void OSC::osc_error(int num, const char* msg, const char* path)
{
    cerr<<"liblo server error "<<num<<" in path "<<path<<": "<<msg<<endl<<flush;
}
