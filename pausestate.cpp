#include "pausestate.h"

#include "start_menu_state.h"

#include <memory>

using namespace CapEngine;
using namespace std;

PauseState::PauseState(Uint32 windowID) :
  m_windowID(windowID), m_escapePressed(false)
  , m_switchToMenuState(false), m_resumePlayState(false) {}

PauseState::~PauseState(){}

void PauseState::render(){
  if(m_uiObjects.size() > 0){
    for ( auto& i : m_uiObjects){
      i->render();
  }
  }

}

void PauseState::update(double ms){
  for (auto& i : m_uiObjects){
    i->update();
  }

  if(!m_escapePressed && Locator::keyboard->keyMap[Keyboard::CAP_ESCAPE].state == Keyboard::CAP_PRESSED){
    m_escapePressed = true;
  }
  
  if(m_escapePressed  && Locator::keyboard->keyMap[Keyboard::CAP_ESCAPE].state == Keyboard::CAP_UNPRESSED){
    CapEngine::popState();
    return;
  }

  if(m_switchToMenuState){
    m_switchToMenuState = false;
    unique_ptr<StartMenuState> p_startMenuState(new StartMenuState(m_windowID));
    CapEngine::switchState(std::move(p_startMenuState));
    return;
  }

  if(m_resumePlayState){
    m_resumePlayState = false;
    CapEngine::popState();
    return;
  }
  
}

bool PauseState::onLoad(){
  // add ui
  unique_ptr<TextButton> pResumeButton(new TextButton(m_windowID, "Resume", "res/fonts/tahoma.ttf", 40, Vector(0, 0)));
  unique_ptr<TextButton> pMainMenuButton(new TextButton(m_windowID, "Main Menu", "res/fonts/tahoma.ttf", 40, Vector(0,0)));
  int numItems = 2;
  int xres, yres;
  Locator::videoManager->getWindowResolution(&xres, &yres);
  int maxwidth = pMainMenuButton->getWidth();
  int xStart = (xres / 2.0) - (maxwidth / 2.0);
  int menuItemSpacing = 10;
  int maxheight = pResumeButton->getHeight() + pMainMenuButton->getHeight() + ((numItems - 1) * menuItemSpacing);
  int yStart = (yres / 2.0) - (maxheight / 2.0);

  int itemHeight = pResumeButton->getHeight();
  pResumeButton->setPosition(Vector(xStart, yStart));
  pMainMenuButton->setPosition(Vector(xStart, yStart + ((numItems - 1) * (itemHeight + menuItemSpacing) )));

  pResumeButton->registerCallback(this->resumeCallback, this);
  pMainMenuButton->registerCallback(this->returnToMenuCallback, this);
  
  unique_ptr<ButtonGroup> pButtonGroup(new ButtonGroup());
  pButtonGroup->addButton(std::move(pResumeButton));
  pButtonGroup->addButton(std::move(pMainMenuButton));

  // m_uiObjects.push_back(std::move(pResumeButton));
  // m_uiObjects.push_back(std::move(pMainMenuButton));
  m_uiObjects.push_back(std::move(pButtonGroup));

  m_escapePressed = false;
  return true;
}

bool PauseState::onDestroy(){
  return true;
}

void PauseState::returnToMenuCallback(void* context){
  reinterpret_cast<PauseState*>(context)->m_switchToMenuState = true;
}

void PauseState::resumeCallback(void* context){
  reinterpret_cast<PauseState*>(context)->m_resumePlayState = true;
}
