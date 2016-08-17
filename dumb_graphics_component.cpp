#include "dumb_graphics_component.h"

using namespace CapEngine;

DumbGraphicsComponent::DumbGraphicsComponent(Uint32 windowID, int width, int height, Colour colour) :
  m_windowID(windowID), m_width(width), m_height(height), m_colour(colour), m_pTexture(nullptr) {}

DumbGraphicsComponent::~DumbGraphicsComponent(){
  if(m_pTexture != nullptr){
    Locator::videoManager->closeTexture(m_pTexture);
  }
}

void DumbGraphicsComponent::render(GameObject* object){
  if(m_pTexture == nullptr){
    Surface* pSurface = CapEngine::createRectangle2(m_width, m_height, m_colour);
    m_pTexture = Locator::videoManager->createTextureFromSurface(pSurface, true);
  }

  Vector position = object->getPosition();
  // position is center based
  int x = position.x - (m_width / 2);
  int y = position.y - (m_height / 2);
  Locator::videoManager->drawTexture(m_windowID, x, y, m_pTexture);
}
