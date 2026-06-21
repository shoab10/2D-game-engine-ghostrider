#include <iostream>
#include <iomanip>
#include <cstring>
#include "ioManager.h"
#include "stb_image.h"
using std::string;

IOManager& IOManager::getInstance() {
  static IOManager io;
  return io;
}

IOManager::IOManager( ) :
  gdata( Gamedata::getInstance() ),
  viewWidth( gdata.getXmlInt("view/width") ),
  viewHeight( gdata.getXmlInt("view/height") ),
  MAX_STRING_SIZE( gdata.getXmlInt("maxStringSize") ),
    // The 3rd and 4th parameters are just as important as the first 2!
    screen(SDL_SetVideoMode(viewWidth, viewHeight, 32, SDL_DOUBLEBUF)),
    font( NULL ), 
    color(),
    inputString("")  
{
  if (screen == NULL) { 
    throw string("Unable to set video mode; screen is NULL in IOMAnager"); 
  }
  if ( TTF_Init() == -1 ) {
    throw string("TTF_Init failed: ") + TTF_GetError();
  }
  font = TTF_OpenFont(
         gdata.getXmlStr("font/file").c_str(), 
         gdata.getXmlInt("font/size")
         );
  if ( !font ) {
    throw string("TTF_OpenFont failed: ") + TTF_GetError();
  }
  color.r = gdata.getXmlInt("font/red");
  color.g = gdata.getXmlInt("font/green");
  color.b = gdata.getXmlInt("font/blue");
  color.unused = gdata.getXmlInt("font/unused");
  SDL_EnableUNICODE( SDL_ENABLE );
  atexit(TTF_Quit);
}

SDL_Surface* IOManager::loadAndSet(const string& filename, bool /*setcolorkey*/) const {
  // Decode with stb_image (SDL_image's loaders return blank surfaces under
  // sdl12-compat). Force RGBA so every sprite carries a per-pixel alpha channel.
  int w = 0, h = 0, channels = 0;
  unsigned char* data = stbi_load(filename.c_str(), &w, &h, &channels, 4);
  if (!data) {
    throw string("Unable to load image ")+filename+": "+stbi_failure_reason();
  }
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  const Uint32 rmask=0xff000000, gmask=0x00ff0000, bmask=0x0000ff00, amask=0x000000ff;
#else
  const Uint32 rmask=0x000000ff, gmask=0x0000ff00, bmask=0x00ff0000, amask=0xff000000;
#endif
  SDL_Surface* surface =
    SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32, rmask, gmask, bmask, amask);
  if (!surface) {
    stbi_image_free(data);
    throw string("SDL_CreateRGBSurface failed for ")+filename;
  }
  SDL_LockSurface(surface);
  for (int y = 0; y < h; ++y) {
    memcpy(static_cast<Uint8*>(surface->pixels) + y*surface->pitch,
           data + y*w*4, static_cast<size_t>(w)*4);
  }
  SDL_UnlockSurface(surface);
  stbi_image_free(data);
  return surface;  // 32-bit RGBA, SDL_SRCALPHA set (per-pixel alpha)
}

void IOManager::printMessageAt(const string& msg, Sint16 x, Sint16 y) const {
   Uint16 zero = 0;
   SDL_Rect dest = {x,y,zero,zero};
   SDL_Surface * stext = TTF_RenderText_Blended(font, msg.c_str(), color);
   if (stext) {
     SDL_BlitSurface( stext, NULL, screen, &dest );
     SDL_FreeSurface(stext);
   }
   else {
     throw 
     string("Couldn't allocate text sureface in printMessageAt");
   }
}

void IOManager::printMessageCenteredAt( const string& msg, Sint16 y) const {
   SDL_Surface *stext = TTF_RenderText_Blended(font, msg.c_str(), color);
   if (stext) {
     Sint16 x = ( viewWidth - stext->w ) / 2;
     SDL_Rect dest = {x,y,0,0};
     SDL_BlitSurface( stext, NULL, screen, &dest );
     SDL_FreeSurface(stext);
   }
   else {
     throw 
     string("Couldn't allocate text sureface in printMessageCenteredAt");
   }
}

void IOManager::printStringAfterMessage( const string& msg,
       Uint32 x, Uint32 y ) const {
   printMessageAt(msg+inputString, x, y);
}

void IOManager::buildString(SDL_Event event) {
  if( inputString.size() <= MAX_STRING_SIZE) {
    unsigned ch = event.key.keysym.sym;
    if ( isalpha(ch) || isdigit(ch) || ch == ' ') {
      inputString += char(event.key.keysym.unicode);
    }
  }     
  if( event.key.keysym.sym == SDLK_BACKSPACE
      && inputString.length() > 0 ) {
      // remove a character from the end
      int length = inputString.size();
      inputString.erase( length - 1 );
  }
}

