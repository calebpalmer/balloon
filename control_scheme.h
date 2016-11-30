#ifndef CONTROL_SCHEME_H
#define CONTROL_SCHEME_H

#include <memory>
#include <vector>

  // forward declaration
  class ControlSchemeListener;

/*
  This class is an abstract interface describing the interface a ControlScheme implementation must follow.
  This allows us to provide multiple Control Schemes whether it is via different hardware or mapping buttons
  to other inputs.
 */
class ControlScheme {
 public:  
  virtual ~ControlScheme() {}
  virtual void listen(ControlSchemeListener* pListener);
  virtual void unlisten(ControlSchemeListener* pListener) ;
  virtual void disable();
  virtual void enable();

 protected:
  bool m_enabled;
  std::vector<ControlSchemeListener*> m_pListeners;
};

#endif // KEYBOARD_CONTROL_SCHEME
