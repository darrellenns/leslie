#ifdef ENABLE_GUI
#include "gui.h"
#include "widget.h"
#include <iostream>

using namespace std;

const double GUI_WIDTH=1.6;
const double GUI_HEIGHT=1.0;
const double GUI_PPU=200;

vector<Widget*> GUI::widget;

void GUI::run()
{
    glutMainLoop();
}

void GUI::init(int argc,char** argv)
{
    dragwidget=NULL;
    updated=true;

    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowSize(GUI_PPU*GUI_WIDTH,GUI_PPU*GUI_HEIGHT);
    glutCreateWindow("Leslie");

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( 0, GUI_WIDTH, 0, GUI_HEIGHT, -1, 1 );
    glMatrixMode(GL_MODELVIEW);

    //callback functions
    glutReshapeFunc(evtResize);
    glutKeyboardFunc(evtKeypress);
    glutTimerFunc(20,evtTimer,0);
    //glutIdleFunc(evtIdle);
    glutDisplayFunc(evtRender);
    glutMouseFunc(evtMouse);
    glutMotionFunc(evtMotion);
}

void GUI::evtKeypress(unsigned char key, int x, int y)
{
}

void GUI::evtTimer(int value)
{
    glutTimerFunc(20,evtTimer,0);
    bool r=false;

    if(GUI::updated)
    {
        GUI::updated=false;
    }else{
        for(int i=0;i<widget.size();i++)
            r=r||widget[i]->updated();
        if(!r) return;
    }
    GUI::evtRender();
}

void GUI::evtIdle(void)
{
    GUI::evtRender();
}

void GUI::evtRender(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    for(int i=0;i<widget.size();i++)
        widget[i]->draw();
    glutSwapBuffers();
}

void GUI::evtResize(int w, int h)
{
    GUI::updated=true;

    glViewport( 0, 0, w, h );

}

void GUI::screenCoordtoModel(int screenx,int screeny,GLdouble coord[3])
{
    GLdouble modelview_matrix[16];
    GLdouble projection_matrix[16];
    GLint viewport[4];

    glGetDoublev(GL_MODELVIEW_MATRIX,modelview_matrix);
    glGetDoublev(GL_PROJECTION_MATRIX,projection_matrix);
    glGetIntegerv(GL_VIEWPORT,viewport);

    gluUnProject(screenx,viewport[3]-screeny,0,
    modelview_matrix,
    projection_matrix,
    viewport,
    &coord[0],&coord[1],&coord[2]);
}

void GUI::evtMouse(int button, int state,int x, int y)
{
    if(button!=0) return;
    if(state==1)
    {
        dragwidget=NULL;
        return;
    }
    GLdouble coord[3];

    GUI::screenCoordtoModel(x,y,coord);

    for(int i=0;i<widget.size();i++)
    {
        if(widget[i]->pick(coord[0],coord[1]))
        {
            widget[i]->click(coord[0],coord[1]);
            dragwidget=widget[i];
            break;
        }
    }
}

void GUI::evtMotion(int x,int y)
{
    if(dragwidget)
    {
        GLdouble coord[3];
        GUI::screenCoordtoModel(x,y,coord);
        dragwidget->drag(coord[0],coord[1]);
    }
}
#endif
