#ifndef PLAYER_INPUT_CONTROLLER_H
#define PLAYER_INPUT_CONTROLLER_H

#include <memory>

#include "CapEngine.h"

#define MAX_VELOCITY 200.0
#define JUMP_MESSAGE "JUMP"

class PlayerInputComponent : public CapEngine::InputComponent, public CapEngine::IEventSubscribe, public ControlSchemeListenerr {
 public:
  PlayerInputComponent(std::shared_ptr<CapEngine::Controller> pController);
  ~PlayerInputComponent();
  void update(CapEngine::GameObject* object) override;
  void receiveInput(ControlScheme::Input input) override;

 protected:
  bool doJump;
  bool doPause;
};

#endif // PLAYER_INPUT_CONTROLLER_H
