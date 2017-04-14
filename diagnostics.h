#ifndef DIAGNOSTICS_H
#define DIAGNOSTICS_H

#include <string>
#include <vector>

#include "CapEngine.h"

#include "player_physics_component.h"

#define PADDING 4

struct DiagnosticData {
  float fps;
  PlayerPhysicsComponent::State m_state;
  CapEngine::Vector position;
  CapEngine::Vector velocity;
  CapEngine::Vector acceleration;
  CapEngine::Matrix mainViewTransform;
};

class Diagnostics {
 public:
  static void display(Uint32 windowID, DiagnosticData data, double x, double y,
		      std::string font, int fontSize);

 private:
  static CapEngine::Surface* getStateSurface(PlayerPhysicsComponent::State state,
					     std::string font, int fontSize);
  static CapEngine::Surface* getSurface(std::string value, std::string font, int fontSize);
  static CapEngine::Texture* getTexture(CapEngine::Surface* surface);
  static CapEngine::Texture* getVectorTexture(CapEngine::Vector vector, std::string label,
					      std::string font, int fontSize);
  static std::vector<CapEngine::Texture*> getMatrixTextures(CapEngine::Matrix, const std::string& label,
					      const std::string& font, int fontSize);
  static void drawTextures(std::vector<CapEngine::Texture*> textures, Uint32 windowID, int x, int y, int padding);
};
#endif // DIAGNOSTICS_H
