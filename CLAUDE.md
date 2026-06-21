# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build & Run

The project uses a flat Makefile that compiles every `*.cpp` in the repo root into one executable named `run`. There are no subdirectories of source — adding a new `.cpp` to the root automatically pulls it into the build.

- `make` / `make gcc` — build with g++ (default)
- `make clang` — build with clang++
- `make clean` — remove `*.o`, `*.d`, the `run` binary, and `frames/*.bmp` (video frames captured at runtime)
- `make r` — run the built `run` binary
- `make vg` — run under valgrind
- `./run` — start the game directly

The Makefile invokes `sdl-config` (SDL **1.2**, not SDL2). On Linux, install: `g++`, `clang`, `libexpat1-dev`, `libsdl1.2-dev`, `libsdl-mixer1.2-dev`, `libsdl-gfx1.2-dev`, `libsdl-sound1.2-dev`, `libsdl-ttf2.0-dev`. SDL_image is **not** required — image decoding uses the bundled `stb_image.h` (the Makefile does not link `-lSDL_image`). macOS has no SDL 1.2; build against `sdl12-compat` per the macOS section of `README.md`.

There is no test suite.

## Architecture

The game is a side-scrolling 2D shooter written against SDL 1.2. Configuration (sprite paths, speeds, world dimensions, parallax factors, sound settings) is driven from `xmlSpec/game.xml`, parsed once at startup via expat in `parseXML.cpp` and exposed through the `Gamedata` singleton. **To change game tuning, edit the XML, not the C++** — almost every constant in the codebase is read by tag name (e.g. `Gamedata::getInstance().getXmlInt("numberOfEnemies")`).

### Control flow

`main.cpp` → `MenuManager::play()` (main menu loop) → on "Start Game" calls `Manager::play()` (the game loop). The menu reuses one `Manager` instance: when the game ends or the player exits, `menuManager.cpp` does an explicit `manager.~Manager(); new(&manager) Manager();` placement-new reset rather than constructing a fresh object. Keep this in mind when changing `Manager`'s lifecycle — it must be safely re-constructible in place.

`Manager` owns: parallax `World` layers (sky/mountains/house/wall/ground), the `PlayerSprite` (ghostrider), an `std::list<Sprite*>` of enemies, an `std::list<ExplodingSprite*>` for active explosions, a `std::vector<ScaledMultiSprite*>` of background birds/flames sorted by scale (painter's algorithm), the HUD, sound, and a health bar. Its `play()` runs the SDL event loop, calls `update()` then `draw()` per frame, and routes input keys (a/d move, space shoot, m music, p pause, s slo-mo, F1 HUD toggle, F4 capture frames to `frames/*.bmp`, q/ESC quit).

### Sprite hierarchy

`Drawable` (abstract) → `Sprite` (single frame) → `MultiSprite` (animation) → `TwoWayMultiSprite` (left/right facing) → `PlayerSprite` and `EnemySprite`. `ScaledMultiSprite` and `RotatingSprite` are additional `MultiSprite` variants. `ExplodingSprite` wraps a sprite as a `std::list<Chunk>` and is the breakup effect.

### Design patterns in use (don't break these)

- **Singleton** — `Gamedata`, `IOManager`, `Clock`, `FrameFactory`, `Viewport`, `Hud`. All have private constructors and deleted copy/assign; always access via `getInstance()`.
- **Flyweight via Factory** — `FrameFactory` caches `SDL_Surface*` and `Frame*` keyed by sprite name in maps (`surfaces`, `multiSurfaces`, `frames`, `multiFrames`, plus `left*` variants for flipped animation). Frames returned from the factory are shared — never delete them at the sprite level.
- **Object Pool** — `MultiBullets` keeps `bulletList` (active) and `freeList` (recycled) of `MultiBullet`. `ExplodingSprite` does the same with `chunks` and `freeList`. When adding projectile-like or chunk-like behavior, follow the existing pool pattern instead of allocating per-shot.
- **Observer** — `PlayerSprite::attach(EnemySprite*)` / `detach()` notifies enemies of player position so they can switch into pursue/shoot mode (`EnemySprite::MODE` `NORMAL`/`EVADE`). `Manager` wires every enemy into the player's observer list at construction.
- **Strategy** — `CollisionStrategy` has `Rectangular`, `MidPoint`, and `PerPixel` implementations; sprites pick one and the strategy is swappable.

### Parallax & coordinates

`World` draws a background image with a `factor` divisor — higher factor = slower scroll (deeper layer). `Viewport` (singleton) tracks the player and offsets all draws. World size (`world/width`, `world/height` in XML) is independent of view size (`view/width`, `view/height`).

### Frame capture / video

Pressing F4 toggles `makeVideo`. The Manager then writes up to `frameMax` BMPs into the `frames/` directory (named `<username>.NNNN.bmp`). `make clean` deletes them.

## Source layout notes

- All `.cpp`/`.h` live in the repo root — there is no `src/` directory.
- `xmlSpec/game.xml` — the canonical game-tuning file; `xmlSpec/menu.xml` — menu layout.
- `images/`, `sound/`, `font/` — runtime assets referenced by paths inside the XML.
- Build artifacts (`*.o`, `*.d`), orphaned dependency-file fragments (`*.d.*` from interrupted makes), the `run` binary, and runtime-captured `frames/*.bmp` are all git-ignored (see `.gitignore`). The older scratch files (`data attr in player`, `video.mp4`, leftover `menuManager.d.NNNNN`) were removed in commit `7fcb151`.
- README.md is a brief project description; the plain `README` file contains the original Clemson CPSC 870 assignment notes — useful for understanding original intent, not for current development.
