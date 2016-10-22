#ifndef PLAYER_PHYSICS_COMPONENT_H
#define PLAYER_PHYSICS_COMPONENT_H

#include "CapEngine.h"

class PlayerPhysicsComponent : public CapEngine::PhysicsComponent {
 public:
  enum State{
    RUNNING,
    AIRBORN,
    NEUTRAL
  };

 PlayerPhysicsComponent() : m_doRun(false),
    m_doAirLeftTurn(false), m_doAirRightTurn(false),
    m_state(NEUTRAL){}
  void update(CapEngine::GameObject* object, double timestep) override;
  CapEngine::Rectangle boundingPolygon(const CapEngine::GameObject* object) const override;
  bool handleCollision(CapEngine::GameObject* object, CapEngine::CollisionType,
		       CapEngine::CollisionClass, CapEngine::GameObject* otherObject,
		       CapEngine::Vector collisionLocation) override;
  bool handlesCollisions();
  void receive(CapEngine::GameObject* object, int messageId, std::string message) override;
  State getState() const;
  void setGravity(int gravity);

 private:
  void handleJumpMessage(CapEngine::GameObject* object);
  void handleRightMessage(CapEngine::GameObject* object);
  void handleLeftMessage(CapEngine::GameObject* object);
  void handleStopLeftRightMessage(CapEngine::GameObject* object);
  void handleHorizontalBoundaryCollisionMessage(CapEngine::GameObject* object, CapEngine::CollisionType collisionType);
  void handleTopBoundaryCollisionMessage(CapEngine::GameObject* object);
  
  const int JUMP_VELOCITY = -200;
  const int JUMP_TURN_ACCELERATION = 300;
  const int JUMP_HORIZONTAL_VELOCITY = 200;
  const int RUN_VELOCITY = 175;
  const int RUN_FRICTION = 1000;
  int m_gravity;
  State m_state;
  bool m_doRun;
  bool m_doAirRightTurn;
  bool m_doAirLeftTurn;
};

#endif // PLAYER_PHYSICS_COMPONENT_H
