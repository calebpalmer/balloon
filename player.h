#ifndef PLAYER_H
#define PLAYER_H

#include <memory>

#include "CapEngine.h"

#include "control_scheme.h"

std::shared_ptr<CapEngine::GameObject> makePlayer(Uint32 windowID, ControlScheme controlScheme);

#endif // PLAYER_H
