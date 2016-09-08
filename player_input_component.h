#ifndef PLAYER_INPUT_CONTROLLER_H
#define PLAYER_INPUT_CONTROLLER_H

#include <memory>
#include <string>
#include <queue>

#include "CapEngine.h"
#include "control_scheme_listener.h"

#define MAX_VELOCITY 200.0
#define JUMP_MESSAGE "JUMP"

class PlayerInputComponent : public CapEngine::InputComponent, public ControlSchemeListener {
 public:
  PlayerInputComponent(ControlScheme*  pControlScheme);
  ~PlayerInputComponent();
  void update(CapEngine::GameObject* object) override;
  void receiveInput(std::string input) override;

 protected:
  std::queue<std::string> m_inputs;
};

#endif // PLAYER_INPUT_CONTROLLER_H






