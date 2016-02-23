#ifndef RAINSPRITE__H
#define RAINSPRITE__H

#include <string>
#include <iostream>
#include <vector>
#include "drawable.h"

class RainSprite : public Drawable {
public:
	RainSprite(const std::string&);
	RainSprite(const RainSprite&);
	virtual ~RainSprite() { }
	RainSprite& operator=(const RainSprite&);

  virtual const Frame* getFrame() const { return frame; }
	virtual void draw() const;
	virtual void update(Uint32 ticks);
  void rotodraw() const;
  int getRandomFactor();
  int getPositionRandomFactor();

private:
  const Frame * frame;
  int frameWidth;
  int frameHeight;
  int worldWidth;
  int worldHeight;
  //int getDistance(const Sprite*) const;

};
#endif
