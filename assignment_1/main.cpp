#include <iostream>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

using namespace std;

uint32_t FPS = 60;
uint32_t frameDuration = 1000 / FPS;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* texture = NULL;
SDL_Event input;


void init();
void cleanup();

int main(void) {
    init();

    uint32_t currentTime = 0;
    uint32_t startTime = 0;
    uint32_t fpsCounter = 0;
    uint32_t fps = 0;
    uint32_t duration = 0;
    SDL_Surface* tempSurf = IMG_Load("./trombone.png");
    SDL_Rect rect;
    rect.x = 0;
    rect.y = 200;
    rect.w = 50;
    rect.h = 50;

    texture = SDL_CreateTextureFromSurface(renderer, tempSurf);
    SDL_FreeSurface(tempSurf);


    bool running = true;

    while(running) {
        currentTime = SDL_GetTicks();

        while(SDL_PollEvent(&input)) {
            if(input.type == SDL_QUIT) {
                running = false;
            }
        }

        rect.x += 5;
        if ( rect.x > SCREEN_WIDTH) rect.x = 0;
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, &rect);
        SDL_RenderPresent(renderer);

        fpsCounter++;
        if(currentTime >= (startTime + 1000)) {
            startTime = currentTime;
            fps = fpsCounter;
            fpsCounter = 0;
        }
        cout << fps << endl;

        if (duration < frameDuration) {
            SDL_Delay(frameDuration - duration);
        }
    }

    cleanup();
    return 0;
}

void init() {
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        cout << "Error initializing SDL: " << SDL_GetError() << endl;
    }
    IMG_Init(IMG_INIT_PNG);

    window = SDL_CreateWindow("assignment_1 - Evan",
               SDL_WINDOWPOS_CENTERED,
               SDL_WINDOWPOS_CENTERED,
               SCREEN_WIDTH,
               SCREEN_HEIGHT,
               0
               );
    renderer = SDL_CreateRenderer(window,-1,0);
}

void cleanup() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(texture);
    SDL_DestroyWindow(window);

    IMG_Quit();
    SDL_Quit();
}
