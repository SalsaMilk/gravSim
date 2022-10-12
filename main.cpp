#include <iostream>
#include <windows.h>
#include <vector>
#include <cmath>
#include <ctime>

#include "SDL/SDL.h"
#include "SDL/SDL_gfxPrimitives.h"

#include "config.h"

SDL_Renderer* ren;
SDL_Window* win;

#include "util.h"
#include "object.h"
//#include "menu.h"

std::vector<Object*> objects;
std::vector<Debris*> extras;

BOOL dragging = false;
int selectedObject = -1;

/*
 * returns the target object's vector index
 * -1 if pos in open space
 */
int getObjectFromPos(SDL_Point point) {
    for (int i = 0; i < objects.size(); i++) {
        float d = distance(Point{(float)point.x, (float)point.y}, objects[i]->pos);
        if (d < objects[i]->radius)
            return i;
    }
    return -1;
}

/*
 * It turns out this is VERY difficult to do
 * https://en.wikipedia.org/wiki/Elastic_collision
 * http://www.vobarian.com/bouncescope/
 */
void handleCollision(Object *o1, Object *o2) {
    // Avoid division by zero below in computing new normal velocities
    // Doing a collision where both balls have no mass makes no sense anyway
    if (o1->mass == 0 && o2->mass == 0) return;

    // Compute unit normal and unit tangent vectors
    Vector2 v_n = o2->pos - o1->pos;                // normal vec
    Vector2 v_un = vec_normalize(v_n);            // unit normal vector
    auto v_ut = Vector2{-v_un.y, v_un.x};       // unit tangent vector

    // Compute scalar projections of velocities onto v_un and v_ut using dot product
    float v1n = v_un.x * o1->velocity.x + v_un.y * o1->velocity.y;
    float v1t = v_ut.x * o1->velocity.x + v_ut.y * o1->velocity.y;
    float v2n = v_un.x * o2->velocity.x + v_un.y * o2->velocity.y;
    float v2t = v_ut.x * o2->velocity.x + v_ut.y * o2->velocity.y;

    // Compute new tangential velocities
    float v1tPrime = v1t; // Note: in reality, the tangential velocities do not change after the collision
    float v2tPrime = v2t;

    // Compute new normal velocities using one-dimensional elastic collision equations in the normal direction
    // Division by zero avoided. See early return above.
    float v1nPrime = (v1n * (o1->mass - o2->mass) + 2.0f * o2->mass * v2n) / (o1->mass + o2->mass);
    float v2nPrime = (v2n * (o2->mass - o1->mass) + 2.0f * o1->mass * v1n) / (o1->mass + o2->mass);

    // Compute new normal and tangential velocity vectors
    Vector2 v_v1nPrime = v_un * v1nPrime;
    Vector2 v_v1tPrime = v_ut * v1tPrime;
    Vector2 v_v2nPrime = v_un * v2nPrime;
    Vector2 v_v2tPrime = v_ut * v2tPrime;

    // Set new velocities in x and y coordinates
    o1->velocity.x = v_v1nPrime.x + v_v1tPrime.x;
    o1->velocity.y = v_v1nPrime.y + v_v1tPrime.y;
    o2->velocity.x = v_v2nPrime.x + v_v2tPrime.x;
    o2->velocity.y = v_v2nPrime.y + v_v2tPrime.y;
}

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

    win = SDL_CreateWindow("Grav Sim",              // title
                           SDL_WINDOWPOS_CENTERED,  // x
                           SDL_WINDOWPOS_CENTERED,  // y
                           1000,                    // w
                           600,                     // h
                           0);                      // flag

    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);

    objects.push_back(new Object(500, 350, 30, 3000.0f, 0, 0, 255, 255, 0, 255));
    objects.push_back(new Object(300, 350, 16, 30.0f, 0, 4, 0, 255, 255, 255));
    objects.push_back(new Object(700, 350, 16, 30.0f, 0, -4, 0, 255, 255, 255));
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

        SDL_SetRenderDrawColor(ren, 255, 255, 255, 100);
        if (cfg::show_grid) {
            for (float y = 0; y <= 600; y += cfg::grid_scale) {
                for (float x = 0; x <= 1000; x += cfg::grid_scale) {
                    auto p = Point{x, y};
                    Vector2 p2 = Vector2{0, 0};
                    for (Object* obj : objects) {
                        float dist = distance(obj->pos, p);
                        float length = (obj->mass / (dist * dist)) * 80;
                        if (length > cfg::grid_scale) length = cfg::grid_scale;
                        p2 = p2 + vec_normalize(obj->pos-p) * length;
                    }
                    //aalineRGBA(ren, p.x, p.y, p.x+p2.x, p.y+p2.y, 255, 255, 255, 100);
                    SDL_RenderDrawPoint(ren, p.x+p2.x, p.y+p2.y);
                }
            }
        }

        if (paused)
            for (auto & o : objects)
                o->acceleration = Vector2 {0, 0};

        for (Object* o1 : objects) {
            for (Object* o2 : objects) {
                if (o1 != o2 && !(o1->selected && dragging)) {
                    float dist = distance(o1->pos, o2->pos);
                    float force = (o1->mass*o2->mass) / (dist*dist);

                    o1->acceleration = o1->acceleration + vec_normalize(o2->pos-o1->pos) * (force/(float)o1->mass);

                    if (distance(o1->pos+o1->velocity, o2->pos+o2->velocity) < o1->radius+o2->radius-3)
                        handleCollision(o1, o2);
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