#include "diagnostics.h"

#include <iostream>
#include <sstream>

/** 
    Displays diagnostics info on the given Window
 **/
void Diagnostics::display(Uint32 windowID, DiagnosticData data, double x, double y,
			  std::string font, int fontSize)
{
  // get texture for fps
  std::ostringstream fpsStream;
  fpsStream << data.fps;
  CapEngine::Surface* fpsSurface = getSurface(fpsStream.str(), font, fontSize);
  CapEngine::Texture* fpsTexture = CapEngine::Locator::videoManager->createTextureFromSurface(fpsSurface, true);
  
  // get textures for state information
  CapEngine::Surface* stateSurface = getStateSurface(data.m_state, font, fontSize);
  CapEngine::Texture*  stateTexture = CapEngine::Locator::videoManager->createTextureFromSurface(stateSurface, true); // true meaning free surface after

  // draw textures
  CapEngine::Locator::videoManager->drawTexture(windowID, x, y, fpsTexture);
  double textureHeight = CapEngine::Locator::videoManager->getTextureHeight(fpsTexture);
  int newY = y + textureHeight + PADDING;
  CapEngine::Locator::videoManager->drawTexture(windowID, x, newY, stateTexture);

  // free textures
  CapEngine::Locator::videoManager->closeTexture(stateTexture);
  CapEngine::Locator::videoManager->closeTexture(fpsTexture);
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
