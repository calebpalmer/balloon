#include "start_menu_state.h"
#include "arena_play_state.h"

#include <iostream>
#include <sstream>

#include "assets.h"

using namespace CapEngine;
using namespace std;

StartMenuState::StartMenuState(Uint32 windowID) : m_loaded(false)
  , m_windowID(windowID)
  , m_startNewGame(false)
  , m_exitGame(false)
  , m_startArenaGame(false) {}

StartMenuState::~StartMenuState(){
}

bool StartMenuState::onLoad(){
  //Locator::assetManager->getSound(SOUND_MUSIC_NES_OVERWORLD);

  string currentDir = stripPath(getCurrentExecutablePath());
  stringstream fontDir;
  fontDir << currentDir << "/res/fonts/tahoma.ttf";
  Colour buttonInactiveColour(0x85, 0xFD, 0x00, 0xFF);
  Colour buttonActiveColour(0xBA, 0xBA, 0xBA, 0xFF);
  unique_ptr<ButtonGroup> pButtonGroup(new ButtonGroup);
  unique_ptr<Button> pStoryButton(
				      new TextButton(m_windowID, "Single Player", fontDir.str(), 65,
						     Vector(486.947, CARTESIAN_Y_TO_WINDOW_Y(Locator::videoManager->getWindowHeight(), 285.151))
						     , buttonInactiveColour, buttonActiveColour
						     ));
  //dynamic_cast<TextButton*>(pStoryButton.get())->setIndicator("res/textures/turtle_shell.png");
  pStoryButton->registerCallback(&arenaButtonCallback, this);

  unique_ptr<Button> pArenaButton(
				      new TextButton(m_windowID, "2 Player", fontDir.str(), 65,
						     Vector(489.343, CARTESIAN_Y_TO_WINDOW_Y(Locator::videoManager->getWindowHeight(), 204.521))
						     , buttonInactiveColour, buttonActiveColour
						     ));
  //dynamic_cast<TextButton*>(pArenaButton.get())->setIndicator("res/textures/turtle_shell.png");
  pArenaButton->registerCallback(&arenaButtonCallback, this);

  unique_ptr<Button> pQuitButton(
				 new TextButton(m_windowID, "Quit", fontDir.str(), 65,
						Vector(520.670, CARTESIAN_Y_TO_WINDOW_Y(Locator::videoManager->getWindowHeight(), 119.252))
						, buttonInactiveColour, buttonActiveColour
						));
  //dynamic_cast<TextButton*>(pQuitButton.get())->setIndicator("res/textures/turtle_shell.png");
  pQuitButton->registerCallback(&exitButtonCallback, this);


  pButtonGroup->addButton(std::move(pStoryButton));
  pButtonGroup->addButton(std::move(pArenaButton));
  pButtonGroup->addButton(std::move(pQuitButton));

  // check for controller to listen to
  auto controllers = Controller::getConnectedControllers();
  if(controllers.size() > 0){
    pButtonGroup->listenController(controllers[0]);
  }
  
  m_uiObjects.push_back(std::move(pButtonGroup));

  return true;
}

bool StartMenuState::onDestroy(){
  Locator::assetManager->stopSound(m_soundId);
  return true;
}

void StartMenuState::render(){
  if(!m_loaded){
    //_m_soundId = Locator::assetManager->playSound(SOUND_MUSIC_NES_OVERWORLD, true);
    m_loaded = true;
  }
  // int textureWidth = Locator::assetManager->getImageWidth(TEXTURE_MENU_LOGO);
  // int textureHeight = Locator::assetManager->getImageHeight(TEXTURE_MENU_LOGO);
  // Rectangle srcRect(0, 0, textureWidth, textureHeight);
  // Rectangle dstRect(100, 35.476, 1000, 361.667);
  // Locator::assetManager->draw(m_windowID, TEXTURE_MENU_LOGO, srcRect, dstRect);

  // render ui
  for(auto& i : m_uiObjects){
    i->render();
  }
}

void StartMenuState::update(double ms){
  // update ui objects
  for(auto& i : m_uiObjects){
    i->update();
  }
  if(m_exitGame){
    CapEngine::end();
  }
  if(m_startArenaGame){
    int arenaId = 2;
    std::unique_ptr<GameState> pStartArenaState(new ArenaPlayState(m_windowID, arenaId));
    CapEngine::switchState(std::move(pStartArenaState));
  }
}

void StartMenuState::startButtonCallback(void* context){
  reinterpret_cast<StartMenuState*>(context)->m_startNewGame = true;
}

void StartMenuState::exitButtonCallback(void* context){
  reinterpret_cast<StartMenuState*>(context)->m_exitGame = true; 
}

void StartMenuState::arenaButtonCallback(void* context){
  reinterpret_cast<StartMenuState*>(context)->m_startArenaGame = true; 
}

/**
   Function to receive input from a ControlScheme
 **/
void StartMenuState::receiveInput(string input){
  
}
