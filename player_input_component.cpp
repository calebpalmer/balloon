#include "player_input_component.h"

#include "pausestate.h"

using namespace std;
using namespace CapEngine;

PlayerInputComponent::PlayerInputComponent(ControlScheme* pControlScheme) {
  // listen to the ControlSchemex
  this->subscribe(pControlScheme);
}

PlayerInputComponent::~PlayerInputComponent(){ }

void PlayerInputComponent::update(GameObject* object){
  while (!m_inputs.empty()){
    // get first input
    string message = m_inputs.front();

    // process the input
    if(message == "JUMP"){
      int messageId = object->generateMessageId();
      object->send(messageId, JUMP_MESSAGE);
    }
    
    else if(message == "LEFT"){
      object->send(-1, message);
    }
    
    else if(message == "RIGHT"){
      object->send(-1, message);
    }

    // pop the input because we're done with it
    m_inputs.pop();
  }
}

/**
   Receives input from a ControlScheme that this object is listening to
 */
void PlayerInputComponent::receiveInput(string  input){
  m_inputs.push(input);
}






















