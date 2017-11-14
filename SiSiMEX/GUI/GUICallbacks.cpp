#include "GUICallbacks.h"
#include "../YellowPages.h"
#include "../MultiAgentSystem.h"
#include <iostream>

////////////////////////////////////////////////////////////////////////////////
// YellowPages
////////////////////////////////////////////////////////////////////////////////

YellowPages *g_YellowPages = nullptr;
MultiAgentSystem *g_MultiAgentApp = nullptr;


////////////////////////////////////////////////////////////////////////////////
// Private functions
////////////////////////////////////////////////////////////////////////////////

void multiagentsApplicationUpdate(void*);
void multiagentsApplicationFinalize(Fl_Window *, void*);
void yellowPagesApplicationUpdate(void*);
void yellowPagesApplicationFinalize(Fl_Window *, void*);


void multiagentsApplicationInitialize()
{
	Fl_Window *w = make_window_multiagents_application();
	w->show();

	Fl::add_idle(&multiagentsApplicationUpdate, nullptr);
	Fl::set_atclose(&multiagentsApplicationFinalize);

	g_MultiAgentApp = new MultiAgentSystem();
	g_MultiAgentApp->initialize();
}

void multiagentsApplicationUpdate(void*)
{
	g_MultiAgentApp->update();
}

void multiagentsApplicationFinalize(Fl_Window *window, void*)
{
	window->hide();

	Fl::remove_idle(&multiagentsApplicationUpdate, nullptr);

	g_MultiAgentApp->finalize();
	delete g_MultiAgentApp;
}


void yellowPagesApplicationInitialize()
{
	Fl_Window *w = make_window_yellowpages_application();
	w->show();

	Fl::add_idle(&yellowPagesApplicationUpdate, nullptr);
	Fl::set_atclose(&yellowPagesApplicationFinalize);

	g_YellowPages = new YellowPages();
	g_YellowPages->initialize();
}

void yellowPagesApplicationUpdate(void*)
{
	g_YellowPages->update();
}

void yellowPagesApplicationFinalize(Fl_Window *window, void*)
{
	window->hide();

	Fl::remove_idle(&yellowPagesApplicationUpdate, nullptr);

	g_YellowPages->finalize();
	delete g_YellowPages;
}


////////////////////////////////////////////////////////////////////////////////
// SiSiMEX main menu
////////////////////////////////////////////////////////////////////////////////

void onguiRadioButtonApplicationModeChanged(Fl_Round_Button*b)
{
	if (b == radioButtonYellowPages) {
		radioButtonAgents->value(0);
		radioButtonYellowPages->value(1);
	} else {
		radioButtonAgents->value(1);
		radioButtonYellowPages->value(0);
	}
}

void onguiButtonStartApplication()
{
	guiWindowApplicationMode->hide();
	if (radioButtonAgents->value() == 1) {
		multiagentsApplicationInitialize();
	} else {
		yellowPagesApplicationInitialize();
	}
}


////////////////////////////////////////////////////////////////////////////////
// Multi-agents application
////////////////////////////////////////////////////////////////////////////////

void onGuiButtonListLocalNodes()
{
	std::cout << "List Local Nodes" << std::endl;
}

void onGuiButtonInspectLocalNode()
{
	std::cout << "Inspect Local Node" << std::endl;
}

void onguiButtonGetContributorsForItem()
{
	std::cout << "Contributors For Item" << std::endl;
}

void onGuiButtonQuitMultiagentApplication()
{
	multiagentsApplicationFinalize(guiWindowMultiagentApplication, nullptr);
}

////////////////////////////////////////////////////////////////////////////////
// YellowPages application
////////////////////////////////////////////////////////////////////////////////

void onGuiButtonQuitYellowPages()
{
	yellowPagesApplicationFinalize(guiWindowYellowPagesApplication, nullptr);
}
