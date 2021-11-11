//
// Created by Salsa on 11/8/2021.
//

#ifndef GRAVSIM_OBJECT_H
#define GRAVSIM_OBJECT_H

typedef struct {
    int32_t x, y;
} Vector2, Point;

class Object {
private:
    int32_t density = 100;
    int32_t mass = 1000;
public:
    Point   pos         = {10, 10};
    Vector2 velocity    = {0, 0};
    SDL_Color color     = {255, 255, 255, 255};
    int32_t radius      = 10;

    void draw() const {
        SDL_SetRenderDrawColor(r, color.r, color.g, color.b, color.a);
        DrawCircle(r, pos.x, pos.y, radius);
        SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
    }

    void move() {
        pos.x += velocity.x;
        pos.y += velocity.y;
    }
    
    Object() = default;
    
    Object(int32_t px, int32_t py, int32_t _radius, int32_t _mass, int32_t vx, int32_t vy) {
        pos.x       = px;
        pos.y       = py;
        radius      = _radius;
        mass        = _mass;
        velocity.x  = vx;
        velocity.y  = vy;
        density     = _mass/_radius;
        color.r = 255 * (1.0f - 1.0f / (float)density);
        color.g = 255 * (1.0f / (float)density);
        //  color.b = 255 * (1.0f / (float)density);
    }
};

#endif //GRAVSIM_OBJECT_H
