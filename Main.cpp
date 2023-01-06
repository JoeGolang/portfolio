#include <time.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"
#include "helper.h"
#include "Hero.h"
#include "Game.h"
#include "tree.h"


using namespace std;

void background(SDL_Surface*& screen, SDL_Surface*& tile){
     for(int y=0; y<480; y+=256){
       for(int x=0; x<640; x+=256){
         apply_surface( x, y, tile, screen );
       }
     }
}

void initialize(SDL_Surface*& screen, SDL_Surface*& tile){
     screen = SDL_SetVideoMode( 640, 480, 32, SDL_DOUBLEBUF|SDL_HWSURFACE|SDL_ANYFORMAT );
     
     SDL_WM_SetCaption( "Dilarang injek rumput !!!", NULL );
     SDL_Surface *icon = SDL_LoadBMP("./image/icon.bmp");
     SDL_WM_SetIcon(icon, NULL);
     
     tile = SDL_LoadBMP( "./image/stoneTile.bmp" );
     background(screen, tile);
}

int collisionTree(Character me, Tree x){
    if( (me.getPosX() + 24 >= x.getPosX()) && (me.getPosX() <= x.getPosX() + 16) && 
        (me.getPosY() + 24 >= x.getPosY()) && (me.getPosY() <= x.getPosY() + 16) )
        return 1;
    else return 0;
}

int main(int argc, char *argv[])
{
    srand(time(NULL));  
    int ctr;
    bool done = false;
    SDL_Surface *screen, *tile, *message;
    Uint8 *key;
    
    Game *game = new Game();
    Character *me = new Character();
    
    SDL_Init( SDL_INIT_EVERYTHING );
    if( TTF_Init() == -1 ) return 0; 
    initialize(screen, tile);
    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL); 
    TTF_Font *font = NULL;
    SDL_Color textColor = { 255, 255, 255 };
    font = TTF_OpenFont( "KGRedHands.ttf", 28 );
    
    Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 );
    Mix_Music *bgm = NULL;
    Mix_Chunk *sfx = NULL;
    bgm = Mix_LoadMUS( "./musics/19.mp3" );
    Mix_VolumeMusic(60);
    sfx = Mix_LoadWAV( "./musics/croc_chomp_x.wav" );
    Mix_VolumeChunk(sfx, 20);
    
    Tree *tree[50];
         for(int i=0; i<50; i++){
         tree[i] = new Tree();
         tree[i]->load();
         }
    
    initialize(screen, tile);
    me->print(screen);
    me->loadSprite();
    me->printMove(screen);
    SDL_Flip( screen );
    Mix_PlayMusic( bgm, -1 );
    
    while ( !game->getDone() ){
    SDL_Event event;
	while ( SDL_PollEvent ( &event ) ){
      if ( event.type == SDL_QUIT ) game->setDone(true);
  	  key = SDL_GetKeyState( NULL );
      if ( key[SDLK_DOWN] ){ me->goDown();}//sfx = Mix_LoadWAV( "./musics/bloop_x.wav" );Mix_PlayChannel( -1, sfx, 0 ); SDL_Delay( 0 );}
      else if ( key[SDLK_UP] ){ me->goUp();}//sfx = Mix_LoadWAV( "./musics/bloop_x.wav" );Mix_PlayChannel( -1, sfx, 0 ); SDL_Delay( 0 );}
      else if ( key[SDLK_RIGHT] ){ me->goRight();}//sfx = Mix_LoadWAV( "./musics/bloop_x.wav" );Mix_PlayChannel( -1, sfx, 0 ); SDL_Delay( 0 );}
      else if ( key[SDLK_LEFT] ){ me->goLeft();}//sfx = Mix_LoadWAV( "./musics/bloop_x.wav" );Mix_PlayChannel( -1, sfx, 0 ); SDL_Delay( 0 );}
      
      background(screen,tile);
      
      for(int i=0; i<50; i++){
      if(collisionTree(*me,*tree[i])==1){
        ctr++; 
        sfx = Mix_LoadWAV( "./musics/ahem_x.wav" );
        Mix_PlayChannel( -1, sfx, 0 );
        char buffer[100];
        message = TTF_RenderText_Solid( font, "Injek Rumput !!", textColor );
        apply_surface( 400, 1, message, screen );
        }
      }
      
      for(int i=0; i<50; i++){
        tree[i]->print(screen);
      }
      
      me->printMove(screen);
      
      SDL_Flip( screen );
      }
    }
    
    if(ctr >= 10){
           message = TTF_RenderText_Solid( font, "Kebanyakan Injek Rumputnya!!!", textColor );
           apply_surface( 100, 200, message, screen );
           Mix_HaltMusic();
           bgm = Mix_LoadMUS( "./musics/10.mp3" );
           Mix_PlayMusic( bgm, -1 );
           }
    SDL_Flip( screen );
    SDL_Delay(3000);
       
    TTF_CloseFont( font );
    TTF_Quit();  
    Mix_FreeChunk( sfx );
    Mix_FreeMusic( bgm );
    Mix_CloseAudio();
    SDL_FreeSurface(tile);
    me->freeSurface();
    SDL_Quit();
    
    return 0;
}
