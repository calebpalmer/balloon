#include <sstream>

#include "arena_play_state.h"

#include <exception>
#include <sstream>
#include <iomanip>
#include <iostream>

#include "player.h"
#include "pausestate.h"
#include "keyboard_control_scheme.h"
#include "gamepad_control_scheme.h"
#include "player_physics_component.h"
#include "diagnostics.h"

using namespace CapEngine;
using namespace std;

ArenaPlayState::ArenaPlayState(Uint32 windowID, int arenaID) :
  m_windowID(windowID), m_platformerMap(buildPlatformerMap("res/arenas.xml", arenaID))
  , m_arenaID(arenaID), m_startButtonPressed(false), m_showDiagnostics(false) {}


/**
   This gets run when the GameState is first loaded for use.
 */
bool ArenaPlayState::onLoad(){
  // If there is a gamepad connected, setup gamepad control scheme
  auto controllers = Controller::getConnectedControllers();
  if( controllers.size() > 0 ){
    shared_ptr<Controller> pController = controllers[0];
    // auto pGamePadControlScheme = std::make_shared<ControlScheme>( new GamePadControlScheme( pController ) );
    // m_pControlScheme.reset( pGamePadControlScheme.release() );
    m_pControlScheme.reset( new GamePadControlScheme( pController ) );
  }
  // if no gamepad, use keyboard
  else{
    // create control scheme using default map (default constructor)
    m_pControlScheme.reset(new KeyboardControlScheme());
  }
  ControlSchemeListener::subscribe(m_pControlScheme.get());
  m_pControlScheme->enable();
  
  // create player
  m_pPlayer = makePlayer(m_windowID, m_pControlScheme.get());
  m_pPlayer->setAcceleration(Vector(0.0, GRAVITY));
  dynamic_cast<PlayerPhysicsComponent*>(m_pPlayer->getPhysicsComponent().get())->setGravity(GRAVITY);

  auto spawnPoints = m_platformerMap.getSpawnPoints();
  if(spawnPoints.size() > 0){
    Vector spawnPoint;
    spawnPoint.setX(spawnPoints[0].getX());
    spawnPoint.setY(spawnPoints[0].getY() - (m_pPlayer->boundingPolygon()).height / 2);
    m_pPlayer->setPosition(spawnPoint);
  }
  else{
    m_pPlayer->setPosition(Vector(60, 400));
  }

  // subscribe to events
  Locator::eventDispatcher->subscribe(this, CapEngine::keyboardEvent | CapEngine::controllerEvent);
  return true;

}

/**
   This is run with the GameState is being removed
 */
bool ArenaPlayState::onDestroy(){
  Locator::eventDispatcher->unsubscribe(this);
  return true;
}

void ArenaPlayState::render(){
  m_platformerMap.render();
  m_pPlayer->render();

  if(m_showDiagnostics){
    // gather diagnostic info for display
    auto physicsComponent = m_pPlayer->getPhysicsComponent();
    PlayerPhysicsComponent::State state = dynamic_cast<PlayerPhysicsComponent*>(physicsComponent.get())->getState();
    float fps = CapEngine::Locator::videoManager->getFPS();
    CapEngine::Vector position = m_pPlayer->getPosition();
    CapEngine::Vector velocity = m_pPlayer->getVelocity();
    CapEngine::Vector acceleration = m_pPlayer->getAcceleration();
    DiagnosticData diagData= {fps, state, position, velocity, acceleration};
    int xStart = 0;
    int yStart = 0;
    string font = "res/fonts/tahoma.ttf";
    int fontSize = 14;
    Diagnostics::display(m_windowID,  diagData, xStart, yStart, font, fontSize);
  }
}

void ArenaPlayState::update(double ms){
  m_platformerMap.update(ms);
  auto pUpdatedPlayer = m_pPlayer->update(ms);

  // check player collisions
  auto collisions = detectBitmapCollisions(pUpdatedPlayer->boundingPolygon(),
					   m_platformerMap.getCollisionMap());

  for(auto& collision : collisions){
    pUpdatedPlayer->handleCollision(collision.collisionType, COLLISION_UNKNOWN,
				    m_pPlayer.get(), collision.collisionPoint);
  }

  // check for boundary collisions
  CapEngine::Rectangle arenaMBR = {0, 0, 1200, 800};
  this->handleBoundaryCollisions(pUpdatedPlayer.get(), arenaMBR);

  m_pPlayer = std::move(pUpdatedPlayer);

  // check events
  if(m_startButtonPressed){
    unique_ptr<GameState> pPauseState(new PauseState(m_windowID));
    CapEngine::switchState(std::move(pPauseState));
  }
}




