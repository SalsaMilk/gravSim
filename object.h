//
// Created by Salsa on 11/8/2021.
//

#ifndef GRAVSIM_OBJECT_H
#define GRAVSIM_OBJECT_H

class Object {
private:
public:
    Vector2 acceleration = {0.0f, 0.0f};
    float mass          = 1000.0f;
    Point   pos         = {10.0f, 10.0f};
    Vector2 velocity    = {0.0f, 0.0f};
    SDL_Color color     = {100, 100, 100, 255};
    int32_t radius      = 10;

    void draw() const {
        SDL_SetRenderDrawColor(r, color.r, color.g, color.b, color.a);
        DrawFilledCircle(r, pos.x, pos.y, radius);
    }

    void drawVelocity() const {
        SDL_SetRenderDrawColor(r, 255, 0, 0, 255);
        SDL_RenderDrawLine(r, pos.x, pos.y, pos.x+velocity.x*20.0f, pos.y+velocity.y*20.0f);
    }

    void drawAcceleration() const {
        SDL_SetRenderDrawColor(r, 0, 255, 0, 255);
        SDL_RenderDrawLine(r, pos.x, pos.y, pos.x+acceleration.x*300.0f, pos.y+acceleration.y*300.0f);
    }

    void printPos() const {
        std::cout << "(" << pos.x << "," << pos.y << ")";
    }

    void move() {
        velocity.x += acceleration.x;
        velocity.y += acceleration.y;
        acceleration = Vector2 {0, 0};
        pos.x += velocity.x;
        pos.y += velocity.y;
    }
    
    Object() = default;
    
    Object(float px, float py, int32_t _radius, float _mass, float vx, float vy) {
        pos         = Point {px, py};
        velocity    = Point {vx, vy};
        radius      = _radius;
        mass        = _mass;
    }

    Object(float px, float py, int32_t _radius, float _mass, float vx, float vy, BYTE _r, BYTE _g, BYTE _b, BYTE _a) {
        pos         = Point {px, py};
        velocity    = Point {vx, vy};
        radius      = _radius;
        mass        = _mass;
        color       = SDL_Color {_r, _g, _b, _a};
    }
};

#endif //GRAVSIM_OBJECT_H
