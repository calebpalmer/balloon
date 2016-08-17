#include "player.h"

#include "dumb_graphics_component.h"
#include "player_input_component.h"
#include "player_physics_component.h"

using namespace std;
using namespace CapEngine;

shared_ptr<GameObject> makePlayer(Uint32 windowID, ControlScheme& controlScheme){
  shared_ptr<GameObject> pGameObject(new GameObject(true));

  shared_ptr<InputComponent> pInputComponent(new PlayerInputComponent(pController));
  shared_ptr<PhysicsComponent> pPhysicsComponent(new PlayerPhysicsComponent());
  shared_ptr<GraphicsComponent> pGraphicsComponent(new DumbGraphicsComponent(windowID, 40, 60, Colour(0, 255, 0)));
  shared_ptr<CustomComponent> pCustomComponent(new NullCustomComponent());
  shared_ptr<AIComponent> pAIComponent(new NullAIComponent());

  pGameObject->setInputComponent(pInputComponent);
  pGameObject->setPhysicsComponent(pPhysicsComponent);
  pGameObject->setGraphicsComponent(pGraphicsComponent);
  pGameObject->setCustomComponent(pCustomComponent);
  pGameObject->setAIComponent(pAIComponent);

  Vector zeroVector(0.0, 0.0, 0.0, 0.0);
  pGameObject->setPosition(zeroVector);
  pGameObject->setOrientation(zeroVector);
  pGameObject->setVelocity(zeroVector);

  controlScheme.listen(pGameObject);
  
  return pGameObject;
}
