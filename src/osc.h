#ifndef OSC_H
#define OSC_H
#include <lo/lo.h>

class OSC
{
    public:

        int message(const char* path,const char* types,lo_arg** argv,int argc,lo_message msg);
        static OSC& getInstance()
        {
            static OSC instance;
            return instance;
        }

    private:
        OSC();//singleton class - constructor is private
        // Stop the compiler generating methods of copy the object
        OSC(OSC const& copy);            // Not Implemented
        OSC& operator=(OSC const& copy); // Not Implemented

        lo_server_thread thread;
        static int rate_cb(const char* path,const char* types,lo_arg** argv,int argc,lo_message msg,void* user_data);
        static int amp_cb(const char* path,const char* types,lo_arg** argv,int argc,lo_message msg,void* user_data);
        static int mod_cb(const char* path,const char* types,lo_arg** argv,int argc,lo_message msg,void* user_data);
        static int phase_cb(const char* path,const char* types,lo_arg** argv,int argc,lo_message msg,void* user_data);
        static int enable_cb(const char* path,const char* types,lo_arg** argv,int argc,lo_message msg,void* user_data);
        static int enable_toggle_cb(const char* path,const char* types,lo_arg** argv,int argc,lo_message msg,void* user_data);
        static void osc_error(int num, const char* msg, const char* path);

};

#endif // OSC_H
