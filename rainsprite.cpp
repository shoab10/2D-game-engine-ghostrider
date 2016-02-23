#include <cmath>
#include "rainsprite.h"
#include "gamedata.h"
#include "frameFactory.h"

int RainSprite::getRandomFactor(){
  int randNumber = Gamedata::getInstance().getRandInRange(10,200);
  randNumber *= rand() % 2 ? -1 : 1;
  return randNumber;
}

int RainSprite::getPositionRandomFactor(){
  int randNumber = Gamedata::getInstance().getRandInRange(10,5000);
  randNumber *= rand() % 2 ? -1 : 1;
  return randNumber;
}

RainSprite::RainSprite(const std::string& name) :
  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x") + getPositionRandomFactor(), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX") , 
                    Gamedata::getInstance().getXmlInt(name+"/speedY") + getRandomFactor()) 
           ),
  frame( FrameFactory::getInstance().getFrame(name) ),
  frameWidth(frame->getWidth()),
  frameHeight(frame->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{ }

RainSprite::RainSprite(const RainSprite& s) :
  Drawable(s), 
  frame(s.frame),
  frameWidth(s.getFrame()->getWidth()),
  frameHeight(s.getFrame()->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{ }

RainSprite& RainSprite::operator=(const RainSprite& rhs) {
  Drawable::operator=( rhs );
  frame = rhs.frame;
  frameWidth = rhs.frameWidth;
  frameHeight = rhs.frameHeight;
  worldWidth = rhs.worldWidth;
  worldHeight = rhs.worldHeight;
  return *this;
}

void RainSprite::draw() const { 
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frame->draw(x, y);
}

void RainSprite::rotodraw() const { 
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  static double angle1 =0;
  angle1 = angle1+10;
  frame->draw(x, y,angle1);
}

void RainSprite::update(Uint32 ticks) { 
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( Y() < 0) {
    velocityY( abs( velocityY() ) );
  }
  if ( Y() > worldHeight-frameHeight) {
    Y(0.0);
    //velocityY( -abs( velocityY() ) );
  }

  if ( X() < 0) {
    velocityX( abs( velocityX() ) );
  }
  if ( X() > worldWidth-frameWidth) {
    velocityX( -abs( velocityX() ) );
  } 
}