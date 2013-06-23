#ifndef main_h
#define main_h

#include "leslie.h"
extern Leslie* leslie;

#ifdef ENABLE_GUI
#include "widget.h"
void GUIRateChange(Widget* widget);
void GUIAmpChange(Widget* widget);
void GUIModChange(Widget* widget);
void GUIPhaseChange(Widget* widget);
void GUIEnableClick(Widget* widget);
#endif

#endif
