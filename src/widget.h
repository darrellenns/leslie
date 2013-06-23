#ifdef ENABLE_GUI
#ifndef WIDGET_H
#define WIDGET_H

#include <boost/signal.hpp>
#include <FTGL/ftgl.h>

typedef uint8_t ColorArray[5][3];

class Widget
{
    public:
        Widget();
        ~Widget();
        virtual void draw();
        bool pick(float x,float y);

        virtual void click(float x,float y) {sigClick(this);};
        virtual void drag(float x,float y) {sigDrag(this);};

        boost::signal<void (Widget* widget)> sigClick;
        boost::signal<void (Widget* widget)> sigDrag;

        virtual void setCoords(float x,float y,float width,float height);

        inline void Size(float width,float height) {this->setCoords(x,y,width,height);}
        inline float Width(float width) {this->setCoords(x,y,width,height);return width;}
        inline float Height(float height) {this->setCoords(x,y,width,height);return height;}
        inline float Width() {return this->width;}
        inline float Height() {return this->height;}

        inline void setPos(float x,float y){this->setCoords(x,y,width,height);}
        inline float X(float x) {this->setCoords(x,y,width,height);return x;}
        inline float Y(float y) {this->setCoords(x,y,width,height);return y;}
        inline float X() {return this->x;}
        inline float Y() {return this->y;}

        inline void relCoord(float* x,float* y){*x-=this->x;*y-=this->y;}
        inline bool updated() {return __sync_bool_compare_and_swap(&_updated,true,false);}; //also clears updated flag on read (atomic)

        inline std::string Caption(std::string caption){_updated=true;return this->caption=caption;};
        inline std::string Caption(){_updated=true;return this->caption;};


    protected:
        void modelCoordtoScreen(float x,float y,double coord[3]);
        static uint8_t color_a[5][3];
        static uint8_t color_b[5][3];
        float width,height,x,y;
        float vert[4][2];
        std::string value_string;
        bool _updated;
        static FTGLPixmapFont font;
        static const int fontsize;
        static const char* fontface;
        std::string caption;
};

class Valuator: public Widget
{
    public:
        Valuator();
        virtual double value() {return _value;};
        virtual double value(double value);
        virtual double Max(double max) {this->max=max;if(_value>max) value(max); else value(_value); return max;};
        virtual double Min(double min) {this->min=min;if(_value<min) value(min); else value(_value); return min;};
        virtual inline double Max() {return max;};
        virtual inline double Min() {return min;};
    protected:
        double _value;
        double max;
        double min;
};

class Switch:public Widget
{
    public:
        Switch();
        ~Switch();
        virtual bool value() {return _value;};
        virtual bool value(bool value) {_updated=true; return this->_value=value;};
        virtual void draw();
    protected:
        bool _value;
};

class Toggle:public Switch
{
    public:
        virtual void click(float x,float y);
        //virtual void drag(float x,float y);

};

class VSlider: public Valuator
{
    public:
        VSlider();
        ~VSlider();
        virtual void draw();
        virtual double value(double value);
        virtual void setCoords(float x,float y,float width,float height);
        virtual void click(float x,float y);
        virtual void drag(float x,float y);

    protected:
        float slider_vert[2][2];

};

#endif // WIDGET_H
#endif // ENABLE_GUI
