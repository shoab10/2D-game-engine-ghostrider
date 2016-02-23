#include <iostream>
#include <cmath>
#include "multibullet.h"
#include "gamedata.h"
#include "frameFactory.h"

MultiBullet::MultiBullet( const string& name, const Vector2f& pos,
							const Vector2f& vel) :
	//MultiSprite(name, pos, vel, FrameFactory::getInstance().getFrames(name)),
	MultiSprite(name, pos, vel),
	distance(0),
	maxDistance(Gamedata::getInstance().getXmlInt(name+"/distance")),
	tooFar(false)
	{ }

MultiBullet::MultiBullet(const MultiBullet& mb) :
	MultiSprite(mb),
	distance(mb.distance),
	maxDistance(mb.maxDistance),
	tooFar(mb.tooFar)
	{ }

void MultiBullet::reset() {
	tooFar = false;
	distance = 0;
 }

void MultiBullet::update(Uint32 ticks) {
	advanceFrame(ticks);
	Vector2f pos = getPosition();
	Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
	setPosition(getPosition()+incr);
	if( Y()+frameHeight < 0 || Y() > worldHeight ) {
		tooFar=true;
	}
	if ( X() < 0 || X() > worldWidth) {
		tooFar = true;
	}
	distance += (hypot(X()-pos[0], Y()-pos[1]));
	if (distance >maxDistance) tooFar = true;
}