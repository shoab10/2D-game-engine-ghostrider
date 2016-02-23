#include <iostream>
#include <cmath>
#include "playersprite.h"
#include "gamedata.h"
#include "frameFactory.h"

void PlayerSprite::advanceFrame(Uint32 ticks) {
  timeSinceLastFrame += ticks;
  if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
    timeSinceLastFrame = 0;
  }
}

PlayerSprite::PlayerSprite( const std::string& name) :
  TwoWayMultiSprite(name),
  keyPressedX(false),
  keyPressedY(false),
  initialVelocity(300,300),
  slowDown(Gamedata::getInstance().getXmlFloat(name+"/slowDown")),
  bulletName("bullet"),
  bulletInterval(Gamedata::getInstance().getXmlFloat(bulletName+"/bulletinterval")),
  timeSinceLastBullet(0),
  minBulletSpeed(Gamedata::getInstance().getXmlFloat(bulletName+"/minbulletspeed")),
  bullets(bulletName),
  observers() 
{ }

PlayerSprite::PlayerSprite(const PlayerSprite& s) :
  TwoWayMultiSprite(s.getName()),
  keyPressedX(s.keyPressedX),
  keyPressedY(s.keyPressedY),
  initialVelocity(s.initialVelocity),
  slowDown(s.slowDown),
  bulletName(s.bulletName),
  bulletInterval(s.bulletInterval),
  timeSinceLastBullet(s.timeSinceLastBullet),
  minBulletSpeed(s.minBulletSpeed),
  bullets(s.bullets),
  observers(s.observers)
  { }

void PlayerSprite::stop() {
  if (!keyPressedX) velocityX( slowDown*velocityX() );
}

void PlayerSprite::right() {
  keyPressedX = true;
  if (X() < worldWidth-frameWidth) {
    velocityX(initialVelocity[0]);
  }
}

void PlayerSprite::left() {
  keyPressedX = true;
  if (X() > 0) {
    velocityX(-initialVelocity[0]);
  }
}

bool PlayerSprite::collidedWith(const Sprite* obj) const {
  return bullets.collidedWith(obj);
}


void PlayerSprite::shoot(){
  if (velocityX() > 0) { frames = rightFrames;}
  else { frames=leftFrames;}

  if (timeSinceLastBullet > bulletInterval) {
  Vector2f vel =getVelocity();
  float x;
  float y = Y()+frameHeight/4;
  if (vel[0] > 0) {
  x = X()+ frameWidth;
  vel[0] += minBulletSpeed;    
  }
  else {
  x=X();
  vel[0] -= minBulletSpeed;
  }
  std::cout <<"computed vel:" << vel << std::endl;
  bullets.shoot(Vector2f(x,y),vel);
  timeSinceLastBullet = 0;

  }
}

void PlayerSprite::detach( EnemySprite* o ) {
  std::list<EnemySprite*>::iterator ptr = observers.begin();
  while ( ptr != observers.end() ) {
    if ( *ptr == o ) {
      ptr = observers.erase(ptr);
      return;
    }
    ++ptr;
  }
}

void PlayerSprite::draw() const { 
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frames[currentFrame]->draw(x,y);
  bullets.draw();
}

void PlayerSprite::update(Uint32 ticks) { 
  advanceFrame(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( Y() < 0) {
    velocityY( abs( velocityY() ) );
  }
  if ( Y() > worldHeight-frameHeight) {
    velocityY( -abs( velocityY() ) );
  }

  if ( X() <= 0) {
    velocityX( abs( velocityX() ) );
  }
  if ( X() >= worldWidth-frameWidth) {
    velocityX( -abs( velocityX() ) );
  }
  //changing directions for the player
  if ( velocityX() > 0) frames = rightFrames;
  if ( velocityX() < 0) frames = leftFrames;
  if (!keyPressedX ) stop();
  keyPressedX = false;

  timeSinceLastBullet += ticks;
  bullets.update(ticks);

  std::list<EnemySprite*>::iterator ptr = observers.begin();
  while ( ptr != observers.end() ) {
    (*ptr)->setPlayerPos( getPosition() );
    ++ptr;
  }

}