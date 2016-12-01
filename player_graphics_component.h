#ifndef PLAYER_GRAPHICS_COMPONENT_H
#define PLAYER_GRAPHICS_COMPONENT_H

#include "CapEngine.h"

class PlayerGraphicsComponent : public CapEngine::GraphicsComponent {
public:
  PlayerGraphicsComponent(Uint32 windowID, int width, int height);
  ~PlayerGraphicsComponent();
  void render(CapEngine::GameObject* object) override;

private:
  Uint32 m_windowID;
  int m_width;
  int m_height;
  CapEngine::Texture* m_pTexture;
 
};

#endif // PLAYER_GRAPHICS_COMPONENT
