/*
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"
#include "helper.h"
#include "Character.h"
#include "Gift.h"
#include "Bomb.h"
#include "Game.h"
#include <time.h>
#include <string.h>
#include <iostream>
#include <stdlib.h>

void background(SDL_Surface*& screen, SDL_Surface*& grassTile){
     //Apply the grassTile to the screen
     for(int y=0; y<480; y+=64){
       for(int x=0; x<640; x+=64){
         apply_surface( x, y, grassTile, screen );
       }
     }
}

void initialize(SDL_Surface*& screen, SDL_Surface*& grassTile){
     //Set up the screen
     screen = SDL_SetVideoMode( 640, 480, 32, SDL_DOUBLEBUF|SDL_HWSURFACE|SDL_ANYFORMAT );
    
     //Set the window caption
     SDL_WM_SetCaption( "Gift Chaser", NULL );
     SDL_Surface *icon = SDL_LoadBMP("./images/icon.bmp");
     SDL_WM_SetIcon(icon, NULL);
     
     //Load the Background Tiles image
     //grassTile = load_image( "./images/grass_tile.bmp" );
     grassTile = SDL_LoadBMP( "./images/grass_tile.bmp" );
     
     background(screen, grassTile);
}

int collision(Character me, Gift gift){
    if( (me.getPosX() + 24 >= gift.getPosX()) && (me.getPosX() <= gift.getPosX() + 16) && 
        (me.getPosY() + 24 >= gift.getPosY()) && (me.getPosY() <= gift.getPosY() + 16) )
        return 1;
    else return 0;
}

int collisionBomb(Character me, Bomb gift){
    if( (me.getPosX() + 24 >= gift.getPosX()) && (me.getPosX() <= gift.getPosX() + 16) && 
        (me.getPosY() + 24 >= gift.getPosY()) && (me.getPosY() <= gift.getPosY() + 16) )
        return 1;
    else return 0;
}

int main(int argc, char *argv[])
{
  srand(time(NULL));  
    
  bool done = false;
  SDL_Surface *screen, *grassTile, *message;
  Uint8 *key;
  
  Game *game = new Game();
  Character *me = new Character();
  Gift *gift = new Gift();
  
  SDL_Init( SDL_INIT_EVERYTHING );
  //Initialize SDL_ttf
  if( TTF_Init() == -1 ) return 0; 
  initialize(screen, grassTile);
  SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL); 
  
  //The font that's going to be used
  TTF_Font *font = NULL;
  //The color of the font
  SDL_Color textColor = { 255, 255, 255 };
  
  //Open the font
  font = TTF_OpenFont( "KGRedHands.ttf", 28 );
  //Render the text
  //message = TTF_RenderText_Solid( font, "The quick brown fox jumps over the lazy dog", textColor );
  //apply_surface( 0, 150, message, screen );    
  
  //The background music that will be played
  Mix_Music *bgm = NULL;
  //The sound effects that will be used
  Mix_Chunk *sfx = NULL;
  
  //Initialize SDL_mixer
  Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 );
  
  //Load the music
  bgm = Mix_LoadMUS( "./musics/pacman_beginning.wav" );
  Mix_VolumeMusic(18);
  //Load the sound effects
  sfx = Mix_LoadWAV( "./musics/pacman_chomp.wav" );
  Mix_VolumeChunk(sfx, 60);
  
  Bomb *bomb[10];
  for(int i=0; i<10; i++){
    bomb[i] = new Bomb();
    bomb[i]->loadSprite();
  }
  
  initialize(screen, grassTile);
  me->loadSprite();
  gift->loadSprite();
  me->printMove(screen);
  SDL_Flip( screen );
  
  //Play the background music
  Mix_PlayMusic( bgm, -1 );
  
  while ( !game->getDone() ){
    SDL_Event event;
	while ( SDL_PollEvent ( &event ) )
	{
      // if "X" Button on Title Bar is hit
      if ( event.type == SDL_QUIT ) game->setDone(true);
	  
      // Get Keyboard Input
  	  key = SDL_GetKeyState( NULL );
      if ( key[SDLK_DOWN] ) me->goDown();
      else if ( key[SDLK_UP] ) me->goUp();
      else if ( key[SDLK_RIGHT] ) me->goRight();
      else if ( key[SDLK_LEFT] ) me->goLeft();
      
      if(collision(*me,*gift)==1){ 
        //Play the scratch effect
        Mix_PlayChannel( -1, sfx, 0 );
        game->addScore();
        
        if(game->getScore() >= 10) game->setDone(true);
        else gift->move(); 
      }
      
      for(int i=0; i<10; i++){
        if(collisionBomb(*me,*bomb[i])==1){
          game->setDone(true);
          sfx = Mix_LoadWAV( "./musics/explosion.wav" );
          Mix_PlayChannel( -1, sfx, 0 );
        }
      }
      
      // Update the grass, character and gift  
      background(screen, grassTile);
      
      for(int i=0; i<10; i++){
        bomb[i]->print(screen);
      }
      
      gift->print(screen);
      me->printMove(screen);
      
      
      
      char buffer[8];
      itoa(game->getScore(),buffer,10);
      strcat(buffer,"/10");
      message = TTF_RenderText_Solid( font, buffer, textColor );
      apply_surface( 560, 1, message, screen );
      SDL_Flip( screen );
    }
  }
  
  char buffer[50];
  if(game->getScore() == 10){
    strcpy(buffer,"YOU WIN - CONGRATS !");
    Mix_HaltMusic();
    bgm = Mix_LoadMUS( "./musics/FF8_The_Winner.mp3" );
    Mix_PlayMusic( bgm, -1 );
  }
  else{
    strcpy(buffer,"You Lose, Game Over !");
    Mix_HaltMusic();
    bgm = Mix_LoadMUS( "./musics/FF8_The_Loser.mp3" );
    Mix_PlayMusic( bgm, -1 );
  }
  
  message = TTF_RenderText_Solid( font, buffer, textColor );
  apply_surface( 160, 210, message, screen );
  SDL_Flip( screen );
  
  while(Mix_PlayingMusic()== 1){
    SDL_Event event;                         
    while ( SDL_PollEvent ( &event ) ){
      // if "X" Button on Title Bar is hit
      if ( event.type == SDL_QUIT ) Mix_HaltMusic();
    }                         
  }
    
  //Free the surfaces
  SDL_FreeSurface( grassTile );
  me->freeSurface();
  
  //Close the font
  TTF_CloseFont( font );
  
  //Free the sound effects
  Mix_FreeChunk( sfx );
  //Free the music
  Mix_FreeMusic( bgm );
  
  //Quit SDL_ttf
  TTF_Quit();
  
  //Quit SDL_mixer
  Mix_CloseAudio();
    
  //Quit SDL
  SDL_Quit();

  return 1;
}
*/
