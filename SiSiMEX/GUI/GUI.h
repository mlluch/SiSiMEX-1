// generated by Fast Light User Interface Designer (fluid) version 1.0304

#ifndef GUI_h
#define GUI_h
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
extern Fl_Double_Window *guiWindowApplicationMode;
#include <FL/Fl_Group.H>
#include <FL/Fl_Round_Button.H>
extern Fl_Round_Button *radioButtonYellowPages;
extern Fl_Round_Button *radioButtonAgents;
#include <FL/Fl_Button.H>
Fl_Double_Window* make_window_select_mode();
extern Fl_Double_Window *guiWindowMultiagentApplication;
#include <FL/Fl_Text_Display.H>
extern Fl_Text_Display *guiTextDisplayAgentsLog;
#include <FL/Fl_Spinner.H>
extern Fl_Spinner *guiSpinnerNodeID;
extern Fl_Spinner *guiSpinnerMCPItemID;
extern Fl_Spinner *guiSpinnerMCPNodeID;
Fl_Double_Window* make_window_multiagents_application();
extern Fl_Double_Window *guiWindowYellowPagesApplication;
extern Fl_Text_Display *guiTextDisplayYPLog;
Fl_Double_Window* make_window_yellowpages_application();
#endif
