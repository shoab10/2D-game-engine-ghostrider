#ifndef ROTATINGSPRITE__H
#define ROTATINGSPRITE__H

#include <string>
#include <iostream>
#include <vector>
#include "drawable.h"

class RotatingSprite : public Drawable {
public:
	RotatingSprite(const std::string&);
	RotatingSprite(const RotatingSprite&);
	virtual ~RotatingSprite() { }
	RotatingSprite& operator=(const RotatingSprite&);

  virtual const Frame* getFrame() const { return frame; }
	virtual void draw() const;
	virtual void update(Uint32 ticks);
  void rotodraw() const;

private:
  const Frame * frame;
  int frameWidth;
  int frameHeight;
  int worldWidth;
  int worldHeight;
  //int getDistance(const Sprite*) const;

};
#endif
