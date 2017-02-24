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

void ControlSchemeListener::unsubscribe(ControlScheme *pControlScheme){
  for(size_t i = 0; i < m_pControlSchemes.size(); i++){
    if( m_pControlSchemes[i] == pControlScheme ){
      (m_pControlSchemes[i])->unlisten(this);
      m_pControlSchemes.erase( m_pControlSchemes.begin() + i );
    }
  }

}

void ControlSchemeListener::unsubscribeAll(){
  for(auto && controlScheme : m_pControlSchemes){
    controlScheme->unlisten(this);
  }
  m_pControlSchemes.clear();
}
