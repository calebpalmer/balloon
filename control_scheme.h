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
  enum Input {
    JUMP,
    GO_LEFT,
    GO_RIGHT,
    MENU
  };

  virtual ~ControlScheme() {}
  virtual void listen(std::shared_ptr<ControlSchemeListener> listener) = 0;
  virtual void unlisten(ControlSchemeListener* listener) = 0;

 protected:
  const unsigned int m_numInputs = 4;

};

#endif // KEYBOARD_CONTROL_SCHEME
