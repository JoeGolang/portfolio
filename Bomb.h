// Bomb.h
#include "SDL/SDL.h"
//#include "helper.h"

class Bomb{
    int posX, posY;
    SDL_Surface *sprite;
  public:
    Bomb();
    
    int getPosX(){return posX;}
    int getPosY(){return posY;}
         
    void loadSprite();
    void print(SDL_Surface *& screen);
    
    void move();
};

Bomb::Bomb(){
  posX = rand()%(640-32) + 1;
  posY = rand()%(480-32) + 1;
}

void Bomb::loadSprite(){
     // Load Sprite Set Image
     sprite = load_image( "./image/bomb.bmp" );
     // Set pink color transparent
     Uint32 colorKey = SDL_MapRGB( sprite->format, 255, 0, 255 );
     SDL_SetColorKey(sprite, SDL_SRCCOLORKEY, colorKey );
}

void Bomb::print(SDL_Surface *& screen){
     apply_surface( posX, posY, sprite, screen );
}

void Bomb::move(){
     posX = rand()%(640-32) + 1;
     posY = rand()%(480-32) + 1;
}
