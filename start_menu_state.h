#ifndef START_MENU_STATE_H
#define START_MENU_STATE_H

#include "CapEngine.h"

#include <vector>
#include <string>
#include <memory>

#include "control_scheme_listener.h"

class StartMenuState : public CapEngine::GameState, public ControlSchemeListener {
 public:
  StartMenuState(Uint32 windowID);
  ~StartMenuState();
  virtual void render() override; // GameState
  virtual void update(double ms) override; // GameState
  virtual bool onLoad() override; // GameState
  virtual bool onDestroy() override; // GameState
  virtual void receiveInput(std::string input) override; //ControlSchemeListener

  static void startButtonCallback(void* context);
  static void exitButtonCallback(void* context);
  static void arenaButtonCallback(void* context);

 protected:
  StartMenuState(const StartMenuState&);
  StartMenuState& operator=(const StartMenuState&);

  std::vector<std::unique_ptr<CapEngine::UIObject>> m_uiObjects;
  Uint32 m_windowID;
  bool m_loaded;
  bool m_startNewGame;
  bool m_startArenaGame;
  bool m_exitGame;
  long m_soundId;
};

#endif // START_MENU_STATE
