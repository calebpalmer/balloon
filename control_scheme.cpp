#include "control_scheme.h"

#include "control_scheme_listener.h"

using namespace std;

/**
   Registers listeners for Input events
 */
void ControlScheme::listen(ControlSchemeListener* pListener){
  // Make sure that this is not already a listener
  for (auto  & i : m_pListeners){
    if(i == pListener){
      return;
    }
  }
  // add listener to list of listeners
  m_pListeners.push_back(pListener);
}

/*
  Remove the provided ControlSchemeListener as an active listener.
 */
void ControlScheme::unlisten(ControlSchemeListener* pListener){
  auto i = m_pListeners.begin();
  for(; i != m_pListeners.end(); i++){
    if(*i == pListener){
	break;
      }
  }
  if(i != m_pListeners.end()){
    m_pListeners.erase(i);
  }
}

/**
   Enables the ControlScheme
 */
void ControlScheme::enable(){
  m_enabled = true;
}


/**
   Disables the ControlScheme
 */ 
void ControlScheme::disable(){
  m_enabled = false;
}

void ControlScheme::dispatchMessage(string message){
  for (auto & listener : m_pListeners){
    listener->receiveInput(message);
  }
}
