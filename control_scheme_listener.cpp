#include "control_scheme_listener.h"

void ControlSchemeListener::subscribe(ControlScheme* pControlScheme){
  pControlScheme->listen(this);
  m_pControlSchemes.push_back(pControlScheme);
}

ControlSchemeListener::~ControlSchemeListener(){
  for(size_t i = 0; i < m_pControlSchemes.size(); i++){
    (m_pControlSchemes[i])->unlisten(this);
  }
}
