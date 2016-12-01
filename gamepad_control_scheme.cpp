#include "gamepad_control_scheme.h"

using namespace CapEngine;
using namespace std;

GamePadControlScheme::GamePadControlScheme(shared_ptr<Controller> pController)
  : m_pController(pController) { }

void GamePadControlScheme::receiveEvent(SDL_Event event, Time* time){

}
