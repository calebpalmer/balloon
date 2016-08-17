#ifndef PAUSE_STATE_H
#define PAUSE_STATE_H

#include <vector>

#include "CapEngine.h"

class PauseState : public CapEngine::GameState {
 public:
  PauseState(Uint32 windowID);
  ~PauseState();
  virtual void render();
  virtual void update(double ms);
  virtual bool onLoad();
  virtual bool onDestroy();
  
  static void returnToMenuCallback(void* context);
  static void resumeCallback(void* context);

 private:
  Uint32 m_windowID;
  bool m_escapePressed;
  bool m_switchToMenuState;
  bool m_resumePlayState;
  std::vector<std::unique_ptr<CapEngine::UIObject> > m_uiObjects;
};


#endif // PAUSE_STATE_H
