#ifndef ARENA_PLAY_STATE_H
#define ARENA_PLAY_STATE_H

#include "CapEngine.h"

#include "control_scheme_listener.h"

class ArenaPlayState : public CapEngine::GameState, public CapEngine::IEventSubscriber, public ControlSchemeListener {
 public:
  ArenaPlayState(Uint32 windowID, int arenaID);
  virtual ~ArenaPlayState() {};
  virtual void render() override; // GameState
  virtual void update(double ms) override;  //GameState
  virtual bool onLoad() override; // GameState
  virtual bool onDestroy() override; // GameState
  virtual void receiveEvent(const SDL_Event event, CapEngine::Time* time) override; // IEventSubscriber
  virtual void receiveInput(std::string input) override; // ControlSchemeListener

 private:
  CapEngine::PlatformerMap buildPlatformerMap(std::string arenaConfigPath, int arenaID);
  void handleBoundaryCollisions(CapEngine::GameObject* pPlayer, CapEngine::Rectangle boundary);
  void checkControllerConnectionEvents(const SDL_Event event);
  void setControlScheme(std::shared_ptr<ControlScheme>);

  const int GRAVITY = 300;
  
  Uint32 m_windowID;
  CapEngine::PlatformerMap m_platformerMap;
  int m_arenaID;
  std::shared_ptr<CapEngine::GameObject> m_pPlayer;
  std::shared_ptr<ControlScheme> m_pControlScheme;
  bool m_startButtonPressed;
  bool m_showDiagnostics;
  std::vector<std::unique_ptr<CapEngine::Animation> > m_animations;
};

#endif // ARENA_PLAY_STATE_H
