#include "keyboard_control_scheme.h"

#include <cassert>

std::vector<std::pair<KeyInput, std::string> > KeyboardControlScheme::s_defaultMap;

/**
   Loads the default map as the input actions
 */
KeyboardControlScheme::KeyboardControlScheme() {
  m_keyMap = getDefaultMap();
  this->registerEvents();
}


/**
   Loads the inputted map as the keyboard to input actions
 **/
KeyboardControlScheme::KeyboardControlScheme(std::vector<std::pair<KeyInput, std::string> > map){
  m_keyMap = map;
  this->registerEvents();
}

/**
   Returns a default keymapping
 */
std::vector<std::pair<KeyInput, std::string> > KeyboardControlScheme::getDefaultMap(){
  if(s_defaultMap.size() == 0){
    // load the static default map

    // keydown inputs
    KeyInput spaceKeyDown(SDLK_SPACE, EventType_KeyDown);
    s_defaultMap.push_back(std::make_pair(spaceKeyDown, "JUMP"));

    KeyInput escapeKeyDown(SDLK_ESCAPE, EventType_KeyDown);
    s_defaultMap.push_back(std::make_pair(escapeKeyDown, "PAUSE"));

    KeyInput leftKeyDown(SDLK_LEFT, EventType_KeyDown);
    s_defaultMap.push_back(std::make_pair(leftKeyDown, "LEFT"));

    KeyInput rightKeyDown(SDLK_RIGHT, EventType_KeyDown);
    s_defaultMap.push_back(std::make_pair(rightKeyDown, "RIGHT"));

    // keyups
    KeyInput leftKeyUp(SDLK_LEFT, EventType_KeyUp);
    s_defaultMap.push_back(std::make_pair(leftKeyUp, "STOPLEFT"));

    KeyInput rightKeyUp(SDLK_RIGHT, EventType_KeyUp);
    s_defaultMap.push_back(std::make_pair(rightKeyUp, "STOPRIGHT"));

    KeyInput tildeKeyUp(SDLK_BACKQUOTE, EventType_KeyUp);
    s_defaultMap.push_back(std::make_pair(tildeKeyUp, "TOGGLE DIAGNOSTICS"));
  }

  return s_defaultMap;
}

void KeyboardControlScheme::remapInput(KeyInput input, std::string message){
  bool found = false;
  for(auto & i : m_keyMap){
    if(i.first == input){
      i.second = message;
      found = true;
    }
  }

  if(!found){
    m_keyMap.push_back(std::make_pair(input, message));
  }
}

/**
   Receive  SDL Events and handle them.
*/
void KeyboardControlScheme::receiveEvent(SDL_Event event, CapEngine::Time *time){
  if(m_enabled){
    // Check if this is a keyboard type event
    if(event.type == SDL_KEYUP || event.type == SDL_KEYDOWN){
      SDL_Keycode keycode = (reinterpret_cast<SDL_KeyboardEvent*>(&event))->keysym.sym;
      for(auto & it : m_keyMap) {

	auto currentKeyInput = it.first;
	KeyInput receivedKeyInput;
	if(event.type == SDL_KEYDOWN){
	  receivedKeyInput =KeyInput(keycode, EventType_KeyDown);
	}
	else if(event.type == SDL_KEYUP){
	  receivedKeyInput =KeyInput(keycode, EventType_KeyUp);
	}
	else{
	  // Do Nothing.  Don't care about non keyboard events.
	}

	//  Process Input
	if(currentKeyInput == receivedKeyInput){
	  // for each listener
	  for(auto &listener : m_pListeners){
	    // notify this listener of the event
	    auto message = it.second;
	    listener->receiveInput(message);
	  }
	}
      }
    }
  }
}

/**
   Registers listeners for Input events
 */
void KeyboardControlScheme::listen(ControlSchemeListener* pListener){
  // Make sure that this is not already a listener
  for (auto  & i : m_pListeners){
    if(i == pListener){
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
    if(*i == pListener){
	break;
      }
  }
  if(i != m_pListeners.end()){
    m_pListeners.erase(i);
  }
}

/**
   Enables the ControlScheme
 */
void KeyboardControlScheme::enable(){
  m_enabled = true;
}


/**
   Disables the ControlScheme
 */ 
void KeyboardControlScheme::disable(){
  m_enabled = false;
}

/**
   Register this IEventSubscriber with the EventDispatcher
 */
void KeyboardControlScheme::registerEvents(){
  int subscriptionMask = CapEngine::keyboardEvent;
  this->subscribe(CapEngine::Locator::eventDispatcher, subscriptionMask);
}
