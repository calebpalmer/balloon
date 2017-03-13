#include "player_physics_component.h"

#include <iostream>
#include <cmath>

#include "locator.h"
#include "logger.h"

using namespace CapEngine;
using namespace std;

/**
   FLOW:
   - update i s called first
   - then handleCollisions
   - receive can be called at any time if there is a message to be handled
 */

void PlayerPhysicsComponent::update(GameObject* object, double timestep){
  Vector velocity = object->getVelocity();

  // apply FRICTION if not running or airborn to slowdown
  if(!m_doRun &&
     (m_state == State::STAND_LEFT or m_state == State::STAND_RIGHT)
     && velocity.getX() != 0.0){

    // apply friction when running to the right
    if(velocity.getX() > 0.0){
      double deltaX = -RUN_FRICTION * (timestep / 1000);
      velocity.setX(velocity.getX() + deltaX);
      if(velocity.getX() < 0.0){
  	velocity.setX(0);
      }
    }

    // apply friction when running to the left
    else {
      double deltaX = RUN_FRICTION * (timestep / 1000);
      velocity.setX(velocity.getX() + deltaX);
      if(velocity.getX() > 0.0){
  	velocity.setX(0);
      }
    }
  }
  
  // apply GRAVITY to velocity
  //Vector gravity(0.0, GRAVITY);
  Vector acceleration = object->getAcceleration();
  Vector newVelocity = CapEngine::applyAcceleration(acceleration, velocity, timestep);

  // cap horixontal velocity
  // I'm not sure if this even appllies
  bool maxHorizontalVelocityReached = std::abs(newVelocity.x) > RUN_VELOCITY ? true : false;
  if(maxHorizontalVelocityReached){
    newVelocity.x = (newVelocity.x < 0) ? (-1 * RUN_VELOCITY) : RUN_VELOCITY;
  }

  object->setVelocity(newVelocity);

  // Save old position
  object->setPreviousPosition(object->getPosition());
  
  //  Calculate new position
  Vector newPosition = CapEngine::applyDisplacement(object->getVelocity(), object->getPosition(), timestep);
    object->setPosition(newPosition);

  // Reset acceleration to gravity in case of ground collision removing it
  acceleration = object->getAcceleration();
  acceleration.setY(m_gravity);
  object->setAcceleration(acceleration);

}

Rectangle PlayerPhysicsComponent::boundingPolygon(const GameObject* object) const{
  const int spriteWidth = 40;
  const int spriteHeight = 60;
  Vector position = object->getPosition();
  Rectangle rect;
  rect.width = spriteWidth;
  rect.height = spriteHeight;
  rect.x = position.getX() - (rect.width / 2);
  rect.y = position.getY() - (rect.height / 2);
  return rect;
}

bool PlayerPhysicsComponent::handlesCollisions(){
  return true;
}

bool PlayerPhysicsComponent::handleCollision(GameObject* object, CollisionType collisionType,
					     CollisionClass collisionClass, GameObject* otherObject,
					     Vector collisionVector){

  if(collisionType == COLLISION_BOTTOM){
    // reset Y position
    Vector currentPosition = object->getPosition();
    Rectangle boundingPolygon = object->boundingPolygon();
    currentPosition.setY(collisionVector.getY() - (boundingPolygon.height / 2));
    object->setPosition(currentPosition);

    // if(m_state != State::WALK_LEFT and m_state != State::WALK_RIGHT){
    //   m_state = object->getVelocity().getX() <= 0.0 ? State::STAND_LEFT : State::STAND_RIGHT;
    // }

    if(m_state == State::AIRBORN){
      m_state = object->getVelocity().getX() <= 0.0 ? State::STAND_LEFT : State::STAND_RIGHT;
    }

    Vector currentVelocity = object->getVelocity();
    currentVelocity.setY(0.0);
    object->setVelocity(currentVelocity);
    
    // remove horizontal acceleration from mid-air turns
    m_doAirRightTurn = false;
    m_doAirLeftTurn = false;
    Vector acceleration = object->getAcceleration();
    acceleration.setX(0.0);

    // set vertical acceleration to 0
    acceleration.setY(0.0);
    object->setAcceleration(acceleration);

    return true;
  }
  return false;
}

void PlayerPhysicsComponent::receive(GameObject* object, int messageId, string message){
  if(message == "JUMP"){
    handleJumpMessage(object);
  }
  
  else if(message == "RIGHT"){
    handleRightMessage(object);
  }

  else if(message == "LEFT"){
    handleLeftMessage(object);
  }

    else if (message == "STOPLEFT" || message == "STOPRIGHT"){
      handleStopLeftRightMessage(object);
    }

    else if (message == "LEFT BOUNDARY COLLISION"){
      handleHorizontalBoundaryCollisionMessage(object, CapEngine::COLLISION_LEFT);
    }

    else if (message == "RIGHT BOUNDARY COLLISION"){
      handleHorizontalBoundaryCollisionMessage(object, CapEngine::COLLISION_RIGHT);
    }

    else if (message == "TOP BOUNDARY COLLISION"){
      handleTopBoundaryCollisionMessage(object);
    }

  else{
    // nothing
  }
}

