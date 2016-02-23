#include <vector>
#include <list>
#include <SDL.h>
#include "ioManager.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"
#include "hud.h"
#include "scaledmultisprite.h"
#include "playersprite.h"
#include "multisprite.h"
#include "explodingSprite.h"
#include "sound.h"
#include "health.h"

class Manager {
public:
  Manager ();
  ~Manager ();
  void play();
  void pause() { clock.pause(); }
  void unpause() { clock.unpause(); }
  void checkForCollisions();
  bool getstate(){ return state;};
  bool getexits(){ return exits;}
  void setexits(bool b){ exits=b;}

private:
  const bool env;
  const IOManager& io;
  Clock& clock;
  const Hud& hud;


  SDL_Surface * const screen;
  World sky;
  World mountains;
  World house;
  World wall;
  World ground;
  Viewport& viewport;

  //std::vector<Drawable*> sprites;
  std::list<Sprite*> sprites;
  //std::list<Drawable*> rain;
  std::vector<ScaledMultiSprite*> flame;
  Sprite* player;
  PlayerSprite* ghostrider;
  Sprite* tree;
  std::list<ExplodingSprite*> explosionsprites;
  SDLSound sound;
  Health bar;

  int currentSprite;

  bool makeVideo;
  int frameCount;
  const std::string username;
  const std::string title;
  const int frameMax;

  void draw() const;
  void update();


  Manager(const Manager&);
  Manager& operator=(const Manager&);
  void makeFrame();
  //void makeScaleSprites();

  bool toggleHUD;
  bool treeexploded;
  unsigned int numberOfEnemies;
  unsigned int enemieskilled;
  bool gameover;
  bool state;
  bool winner;
  bool exits;
};
