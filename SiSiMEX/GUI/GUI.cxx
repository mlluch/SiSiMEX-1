// generated by Fast Light User Interface Designer (fluid) version 1.0304

#include "GUI.h"
#include "GUICallbacks.h"

Fl_Double_Window *guiWindowApplicationMode=(Fl_Double_Window *)0;

Fl_Round_Button *radioButtonYellowPages=(Fl_Round_Button *)0;

static void cb_radioButtonYellowPages(Fl_Round_Button* o, void*) {
  onguiRadioButtonApplicationModeChanged(o);
}

Fl_Round_Button *radioButtonAgents=(Fl_Round_Button *)0;

static void cb_radioButtonAgents(Fl_Round_Button* o, void*) {
  onguiRadioButtonApplicationModeChanged(o);
}

static void cb_Start(Fl_Button*, void*) {
  onguiButtonStartApplication();
}

/**
 Selects the mode of the application
 - YellowPages
 - Agents
*/
Fl_Double_Window* make_window_select_mode() {
  { guiWindowApplicationMode = new Fl_Double_Window(216, 100, "SiSiMEX");
    { Fl_Group* o = new Fl_Group(5, 20, 205, 110, "Application Mode");
      o->align(Fl_Align(65));
      { radioButtonYellowPages = new Fl_Round_Button(5, 45, 205, 25, "YellowPages");
        radioButtonYellowPages->down_box(FL_ROUND_DOWN_BOX);
        radioButtonYellowPages->callback((Fl_Callback*)cb_radioButtonYellowPages);
        radioButtonYellowPages->align(Fl_Align(68|FL_ALIGN_INSIDE));
      } // Fl_Round_Button* radioButtonYellowPages
      { radioButtonAgents = new Fl_Round_Button(5, 20, 205, 25, "Agents");
        radioButtonAgents->down_box(FL_ROUND_DOWN_BOX);
        radioButtonAgents->value(1);
        radioButtonAgents->callback((Fl_Callback*)cb_radioButtonAgents);
        radioButtonAgents->align(Fl_Align(68|FL_ALIGN_INSIDE));
      } // Fl_Round_Button* radioButtonAgents
      { Fl_Button* o = new Fl_Button(5, 70, 205, 25, "Start Application");
        o->down_box(FL_UP_FRAME);
        o->selection_color(FL_FOREGROUND_COLOR);
        o->callback((Fl_Callback*)cb_Start);
      } // Fl_Button* o
      o->end();
    } // Fl_Group* o
    { Fl_Group* o = new Fl_Group(10, 105, 205, 30);
      o->end();
    } // Fl_Group* o
    guiWindowApplicationMode->size_range(216, 100);
    guiWindowApplicationMode->end();
    guiWindowApplicationMode->resizable(guiWindowApplicationMode);
  } // Fl_Double_Window* guiWindowApplicationMode
  return guiWindowApplicationMode;
}

Fl_Double_Window *guiWindowMultiagentApplication=(Fl_Double_Window *)0;

static void cb_List(Fl_Button*, void*) {
  onGuiButtonListLocalNodes();
}

static void cb_Inspect(Fl_Button*, void*) {
  onGuiButtonInspectLocalNode();
}

static void cb_Spawn(Fl_Button*, void*) {
  onguiButtonSpawnMCP();
}

static void cb_Quit(Fl_Button*, void*) {
  onGuiButtonQuitMultiagentApplication();
}

Fl_Text_Display *guiTextDisplayAgentsLog=(Fl_Text_Display *)0;

Fl_Spinner *guiSpinnerNodeID=(Fl_Spinner *)0;

Fl_Spinner *guiSpinnerMCPItemID=(Fl_Spinner *)0;

Fl_Spinner *guiSpinnerMCPNodeID=(Fl_Spinner *)0;

static void cb_Spawn1(Fl_Button*, void*) {
  onguiButtonSpawnMCC();
}

Fl_Spinner *guiSpinnerMCCNodeID=(Fl_Spinner *)0;

Fl_Spinner *guiSpinnerMCCContributedItemID=(Fl_Spinner *)0;

Fl_Spinner *guiSpinnerMCCConstraintItemID=(Fl_Spinner *)0;

