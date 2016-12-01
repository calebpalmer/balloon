#ifndef GAMEPAD_CONTROL_SCHEME_H
#define GAMEPAD_CONTROL_SCHEME_H

#include "CapEngine.h"
#include "control_scheme.h"

class GamePadControlScheme : public ControlScheme, public CapEngine::IEventSubscriber {
 public:
  GamePadControlScheme(std::shared_ptr<CapEngine::Controller> pController);
  GamePadControlScheme(GamePadControlScheme&) = delete;
  GamePadControlScheme& operator=(const GamePadControlScheme&) = delete;
    // IEventSubscriber
  virtual void receiveEvent(SDL_Event event, CapEngine::Time* time) override;

 private:
  std::shared_ptr<CapEngine::Controller> m_pController;
};

#endif // GAMEPAD_CONTROL_SCHEME_H
