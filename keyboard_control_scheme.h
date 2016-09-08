#ifndef KEYBOARD_CONTROL_SCHEME
#define KEYBOARD_CONTROL_SCHEME

#include <map>
#include <memory>
#include <string>

#include "CapEngine.h"

#include "control_scheme.h"
#include "control_scheme_listener.h"

enum EventType {
  EventType_KeyUp,
  EventType_KeyDown,
  EventType_KeyPressAndHold
};
  
  class KeyInput{
  public:
  KeyInput(std::string input, EventType eventType) :
    m_input(input), m_eventType(eventType) {}

    std::string m_input;
    EventType m_eventType;
  };


class KeyboardControlScheme : public ControlScheme,  public CapEngine::IEventSubscriber {
 public:
  KeyboardControlScheme();
  KeyboardControlScheme(std::map<SDL_Keycode, KeyInput> map);
  
  // ControlScheme
  virtual void listen(ControlSchemeListener* pListener) override;
  virtual void unlisten(ControlSchemeListener* pListener) override;
  virtual void enable() override;
  virtual void disable() override;

  // IEventSubscriber
  virtual void receiveEvent(SDL_Event event, CapEngine::Time* time) override;

  void remapInput(SDL_Keycode keycode, KeyInput input);
  void loadMap(std::map<SDL_Keycode, KeyInput>);
  
 private:
  bool m_enabled;
  static std::map<SDL_Keycode, KeyInput> s_defaultMap;
  std::vector<ControlSchemeListener*> m_pListeners;
  std::map<SDL_Keycode, KeyInput> m_keyMap;
  std::map<SDL_Keycode, bool> m_keyPressAndHoldStates;

  // disable copy constructor and assignment operator
  KeyboardControlScheme(const KeyboardControlScheme&);
  KeyboardControlScheme& operator=(const KeyboardControlScheme&);
  
  std::map<SDL_Keycode, KeyInput> getDefaultMap();
  void registerEvents();
};

#endif // KEYBOARD_CONTROL_SCHEME
