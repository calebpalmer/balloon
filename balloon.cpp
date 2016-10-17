#include "CapEngine.h"
#include "start_menu_state.h"

#include <memory>
#include <iostream>

using namespace CapEngine;
using namespace std;

struct Arguments {
  bool audioOff;
  bool fullscreen;
};

Arguments parseArgs(int argc, char* argv[]){
  Arguments args;
  args.audioOff = false;
  args.fullscreen = false;
  
  for(int i = 1; i < argc; i++){
    string arg = argv[i];
    if(arg == "--noaudio"){
      args.audioOff = true;
    }
    if(arg == "-f"){
      args.fullscreen = true;
    }
  }

  return args;
}

int main(int argc, char* argv[]) {
  try{
    Arguments args = parseArgs(argc, argv);
    
    WindowParams windowParams;
    windowParams.width = 1200;
    windowParams.height = 800;
    windowParams.pDepth = 32;
    windowParams.fullScreen = args.fullscreen;
    windowParams.opengl = false;
  
    auto windowID = CapEngine::init(windowParams);

    // turn off sound
    if(args.audioOff){
      CapEngine::Locator::soundPlayer->setState(CapEngine::SoundState::PAUSE);
    }
    
    CapEngine::loadAssetFile("res/assets.xml");
    std::unique_ptr<StartMenuState> pStartMenuState(new StartMenuState(windowID));
    CapEngine::startLoop(std::move(pStartMenuState));
  }
  catch (CapEngineException& e){
    CapEngine::Locator::logger->log(e.what(), CapEngine::Logger::CERROR);
  }
  catch(exception& e){
    CapEngine::Locator::logger->log(e.what(), CapEngine::Logger::CERROR);
  }

  return 1;

}
