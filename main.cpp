#include <iostream>
#include <windows.h>
#include <vector>
#include <cmath>
#include <ctime>

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_gfxPrimitives.h"

#include "config.h"

SDL_Renderer* ren;
SDL_Window* win;

#include "util.h"
#include "object.h"
#include "menu.h"

std::vector<Object*> objects;
std::vector<Debris*> extras;

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

BOOL dragging = false;
int selectedObject = -1;

void explode(Object *o) {
    for (int i = 0; i < o->radius/2; i++) {
        extras.push_back(new Debris(o->pos, rand(), rotate_point(o->pos.x, o->pos.y, (ZEROTOHALF - 0.25f)*0.01f, o->velocity/(2.0f-ZEROTOHALF)),
                                    4, SDL_Color{255, (unsigned char) (ZEROTOHALF * 255), 0, 255}));
    }

    for (auto i = 0; i < objects.size(); i++) {
        if (objects[i] == o) {
            if (selectedObject == -1) {
                objects.erase(objects.begin() + i);
                break;
            }
            if (objects[selectedObject] == o) {
                selectedObject = -1;
                dragging = false;
            }
            objects.erase(objects.begin() + i);
            break;
        }
    }
}

BOOL running = true;
BOOL paused = false;

int main(int argc, char *argv[]) {
    srand(time(NULL));

    // SDL_Init returns 0 on success
    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        printf("error initializing SDL: %s\n", SDL_GetError());
        return -1;
    }

    win = SDL_CreateWindow("Grav Sim",
                           SDL_WINDOWPOS_CENTERED, // x
                           SDL_WINDOWPOS_CENTERED, // y
                           1000,  // w
                           600, // h
                           0); // flag

    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);

    //int Object::objectCounter = 0;

    objects.push_back(new Object(500, 350, 50, 3000.0f, 0, 0, 255, 255, 0, 255));
    objects.push_back(new Object(300, 350, 16, 30.0f, 0, 4, 0, 255, 255, 255));
    objects.push_back(new Object(700, 350, 16, 30.0f, 0, 0, 0, 255, 255, 255));
    objects.push_back(new Object(500, 550, 16, 30.0f, 4, 0, 0, 255, 255, 255));
    objects.push_back(new Object(500, 150, 16, 30.0f, -4, 0, 0, 255, 255, 255));

    // mouse drag variables
    SDL_Point lastPos;
    SDL_Point currentPos;

    Uint32 start_time, frame_time;
    while(running) {
        start_time = SDL_GetTicks();

        SDL_Event event;

        if (SDL_PollEvent(&event)) {
            switch (event.type) {

                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    dragging = true;
                    currentPos = getLocalCursorPos();
                    if (selectedObject >= 0) objects[selectedObject]->selected = false;
                    selectedObject = getObjectFromPos(currentPos);
                    if (selectedObject >= 0) objects[selectedObject]->selected = true;
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
            currentPos = getLocalCursorPos();
            auto diffPos = Point{(float)currentPos.x-lastPos.x, (float)currentPos.y-lastPos.y};
            if (selectedObject == -1) {
                for (auto &o: objects) {
                    o->pos = o->pos + diffPos;
                }
            }
            else {
                objects[selectedObject]->pos = objects[selectedObject]->pos + diffPos;
            }
        }

        SDL_SetRenderDrawColor(ren, cfg::bg);

        // clear the screen
        SDL_RenderClear(ren);

        if (paused) {
            for (auto & o : objects) {
                o->acceleration = Vector2 {0, 0};
            }
        }
        for (Object* o1 : objects) {
            for (Object* o2 : objects) {
                if (o1 != o2 && !(o1->selected && dragging)) {
                    float dist = distance(o1->pos, o2->pos);
                    float force = (o1->mass*o2->mass)/(dist*dist);

                    o1->acceleration = o1->acceleration + vec_normalize(o2->pos-o1->pos) * (force/(float)o1->mass);

                    if (distance(o1->pos+o1->velocity, o2->pos+o2->velocity) < o1->radius+o2->radius-3)
                        o1->radius > o2->radius ? explode(o2) : explode(o1);
                }
            }
        }

        for (Object* o : objects) {
            o->draw();
            o->drawVelocity();
            o->drawAcceleration();
        }
        if (selectedObject != -1)
            objects[selectedObject]->drawSelected();

        if (!paused) {
            for (int i = 0; i < objects.size(); i++) {
                if (i == selectedObject && dragging) continue;
                objects[i]->move();
            }
            for (int i = 0; i < extras.size(); i++) {
                extras[i]->update();
                if (extras[i]->color.a == 0) {
                    extras.erase(extras.begin() + i);
                    continue;
                }
                extras[i]->draw();
            }
        }

        SDL_RenderPresent(ren);

        frame_time = SDL_GetTicks()-start_time;

        auto frameDelay = (Uint8)((1000.0f / cfg::framerate) - frame_time);
        if (frameDelay < 0) continue;
        SDL_Delay(frameDelay);
    }

    // time to clean up all the garbage

    for (Object *o : objects) { free(o); }
    objects.clear();

    SDL_DestroyRenderer(ren);

    SDL_DestroyWindow(win);

    SDL_Quit();

    return 0;
}