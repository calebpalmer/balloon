#include "diagnostics.h"

#include <iostream>
#include <sstream>

/** 
    Displays diagnostics info on the given Window
 **/
void Diagnostics::display(Uint32 windowID, DiagnosticData data, double x, double y,
			  std::string font, int fontSize)
{
  // draw fps
  std::ostringstream fpsStream;
  fpsStream << data.fps;
  CapEngine::Surface* fpsSurface = getSurface(fpsStream.str(), font, fontSize);
  CapEngine::Texture* fpsTexture = CapEngine::Locator::videoManager->createTextureFromSurface(fpsSurface, true);
  double textureHeight = CapEngine::Locator::videoManager->getTextureHeight(fpsTexture);
  int newY = y + textureHeight + PADDING;
  CapEngine::Locator::videoManager->drawTexture(windowID, x, y, fpsTexture);
  CapEngine::Locator::videoManager->closeTexture(fpsTexture);

  // draw state
  CapEngine::Surface* stateSurface = getStateSurface(data.m_state, font, fontSize);
  CapEngine::Texture*  stateTexture = CapEngine::Locator::videoManager->createTextureFromSurface(stateSurface, true); // true meaning free surface after
  CapEngine::Locator::videoManager->drawTexture(windowID, x, newY, stateTexture);
  CapEngine::Locator::videoManager->closeTexture(stateTexture);

  // draw position
  CapEngine::Texture* positionTexture = getVectorTexture(data.position, "Position: ", font, fontSize);
  newY = y + 2 * (textureHeight + PADDING);
  CapEngine::Locator::videoManager->drawTexture(windowID, x, newY, positionTexture);
  CapEngine::Locator::videoManager->closeTexture(positionTexture);

  // draw velocity
  CapEngine::Texture* velocityTexture = getVectorTexture(data.position, "Velocity: ", font, fontSize);
  newY = y + 3 * (textureHeight + PADDING);
  CapEngine::Locator::videoManager->drawTexture(windowID, x, newY, velocityTexture);
  CapEngine::Locator::videoManager->closeTexture(velocityTexture);

  // draw acceleration
  CapEngine::Texture* accelerationTexture = getVectorTexture(data.position, "Acceleration: ", font, fontSize);
  newY = y + 4 * (textureHeight + PADDING);
  CapEngine::Locator::videoManager->drawTexture(windowID, x, newY, accelerationTexture);
  CapEngine::Locator::videoManager->closeTexture(accelerationTexture);
}

/**
   returns a Surface representing a PlayerPhysicscomponent::State
 */
CapEngine::Surface* Diagnostics::getStateSurface(PlayerPhysicsComponent::State state, std::string font, int fontSize){

  // get the description
  std::string stateDescription;
  switch(state){
  case PlayerPhysicsComponent::AIRBORN:
    stateDescription = "AIRBORN";
    break;
  case PlayerPhysicsComponent::NEUTRAL:
    stateDescription = "NEUTRAL";
    break;
  case PlayerPhysicsComponent::RUNNING:
    stateDescription = "RUNNING";
    break;
  default:
    stateDescription = "Unknown";
    break;
  }

  //std::cout << stateDescription << std::endl;
  // get surface
  CapEngine::Surface* surface = getSurface(stateDescription, font, fontSize);
  return surface;
}

CapEngine::Surface* Diagnostics::getSurface(std::string value, std::string font, int fontSize){
  // get surface
  static CapEngine::FontManager fontManager;
  CapEngine::Surface* surface = fontManager.getTextSurface(font, value, fontSize,
							   0x00, 0x00, 0x00);
  return surface;
}

/**
   returns Texture for given Surface.  Frees the Surface.
 */
CapEngine::Texture* getTexture(CapEngine::Surface* surface){
  CapEngine::Texture* texture = CapEngine::Locator::videoManager->createTextureFromSurface(surface, true);
  return texture;
}

/**

   Create Texture for Vector Diagnostics
 */
CapEngine::Texture* Diagnostics::getVectorTexture(CapEngine::Vector vector, std::string label, std::string font, int fontSize){
  std::string vectorText = vector.toString();
  std::stringstream textStream;
  textStream << label << vectorText;
  CapEngine::Surface* surface = getSurface(textStream.str(), font, fontSize);
  CapEngine::Texture* texture = CapEngine::Locator::videoManager->createTextureFromSurface(surface, true);
  return texture;
}
