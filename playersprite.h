#ifndef PLAYERSPRITE__H
#define PLAYERSPRITE__H

#include <string>
#include <iostream>
#include <vector>
#include "twowaymultisprite.h"
#include "multibullets.h"
#include "enemysprite.h"

class PlayerSprite : public TwoWayMultiSprite {
public:
	PlayerSprite(const std::string&);
	PlayerSprite(const PlayerSprite&);
	virtual ~PlayerSprite() { }
	virtual void draw() const;
	virtual void update(Uint32 ticks);
	void stop();
	void right();
	void left();
	void shoot();
	bool collidedWith(const Sprite* obj) const;
	unsigned int bulletCount() const {return bullets.bulletCount();}
	unsigned int freeCount() const {return bullets.freeCount();}
	void attach( EnemySprite* o ) { observers.push_back(o); } 
    void detach( EnemySprite* o );

protected:
	//float frameInterval; //for the flapping in malloys peter
	//float minFrameDelay;
	//float frameDelay;

	bool keyPressedX;
	bool keyPressedY;

	Vector2f initialVelocity;
	const float slowDown;

	std::string bulletName;
	float bulletInterval;
	float timeSinceLastBullet;
	float minBulletSpeed;
	MultiBullets bullets;
	std::list<EnemySprite*> observers;

	PlayerSprite& operator=(const PlayerSprite&);
	void advanceFrame(Uint32);

};
#endif