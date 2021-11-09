#include <iostream>
#include <windows.h>
#include "SDL/SDL.h"

#define NOP

#include "object.h"

SDL_Renderer* r;
BOOL running = true;

int main(int argc, char *argv[]) {
    srand(time(NULL));

    // SDL_Init returns 0 on success
    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        printf("error initializing SDL: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Window* win = SDL_CreateWindow("Grav Sim",
                                       SDL_WINDOWPOS_CENTERED, // x
                                       SDL_WINDOWPOS_CENTERED, // y
                                       800,  // w
                                       500, // h
                                       0); // flag

    r = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);



    while(running) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            switch (event.type) {

                case SDL_QUIT:
                    running = false;
                    break;

                case SDL_KEYDOWN:
                    switch (event.key.keysym.scancode) {
                        case SDL_SCANCODE_ESCAPE:
                            break;
                        default:
                            break;
                    }
            }
        }
        SDL_SetRenderDrawColor(r, 0, 0, 0, 255);

        // clear the screen
        SDL_RenderClear(r);

        SDL_RenderPresent(r);

        SDL_Delay(1000 / 60);
    }

    SDL_DestroyRenderer(r);

    SDL_DestroyWindow(win);

    SDL_Quit();

    return 0;
}