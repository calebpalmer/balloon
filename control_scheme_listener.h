#ifndef CONTROL_SCHEME_LISTENER
#define CONTROL_SCHEME_LISTENER

#include <string>
#include <vector>

#include "control_scheme.h"

/**
   This class defines an object that listens to a ControlScheme(s) for actions
   associated with that ControlScheme.
 */
class ControlSchemeListener {
 public:
  /**
     Receives a message from a ControlScheme
     @param input The message received.
   */
  virtual void receiveInput(std::string input) = 0;

  /**
     Registers itself to listen for messages from the give ControlScheme
     #param pControlScheme the ControlScheme to listen to.
   */
  virtual void subscribe(ControlScheme* pControlScheme);
  
  virtual ~ControlSchemeListener();

 private:
  std::vector<ControlScheme*> m_pControlSchemes;  // keeps track of ControlSchemes listening to.
}; 

#endif // CONTROL_SCHEME_LISTENER
