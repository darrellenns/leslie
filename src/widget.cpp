#ifdef ENABLE_GUI
#include "widget.h"
#include <GL/freeglut.h>
#include <FTGL/ftgl.h>
#include <string>
#include <sstream>

//blues
ColorArray Widget::color_a=
{
    {0x00,0x99,0x99},
    {0x1D,0x73,0x73},
    {0x00,0x63,0x63},
    {0x33,0xCC,0xCC},
    {0x5C,0xCC,0xCC},
};

//oranges
ColorArray Widget::color_b=
{
    {0xFF,0x74,0x00},
    {0xBF,0x71,0x30},
    {0xA6,0x4B,0x00},
    {0xFF,0x96,0x40},
    {0xFF,0xB2,0x73},
};

const int Widget::fontsize=12;
const char* Widget::fontface="/usr/share/fonts/TTF/DejaVuSans.ttf";

FTGLPixmapFont Widget::font(fontface);

Widget::Widget()
{
    _updated=true;
    x=0;
    y=0;
    width=1;
    height=1;
    vert[0][0]=0;
    vert[0][1]=0;
    vert[1][0]=0;
    vert[1][1]=0;
    vert[2][0]=0;
    vert[2][1]=0;
    vert[3][0]=0;
    vert[3][1]=0;
    if(font.Error()) throw;
    font.FaceSize(fontsize);
}
Widget::~Widget()
{

}

void Widget::modelCoordtoScreen(float x,float y,double coord[3])
{
    GLdouble modelview_matrix[16];
    GLdouble projection_matrix[16];
    GLint viewport[4];

    glGetDoublev(GL_MODELVIEW_MATRIX,modelview_matrix);
    glGetDoublev(GL_PROJECTION_MATRIX,projection_matrix);
    glGetIntegerv(GL_VIEWPORT,viewport);

    gluProject(x,y,0,
    modelview_matrix,
    projection_matrix,
    viewport,
    &coord[0],&coord[1],&coord[2]);
}

double Valuator::value(double value) {
    value=fmax(fmin(value,max),min);

    _value=value;

    std::ostringstream ostr;
    ostr.precision(3);
    ostr<<std::showpoint<<std::fixed<<_value;
    value_string=ostr.str();

    _updated=true;

    return _value;
}

bool Widget::pick(float x,float y)
{
    relCoord(&x,&y);
    if(x<0 || y<0) return false;
    if(x>width || y>height) return false;
    return true;
}

void Widget::draw()
{
    glColor3f(1.0,0.0,0.0);

    glBegin(GL_QUADS);
    glVertex2f(vert[0][0],vert[0][1]);
    glVertex2f(vert[1][0],vert[1][1]);
    glVertex2f(vert[2][0],vert[2][1]);
    glVertex2f(vert[3][0],vert[3][1]);
    glEnd();

    double coord[3];

    modelCoordtoScreen(vert[0][0],vert[0][1],coord);
    FTBBox bbox=font.BBox(caption.c_str());
    FTPoint p(coord[0],coord[1]-bbox.Upper().Y());
    font.Render(caption.c_str(),-1,p);

}

void Widget::setCoords(float x,float y,float width,float height)
{
    this->x=x;
    this->y=y;
    this->width=width;
    this->height=height;

    vert[0][0]=this->x;
    vert[0][1]=this->y;

    vert[1][0]=this->x+this->width;
    vert[1][1]=this->y;

    vert[2][0]=this->x+this->width;
    vert[2][1]=this->y+this->height;

    vert[3][0]=this->x;
    vert[3][1]=this->y+this->height;
}

double VSlider::value(double value)
{
    Valuator::value(value);
    slider_vert[0][1]=slider_vert[1][1]=y+height*((_value-min)/(max-min));
}

VSlider::VSlider():Valuator()
{
    slider_vert[0][0]=0;
    slider_vert[0][1]=0;
    slider_vert[1][0]=0;
    slider_vert[1][1]=0;
}

void VSlider::draw()
{

    glBegin(GL_QUADS);
    glColor3ubv(color_a[0]);
    glVertex2f(vert[0][0],vert[0][1]);
    glVertex2f(vert[1][0],vert[1][1]);
    glColor3ubv(color_a[2]);
    glVertex2f(vert[2][0],vert[2][1]);
    glVertex2f(vert[3][0],vert[3][1]);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ubv(color_b[2]);
    glVertex2f(slider_vert[0][0],slider_vert[0][1]);
    glVertex2f(slider_vert[1][0],slider_vert[1][1]);
    glColor3ubv(color_b[0]);
    glVertex2f(vert[1][0],vert[1][1]);
    glVertex2f(vert[0][0],vert[0][1]);
    glEnd();

    double coord[3];

    {
        modelCoordtoScreen(slider_vert[0][0],slider_vert[0][1],coord);
        FTPoint p(coord[0],coord[1]);
        font.Render(value_string.c_str(),-1,p);
    }

    {
        modelCoordtoScreen(vert[0][0],vert[0][1],coord);
        FTBBox bbox=font.BBox(caption.c_str());
        FTPoint p(coord[0],coord[1]-bbox.Upper().Y());
        font.Render(caption.c_str(),-1,p);
    }

}

void VSlider::setCoords(float x,float y,float width,float height)
{
    Widget::setCoords(x,y,width,height);
    slider_vert[0][0]=x;
    slider_vert[1][0]=x+width;
    slider_vert[1][1]=slider_vert[0][1]=y+height*_value;
}

void VSlider::click(float x,float y)
{
    relCoord(&x,&y);
    float v=y/height;
    if(v<0.0) v=0.0;
    else if(v>1.0) v=1.0;
    v=v*(max-min)+min;
    if(v==_value) return;
    value(v);
    Widget::click(x,y);
}
void VSlider::drag(float x,float y)
{
    relCoord(&x,&y);
    float v=y/height;
    if(v<0.0) v=0.0;
    else if(v>1.0) v=1.0;
    v=v*(max-min)+min;
    if(v==_value) return;
    value(v);
    Widget::drag(x,y);
}

Valuator::Valuator():Widget()
{
    min=0;
    max=1;
    value(0);
}

Switch::Switch():Widget()
{
    value(false);
}

void Switch::draw()
{
    ColorArray* color;
    if(_value) color=&color_b;
    else color=&color_a;

    glBegin(GL_QUADS);
    glColor3ubv((*color)[3]);
    glVertex2f(vert[0][0],vert[0][1]);
    glColor3ubv((*color)[1]);
    glVertex2f(vert[1][0],vert[1][1]);
    glColor3ubv((*color)[2]);
    glVertex2f(vert[2][0],vert[2][1]);
    glColor3ubv((*color)[0]);
    glVertex2f(vert[3][0],vert[3][1]);
    glEnd();

    double coord[3];

    modelCoordtoScreen(vert[0][0],vert[0][1],coord);
    FTBBox bbox=font.BBox(caption.c_str());
    FTPoint p(coord[0],coord[1]-bbox.Upper().Y());
    font.Render(caption.c_str(),-1,p);

}

void Toggle::click(float x,float y)
{
    value(!value());
    Widget::click(x,y);
}
#endif // ENABLE_GUI
