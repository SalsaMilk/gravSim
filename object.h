//
// Created by Salsa on 11/8/2021.
//

#ifndef GRAVSIM_OBJECT_H
#define GRAVSIM_OBJECT_H

class Object {
private:
public:
    Vector2 acceleration = {0.0, 0.0};
    double mass          = 1000.0;
    Point   pos         = {10.0, 10.0};
    Vector2 velocity    = {0.0, 0.0};
    SDL_Color color     = {100, 100, 100, 255};
    int32_t radius      = 10;

    void draw() const {
        SDL_SetRenderDrawColor(r, color.r, color.g, color.b, color.a);
        DrawCircle(r, pos.x, pos.y, radius);
    }

    void drawVelocity() const {
        SDL_SetRenderDrawColor(r, 255, 0, 0, 255);
        SDL_RenderDrawLine(r, pos.x, pos.y, pos.x+velocity.x*20.0, pos.y+velocity.y*20.0);
    }

    void drawAcceleration() const {
        SDL_SetRenderDrawColor(r, 0, 255, 0, 255);
        SDL_RenderDrawLine(r, pos.x, pos.y, pos.x+acceleration.x*300.0, pos.y+acceleration.y*300.0);
    }

    void move() {
        velocity.x += acceleration.x;
        velocity.y += acceleration.y;
        acceleration = Vector2 {0, 0};
        pos.x += velocity.x;
        pos.y += velocity.y;
    }
    
    Object() = default;
    
    Object(double px, double py, int32_t _radius, double _mass, double vx, double vy) {
        pos         = Point {px, py};
        velocity    = Point {vx, vy};
        radius      = _radius;
        mass        = _mass;
    }

    Object(double px, double py, int32_t _radius, double _mass, double vx, double vy, BYTE _r, BYTE _g, BYTE _b, BYTE _a) {
        pos         = Point {px, py};
        velocity    = Point {vx, vy};
        radius      = _radius;
        mass        = _mass;
        color       = SDL_Color {_r, _g, _b, _a};
    }
};

#endif //GRAVSIM_OBJECT_H
