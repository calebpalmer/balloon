#include "player_physics_component.h"

#include "locator.h"
#include "logger.h"
#include <iostream>

using namespace CapEngine;
using namespace std;

void PlayerPhysicsComponent::update(GameObject* object, double timestep){
  Vector position = object->getPosition();
  Vector velocity = object->getVelocity();
  Vector acceleration = object->getAcceleration();

  // apply friction if not runnig or airborn
  if(m_state == State::NEUTRAL && velocity.getX() != 0.0){
    if(velocity.getX() > 0.0){
      double deltaX = -RUN_FRICTION * (timestep / 1000);
      velocity.setX(velocity.getX() + deltaX);
      if(velocity.getX() < 0.0){
  	velocity.setX(0);
      }
    }

    else {
      double deltaX = RUN_FRICTION * (timestep / 1000);
      velocity.setX(velocity.getX() + deltaX);
      if(velocity.getX() > 0.0){
  	velocity.setX(0);
      }
    }
  }
  
  // apply gravity to velocity
  //Vector gravity(0.0, GRAVITY);
  Vector newVelocity = CapEngine::applyAcceleration(acceleration, velocity, timestep);
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
  if(message == "JUMP" && m_state != State::AIRBORN){
    Vector velocity = object->getVelocity();
    velocity.setY(JUMP_VELOCITY);
    object->setVelocity(velocity);

    m_state = State::AIRBORN;
  }
  
  else if(message == "GO RIGHT"){
    Vector velocity = object->getVelocity();
    if(m_state != State::AIRBORN){
      velocity.setX(RUN_VELOCITY);
      object->setVelocity(velocity);
      m_state = State::RUNNING;
    }
    else{
      if(velocity.getY() <= 0.0){
	Vector acceleration = object->getAcceleration();
	acceleration.setX(JUMP_TURN_ACCELERATION);
	object->setAcceleration(acceleration);
      }
    }
  }

  else if(message == "GO LEFT"){
    Vector velocity = object->getVelocity();
    if(m_state != State::AIRBORN){
      velocity.setX(-RUN_VELOCITY);
      m_state = State::RUNNING;
      object->setVelocity(velocity);
    }
    else{
      if(velocity.getY() >= 0.0){
	Vector acceleration = object->getAcceleration();
	acceleration.setX(-JUMP_TURN_ACCELERATION);
	object->setAcceleration(acceleration);
      }
    }
  }

  else{
    if(m_state != State::AIRBORN){
      m_state == State::NEUTRAL;
    }
  }
}
