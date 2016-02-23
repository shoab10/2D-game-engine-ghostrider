#ifndef MULTIBULLETS__H
#define MULTIBULLETS__H

#include <list>
#include "multibullet.h"
#include "collisionStrategy.h"

class MultiBullets {
public:
	MultiBullets(const std::string&);
	MultiBullets(const MultiBullets&);
	~MultiBullets() {}
	void draw() const;
	void update(Uint32 ticks);
	void shoot(const Vector2f& pos, const Vector2f& objVel);

	unsigned int bulletCount() const {return bulletList.size();}
	unsigned int freeCount() const {return freeList.size();}
	bool shooting() const {return ~bulletList.empty();}
	bool collidedWith(const Sprite* obj) const;

private:
	std::string name;
	Vector2f myVelocity;
	std::vector<Frame*> bulletFrames;
	mutable std::list<MultiBullet> bulletList; //contains MultiBullet sprite
	mutable std::list<MultiBullet> freeList; //for when Multibullet goes too far of
	CollisionStrategy* strategy;

	MultiBullets& operator=(const MultiBullets&);

};
#endif