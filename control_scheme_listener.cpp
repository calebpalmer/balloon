#include "control_scheme_listener.h"

/**
   This registers the listener with a specific control scheme
 */
void ControlSchemeListener::subscribe(ControlScheme* pControlScheme){
  pControlScheme->listen(this);
  m_pControlSchemes.push_back(pControlScheme);
}

/**
   Destructor:  unregisters from the ControlScheme
 */
ControlSchemeListener::~ControlSchemeListener(){
  for(size_t i = 0; i < m_pControlSchemes.size(); i++){
    (m_pControlSchemes[i])->unlisten(this);
  }
}
