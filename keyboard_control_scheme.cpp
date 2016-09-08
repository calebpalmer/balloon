#include "keyboard_control_scheme.h"

#include <cassert>

std::map<SDL_Keycode, KeyInput> KeyboardControlScheme::s_defaultMap;

/**
   Loads the default map as the input actions
 */
KeyboardControlScheme::KeyboardControlScheme(){
  m_keyMap = getDefaultMap();
  this->registerEvents();
}


/**
   Loads the inputted map as the keyboard to input actions
 **/
KeyboardControlScheme::KeyboardControlScheme(std::map<SDL_Keycode, KeyInput> map){
  m_keyMap = map;
  this->registerEvents();
}

/**
   Returns a default keymapping
 */
std::map<SDL_Keycode, KeyInput> KeyboardControlScheme::getDefaultMap(){
  if(s_defaultMap.size() == 0){
    // load the static default map

    // keyup inputs
    s_defaultMap[SDLK_SPACE] = KeyInput("JUMP", EventType_KeyDown);;
    s_defaultMap[SDLK_ESCAPE] = KeyInput("PAUSE", EventType_KeyDown);

    // hold key inputs
    s_defaultMap[SDLK_LEFT] = KeyInput("LEFT", EventType_KeyPressAndHold);
    s_defaultMap[SDLK_RIGHT] = KeyInput("RIGHT", EventType_KeyPressAndHold);
    

  }

  return s_defaultMap;
}

void KeyboardControlScheme::remapInput(SDL_Keycode keycode, KeyInput input){
  m_keyMap[keycode] = input;
}

/**
   Receive  SDL Events and handle them.
*/
void KeyboardControlScheme::receiveEvent(SDL_Event event, CapEngine::Time *time){
  if(m_enabled){
    // Check if this is a keyboard type event
    if(event.type == SDL_KEYUP || event.type == SDL_KEYDOWN){
      SDL_Keycode keycode = (reinterpret_cast<SDL_KeyboardEvent*>(&event))->keysym.sym;
      auto it = m_keyMap.find(keycode);
      if(it != m_keyMap.end()){

	//  Process Key Down inputs
	if(event.type == SDL_KEYDOWN && it->m_eventType == EventType_KeyDown){
	  // for each listener
	  for(auto &listener : m_pListeners){
	    // notify this listener of the event
	    listener->receiveInput((it->second).m_input);
	  }
	}

	// Process KeyPressAndHold events
	// Update their states and send inputs to listeners later
	if(event.type == SDL_KEYDOWN && it->m_eventType == EventType_KeyDown){
	  m_keyPressAndHoldStates[keycode] = true;
	}
	if(event.type == SDL_KEYUP && it->m_eventType == EventType_KeyDown){
	  m_keyPressAndHoldStates[keycode] = false;
	}
      }
    }

    // send inputs for KeyPressAndHold events
    for (const auto& keyState : m_keyPressAndHoldStates){
      if(keyState.second == true){
	// notify each listener
	  for(auto &listener : m_pListeners){
	    // notify this listener of the event
	    std::string input (m_keyMap[keyState.first]).m_input;
	    listener->receiveInput();
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
