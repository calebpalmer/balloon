#include "player_physics_component.h"

#include <iostream>
#include <cmath>

#include "locator.h"
#include "logger.h"

using namespace CapEngine;
using namespace std;

void PlayerPhysicsComponent::update(GameObject* object, double timestep){
  // get current vectors
  Vector position = object->getPosition();
  Vector velocity = object->getVelocity();
  Vector acceleration = object->getAcceleration();

  // apply FRICTION if not runnig or airborn to slowdown
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
  Vector newVelocity = CapEngine::applyAcceleration(acceleration, velocity, timestep);
  bool maxHorizontalVelocityReached = std::abs(newVelocity.x) > RUN_VELOCITY ? true : false;
  if(maxHorizontalVelocityReached){
    newVelocity.x = (newVelocity.x < 0) ? (-1 * RUN_VELOCITY) : RUN_VELOCITY;
  }

  object->setVelocity(newVelocity);


  // apply velocity to displacement
  Vector newPosition = CapEngine::applyDisplacement(object->getVelocity(), object->getPosition(), timestep);
  object->setPosition(newPosition);

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
    m_state = State::NEUTRAL;

    // remove horizontal acceleration from mid-air turns
    Vector acceleration = object->getAcceleration();
    acceleration.setX(0.0);
    object->setAcceleration(acceleration);
  }
  return false;
}

void PlayerPhysicsComponent::receive(GameObject* object, int messageId, string message){
  if(message == "JUMP"){
    Vector velocity = object->getVelocity();
    velocity.setY(JUMP_VELOCITY);
    object->setVelocity(velocity);

    m_state = State::AIRBORN;
  }
  
  else if(message == "RIGHT"){
    m_doRun = true;
    Vector velocity = object->getVelocity();
    // If not airborn, then we can run and turn at normal velocity
    if(m_state != State::AIRBORN){
      velocity.setX(RUN_VELOCITY);
      object->setVelocity(velocity);
      m_state = State::RUNNING;
    }
    // If we ARE airborn, apply an acceleration to the turning
    else{
      if(velocity.getY() <= 0.0){
	Vector acceleration = object->getAcceleration();
	acceleration.setX(JUMP_TURN_ACCELERATION);
	object->setAcceleration(acceleration);
      }
    }
  }

  else if(message == "LEFT"){
    m_doRun = true;
    Vector velocity = object->getVelocity();
    // If not airborn, then we can run and turn at normal velocity
    if(m_state != State::AIRBORN){
      velocity.setX(-RUN_VELOCITY);
      m_state = State::RUNNING;
      object->setVelocity(velocity);
    }
    else{
      // If we ARE airborn, apply an acceleration to the turning
      if(velocity.getY() >= 0.0){
	Vector acceleration = object->getAcceleration();
	acceleration.setX(-JUMP_TURN_ACCELERATION);
	object->setAcceleration(acceleration);
      }
    }
  }

    else if (message == "STOPLEFT" || message == "STOPRIGHT"){
      m_doRun = false;
      Vector velocity = object->getVelocity();
      // on ground
      if(m_state != State::AIRBORN){
	velocity.setX(0.0);
	object->setVelocity(velocity);
	m_state = State::NEUTRAL;
      }
      // in air
      else{
	Vector acceleration = object->getAcceleration();
	acceleration.setX(0.0);
	object->setAcceleration(acceleration);
      }
    }

  else{
    // if(m_state != State::AIRBORN){
    //   m_state == State::NEUTRAL;
    // }
    // do nothing
  }
}
