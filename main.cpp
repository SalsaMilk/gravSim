#include <iostream>
#include <windows.h>

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

#define NOP

SDL_Renderer* r;

#include "util.h"
#include "object.h"

BOOL running = true;

int main(int argc, char *argv[]) {
    // SDL_Init returns 0 on success
    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        printf("error initializing SDL: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Window* win = SDL_CreateWindow("Grav Sim",
                                       SDL_WINDOWPOS_CENTERED, // x
                                       SDL_WINDOWPOS_CENTERED, // y
                                       800,  // w
                                       600, // h
                                       0); // flag

    r = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);

    auto *earth = new Object(150, 200, 40, 100, 3, 3);
    auto *moon  = new Object(300, 200, 16, 50, 0, 0);



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
                            running = false;
                            break;
                        default:
                            break;
                    }
            }
        }

        SDL_SetRenderDrawColor(r, 0, 0, 0, 255);

        // clear the screen
        SDL_RenderClear(r);

        earth->move();
        earth->draw();
        earth->drawVelocity();
        earth->drawAcceleration();

        moon->move();
        moon->draw();

        SDL_RenderPresent(r);

        SDL_Delay(1000 / 60);
    }

    SDL_DestroyRenderer(r);

    SDL_DestroyWindow(win);

    SDL_Quit();

    return 0;
}