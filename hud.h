#ifndef HUD__H
#define HUD__H

#include <iostream>
#include "ioManager.h"

class Hud {
public:
	static Hud& getInstance();
	~Hud();
	void drawHUD(int, int,unsigned int,unsigned int) const;
	void drawVictoryHUD() const;
	void drawLostHUD() const;

private:
	Hud();
	const IOManager& io;
	SDL_Surface * const screen;
	const int height;
	const int width;
	const int x;
	const int y;

	Hud(const Hud&);
    Hud& operator=(const Hud&);

};

#endif