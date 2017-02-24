/**
   This file contains both GamePadButtonInput and GamePadControScheme.  They are only ever used together.
 **/

#ifndef GAMEPAD_CONTROL_SCHEME_H
#define GAMEPAD_CONTROL_SCHEME_H

#include "CapEngine.h"
#include "control_scheme.h"

/**
   Represents a type of button press on a gamepad (button pressed, keyup/down)
 */
class GamePadButtonInput {
 public:
  enum EventType {
    EventType_Unknown,
    EventType_KeyUp,
    EventType_KeyDown,
  };

  GamePadButtonInput(SDL_GameControllerButton button, EventType eventType);
  bool operator==(const GamePadButtonInput& rightHandSide);
  
 private:
  EventType m_eventType;
  SDL_GameControllerButton m_button;
};


/**
   ControlScheme that takes input from a Gamepad.
 */
class GamePadControlScheme : public ControlScheme, public CapEngine::IEventSubscriber {
 public:
  GamePadControlScheme(std::shared_ptr<CapEngine::Controller> pController);
  GamePadControlScheme(GamePadControlScheme&) = delete;
  GamePadControlScheme& operator=(const GamePadControlScheme&) = delete;

  // IEventSubscriber
  virtual void receiveEvent(SDL_Event event, CapEngine::Time* time) override;

  std::vector<std::pair<GamePadButtonInput, std::string> > getDefaultButtonMap() const;
  void changeButtonMap( std::vector<std::pair<GamePadButtonInput, std::string> > buttonMap);
  std::shared_ptr<CapEngine::Controller> getController();
  int getControllerID() const;

 private:
  std::shared_ptr<CapEngine::Controller> m_pController;  // The Game Controller the scheme is attached to
  std::vector<std::pair<GamePadButtonInput, std::string> > m_inputMap;  // A map of buttons and messages to send

  SDL_GameControllerButton getButtonFromEvent(SDL_Event event) const;
  bool isButtonInputInScheme(GamePadButtonInput input, std::string& message) const;
};

#endif // GAMEPAD_CONTROL_SCHEME_H
