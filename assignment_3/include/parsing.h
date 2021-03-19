#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <vector>

class MapParser {
};

class Level {
   public:
      Level();

   private:
      std::vector<std::vector<Tile>> map
};