PlatformerMap ArenaPlayState::buildPlatformerMap(string arenaConfigPath, int arenaID){
  // open config files
  XmlParser xmlParser(arenaConfigPath);
    
  // find arena
  stringstream xpathExpression;
  xpathExpression <<  "/arenas/arena[@id=" << arenaID << "]";
  auto nodes = xmlParser.getNodes(xpathExpression.str());
  if(nodes.size() < 1){
    stringstream errorMsg;
    errorMsg <<  "Arena with ID" << arenaID << " not found in " << arenaConfigPath;
    throw CapEngineException(errorMsg.str());
  }

  int mapAssetID = -1;
  int collisionMapAssetID = -1;
  vector<Vector> arenaSpawnPoints;
  auto children = xmlParser.getNodeChildren(nodes[0]);
  for (auto& i : children){
    if(xmlParser.nodeNameCompare(i, "texture")){
      string temp = xmlParser.getStringValue(i);
      mapAssetID = std::stoi(temp);
    }
    else if(xmlParser.nodeNameCompare(i, "collisions")){
      string temp = xmlParser.getStringValue(i);
      collisionMapAssetID = std::stoi(temp);
    }
    else if(xmlParser.nodeNameCompare(i, "spawn_points")){
      auto spawnPoints = xmlParser.getNodeChildren(i);
      for(auto& spawnPoint : spawnPoints){
	if(xmlParser.nodeNameCompare(spawnPoint, "spawn_point")){
	  try{
	    int x = atoi((xmlParser.getAttribute(spawnPoint, "x")).c_str());
	    int y = atoi((xmlParser.getAttribute(spawnPoint, "y")).c_str());
	    arenaSpawnPoints.push_back(Vector(x, y));
	  }
	  catch(exception& e){
	    Locator::logger->log(e.what(), Logger::CWARNING);
	  }
	}
      }
    }
  }

  if(mapAssetID == -1 || collisionMapAssetID == -1){
    stringstream errorMsg;
    errorMsg << "Map Asset ID or Collision Map Asset ID not found in " << arenaConfigPath
	     << " for area ID " << arenaID;
    throw CapEngineException(errorMsg.str());
  }


  PlatformerMap platformerMap(m_windowID, mapAssetID, collisionMapAssetID);
  for(auto& spawnPoint : arenaSpawnPoints){
    platformerMap.addSpawnPoint(spawnPoint);
  }
 
  return platformerMap;

}


void ArenaPlayState::receiveEvent(const SDL_Event event, CapEngine::Time* time){
  // check for controller added or controller removed events
  // if controller added, create controlscheme for it, and start using it
  // if controller removed, remove the control schems for it, and revert back to keyboard control scheme
}


void ArenaPlayState::receiveInput(std::string input) {
  if(input == "TOGGLE DIAGNOSTICS"){
    // flip around m_showDiagnostics
    m_showDiagnostics = !m_showDiagnostics;
  }

}
/**
   check if player is colliding with outer edges of screen and handle it
 */
void ArenaPlayState::handleBoundaryCollisions(CapEngine::GameObject* pPlayerObject,
					      CapEngine::Rectangle boundary){
  CollisionType collisionType = CapEngine::detectMBRCollisionInterior(pPlayerObject->boundingPolygon(), boundary);
  while(collisionType != COLLISION_NONE){
    switch(collisionType){
    case COLLISION_LEFT:
      pPlayerObject->send(-1, "LEFT BOUNDARY COLLISION");
      break;
    case COLLISION_RIGHT:
      pPlayerObject->send(-1, "RIGHT BOUNDARY COLLISION");
      break;
    case COLLISION_TOP:
      pPlayerObject->send(-1, "TOP BOUNDARY COLLISION");
      break;
    case COLLISION_BOTTOM:
      break;
    default:
      break;
    }
    // check to make sure there are no more collisions
    collisionType = CapEngine::detectMBRCollisionInterior(pPlayerObject->boundingPolygon(), boundary);
  }
}
