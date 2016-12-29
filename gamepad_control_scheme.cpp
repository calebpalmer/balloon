#include "gamepad_control_scheme.h"

using namespace CapEngine;
using namespace std;

// GamePadButton implementation
GamePadButtonInput::GamePadButtonInput(SDL_GameControllerButton button, EventType eventType) :
  m_button(button), m_eventType(eventType) {}


bool GamePadButtonInput::operator==(const GamePadButtonInput& rightHandSide){
  return (this->m_button == rightHandSide.m_button) && (this->m_eventType == rightHandSide.m_eventType);  
}


// GamePadControlScheme implementation
GamePadControlScheme::GamePadControlScheme(shared_ptr<Controller> pController)
  : m_pController(pController)  {

  m_inputMap = getDefaultButtonMap();
  int subscriptionMask = controllerEvent;
  IEventSubscriber::subscribe( Locator::eventDispatcher, subscriptionMask );
}


void GamePadControlScheme::receiveEvent(SDL_Event event, Time* time){
  if(m_enabled){
    if(event.type == SDL_CONTROLLERBUTTONUP || event.type == SDL_CONTROLLERBUTTONDOWN){
      // construct GamePadButtonInput from event details
      SDL_GameControllerButton button = getButtonFromEvent(event);
      GamePadButtonInput::EventType type = (event.type == SDL_CONTROLLERBUTTONDOWN) ? GamePadButtonInput::EventType_KeyDown :
	GamePadButtonInput::EventType_KeyUp;

      GamePadButtonInput buttonInput(button, type);

      // Is the button input a part of the control scheme
      string message;
      bool isInScheme = isButtonInputInScheme(buttonInput, message);
      
      // if matching, dispatch to listeners
      ControlScheme::dispatchMessage(message);
      
    }
  }
}


SDL_GameControllerButton GamePadControlScheme::getButtonFromEvent(SDL_Event event) const{
  if(event.type == SDL_CONTROLLERBUTTONUP ||
     event.type == SDL_CONTROLLERBUTTONDOWN ||
     event.type == SDL_CONTROLLERAXISMOTION){
    
      const SDL_ControllerButtonEvent* controllerEvent = reinterpret_cast<const SDL_ControllerButtonEvent*>(&event);
      SDL_GameControllerButton button = static_cast<SDL_GameControllerButton>(controllerEvent->button);
      return button;
  }
  else{
    throw CapEngineException("Event is not a Controller event");
  }
}


bool GamePadControlScheme::isButtonInputInScheme(GamePadButtonInput input, std::string& message) const {
  for (auto & trackedInputMessagePair : m_inputMap){
    if (input == trackedInputMessagePair.first){
      message = trackedInputMessagePair.second;
      return true;
    }
  }
  // wasn't found so return false
  return false;
}


std::vector<std::pair<GamePadButtonInput, std::string> > GamePadControlScheme::getDefaultButtonMap() const{
  vector< pair< GamePadButtonInput, string > > buttonMap;
  
  GamePadButtonInput jumpButton(SDL_CONTROLLER_BUTTON_A, GamePadButtonInput::EventType_KeyDown);
  string message = "JUMP";
  buttonMap.push_back( std::make_pair( jumpButton, message ));

  GamePadButtonInput pauseButton(SDL_CONTROLLER_BUTTON_START, GamePadButtonInput::EventType_KeyDown);
  message = "PAUSE";
  buttonMap.push_back( std::make_pair( pauseButton, message ));

  GamePadButtonInput moveLeftButton(SDL_CONTROLLER_BUTTON_DPAD_LEFT, GamePadButtonInput::EventType_KeyDown);
  message = "LEFT";
  buttonMap.push_back( std::make_pair( moveLeftButton, message ));

  GamePadButtonInput moveRightButton(SDL_CONTROLLER_BUTTON_DPAD_RIGHT, GamePadButtonInput::EventType_KeyDown);
  message = "RIGHT";
  buttonMap.push_back( std::make_pair( moveRightButton, message ));

  GamePadButtonInput stopRightButton(SDL_CONTROLLER_BUTTON_DPAD_RIGHT, GamePadButtonInput::EventType_KeyUp);
  message = "STOPRIGHT";
  buttonMap.push_back( std::make_pair( stopRightButton, message ));

  GamePadButtonInput stopLeftButton(SDL_CONTROLLER_BUTTON_DPAD_LEFT, GamePadButtonInput::EventType_KeyUp);
  message = "STOPLEFT";
  buttonMap.push_back( std::make_pair( stopLeftButton, message ));

  GamePadButtonInput toggleDiagnosticsButton( SDL_CONTROLLER_BUTTON_BACK, GamePadButtonInput::EventType_KeyDown );
  message = "TOGGLE DIAGNOSTICS";
  buttonMap.push_back( std::make_pair( toggleDiagnosticsButton, message ));

  return buttonMap;
}
