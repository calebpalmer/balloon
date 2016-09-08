#ifndef CONTROL_SCHEME_LISTENER
#define CONTROL_SCHEME_LISTENER

#include <string>
#include <vector>

#include "control_scheme.h"

class ControlSchemeListener {
 public:
  virtual void receiveInput(std::string input) = 0;
  virtual void subscribe(ControlScheme* pControlScheme);
  virtual ~ControlSchemeListener();

 private:
  std::vector<ControlScheme*> m_pControlSchemes;
};

#endif // CONTROL_SCHEME_LISTENER
