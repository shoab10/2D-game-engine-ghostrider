#include <iostream>
#include "aaline.h"
#include "hud.h"
#include "gamedata.h"
Hud& Hud::getInstance() {
  static Hud hud;
  return hud;
}

Hud::Hud(): io( IOManager::getInstance() ),
screen( io.getScreen() ),
height(Gamedata::getInstance().getXmlInt("Hud/height")),
width(Gamedata::getInstance().getXmlInt("Hud/weight")),
x(Gamedata::getInstance().getXmlInt("Hud/startLoc/x")),
y(Gamedata::getInstance().getXmlInt("Hud/startLoc/y"))
{}

Hud::~Hud()
{}

void Hud::drawHUD(int seconds,int avgfps,unsigned int bcount,unsigned int fcount) const {
  //const Uint32 RED = SDL_MapRGB(screen->format, 0xff, 0, 0);
  Draw_AALine(screen, x, y+height/2, 
                      x+width,y+height/2, 
                      height, 0xff, 0xff, 0xff, 0xff/2);
		//Draw_AALine(screen, x,y, x+width,y, RED);
  io.printMessageAt("Press F1 to toggle HUD", 10, 20);
  io.printMessageValueAt("Seconds: ",seconds, 10, 40);
  io.printMessageValueAt("fps: ", avgfps, 10, 60);
  io.printMessageValueAt("Bullet list: ", bcount, 10, 80);
  io.printMessageValueAt("Free list: ", fcount, 10, 100);
  //io.printMessageAt("Press T to switch sprites", 10, 120);
  io.printMessageAt("Press a or d to move Ghost Rider's direction", 10, 120);
  io.printMessageAt("Press space to shoot", 10, 140);
  io.printMessageAt("Press m to toggle music", 10, 160);
  io.printMessageAt("SHOOT THE BURNING TREE", 10, 180);
 // io.printMessageAt("Press A to add boomerang", 10, 130);
}

void Hud::drawVictoryHUD() const {
  //const Uint32 RED = SDL_MapRGB(screen->format, 0xff, 0, 0);
  Draw_AALine(screen, 300, 100+height/2, 
                      300+width,100+height/2, 
                      height, 0xff, 0xff, 0xff, 0xff/2);
    //Draw_AALine(screen, x,y, x+width,y, RED);
  io.printMessageAt("You Win !!!", 400, 200);
}

void Hud::drawLostHUD() const {
  //const Uint32 RED = SDL_MapRGB(screen->format, 0xff, 0, 0);
  Draw_AALine(screen, 300, 100+height/2, 
                      300+width,100+height/2, 
                      height, 0xff, 0xff, 0xff, 0xff/2);
    //Draw_AALine(screen, x,y, x+width,y, RED);
  io.printMessageAt("You Lose !!!", 400, 200);
}