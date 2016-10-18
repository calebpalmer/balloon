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
  std::string positionStr = (data.position).toString();
  std::stringstream positionStream;
  positionStream << "Position: " << positionStr;
  CapEngine::Surface* positionSurface = getSurface(positionStream.str(), font, fontSize);
  CapEngine::Texture* positionTexture = CapEngine::Locator::videoManager->createTextureFromSurface(positionSurface, true);
  newY = y + 2 * (textureHeight + PADDING);
  CapEngine::Locator::videoManager->drawTexture(windowID, x, newY, positionTexture);
  CapEngine::Locator::videoManager->closeTexture(positionTexture);

    // draw velocity
  std::string velocityStr = (data.velocity).toString();
  std::stringstream velocityStream;
  velocityStream << "Velocity: " << velocityStr;
  CapEngine::Surface* velocitySurface = getSurface(velocityStream.str(), font, fontSize);
  CapEngine::Texture* velocityTexture = CapEngine::Locator::videoManager->createTextureFromSurface(velocitySurface, true);
  newY = y + 3 * (textureHeight + PADDING);
  CapEngine::Locator::videoManager->drawTexture(windowID, x, newY, velocityTexture);
  CapEngine::Locator::videoManager->closeTexture(velocityTexture);

  // draw acceleration
  std::string accelerationStr = (data.acceleration).toString();
  std::stringstream accelerationStream;
  accelerationStream << "Acceleration: " << accelerationStr;
  CapEngine::Surface* accelerationSurface = getSurface(accelerationStream.str(), font, fontSize);
  CapEngine::Texture* accelerationTexture = CapEngine::Locator::videoManager->createTextureFromSurface(accelerationSurface, true);
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
