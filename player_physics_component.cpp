#include "player_physics_component.h"

#include <iostream>
#include <cmath>

#include "locator.h"
#include "logger.h"

using namespace CapEngine;
using namespace std;

void PlayerPhysicsComponent::update(GameObject* object, double timestep){
  Vector velocity = object->getVelocity();

  // apply FRICTION if not running or airborn to slowdown
  if(!m_doRun && m_state == State::NEUTRAL && velocity.getX() != 0.0){
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
    if(m_state != State::RUNNING){
      m_state = State::NEUTRAL;
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

  }
  return false;
}

void PlayerPhysicsComponent::receive(GameObject* object, int messageId, string message){
  if(message == "JUMP"){
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
  
  else if(message == "RIGHT"){
    Vector velocity = object->getVelocity();
    // If not airborn, then we can run and turn at normal velocity
    if(m_state != State::AIRBORN){
      m_doRun = true;
      velocity.setX(RUN_VELOCITY);
      object->setVelocity(velocity);
      m_state = State::RUNNING;
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

  else if(message == "LEFT"){
    Vector velocity = object->getVelocity();
    // If not airborn, then we can run and turn at normal velocity
    if(m_state != State::AIRBORN){
      m_doRun = true;
      velocity.setX(-RUN_VELOCITY);
      m_state = State::RUNNING;
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

    else if (message == "STOPLEFT" || message == "STOPRIGHT"){
      m_doRun = false;
      m_doAirRightTurn = false;
      m_doAirLeftTurn = false;
      Vector velocity = object->getVelocity();
      // on ground
      if(m_state != State::AIRBORN){
	velocity.setX(0.0);
	object->setVelocity(velocity);
	m_state = State::NEUTRAL;
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

    else if (message == "LEFT BOUNDARY COLLISION"){
      CapEngine::Vector currentPosition = object->getPosition();
      CapEngine::real currentX = currentPosition.getX();
      currentPosition.setX(currentX + 1);
      object->setPosition(currentPosition);

      // update X velocity to 0
      CapEngine::Vector velocity = object->getVelocity();
      velocity.setX(0.0);
      object->setVelocity(velocity);
    }

    else if (message == "RIGHT BOUNDARY COLLISION"){
      CapEngine::Vector currentPosition = object->getPosition();
      CapEngine::real currentX = currentPosition.getX();
      currentPosition.setX(currentX - 1);
      object->setPosition(currentPosition);

      // update X velocity to 0
      CapEngine::Vector velocity = object->getVelocity();
      velocity.setX(0.0);
      object->setVelocity(velocity);

    }

    else if (message == "TOP BOUNDARY COLLISION"){
      CapEngine::Vector currentPosition = object->getPosition();
      CapEngine::real currentY = currentPosition.getY();
      currentPosition.setY(currentY + 1);
      object->setPosition(currentPosition);

      // update Y velocity to 0
      CapEngine::Vector velocity = object->getVelocity();
      velocity.setY(0.0);
      object->setVelocity(velocity);
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
