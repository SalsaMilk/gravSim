//
// Created by Salsa on 11/8/2021.
//

#ifndef GRAVSIM_OBJECT_H
#define GRAVSIM_OBJECT_H

class Object {
private:
    int32_t density      = 100;
    int32_t mass         = 1000;
    Vector2 acceleration = {0.0f, 0.0f};
public:
    Point   pos         = {10.0f, 10.0f};
    Vector2 velocity    = {0.0f, 0.0f};
    SDL_Color color     = {255, 255, 255, 255};
    int32_t radius      = 10;

    void draw() const {
        SDL_SetRenderDrawColor(r, color.r, color.g, color.b, color.a);
        DrawCircle(r, pos.x, pos.y, radius);
        SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
    }

    void drawVelocity() const {
        SDL_SetRenderDrawColor(r, 255, 0, 0, 255);
        SDL_RenderDrawLine(r, pos.x, pos.y, pos.x+velocity.x*20.0f, pos.y+velocity.y*20.0f);
        SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
    }

    void drawAcceleration() const {
        SDL_SetRenderDrawColor(r, 0, 255, 0, 255);
        SDL_RenderDrawLine(r, pos.x, pos.y, pos.x+acceleration.x*1000.0f, pos.y+acceleration.y*1000.0f);
        SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
    }

    void move() {
        pos.x += velocity.x;
        pos.y += velocity.y;
        velocity.x += acceleration.x;
        velocity.y += acceleration.y;
    }
    
    Object() = default;
    
    Object(float px, float py, int32_t _radius, int32_t _mass, float vx, float vy) {
        pos         = Point {px, py};
        velocity    = Point {vx, vy};
        radius      = _radius;
        mass        = _mass;
        density     = _mass/_radius;
        color.r = 255 * (1.0f - 1.0f / (float)density);
        color.g = 255 * (1.0f / (float)density);
    }
};

#endif //GRAVSIM_OBJECT_H
