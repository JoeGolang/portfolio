#include "SDL/SDL.h"

class Character{
    int posX, posY;
    int cropX, cropY, dir;
  public:
    SDL_Surface *sprite;     
    Character();
    
    int getPosX(){return posX;}
    int getPosY(){return posY;}
    
    void loadSprite();
    void print(SDL_Surface *& screen);
    void printMove(SDL_Surface *& screen);
    void freeSurface();
    
    void calcCrop();
    
    void goUp();
    void goDown();
    void goLeft();
    void goRight();
};

Character::Character(){
  posX = posY = 1;
  dir = 30;
}

void Character::loadSprite(){
     sprite = load_image( "./image/character5.bmp" );
     Uint32 colorKey = SDL_MapRGB( sprite->format, 255, 0, 255 );
     SDL_SetColorKey(sprite, SDL_SRCCOLORKEY, colorKey );
}

void Character::print(SDL_Surface *& screen){
     apply_surface( posX, posY, sprite, screen );
}

void Character::freeSurface(){
     SDL_FreeSurface( sprite );
}

void Character::calcCrop(){ 
     int x = (dir%10);
     int y = (dir/10);
     
     cropX = x*32;
     cropY = y*32;
}

void Character::goUp(){
     posY -= 8;
     if(posY > 0){ 
       if( dir/10 == 2 ){ dir++; if(dir%10 > 4)dir=21; }
       else dir = 20;
     }
     else posY = 1;
}

void Character::goDown(){
     posY += 8;
     if(posY < 480-32){ 
       if( dir/10 == 3 ){ dir++; if(dir%10 > 4)dir=31;}
       else dir = 30;
     }
     else posY = 480-32;
}

void Character::goLeft(){
     posX -= 8;
     if(posX > 0){ 
       if( dir/10 == 0 ){ dir++; if(dir%10 > 4)dir=1; }
       else dir = 0;
     }
     else posX = 1;
}

void Character::goRight(){
     posX += 8;
     if(posX < 640-32){ 
       if( dir/10 == 1 ){ dir++; if(dir%10 > 4)dir=11;}
       else dir = 10;
     }
     else posX = 640-32;
}

void Character::printMove(SDL_Surface *& screen){
     calcCrop();
     apply_cropped_surface( posX, posY, cropX, cropY, sprite, screen );
}
