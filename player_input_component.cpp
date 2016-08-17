#include "player_input_component.h"

#include "pausestate.h"

using namespace std;
using namespace CapEngine;

PlayerInputComponent::PlayerInputComponent() :
  doJump(false) {

  Locator::eventDispatcher->subscribe(this, controllerEvent);
}

PlayerInputComponent::~PlayerInputComponent(){
  Locator::eventDispatcher->unsubscribe(this);
}

void PlayerInputComponent::update(GameObject* object){
  if(doJump){
    int messageId = object->generateMessageId();
    object->send(messageId, JUMP_MESSAGE);
    doJump = false;
  }
  if(doPause){
    //unique_ptr<GameState> pPauseState(new PauseState(m_windowID));
    //CapEngine::pushState(std::move(pPauseState));
    doPause= false;
  }
  
  if(m_pController){
    if(SDL_GameControllerGetButton(m_pController->getGameController(), SDL_CONTROLLER_BUTTON_DPAD_LEFT) == 1){
      object->send(-1, "GO LEFT");
    }
    else if(SDL_GameControllerGetButton(m_pController->getGameController(), SDL_CONTROLLER_BUTTON_DPAD_RIGHT) == 1){
      object->send(-1, "GO RIGHT");
    }
  }  
}

// void PlayerInputComponent::receiveEvent(SDL_Event event, Time* time){
//   if(event.type == SDL_CONTROLLERBUTTONDOWN){
//     const SDL_ControllerButtonEvent* controllerEvent = reinterpret_cast<const SDL_ControllerButtonEvent*>(&event);
//     SDL_GameControllerButton button = static_cast<SDL_GameControllerButton>(controllerEvent->button);
//     if(button == SDL_CONTROLLER_BUTTON_A){
//       doJump = true;
//     }
//   }
//   else if (event.type == SDL_CONTROLLERBUTTONUP){
//     const SDL_ControllerButtonEvent* controllerEvent = reinterpret_cast<const SDL_ControllerButtonEvent*>(&event);
//     SDL_GameControllerButton button = static_cast<SDL_GameControllerButton>(controllerEvent->button);
//     if(button == SDL_CONTROLLER_BUTTON_START){
//       doPause = true;
//     }
//   }
// }

void PlayerInputComponent::receiveInput(ControlScheme::Input input){
  
}
