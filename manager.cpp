#include <iostream>
#include <string>
#include <iomanip>
#include "twowaymultisprite.h"
#include "multisprite.h"
#include "sprite.h"
#include "rotatingsprite.h"
#include "rainsprite.h"
#include "playersprite.h"
#include "gamedata.h"
#include "manager.h"
#include "hud.h"
#include "scaledmultisprite.h"
#include "enemysprite.h"
//#include "explodingSprite.h"
#include <algorithm>

class ScaledSpriteCompare {
public:
  bool operator()(const ScaledMultiSprite* lhs,const ScaledMultiSprite* rhs) {
    return lhs->getScale() < rhs->getScale();
  }

};

Manager::~Manager() { 
  // These deletions eliminate "definitely lost" and
  // "still reachable"s in Valgrind.
  delete ghostrider;
  if(!treeexploded)
  delete tree;


  Sprite* temp;
  std::list<Sprite*>::const_iterator spIt = sprites.begin();
  while( spIt != sprites.end()) {
    temp=(*spIt);
    ++spIt;
    delete temp;
  }

  ExplodingSprite* temp1;
  std::list<ExplodingSprite*>::iterator ptr1 = explosionsprites.begin();//check this
  while (ptr1 != explosionsprites.end()) {
    temp1=(*ptr1);
    ++ptr1;
    delete temp1;
  }

  for (unsigned i = 0; i < flame.size(); ++i)
    delete flame[i];
  
}

Manager::Manager() :
  env( SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center")) ),
  io( IOManager::getInstance() ),
  clock( Clock::getInstance() ),
  hud(Hud::getInstance()),
  screen( io.getScreen() ),
  sky("sky", Gamedata::getInstance().getXmlInt("sky/factor") ),
  mountains("mountains", Gamedata::getInstance().getXmlInt("mountains/factor") ),
  house("house", Gamedata::getInstance().getXmlInt("house/factor") ),
  wall("wall", Gamedata::getInstance().getXmlInt("wall/factor") ),
  ground("ground", Gamedata::getInstance().getXmlInt("ground/factor") ),
  viewport( Viewport::getInstance() ),
  sprites(),
  //rain(),
  flame(),
  player(new PlayerSprite("ghostrider")),
  //ghostrider(new PlayerSprite("ghostrider")),
  ghostrider(),
  tree(new MultiSprite("burningtree")),
  explosionsprites(),
  sound(),
  bar(),
  currentSprite(0),

  makeVideo( false ),
  frameCount( 0 ),
  username(  Gamedata::getInstance().getXmlStr("username") ),
  title( Gamedata::getInstance().getXmlStr("screenTitle") ),
  frameMax( Gamedata::getInstance().getXmlInt("frameMax") ),
  toggleHUD(true),
  treeexploded(false),
  numberOfEnemies(Gamedata::getInstance().getXmlInt("numberOfEnemies")),
  enemieskilled(0),
  gameover(false),
  state(false),
  winner(false),
  exits(false)
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw string("Unable to initialize SDL: ");
  }
  SDL_WM_SetCaption(title.c_str(), NULL);
  atexit(SDL_Quit);
  //sprites.push_back( new PlayerSprite("spikey") );
  //sprites.push_back( new MultiSprite("burningtree") );

  ghostrider=static_cast<PlayerSprite*>(player);

//Pushing enemies into the list
  for (unsigned int i = 0; i < numberOfEnemies; ++i) {
    sprites.push_back( new EnemySprite("enemy",ghostrider->getFrame()->getWidth()/2,ghostrider->getFrame()->getHeight()/2) );
  }
  
  //for(int i=1;i<Gamedata::getInstance().getXmlInt("rain/number");i++)
    //rain.push_back( new RainSprite("rain") );

  
  //std::list<Drawable*>::iterator ptr = sprites.begin();
  //for (int i=0; i<currentSprite; i++)
  //  ++ptr;
  
  viewport.setObjectToTrack(ghostrider);
  

 // Creating Scaled Sprites here and sorting
  unsigned numberOfSprites = Gamedata::getInstance().getXmlInt("bird/number");
  flame.reserve (numberOfSprites);
  for ( unsigned i =0; i< numberOfSprites; ++i) {
    flame.push_back(new ScaledMultiSprite("bird"));
  }
  sort (flame.begin(),flame.end(),ScaledSpriteCompare());

  //attaching the observers
  std::list<Sprite*>::const_iterator spIt = sprites.begin();
  while( spIt != sprites.end()) {
    ghostrider->attach(static_cast <EnemySprite*>(*spIt));
    ++spIt;
  }
 
}



