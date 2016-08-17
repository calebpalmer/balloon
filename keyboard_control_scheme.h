#ifndef KEYBOARD_CONTROL_SCHEME
#define KEYBOARD_CONTROL_SCHEME

#include <map>
#include <memory>

#include "CapEngine.h"

#include "control_scheme.h"
#include "control_scheme_listener.h"

class KeyboardControlScheme : public ControlScheme,  public CapEngine::IEventSubscriber {
 public:
  KeyboardControlScheme();
  KeyboardControlScheme(std::map<SDL_Keycode, ControlScheme::Input> map);
  
  // ControlScheme
  virtual void listen(std::shared_ptr<ControlSchemeListener>) override;
  virtual void unlisten(ControlSchemeListener* listener) override;

  // IEventSubscriber
  virtual void receiveEvent(SDL_Event event, CapEngine::Time* time) override;

  void remapInput(SDL_Keycode keycode, ControlScheme::Input input);
  void loadMap(std::map<SDL_Keycode, ControlScheme::Input>);
  
 private:
  static std::map<SDL_Keycode, ControlScheme::Input> s_defaultMap;
  std::vector<std::shared_ptr<ControlSchemeListener> > m_pListeners;
  std::map<SDL_Keycode, ControlScheme::Input> m_keyMap;

  // disable copy constructor and assignment operator
  KeyboardControlScheme(const KeyboardControlScheme&);
  KeyboardControlScheme& operator=(const KeyboardControlScheme&);

  std::map<SDL_Keycode, ControlScheme::Input> getDefaultMap();
};

#endif // KEYBOARD_CONTROL_SCHEME
