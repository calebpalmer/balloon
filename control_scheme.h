#ifndef CONTROL_SCHEME_H
#define CONTROL_SCHEME_H

#include <memory>
#include <vector>

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
  /**
     Add a ControlSchemeListener that will be notified of any actions in
     the control schems
     @param pListener The ControlSchemeListener
   */
  virtual void listen(ControlSchemeListener* pListener);

  /**
     Remove a ControlSchemeListener so that it is no longer listening for 
     actions on this control schems
   */
  virtual void unlisten(ControlSchemeListener* pListener) ;

  /**
     Disables the control scheme so that it ignores any Input devices 
     it is listening to.  Useful if a GameState still exists but is not in the foreground.
     It is disabled by default.
   */
  virtual void disable();

  /**
     Enables the control scheme so that it starts listening to events.
   */
  virtual void enable();

 protected:
  bool m_enabled;
  std::vector<ControlSchemeListener*> m_pListeners;
};

#endif // KEYBOARD_CONTROL_SCHEME
