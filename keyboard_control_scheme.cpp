#include "keyboard_control_scheme.h"

#include <cassert>

/**
   Loads the default map as the input actions
 */
KeyboardControlScheme::KeyboardControlScheme(){
  m_keyMap = getDefaultMap();
  assert(m_keyMap.size() >= ControlScheme::m_numInputs);
}


/**
   Loads the inputted map as the keyboard to input actions
 **/
KeyboardControlScheme::KeyboardControlScheme(std::map<SDL_Keycode, ControlScheme::Input> map){
  m_keyMap = map;
  assert(m_keyMap.size() >= ControlScheme::m_numInputs);
}

/**
   Returns a default keymapping
 */
std::map<SDL_Keycode, ControlScheme::Input> KeyboardControlScheme::getDefaultMap(){
  if(s_defaultMap.size() == 0){
    // load the static default map

    s_defaultMap[SDLK_SPACE] = ControlScheme::JUMP;
    s_defaultMap[SDLK_LEFT] = ControlScheme::GO_LEFT;
    s_defaultMap[SDLK_RIGHT] = ControlScheme::GO_RIGHT;
    s_defaultMap[SDLK_ESCAPE] = ControlScheme::MENU;
  }

  assert(s_defaultMap.size() >= ControlScheme::m_numInputs);
  return s_defaultMap;
}

void KeyboardControlScheme::remapInput(SDL_Keycode keycode, ControlScheme::Input input){
  m_keyMap[keycode] = input;
}

/**
   Receive  SDL Events and handle them.
 */
void KeyboardControlScheme::receiveEvent(SDL_Event event, CapEngine::Time *time){
  // Check if this is a keyboard type event
  if(event.type == SDL_KEYUP || event.type == SDL_KEYDOWN){
    SDL_Keycode keycode = (reinterpret_cast<SDL_KeyboardEvent*>(&event))->keysym.sym;
    auto it = m_keyMap.find(keycode);
    if(it != m_keyMap.end()){
      // for each listener
      for(auto &listener : m_pListeners){
	// notify this listener of the event
	listener->receiveInput(it->second);
      }
    }
  }
}

/**
   Registers listeners for Input events
 */
void KeyboardControlScheme::listen(std::shared_ptr<ControlSchemeListener> pListener){
  // Make sure that this is not already a listener
  for (auto  & i : m_pListeners){
    if(i.get() == pListener.get()){
      return;
    }
  }
  // add listener to list of listeners
  m_pListeners.push_back(pListener);
}

/*
  Remove the provided ControlSchemeListener as an active listener.
 */
void KeyboardControlScheme::unlisten(ControlSchemeListener* pListener){
  auto i = m_pListeners.begin();
  for(; i != m_pListeners.end(); i++){
    if(i->get() == pListener){
	break;
      }
  }
  if(i != m_pListeners.end()){
    m_pListeners.erase(i);
  }
}



