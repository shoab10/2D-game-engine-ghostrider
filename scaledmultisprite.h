#ifndef SCALEDMULTISPRITE__H
#define SCALEDMULTISPRITE__H
#include <string>
#include <vector>
#include "drawable.h"

class ScaledMultiSprite : public Drawable {
public:
  ScaledMultiSprite(const std::string&);
  ScaledMultiSprite(const ScaledMultiSprite&);
  virtual ~ScaledMultiSprite() { 
    for (unsigned i=0;i<scaledSurface.size();++i) {
      SDL_FreeSurface( scaledSurface[i]);
      delete scaledFrames [i];
    }

  } 

  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual const Frame* getFrame() const { 
    return frames[currentFrame]; 
  }
  float getScale() const { return scale; }
  float getRandFloat(float,float);
  int getRandomFactor();
  int getPositionRandomFactor(int,int);

protected:
  double scale;
  const std::vector<Frame *> frames;
  const std::vector<SDL_Surface *>surfaces;
  std::vector<SDL_Surface *> scaledSurface;
  std::vector<Frame *> scaledFrames;
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