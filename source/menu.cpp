#include <sstream>
#include <citrus/gpu.hpp>
#include <citrus/gput.hpp>

#include "menu.h"
#include "workaround.h"

using namespace std;
using namespace ctr;

Menu::Menu(MenuManager* parentManager, Menu* parentMenu)
{
    this->currentSelection = 0;  
    this->parentManager = parentManager;
    this->parentMenu = parentMenu;

    string backName="";
    if(this->parentMenu==nullptr)
        backName="exit";
    else
        backName="back";

    BackMenuEntry* BackEntry = new BackMenuEntry((MenuManagerM*)this->parentManager,backName,"");
    this->menuEntrys.push_back((MenuEntry*)BackEntry);
}

void Menu::addEntry(MenuEntry* entry)
{
    if(entry!=nullptr)
    {
        std::vector<MenuEntry*>::iterator it;

        it = this->menuEntrys.end();
        it--;
        it = this->menuEntrys.insert ( it , entry);
    }
}

MenuManager* Menu::getParentManager()
{
    return this->parentManager;
}

Menu* Menu::getParentMenu()
{
    return this->parentMenu;
}


void Menu::menuChangeSelection(string direction)
{
    if(direction=="up")
    {
        if(this->currentSelection > 0)
            this->currentSelection --;
    }
    else if(direction=="down")
    {
        if(this->currentSelection < this->menuEntrys.size()-1)
            this->currentSelection ++;
    }
}

void Menu::selectionDoAAction()
{
    if(this->menuEntrys.size() > 0)
    {
        this->menuEntrys.at(this->currentSelection)->aAction();
    }
}

void Menu::selectionDoSiteAction()
{
    if(this->menuEntrys.size() > 0)
    {
        this->menuEntrys.at(this->currentSelection)->sideAction();
    }
}

Menu* Menu::back()
{
    this->currentSelection = 0;
    return this->parentMenu;
}

short Menu::getNumberOfEntrys()
{
    return this->menuEntrys.size();
}

void Menu::drawMenu()
{
    stringstream menuStream;
    unsigned int i=0;
    for(std::vector<MenuEntry*>::iterator it = menuEntrys.begin(); it != menuEntrys.end(); ++it)
    {
	    if(i == this->currentSelection)
		    menuStream << "-> ";
	    else
		    menuStream << "   ";
	
	    menuStream <<(*it)->getRow();
	    menuStream << "\n";
	    i++;
    }

    stringstream descriptionStream;
    descriptionStream << menuEntrys.at(this->currentSelection)->getDescription();
  
    string menu = menuStream.str();
    string description = descriptionStream.str();
	
	u32 screenWidth;
	u32 screenHeight;
	gpu::setViewport(gpu::SCREEN_BOTTOM, 0, 0, gpu::BOTTOM_WIDTH, gpu::BOTTOM_HEIGHT);
	gput::setOrtho(0, gpu::BOTTOM_WIDTH, 0, gpu::BOTTOM_HEIGHT, -1, 1);
	gpu::setClearColor(0xFF, 0xFF, 0xFF, 0xFF);
	gpu::clear();

	gpu::getViewportWidth(&screenWidth);
	gpu::getViewportHeight(&screenHeight);

	gput::drawString(menu, 15, screenHeight -30-gput::getStringHeight(menu, 8) , 8, 8, 0, 0, 0);
	gput::drawString(description, 25, 50-gput::getStringHeight(description, 8), 8, 8, 0, 0, 0);

	gpu::flushCommands();
	gpu::flushBuffer();

	gpu::swapBuffers(true);
}

