#include "player_graphics_component.h"

#include "assets.h"

#include <cassert>
#include <memory>
#include <sstream>

using namespace CapEngine;

const std::string kWalkRightFrameName = "Walk Right";
const std::string kStandingFrame = "Stand Right";

PlayerGraphicsComponent::PlayerGraphicsComponent(Uint32 windowID, int width, int height) :
  m_windowID(windowID), m_width(width), m_timeMS(0.0), m_currentFrame(0), m_currentRow(0),
  m_height(height), m_state(PlayerPhysicsComponent::UNKNOWN) {}

PlayerGraphicsComponent::~PlayerGraphicsComponent(){ }

void PlayerGraphicsComponent::render(GameObject* object){
  Vector position = object->getPosition();
  // position is center based
  int x = position.x - (m_width / 2);
  int y = position.y - (m_height / 2);
  Rectangle rect = {static_cast<float>(x), static_cast<float>(y), 40, 60};
  //Locator::assetManager->draw(m_windowID, ASSET_PLAYER_TEXTURE, rect, 0, 0);
  Locator::assetManager->draw(m_windowID, ASSET_PLAYER_TEXTURE, rect, m_currentRow, m_currentFrame);
}

void PlayerGraphicsComponent::update(GameObject* pObject, double ms){
  assert(pObject != nullptr);

  std::shared_ptr<PhysicsComponent>  pPhysicsComponent = pObject->getPhysicsComponent();
  auto pPlayerPhysicsComponent = dynamic_cast<PlayerPhysicsComponent*>(pPhysicsComponent.get());
  assert(pPlayerPhysicsComponent != nullptr);
	 
  PlayerPhysicsComponent::State previousState = m_state;
  m_state = pPlayerPhysicsComponent->getState();

  std::string frameName = "";
  switch(m_state){
  case PlayerPhysicsComponent::RUNNING:
    frameName = kWalkRightFrameName;
    break;
  case PlayerPhysicsComponent::NEUTRAL:
    frameName = kStandingFrame;
    break;
  default:
    frameName = "Unknown";
    break;
  }

  
  Image* pImage = Locator::assetManager->getImage(ASSET_PLAYER_TEXTURE);
  if(pImage->frames.find(frameName) != pImage->frames.end()){
    Frame frame = pImage->frames[frameName];
    m_timePerFrameMS = frame.animationTime / m_timeMS;

    // update current time
    // new animation
    if(m_state != previousState){
      m_timeMS = 0.0;
      m_currentRow = frame.rowNum;
      m_currentFrame = 0;
    }
    // update existing animation
    else{
      m_timeMS += ms;
      if(m_timeMS >= m_timePerFrameMS){
	m_timeMS -= m_timePerFrameMS;
	m_currentFrame = (m_currentFrame + 1) % frame.numFrames;
      }
    }
  }

  else{
    std::ostringstream msg;
    msg << "Frame \"" << frameName << "\" does not exist";
    Locator::logger->log(msg.str(), Logger::CDEBUG);
  }
}
