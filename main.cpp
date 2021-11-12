#include <iostream>
#include <windows.h>
#include <vector>
#include <cmath>

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

    std::vector<Object*> objects;
    objects.push_back(new Object(300, 300, 50, 3000.0f, 0, 0));
    objects.push_back(new Object(450, 300, 10, 30.0f, 0, 4.0f));


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
        
        for (Object* o1 : objects) {
            for (Object* o2 : objects) {
                if (o1 != o2) {
                    float dist = distance(o1->pos, o2->pos);
                    float force = (o1->mass*o2->mass)/(dist*dist);

                    o1->acceleration = vec_normalize(o2->pos-o1->pos) * (force/(float)o1->mass);
                    o2->acceleration = vec_normalize(o1->pos-o2->pos) * (force/(float)o2->mass);
                    /*
                    if (distance(o1->pos+o1->velocity, o2->pos+o2->velocity) < o1->radius+o2->radius-3) {
                        o1->velocity = o1->velocity*-1;
                        o2->velocity = o2->velocity*-1;
                    }
                    */
                }
            }
        }


        for (Object* o : objects) {
            o->move();
            o->draw();
            o->drawVelocity();
            o->drawAcceleration();
        }

        SDL_RenderPresent(r);

        SDL_Delay(1000 / 60);
    }
    while(!GetAsyncKeyState(VK_ESCAPE));

    SDL_DestroyRenderer(r);

    SDL_DestroyWindow(win);

    SDL_Quit();

    return 0;
}