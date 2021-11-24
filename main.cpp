#include <iostream>
#include <windows.h>
#include <vector>
#include <cmath>

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

#include "config.h"

SDL_Renderer* r;

#include "util.h"
#include "object.h"

std::vector<Object*> objects;

/* returns the target object's vector index
 * -1 if pos in open space */
int getObjectFromPos(SDL_Point point) {
    for (int i = 0; i < objects.size(); i++) {
        float d = distance(Point{(float)point.x, (float)point.y}, objects[i]->pos);
        if (d < objects[i]->radius)
            return i;
    }
    return -1;
}

SDL_Point windowPos = {0, 0};
BOOL dragging = false;
int dragObject = -1;

void explode(Object *o) {
    for (auto i = 0; i < objects.size(); i++) {
        if (objects[i] == o) {
            if (dragObject == -1) {
                objects.erase(objects.begin() + i);
                break;
            }
            if (objects[dragObject] == o)
                dragging = false;
            objects.erase(objects.begin() + i);
            break;
        }
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

    objects.push_back(new Object(500, 350, 50, 3000.0f, 0, 0, 255, 255, 0, 255));
    objects.push_back(new Object(300, 350, 16, 30.0f, 0, 4, 0, 255, 255, 255));
    objects.push_back(new Object(700, 350, 16, 30.0f, 0, -4, 0, 255, 255, 255));
    objects.push_back(new Object(500, 550, 16, 30.0f, 4, 0, 0, 255, 255, 255));
    objects.push_back(new Object(500, 150, 16, 30.0f, -4, 0, 0, 255, 255, 255));
    //objects.push_back(new Object(300, 350, 16, 30.0f, 0, 4, 0, 255, 255, 255));
    //objects.push_back(new Object(300, 350, 16, 30.0f, 0, 4, 0, 255, 255, 255));

    // mouse drag variables
    SDL_Point lastPos;
    SDL_Point currentPos;

    while(running) {
        SDL_Event event;

        if (SDL_PollEvent(&event)) {
            switch (event.type) {

                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    dragging = true;
                    SDL_GetWindowPosition(win, &windowPos.x, &windowPos.y);
                    GetCursorPos(reinterpret_cast<LPPOINT>(&currentPos));
                    currentPos = currentPos-windowPos;
                    dragObject = getObjectFromPos(currentPos);
                    //dragObject = getObjectFromPos<LPPOINT>(currentPos);
                    break;
                case SDL_MOUSEBUTTONUP:
                    dragging = false;
                    break;

                case SDL_KEYDOWN:
                    switch (event.key.keysym.scancode) {
                        case SDL_SCANCODE_ESCAPE:
                            running = false;
                            break;
                        case SDL_SCANCODE_P:
                            paused = !paused;
                            break;
                        default:
                            break;
                    }
            }
        }

        if (dragging) {
            lastPos = currentPos;
            GetCursorPos(reinterpret_cast<LPPOINT>(&currentPos));
            currentPos = currentPos-windowPos;
            auto diffPos = Point{(float)currentPos.x-lastPos.x, (float)currentPos.y-lastPos.y};
            if (dragObject == -1) {
                for (auto &o: objects) {
                    o->pos = o->pos + diffPos;
                }
            }
            else {
                objects[dragObject]->pos = objects[dragObject]->pos + diffPos;
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
                    float dist = distance(o1->pos, o2->pos);
                    float force = (o1->mass*o2->mass)/(dist*dist);

                    o1->acceleration = o1->acceleration + vec_normalize(o2->pos-o1->pos) * (force/(float)o1->mass);

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

        SDL_Delay(1000 / cfg::framerate);
    }

    // time to clean up all the garbage

    for (Object *o : objects) { free(o); }
    objects.clear();

    SDL_DestroyRenderer(r);

    SDL_DestroyWindow(win);

    SDL_Quit();

    return 0;
}