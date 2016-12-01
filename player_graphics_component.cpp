#include "player_graphics_component.h"

#include "assets.h"

using namespace CapEngine;

PlayerGraphicsComponent::PlayerGraphicsComponent(Uint32 windowID, int width, int height) :
  m_windowID(windowID), m_width(width), m_height(height), m_pTexture(nullptr) {}

PlayerGraphicsComponent::~PlayerGraphicsComponent(){
  if(m_pTexture != nullptr){
    Locator::videoManager->closeTexture(m_pTexture);
  }
}

void PlayerGraphicsComponent::render(GameObject* object){
  Vector position = object->getPosition();
  // position is center based
  int x = position.x - (m_width / 2);
  int y = position.y - (m_height / 2);
  Rectangle rect = {static_cast<float>(x), static_cast<float>(y), 40, 60};
  //Locator::videoManager->drawTexture(m_windowID, x, y, m_pTexture);
  Locator::assetManager->draw(m_windowID, ASSET_PLAYER_TEXTURE, rect, 0, 0);
}

