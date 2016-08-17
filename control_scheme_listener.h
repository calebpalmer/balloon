#ifndef CONTROL_SCHEME_LISTENER
#define CONTROL_SCHEME_LISTENER

#include "control_scheme.h"

class ControlSchemeListener {
 public:
  virtual void receiveInput(ControlScheme::Input input) = 0;

};

#endif // CONTROL_SCHEME_LISTENER