void Manager::draw() const {
  sky.draw();
  for(unsigned int i=0;i<flame.size()/2;++i)
  flame[i]->draw();
  mountains.draw();
  house.draw();
  for(unsigned int i=flame.size()/2;i<flame.size();++i)
  flame[i]->draw();
  ground.draw(); 
  wall.draw();
  
 // std::list<Drawable*>::const_iterator ptr = sprites.begin();
 // while (ptr != sprites.end()) {
 //   (*ptr)->draw();
  //  ++ptr;
  //}

  std::list<Sprite*>::const_iterator spIt = sprites.begin();
  while( spIt != sprites.end()) {
    (*spIt)->draw();
    ++spIt;
  }

 /* std::list<Drawable*>::const_iterator ptr1 = rain.begin();
  while (ptr1 != rain.end()) {
    if (rotoflag ==1)
      static_cast <RainSprite*>(*ptr1)->rotodraw();
    else
    (*ptr1)->draw();
    ++ptr1;
  }*/

  std::list<ExplodingSprite*>::const_iterator expIt = explosionsprites.begin();
  while( expIt != explosionsprites.end()) {
    (*expIt)->draw();
    ++expIt;
  }

  if(!treeexploded)
    tree->draw();
    //ghostrider->draw();
    player->draw();

 
//drawing the HUD here
  if (toggleHUD) 
  hud.drawHUD(clock.getSeconds(),clock.getAvgFps(),ghostrider->bulletCount(),ghostrider->freeCount());
  bar.draw();

  if(gameover) {
          hud.drawLostHUD();
        }

  if(winner) {
    hud.drawVictoryHUD();
  }



  io.printMessageAt(title, 10, 450);
  viewport.draw(); 

  SDL_Flip(screen);
}

void Manager::checkForCollisions() {
  if(!treeexploded) {
  if (ghostrider->collidedWith(tree)){
    std::cout<<"collided";
    explosionsprites.push_back(new ExplodingSprite(*tree));
    treeexploded=true;
    delete tree;
    sound[5];
    //it = orbs.erase(it);
  }
}

  std::list<Sprite*>::iterator spIt = sprites.begin();
  while( spIt != sprites.end()) {
    Sprite* sprite = static_cast<Sprite*>(*spIt);
    //ExplodingSprite* e = dynamic_cast<ExplodingSprite*>(player);

    /*if ( e && e->chunkCount() == 0 ) { 
      // Make a smart sprite
      SmartSprite* ss =
        new SmartSprite(sprite->getName(),sprite->getPosition(),
                        sprite->getFrame()->getWidth()/2, 
                        sprite->getFrame()->getHeight()/2);
      sprites[i] = ss;
      delete sprite;
      static_cast<SubjectSprite*>(sprites[0])->attach(ss);
      ++numberOfSmart;
    }*/




    if (ghostrider->collidedWith(sprite) ) {
      sound[5];
      ++enemieskilled;
      explosionsprites.push_back(new ExplodingSprite(*sprite));
      ghostrider->detach(static_cast<EnemySprite*>(sprite) );
      delete *spIt;
      spIt=sprites.erase(spIt);
    }


    else if (static_cast<EnemySprite*>(sprite)->collidedWith(static_cast<Sprite*>(ghostrider))) {
      sound[5];
      //bar.update(ghostrider->getHealth());
      if (bar.getHealth()<=0){
       gameover=true;
       state=true;
       explosionsprites.push_back(new ExplodingSprite(*(static_cast<Sprite*>(ghostrider)) ));
       ghostrider->X(5000);
       break;
      }
      bar.update(50);
      explosionsprites.push_back(new ExplodingSprite(*(static_cast<Sprite*>(ghostrider)) ));
      //playerExploded = true;
      Sprite *temp = player;
      //player = new ExplodingSprite(*player);
      //delete temp;

    }
    
    else
      ++spIt;    
  }




  // sprites[0] is the spinning star:
  //Sprite* player = static_cast<Sprite*>( sprites[0] );
  /*for (unsigned i = 0; i < sprites.size(); ++i) {
    Sprite* sprite = static_cast<Sprite*>( sprites[i] );
    ExplodingSprite* e = dynamic_cast<ExplodingSprite*>(sprite);
    if ( e && e->chunkCount() == 0 ) { 
      // Make a smart sprite
      SmartSprite* ss =
        new SmartSprite(sprite->getName(),sprite->getPosition(),
                        sprite->getFrame()->getWidth()/2, 
                        sprite->getFrame()->getHeight()/2);
      sprites[i] = ss;
      delete sprite;
      static_cast<SubjectSprite*>(sprites[0])->attach(ss);
      ++numberOfSmart;
    }
    else if ( !e && player->collidedWith(sprite) ) { 
      if ( dynamic_cast<SmartSprite*>(sprite) ) {
        static_cast<SubjectSprite*>(sprites[0])->
          detach(static_cast<SmartSprite*>(sprite) );
      }
      sprites[i] = new ExplodingSprite(sprite);
      delete sprite;
      if ( numberOfYellow ) --numberOfYellow;
    }
  }*/
}