/**
 Main menu of the multi-agents application.
 It provides buttons to execute several actions.
*/
Fl_Double_Window* make_window_multiagents_application() {
  { guiWindowMultiagentApplication = new Fl_Double_Window(594, 580, "SiSiMEX Multi Agent Application");
    { Fl_Button* o = new Fl_Button(5, 5, 585, 25, "List local nodes");
      o->callback((Fl_Callback*)cb_List);
      o->align(Fl_Align(FL_ALIGN_CLIP));
    } // Fl_Button* o
    { Fl_Button* o = new Fl_Button(5, 30, 495, 25, "Inspect local node");
      o->callback((Fl_Callback*)cb_Inspect);
      o->align(Fl_Align(FL_ALIGN_CLIP));
    } // Fl_Button* o
    { Fl_Button* o = new Fl_Button(5, 55, 370, 25, "Spawn MCP");
      o->callback((Fl_Callback*)cb_Spawn);
      o->align(Fl_Align(FL_ALIGN_CLIP));
    } // Fl_Button* o
    { Fl_Button* o = new Fl_Button(5, 550, 585, 25, "Quit Multi Agent Application");
      o->callback((Fl_Callback*)cb_Quit);
      o->align(Fl_Align(FL_ALIGN_CLIP));
    } // Fl_Button* o
    { guiTextDisplayAgentsLog = new Fl_Text_Display(5, 125, 585, 420, "Application log");
      guiTextDisplayAgentsLog->align(Fl_Align(FL_ALIGN_TOP_LEFT));
      Fl_Group::current()->resizable(guiTextDisplayAgentsLog);
    } // Fl_Text_Display* guiTextDisplayAgentsLog
    { guiSpinnerNodeID = new Fl_Spinner(555, 30, 35, 25, "Node ID");
      guiSpinnerNodeID->minimum(0);
      guiSpinnerNodeID->value(0);
    } // Fl_Spinner* guiSpinnerNodeID
    { guiSpinnerMCPItemID = new Fl_Spinner(555, 55, 35, 25, "Requested");
      guiSpinnerMCPItemID->tooltip("Requested Item ID");
      guiSpinnerMCPItemID->minimum(0);
      guiSpinnerMCPItemID->value(0);
    } // Fl_Spinner* guiSpinnerMCPItemID
    { guiSpinnerMCPNodeID = new Fl_Spinner(435, 55, 35, 25, "Node ID");
      guiSpinnerMCPNodeID->tooltip("Node on which the agent will be spawn");
      guiSpinnerMCPNodeID->minimum(0);
      guiSpinnerMCPNodeID->value(0);
    } // Fl_Spinner* guiSpinnerMCPNodeID
    { Fl_Button* o = new Fl_Button(5, 80, 245, 25, "Spawn MCC");
      o->callback((Fl_Callback*)cb_Spawn1);
      o->align(Fl_Align(FL_ALIGN_CLIP));
    } // Fl_Button* o
    { guiSpinnerMCCNodeID = new Fl_Spinner(310, 80, 35, 25, "Node ID");
      guiSpinnerMCCNodeID->tooltip("Node on which the agent will be spawn");
      guiSpinnerMCCNodeID->minimum(0);
      guiSpinnerMCCNodeID->value(0);
    } // Fl_Spinner* guiSpinnerMCCNodeID
    { guiSpinnerMCCContributedItemID = new Fl_Spinner(435, 80, 35, 25, "Contributed");
      guiSpinnerMCCContributedItemID->tooltip("Contributed Item ID");
      guiSpinnerMCCContributedItemID->minimum(0);
      guiSpinnerMCCContributedItemID->value(0);
    } // Fl_Spinner* guiSpinnerMCCContributedItemID
    { guiSpinnerMCCConstraintItemID = new Fl_Spinner(555, 80, 35, 25, "Constraint");
      guiSpinnerMCCConstraintItemID->tooltip("Constraint Item ID");
      guiSpinnerMCCConstraintItemID->minimum(-1);
      guiSpinnerMCCConstraintItemID->value(-1);
    } // Fl_Spinner* guiSpinnerMCCConstraintItemID
    guiWindowMultiagentApplication->end();
  } // Fl_Double_Window* guiWindowMultiagentApplication
  return guiWindowMultiagentApplication;
}

Fl_Double_Window *guiWindowYellowPagesApplication=(Fl_Double_Window *)0;

static void cb_Quit1(Fl_Button*, void*) {
  onGuiButtonQuitYellowPages();
}

Fl_Text_Display *guiTextDisplayYPLog=(Fl_Text_Display *)0;

static void cb_Clear(Fl_Button*, void*) {
  onguiButtonClearYellowPages();
}

/**
 Main menu of the yellowpages application.
*/
Fl_Double_Window* make_window_yellowpages_application() {
  { guiWindowYellowPagesApplication = new Fl_Double_Window(594, 580, "SiSiMEX Yellow Pages Application");
    { Fl_Button* o = new Fl_Button(5, 550, 585, 25, "Quit Yellow Pages");
      o->callback((Fl_Callback*)cb_Quit1);
      o->align(Fl_Align(FL_ALIGN_CLIP));
    } // Fl_Button* o
    { guiTextDisplayYPLog = new Fl_Text_Display(5, 50, 585, 495, "Application log");
      guiTextDisplayYPLog->align(Fl_Align(FL_ALIGN_TOP_LEFT));
      Fl_Group::current()->resizable(guiTextDisplayYPLog);
    } // Fl_Text_Display* guiTextDisplayYPLog
    { Fl_Button* o = new Fl_Button(5, 5, 585, 25, "Clear Yellow Pages");
      o->callback((Fl_Callback*)cb_Clear);
    } // Fl_Button* o
    guiWindowYellowPagesApplication->end();
  } // Fl_Double_Window* guiWindowYellowPagesApplication
  return guiWindowYellowPagesApplication;
}
