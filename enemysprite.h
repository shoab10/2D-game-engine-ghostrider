#ifndef ENEMY__H
#define ENEMY__H

#include <string>
#include <iostream>
#include <vector>
#include "twowaymultisprite.h"
#include "multibullets.h"

class EnemySprite : public TwoWayMultiSprite {
public:
	EnemySprite(const std::string&,int w, int h);
	EnemySprite(const EnemySprite&);
	virtual ~EnemySprite() { }
	virtual void draw() const;
	virtual void update(Uint32 ticks);
	void stop();
	void right();
	void left();
	void shoot();
	bool collidedWith(const Sprite* obj) const;
	unsigned int bulletCount() const {return bullets.bulletCount();}
	unsigned int freeCount() const {return bullets.freeCount();}
	void setPlayerPos(const Vector2f& p) { playerPos = p; }
	int getPositionRandomFactor(int, int);

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
	
	enum MODE {NORMAL, EVADE};
	Vector2f playerPos;
	int playerWidth;
    int playerHeight;
    float safeDistance;
    MODE currentMode;

	EnemySprite& operator=(const EnemySprite&);
	void advanceFrame(Uint32);

};
#endif