#include "scaledmultisprite.h"
#include "gamedata.h"
#include "frameFactory.h"
#include <SDL_rotozoom.h>

void ScaledMultiSprite::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}
}

float ScaledMultiSprite::getRandFloat(float min,float max) {
  float randNumber = Gamedata::getInstance().getRandFloat(min,max);
  return randNumber;
}

int ScaledMultiSprite::getRandomFactor(){
  int randNumber = Gamedata::getInstance().getRandInRange(30,80);
  //randNumber *= rand() % 2 ? -1 : 1;
  return randNumber;
}

int ScaledMultiSprite::getPositionRandomFactor(int a, int b){
  int randNumber = Gamedata::getInstance().getRandInRange(a,b);
  //randNumber *= rand() % 2 ? -1 : 1;
  return randNumber;
}



ScaledMultiSprite::ScaledMultiSprite( const std::string& name) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x")  + getPositionRandomFactor(10,5000), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y") + getPositionRandomFactor(10,200)), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX")+ getRandomFactor(),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  scale(getRandFloat( Gamedata::getInstance().getXmlFloat(name+"/scale/min"),
                      Gamedata::getInstance().getXmlFloat(name+"/scale/max") ) ),
  //frames( FrameFactory::getInstance().getScaledFrames(name,scale) ),
  frames( FrameFactory::getInstance().getFrames(name)),
  surfaces(FrameFactory::getInstance().getSurfaces(name)),
  scaledSurface(),
  scaledFrames(),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),

  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval") ),
  timeSinceLastFrame( 0 ),
  frameWidth(frames[0]->getWidth()),
  frameHeight(frames[0]->getHeight())
{ 
  for (unsigned i=0 ; i<surfaces.size();++i) {
  scaledSurface.push_back( rotozoomSurface(surfaces[i],0,scale,SMOOTHING_ON));

}

  for (unsigned i=0 ; i<scaledSurface.size();++i) {
  scaledFrames.push_back( new Frame(scaledSurface[i],scaledSurface[i]->w,scaledSurface[i]->h,
      Gamedata::getInstance().getXmlInt(name+"/srcX"),
      Gamedata::getInstance().getXmlInt(name+"/srcY")) );
}

}

ScaledMultiSprite::ScaledMultiSprite(const ScaledMultiSprite& s) :
  Drawable(s),
  scale(s.scale),
  frames(s.frames),
  surfaces(s.surfaces),
  scaledSurface(s.scaledSurface),
  scaledFrames(s.scaledFrames),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  frameWidth( s.frameWidth ),
  frameHeight( s.frameHeight )
  { }

void ScaledMultiSprite::draw() const { 
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  scaledFrames[currentFrame]->draw(x,y);
}

void ScaledMultiSprite::update(Uint32 ticks) { 
  advanceFrame(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( Y() < 0) {
    velocityY( abs( velocityY() ) );
  }
  if ( Y() > worldHeight-frameHeight) {
    //velocityY( -abs( velocityY() ) );
    Y(0.0);
  }

  if ( X() < 0) {
    velocityX( abs( velocityX() ) );
  }
  if ( X() > worldWidth-frameWidth) {
    //velocityX( -abs( velocityX() ) );
    X(0.0);
  }  

}
