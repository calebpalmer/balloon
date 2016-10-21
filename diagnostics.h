#ifndef DIAGNOSTICS_H
#define DIAGNOSTICS_H

#include <string>

#include "CapEngine.h"

#include "player_physics_component.h"

#define PADDING 4

struct DiagnosticData {
  float fps;
  PlayerPhysicsComponent::State m_state;
  CapEngine::Vector position;
  CapEngine::Vector velocity;
  CapEngine::Vector acceleration;
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
};
#endif // DIAGNOSTICS_H
