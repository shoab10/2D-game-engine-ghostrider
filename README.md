# 2D-game-engine-ghostrider

A fun and entertaining 2D game featuring ghostrider killing the undead.

This game was built using C++, STL, SDL 1.2 API. It was a part of Object Oriented Software Design course (CPSC 870) at Clemson University. Object Oriented design patterns implemented are Singleton, Factory, Object Pooling, Observer.

Dependencies (Linux):
- sudo apt-get install g++ clang libexpat1-dev
- sudo apt-get install libsdl1.2-dev libsdl-mixer1.2-dev libsdl-image1.2-dev libsdl-gfx1.2-dev libsdl-sound1.2-dev libsdl-ttf2.0-dev

Dependencies (macOS, Apple Silicon / Homebrew):

macOS has no SDL 1.2, so the game runs on `sdl12-compat` (the SDL 1.2 API on top of SDL2). Image decoding uses the bundled `stb_image.h` (the legacy SDL_image 1.2 returns blank surfaces under sdl12-compat), so SDL_image is not required.

1. Install what Homebrew provides:
   - `brew install sdl12-compat sdl_gfx expat libpng jpeg mad`
   - This gives `sdl-config`, the core SDL 1.2 API, and `SDL_rotozoom.h`.

2. Build the two add-ons that are no longer in Homebrew, from the libsdl.org 1.x sources, against sdl12-compat (`./configure --prefix=/opt/homebrew --with-sdl-prefix=/opt/homebrew`, then `make`/install the library into `/opt/homebrew`):
   - **SDL_ttf 2.0.11** (needs freetype).
   - **SDL_mixer 1.2.12** (configure with `--enable-music-mp3-mad-gpl` so the MP3 soundtrack works). On modern clang, patch `music_mad.c` line 227: change the bare `return;` inside `int mad_getSamples(...)` to `return 0;`.

   Headers must land in `/opt/homebrew/include/SDL/` and libraries in `/opt/homebrew/lib/` (the Makefile adds `-I/opt/homebrew/include` and `-L/opt/homebrew/lib`).

3. Build and run with the same commands below.

Commands to run
- make  #to compile and make the project
- make clean	# to clean the compiled file
- ./run			# to run the game

Enjoy !!