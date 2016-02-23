#include "multibullets.h"
#include "gamedata.h"
#include "frameFactory.h"

CollisionStrategy* getStrategy(const string& name) {
	std::string sName = Gamedata::getInstance().getXmlStr(name+"/strategy");
	if( sName == "midpoint") return new MidPointCollisionStrategy;
	if( sName == "rectangular") return new RectangularCollisionStrategy;
	if( sName == "perpixel") return new PerPixelCollisionStrategy;
	throw std::string("No strategy in getStrategy");
}




MultiBullets::MultiBullets(const std::string& n) :
	name(n),
	myVelocity(
	Gamedata::getInstance().getXmlInt(name+"/speed/x"),
	Gamedata::getInstance().getXmlInt(name+"/speed/x")
	),
	bulletFrames (FrameFactory::getInstance().getFrames(name)),
	bulletList(),
	freeList(),
	strategy( getStrategy(name))
{ }

MultiBullets::MultiBullets(const MultiBullets& b) :
	name(b.name),
	myVelocity(b.myVelocity),
	bulletFrames(b.bulletFrames),
	bulletList(b.bulletList),
	freeList(b.freeList),
	strategy(b.strategy)

	{}

bool MultiBullets::collidedWith(const Sprite* obj) const {
	std::list<MultiBullet>::iterator ptr = bulletList.begin();
	while (ptr != bulletList.end()) {
		if ( strategy->execute(*ptr,*obj)) {
			freeList.push_back(*ptr);
			ptr = bulletList.erase(ptr);
			return true;
		}
		++ptr;
	}
	return false;
}

void MultiBullets::shoot(const Vector2f& pos, const Vector2f& objVel) {
	if (freeList.empty()) {
		MultiBullet b( name, pos, objVel);
		bulletList.push_back( b);
	}
	else {
		MultiBullet b = freeList.front();
		freeList.pop_front();
		b.reset();
		b.setVelocity(objVel);
		b.setPosition(pos);
		bulletList.push_back(b);
	}
}
//check if this function is complete
void MultiBullets::draw() const {
	std::list<MultiBullet>::const_iterator ptr = bulletList.begin();
	while (ptr !=bulletList.end()) {
		ptr->draw();
		++ptr;
	}
}

//complete this fn
void MultiBullets::update(Uint32 ticks) {
	std::list<MultiBullet>::iterator ptr = bulletList.begin();
	while (ptr !=bulletList.end()) {
		ptr->update(ticks);

		if(ptr->goneTooFar()) {
			freeList.push_back(*ptr);
			ptr=bulletList.erase(ptr);
		}
		else {
			++ptr;
		}
	}
}