void Manager::makeFrame() {
  std::stringstream strm;
  strm << "frames/" << username<< '.' 
       << std::setfill('0') << std::setw(4) 
       << frameCount++ << ".bmp";
  std::string filename( strm.str() );
  std::cout << "Making frame: " << filename << std::endl;
  SDL_SaveBMP(screen, filename.c_str());
}

void Manager::update() {
  ++clock;
  Uint32 ticks = clock.getElapsedTicks();

  for (unsigned int i=0;i<flame.size();i++)
    flame[i]->update(ticks);

  std::list<Sprite*>::const_iterator spIt = sprites.begin();
  while( spIt != sprites.end()) {
    (*spIt)->update(ticks);
    ++spIt;
  }

  /*std::list<Drawable*>::const_iterator ptr2 = rain.begin();
  while (ptr2 != rain.end()) {
    (*ptr2)->update(ticks);
    ++ptr2;
  }*/

  std::list<ExplodingSprite*>::iterator expIt = explosionsprites.begin();
  while( expIt != explosionsprites.end()) {
    (*expIt)->update(ticks);
    if ( (*expIt)->done()) {
      delete *expIt;
      expIt = explosionsprites.erase(expIt);
    }
    else ++expIt;
  }

  ghostrider->update(ticks);
  if(!treeexploded)
  tree->update(ticks);


  if ( makeVideo && frameCount < frameMax ) {
    makeFrame();
  }

//Hide HUD after 4 seconds
  if ( clock.getSeconds() == 4 ) {
    toggleHUD=false;
  }

//Collision Detection  
  checkForCollisions();

//check for victory
  if(enemieskilled >=8) {
  state=true;
  winner=true; 
    }



  
  ground.update();
  wall.update();
  house.update();
  mountains.update();
  sky.update();
  if (!gameover)
  viewport.update(); // always update viewport last
}

void Manager::play() {
  SDL_Event event;

  bool done = false;
  bool keyCatch = false;
  while ( not done ) {

    if(state) {
      int sec=clock.getSeconds();
      int a=1;
      while(a==1){
        draw();
        update();
        if(clock.getSeconds()-sec > 4){
          a=0;
        }
      }
    exits=true;
    return;
  }

    SDL_PollEvent(&event);
    Uint8 *keystate = SDL_GetKeyState(NULL);
    if (event.type ==  SDL_QUIT) { done = true; break; }
    if(event.type == SDL_KEYUP) { 
      keyCatch = false; 
    }
    if(event.type == SDL_KEYDOWN) {
      if (keystate[SDLK_ESCAPE] || keystate[SDLK_q]) {
        done = true;
        exits=true;
        break;
      }
      if (keystate[SDLK_t] && !keyCatch) {
        keyCatch = true;
        currentSprite = (currentSprite+1) % sprites.size();
        //std::list<Drawable*>::iterator ptr = sprites.begin();
        //for (int i=0; i<currentSprite; i++)
         // ++ptr;
        //viewport.setObjectToTrack(sprites[currentSprite]);
        
      }
      if (keystate[SDLK_s] && !keyCatch) {
        keyCatch = true;
        clock.toggleSloMo();
      }
      if (keystate[SDLK_p] && !keyCatch) {
          keyCatch = true;
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
      if (keystate[SDLK_F4] && !makeVideo) {
          std::cout << "Making video frames" << std::endl;
          makeVideo = true;
        }

      if (keystate[SDLK_r] && !keyCatch) {
          keyCatch = true;
          return;
        }

      if (keystate[SDLK_F1] && !keyCatch) {
          keyCatch = true;
          toggleHUD=!toggleHUD;
          
        }
      if (keystate[SDLK_m] && !keyCatch) {
          keyCatch = true;
          sound.toggleMusic();
          
        }

      if (keystate[SDLK_SPACE] && !keyCatch) {
        keyCatch = true;
        ghostrider->shoot();
      }
                 
    }
    if (keystate[SDLK_a]) {
      ghostrider->left();
    }

    if (keystate[SDLK_d]) {
      ghostrider->right();
    }

    /*if (keystate[SDLK_SPACE]) {
      std::list<Drawable*>::iterator playeriterator = sprites.begin();
      static_cast <PlayerSprite*>(*playeriterator)->shoot();
    }*/

    draw();
    update();
  }
}
