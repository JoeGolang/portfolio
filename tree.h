#include "SDL/SDL.h"

class Tree{
    int posX, posY;
    
  public:
    SDL_Surface *img;     
    Tree();
    
    int getPosX(){return posX;}
    int getPosY(){return posY;}
    
    void load();
    
    void print(SDL_Surface *& screen);
    
    void freeSurface();
    
};

Tree::Tree(){
  posX = rand()%(640-32) + 1;
  posY = rand()%(480-32) + 1;
}

void Tree::load(){
     img = SDL_LoadBMP( "./image/tree.bmp" );
}

void Tree::print(SDL_Surface *& screen){
     apply_surface( posX, posY, img, screen );
}

void Tree::freeSurface(){
     SDL_FreeSurface( img );
}
