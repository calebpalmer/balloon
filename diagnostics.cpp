#include "diagnostics.h"

#include <iostream>
#include <sstream>
#include <algorithm>

/** 
    Displays diagnostics info on the given Window
 **/
void Diagnostics::display(Uint32 windowID, DiagnosticData data, double x, double y,
			  std::string font, int fontSize)
{
  std::vector<CapEngine::Texture*> textures;

  // draw fps
  std::ostringstream fpsStream;
  fpsStream << data.fps;
  CapEngine::Surface* fpsSurface = getSurface(fpsStream.str(), font, fontSize);
  CapEngine::Texture* fpsTexture = CapEngine::Locator::videoManager->createTextureFromSurface(fpsSurface, true);
  textures.push_back(fpsTexture);

  // draw state
  CapEngine::Surface* stateSurface = getStateSurface(data.m_state, font, fontSize);
  CapEngine::Texture*  stateTexture = CapEngine::Locator::videoManager->createTextureFromSurface(stateSurface, true); // true meaning free surface after
  textures.push_back(stateTexture);

  // draw position
  CapEngine::Texture* positionTexture = getVectorTexture(data.position, "Position: ", font, fontSize);
  textures.push_back(positionTexture);

  // draw velocity
  CapEngine::Texture* velocityTexture = getVectorTexture(data.velocity, "Velocity: ", font, fontSize);
  textures.push_back(velocityTexture);

  // draw acceleration
  CapEngine::Texture* accelerationTexture = getVectorTexture(data.acceleration, "Acceleration: ", font, fontSize);
  textures.push_back(accelerationTexture);

  // draw matrix
  std::vector<CapEngine::Texture*> matrixTextures = getMatrixTextures(data.mainViewTransform, "Matrix", font, fontSize);
  textures.insert(textures.end(), matrixTextures.begin(), matrixTextures.end());

  drawTextures(textures, windowID, x, y, PADDING);
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
  case PlayerPhysicsComponent::WALK_LEFT:
    stateDescription = "WALK_LEFT";
    break;
  case PlayerPhysicsComponent::WALK_RIGHT:
    stateDescription = "WALK_RIGHT";
    break;
  case PlayerPhysicsComponent::STAND_LEFT:
    stateDescription = "STAND_LEFT";
    break;
  case PlayerPhysicsComponent::STAND_RIGHT:
    stateDescription = "STAND_RIGHT";
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

std::vector<CapEngine::Texture*> Diagnostics::getMatrixTextures(CapEngine::Matrix matrix, const std::string& label,
						  const std::string& font, int fontSize)
{
  std::vector<CapEngine::Texture*> textures;

  CapEngine::Surface* labelSurface = getSurface(label, font, fontSize);
  CapEngine::Texture* labelTexture = CapEngine::Locator::videoManager->createTextureFromSurface(labelSurface, true);
  //CapEngine::Locator::videoManager->closeSurface(labelSurface);
  textures.push_back(labelTexture);
  CapEngine::Texture* rowOneTexture = getVectorTexture(matrix.getRowVector(0), "", font, fontSize);
  textures.push_back(rowOneTexture);
  CapEngine::Texture* rowTwoTexture = getVectorTexture(matrix.getRowVector(1), "", font, fontSize);
  textures.push_back(rowTwoTexture);
  CapEngine::Texture* rowThreeTexture = getVectorTexture(matrix.getRowVector(2), "", font, fontSize);
  textures.push_back(rowThreeTexture);  
  CapEngine::Texture* rowFourTexture = getVectorTexture(matrix.getRowVector(3), "", font, fontSize);
  textures.push_back(rowFourTexture);

  return textures;
}

/**
   draws textures starting at given (x,y).  Frees the texture after drawing it.
 */
void Diagnostics::drawTextures(std::vector<CapEngine::Texture*> textures, Uint32 windowID, int x, int y, int padding){
  int yStart = y;
  for(auto & i : textures){
    CapEngine::Locator::videoManager->drawTexture(windowID, x, yStart, i, nullptr, false);
    
    CapEngine::real textureHeight = CapEngine::Locator::videoManager->getTextureHeight(i);
    yStart = yStart + textureHeight + padding;
    
    CapEngine::Locator::videoManager->closeTexture(i);
  }
}
