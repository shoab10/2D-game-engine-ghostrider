#ifndef TWOWAYMULTISPRITE__H
#define TWOWAYMULTISPRITE__H

#include <string>
#include <iostream>
#include <vector>
//#include "drawable.h"
#include "sprite.h"

class TwoWayMultiSprite : public Sprite {
public:
	TwoWayMultiSprite(const std::string&);
	TwoWayMultiSprite(const TwoWayMultiSprite&);
	virtual ~TwoWayMultiSprite() { }
	virtual void draw() const;
	virtual void update(Uint32 ticks);
	virtual const Frame* getFrame() const {
		return frames[currentFrame];
	}


protected:
	const std::vector<Frame *> rightFrames;
	const std::vector<Frame *> leftFrames;
	std::vector<Frame *> frames;
	int worldWidth;
	int worldHeight;

	unsigned currentFrame;
	unsigned numberOfFrames;
	unsigned frameInterval;
	float timeSinceLastFrame;
	int frameWidth;
	int frameHeight;

	void advanceFrame(Uint32 ticks);

};
#endif
