#include "SDL/SDL.h"

class Game{
  private:
    bool done;
    Character *player;
          
  public:    
    Game();
    void setDone(bool done){ this->done = done; }          
    bool getDone(){return done;}
};


Game::Game(){
  this->done = false;
}
