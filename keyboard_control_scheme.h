#ifndef KEYBOARD_CONTROL_SCHEME
#define KEYBOARD_CONTROL_SCHEME

#include <map>
#include <memory>
#include <string>

#include "CapEngine.h"

#include "control_scheme.h"
#include "control_scheme_listener.h"

enum EventType {
  EventType_Unknown,
  EventType_KeyUp,
  EventType_KeyDown,
  EventType_KeyPressAndHold
};
  
class KeyInput{
  public:
  KeyInput() :
    m_keycode(SDL_SCANCODE_RETURN2), m_eventType(EventType_Unknown) {}
    
  KeyInput(SDL_Keycode keycode, EventType eventType) :
    m_keycode(keycode), m_eventType(eventType) {}

    bool operator==(const KeyInput& rightHandSide) {
      return (this->m_keycode == rightHandSide.m_keycode) && (this->m_eventType == rightHandSide.m_eventType);
    }

    SDL_Keycode m_keycode;
    EventType m_eventType;
  };


class KeyboardControlScheme : public ControlScheme,  public CapEngine::IEventSubscriber {
 public:
  KeyboardControlScheme();
  KeyboardControlScheme(std::vector<std::pair<KeyInput, std::string> > map);
  
  // ControlScheme
  virtual void listen(ControlSchemeListener* pListener) override;
  virtual void unlisten(ControlSchemeListener* pListener) override;
  virtual void enable() override;
  virtual void disable() override;

  // IEventSubscriber
  virtual void receiveEvent(SDL_Event event, CapEngine::Time* time) override;

  void remapInput(KeyInput input, std::string);
  void loadMap(std::vector<KeyInput, std::string>);
  
 private:
  bool m_enabled;
  static std::vector<std::pair<KeyInput, std::string> > s_defaultMap;
  std::vector<ControlSchemeListener*> m_pListeners;
  std::vector<std::pair<KeyInput, std::string> > m_keyMap;

  // disable copy constructor and assignment operator
  KeyboardControlScheme(const KeyboardControlScheme&);
  KeyboardControlScheme& operator=(const KeyboardControlScheme&);
  
  std::vector<std::pair<KeyInput, std::string> > getDefaultMap();
  void registerEvents();
};

#endif // KEYBOARD_CONTROL_SCHEME
