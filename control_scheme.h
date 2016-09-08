#ifndef CONTROL_SCHEME_H
#define CONTROL_SCHEME_H

#include <memory>

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
  virtual void listen(ControlSchemeListener* pListener) = 0;
  virtual void unlisten(ControlSchemeListener* pListener) = 0;
  virtual void disable() = 0;
  virtual void enable() = 0;
};

#endif // KEYBOARD_CONTROL_SCHEME
