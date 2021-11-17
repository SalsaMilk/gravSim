#include <iostream>
#include <windows.h>
#include <vector>
#include <cmath>

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

#define FRAMERATE 60

SDL_Renderer* r;

#include "util.h"
#include "object.h"

std::vector<Object*> objects;

void explode(Object *o) {
    for (auto i = 0; i < objects.size(); i++) {
        if (objects[i] == o)
            objects.erase(objects.begin() + i);
    }
    /* somehow do a cool explosion*/
}

BOOL running = true;
BOOL paused = false;

int main(int argc, char *argv[]) {
    // SDL_Init returns 0 on success
    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        printf("error initializing SDL: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Window* win = SDL_CreateWindow("Grav Sim",
                                       SDL_WINDOWPOS_CENTERED, // x
                                       SDL_WINDOWPOS_CENTERED, // y
                                       1000,  // w
                                       600, // h
                                       0); // flag

    r = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);

    objects.push_back(new Object(500, 350, 50, 30000.0, 0, 0, 255, 255, 0, 255));
    objects.push_back(new Object(200, 350, 16, 8.0, 0, 8, 0, 255, 255, 255));

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
                        case SDL_SCANCODE_P:
                            paused = !paused;
                            break;
                        case SDL_SCANCODE_UP:
                            for (auto & o : objects) {
                                o->pos = o->pos+Point {0, -30};
                            }
                            break;
                        case SDL_SCANCODE_DOWN:
                            for (auto & o : objects) {
                                o->pos = o->pos+Point {0, 30};
                            }
                            break;
                        case SDL_SCANCODE_LEFT:
                            for (auto & o : objects) {
                                o->pos = o->pos+Point {-30, 0};
                            }
                            break;
                        case SDL_SCANCODE_RIGHT:
                            for (auto & o : objects) {
                                o->pos = o->pos+Point {30, 0};
                            }
                            break;
                        default:
                            break;
                    }
            }
        }

        SDL_SetRenderDrawColor(r, 0, 0, 0, 255);

        // clear the screen
        SDL_RenderClear(r);

        if (paused) {
            for (auto & o : objects) {
                o->acceleration = Vector2 {0, 0};
            }
        }
        for (Object* o1 : objects) {
            for (Object* o2 : objects) {
                if (o1 != o2) {
                    double dist = distance(o1->pos, o2->pos);
                    double force = (o1->mass*o2->mass)/(dist*dist);

                    o1->acceleration = o1->acceleration + vec_normalize(o2->pos-o1->pos) * (force/(double)o1->mass);
                    //o2->acceleration = vec_normalize(o1->pos-o2->pos) * (force/(double)o2->mass);

                    if (distance(o1->pos+o1->velocity, o2->pos+o2->velocity) < o1->radius+o2->radius-3) {
                        o1->radius > o2->radius ? explode(o2) : explode(o1);
                    }

                }
            }
        }

        for (Object* o : objects) {
            o->draw();
            o->drawVelocity();
            o->drawAcceleration();
            if (!paused)
                o->move();
        }

        SDL_RenderPresent(r);

        SDL_Delay(1000 / FRAMERATE);
    }
    for (Object *o : objects) { free(o); }

    SDL_DestroyRenderer(r);

    SDL_DestroyWindow(win);

    SDL_Quit();

    return 0;
}