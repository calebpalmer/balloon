#ifndef DUMB_GRAPHICS_COMPONENT_H
#define DUMB_GRAPHICS_COMPONENT_H

#include "CapEngine.h"

class DumbGraphicsComponent : public CapEngine::GraphicsComponent {
public:
  DumbGraphicsComponent(Uint32 windowID, int width, int height, CapEngine::Colour colour);
  ~DumbGraphicsComponent();
  void render(CapEngine::GameObject* object) override;
  void update(CapEngine::GameObject* pObject, double ms) override;

private:
  Uint32 m_windowID;
  CapEngine::Colour m_colour;
  int m_width;
  int m_height;
  CapEngine::Texture* m_pTexture;
 
};

#endif // DUMB_GRAPHICS_COMPONENT
