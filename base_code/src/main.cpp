/*
    Evan Hastings
    CPSC 6160-001
    Spring 2021
    Assignment 1

*/
#include <iostream>
#include <string>
#include <random>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

#define randLower 0
#define randUpper 20
using namespace std;

// Global vars for handling framerate and screen size
uint32_t FPS = 60;
uint32_t frameDuration = 1000 / FPS;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// Global SDL vars
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* texture = NULL;
SDL_Event input;

// Prototypes for initialization and cleanup routines
void init();
void cleanup();

int main(void) {
    init();

    uint32_t currentTime = 0;
    uint32_t startTime = 0;
    uint32_t fpsCounter = 0;
    uint32_t fps = 0;
    uint32_t duration = 0;

    // Load image into a rect on screen; I chose a small image of a trombone :)
    SDL_Surface* tempSurf = IMG_Load("./trombone.png");
    SDL_Rect rect;
    rect.x = 0;
    rect.y = 200;
    rect.w = 50;
    rect.h = 50;
    
    // Set up "background" rect. It is just a rect behind the image.
    SDL_Rect background;
    background.x = SCREEN_WIDTH / 4;
    background.y = SCREEN_HEIGHT / 4;
    background.w = SCREEN_WIDTH / 2;
    background.h = SCREEN_HEIGHT / 2;

    // Create texture from image surface
    texture = SDL_CreateTextureFromSurface(renderer, tempSurf);
    SDL_FreeSurface(tempSurf);

    bool running = true;
    
    // My idea for animation was to have some kind of random motion to make it seem a little more crazy.
    // This is a random distribution generator.
    default_random_engine generator;
    uniform_int_distribution<int> distribution(randLower, randUpper);

    while(running) {
        // Get current ticks passed
        currentTime = SDL_GetTicks();

        // Grab events out of event queue and quit if the X is clicked
        while(SDL_PollEvent(&input)) {
            if(input.type == SDL_QUIT) {
                running = false;
            }
        }

        // Moves the image horizontally at a fixed rate, and moves it vertically randomly
        rect.x += 5;
        rect.y += distribution(generator) - 10;

        // Position resetting if the image goes off screen
        if ( rect.x > SCREEN_WIDTH) rect.x = 0;
        if ( rect.y > SCREEN_HEIGHT) rect.y = 0;
        else if ( rect.y < 0) rect.y = SCREEN_HEIGHT;

        // Draw background rect (not background color)
        SDL_SetRenderDrawColor(renderer, 10,10,200,255);
        SDL_RenderFillRect(renderer, &background);

        // Copy image onto the renderer and present what we have so far
        SDL_RenderCopy(renderer, texture, NULL, &rect);
        SDL_RenderPresent(renderer);

        // Set the renderer background color to black
        SDL_SetRenderDrawColor(renderer, 255,255,255,255);
        SDL_RenderClear(renderer);

        // Check the fps to make sure it is close to the set rate
        fpsCounter++;
        if(currentTime >= (startTime + 1000)) {
            startTime = currentTime;
            fps = fpsCounter;
            fpsCounter = 0;
        }

        // If fps is greater than desired, delay accordingly
        if (duration < frameDuration) {
            SDL_Delay(frameDuration - duration);
        }
    }

    // Destory SDL vars and finish
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
