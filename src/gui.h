#ifdef	ENABLE_GUI
#ifndef GUI_H
#define GUI_H

#define GL_GLEXT_PROTOTYPES
#include <GL/freeglut.h>
#include "widget.h"
#include <vector>

using namespace std;

namespace GUI
{
        void init(int argc,char** argv);
        void run();
        extern vector<Widget*> widget;

        static void evtKeypress(unsigned char key, int x, int y);
        static void evtTimer(int value);
        static void evtRender(void);
        static void evtResize(int w, int h);
        static void evtMouse(int button, int state,int x, int y);
        static void evtMotion(int x, int y);
        static void evtIdle(void);
        static double windowXmin, windowXmax, windowYmin, windowYmax;

        static Widget* dragwidget;
        static void screenCoordtoModel(int screenx,int screeny,GLdouble coord[3]);
        static bool updated;
};

//extern GUI* gui;

#endif // GUI_H
#endif // ENABLE_GUI
