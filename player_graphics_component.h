#ifndef PLAYER_GRAPHICS_COMPONENT_H
#define PLAYER_GRAPHICS_COMPONENT_H

#include "player_physics_component.h"

#include "CapEngine.h"

class PlayerGraphicsComponent : public CapEngine::GraphicsComponent {
public:
  PlayerGraphicsComponent(Uint32 windowID, int width, int height);
  ~PlayerGraphicsComponent();
  void render(CapEngine::GameObject* object) override;
  void update(CapEngine::GameObject* pObject, double ms);

private:
  Uint32 m_windowID;
  int m_width;
  int m_height;
  double m_timeMS;
  double m_timePerFrameMS;
  int m_currentFrame;
  int m_currentRow;
  PlayerPhysicsComponent::State m_state;
  PlayerPhysicsComponent::State m_previousState;
};

#endif // PLAYER_GRAPHICS_COMPONENT
