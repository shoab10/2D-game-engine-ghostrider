#include <iostream>
#include <cmath>
#include "enemysprite.h"
#include "gamedata.h"
#include "frameFactory.h"

float distance(float x1, float y1, float x2, float y2) {
  float x = x1-x2;
  float y = y1-y2;
  return hypot(x, y);
}

void EnemySprite::advanceFrame(Uint32 ticks) {
  timeSinceLastFrame += ticks;
  if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
    timeSinceLastFrame = 0;
  }
}

int EnemySprite::getPositionRandomFactor(int a, int b){
  int randNumber = Gamedata::getInstance().getRandInRange(a,b);
  //randNumber *= rand() % 2 ? -1 : 1;
  return randNumber;
}

EnemySprite::EnemySprite( const std::string& name,int w, int h) :
  TwoWayMultiSprite(name),
  keyPressedX(false),
  keyPressedY(false),
  initialVelocity(100,300),
  slowDown(Gamedata::getInstance().getXmlFloat(name+"/slowDown")),
  bulletName("bulletenemy"),
  bulletInterval(Gamedata::getInstance().getXmlFloat(bulletName+"/bulletinterval")),
  timeSinceLastBullet(0),
  minBulletSpeed(Gamedata::getInstance().getXmlFloat(bulletName+"/minbulletspeed")),
  bullets(bulletName),
  playerPos(),
  playerWidth(w),
  playerHeight(h),
  safeDistance(Gamedata::getInstance().getXmlFloat("safeDistance")),
 currentMode(NORMAL)  
{ float x = X();
  x= x+getPositionRandomFactor(700,4000);
  std::cout<<x;
  X(x);
}

EnemySprite::EnemySprite(const EnemySprite& s) :
  TwoWayMultiSprite(s.getName()),
  keyPressedX(s.keyPressedX),
  keyPressedY(s.keyPressedY),
  initialVelocity(s.initialVelocity),
  slowDown(s.slowDown),
  bulletName(s.bulletName),
  bulletInterval(s.bulletInterval),
  timeSinceLastBullet(s.timeSinceLastBullet),
  minBulletSpeed(s.minBulletSpeed),
  bullets(s.bullets)
  { }

void EnemySprite::stop() {
  if (!keyPressedX) velocityX( slowDown*velocityX() );
}

void EnemySprite::right() {
  keyPressedX = true;
  if (X() < worldWidth-frameWidth) {
    velocityX(initialVelocity[0]);
  }
}

void EnemySprite::left() {
  keyPressedX = true;
  if (X() > 0) {
    velocityX(-initialVelocity[0]);
  }
}

bool EnemySprite::collidedWith(const Sprite* obj) const {
  return bullets.collidedWith(obj);
}


void EnemySprite::shoot(){
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
  bullets.shoot(Vector2f(x,y),vel);
  timeSinceLastBullet = 0;

  }
}

void EnemySprite::draw() const { 
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frames[currentFrame]->draw(x,y);
  bullets.draw();
}

void EnemySprite::update(Uint32 ticks) { 
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
//added here
  float x= X()+getFrame()->getWidth()/2;
  float y= Y()+getFrame()->getHeight()/2;
  float ex= playerPos[0]+playerWidth/2;
  float ey= playerPos[1]+playerHeight/2;
  float distanceToEnemy = ::distance( x, y, ex, ey );

  if  ( currentMode == NORMAL ) {
    //std::cout<<"NORMAL MODE";
    //right();
    stop();
    if(distanceToEnemy < safeDistance) 
      currentMode = EVADE;
  }
  else if  ( currentMode == EVADE ) {
    //std::cout<<"EVADE MODE";
    if(distanceToEnemy > safeDistance) 
      currentMode=NORMAL;
    else {
      if ( x < ex ) {
      right();
      shoot();
    }
      
      if ( x > ex ) {
        left();
        shoot();
      }
      //if ( y < ey ); //goUp();
      //if ( y > ey ) ;//goDown();
    }
  }


}