/**
   Accessor function for current state
 */
PlayerPhysicsComponent::State PlayerPhysicsComponent::getState() const{
  return m_state;
}

/**
   Sets the default gravity
 */
void PlayerPhysicsComponent::setGravity(int gravity){
  m_gravity = gravity;
}

/**
   Handle "JUMP" messages
 */
void PlayerPhysicsComponent::handleJumpMessage(GameObject* object){
  Vector velocity = object->getVelocity();

  // set vertical velocity
  velocity.setY(JUMP_VELOCITY);
    
  // apply x velocity change if doing turn with jump
  // In air right turn
  if(m_doAirRightTurn){
    double newHorizontalVelocity = JUMP_HORIZONTAL_VELOCITY;
    velocity.setX(newHorizontalVelocity);
  }
  // In air left turn
  if(m_doAirLeftTurn){
    double newHorizontalVelocity = (-1) * JUMP_HORIZONTAL_VELOCITY;
    velocity.setX(newHorizontalVelocity);
  }

  // set the new velocity
  object->setVelocity(velocity);

  m_state = State::AIRBORN;
}

/**
   handle "RIGHT" messages
 */
void PlayerPhysicsComponent::handleRightMessage(GameObject* object){
    Vector velocity = object->getVelocity();
    // If not airborn, then we can run and turn at normal velocity
    if(m_state != State::AIRBORN){
      m_doRun = true;
      velocity.setX(RUN_VELOCITY);
      object->setVelocity(velocity);
      m_state = State::WALK_RIGHT;
    }
    // If we ARE airborn, apply an acceleration to the turning
    else{
      // set a flag to be checked on next JUMP message
      m_doAirRightTurn = true;

      // You can turn a bit without doing an in air jump
      if(velocity.getY() <= 0.0){
	Vector acceleration = object->getAcceleration();
	acceleration.setX(JUMP_TURN_ACCELERATION);
	object->setAcceleration(acceleration);
      }
    }
}

/**
   handle "LEFT" messages
 */
void PlayerPhysicsComponent::handleLeftMessage(GameObject* object){
  Vector velocity = object->getVelocity();

  // If not airborn, then we can run and turn at normal velocity
  if(m_state != State::AIRBORN){
    m_doRun = true;
    velocity.setX(-RUN_VELOCITY);
    m_state = State::WALK_LEFT;
    object->setVelocity(velocity);
  }

  // if AIRBORN
  else{
    // set a flag to be checked on next JUMP message
    m_doAirLeftTurn = true;
	
    // You can turn a bit without doing an in air jump
    if(velocity.getY() >= 0.0){
      Vector acceleration = object->getAcceleration();
      acceleration.setX(-JUMP_TURN_ACCELERATION);
      object->setAcceleration(acceleration);
    }
  }
}

/**
   handle "STOPLEFT" and "STOPRIGHT" messages
 */
void PlayerPhysicsComponent::handleStopLeftRightMessage(GameObject* object){
  m_doRun = false;
  m_doAirRightTurn = false;
  m_doAirLeftTurn = false;
  Vector velocity = object->getVelocity();

  // on ground
  if(m_state != State::AIRBORN){
    // Update State
    m_state = velocity.getX() <= 0.0 ? State::STAND_LEFT : State::STAND_RIGHT;
    
    velocity.setX(0.0);
    object->setVelocity(velocity);
  }

  // in air
  else{
    // turn off in air turn acceleration
    Vector acceleration = object->getAcceleration();
    acceleration.setX(0.0);
    object->setAcceleration(acceleration);

    // turn off in air jump flag
    m_doAirRightTurn = false;
    m_doAirLeftTurn = false;
  }
}

/**
   Handle "LEFT BOUNDARY COLLISION" and "RIGHT BOUNDARY COLLISION" messages
 */
void PlayerPhysicsComponent::handleHorizontalBoundaryCollisionMessage(GameObject* object, CollisionType collisionType){
  CapEngine::Vector currentPosition = object->getPosition();
  CapEngine::real currentX = currentPosition.getX();

  if(collisionType == COLLISION_LEFT){
    currentPosition.setX(currentX + 1);
  }
  else if(collisionType == COLLISION_RIGHT){
    currentPosition.setX(currentX - 1);
  }
  else{
    // do nothing
  }
  object->setPosition(currentPosition);

  // update X velocity to 0
  CapEngine::Vector velocity = object->getVelocity();
  velocity.setX(0.0);
  object->setVelocity(velocity);
}

/**
   handle "TOP BOUNDARY COLLISION" messages
 */
void PlayerPhysicsComponent::handleTopBoundaryCollisionMessage(GameObject* object){
      CapEngine::Vector currentPosition = object->getPosition();
      CapEngine::real currentY = currentPosition.getY();
      currentPosition.setY(currentY + 1);
      object->setPosition(currentPosition);

      // update Y velocity to 0
      CapEngine::Vector velocity = object->getVelocity();
      velocity.setY(0.0);
      object->setVelocity(velocity);
}
