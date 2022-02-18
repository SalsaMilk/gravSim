//
// Created by Salsa on 11/8/2021.
//

#ifndef GRAVSIM_OBJECT_H
#define GRAVSIM_OBJECT_H

class Object {
private:
public:
    Vector2 acceleration = {0.0f, 0.0f};
    Point   pos         = {10.0f, 10.0f};
    Vector2 velocity    = {0.0f, 0.0f};
    SDL_Color color     = {100, 100, 100, 255};

    float mass          = 1000.0f;
    int32_t radius      = 10;

    bool selected = false;

    void draw() const {
        aaFilledCircle(ren, pos.x, pos.y, radius, color.r, color.g, color.b, color.a);
    }

    void drawSelected() const {
        aacircleRGBA(ren, pos.x, pos.y, radius + 3, 255, 255, 255, 255);
        aaellipseRGBA(ren, pos.x, pos.y, radius + 4, radius + 5, 255, 255, 255, 255);
        aaellipseRGBA(ren, pos.x, pos.y, radius + 5, radius + 4, 255, 255, 255, 255);
        aacircleRGBA(ren, pos.x, pos.y, radius + 5, 255, 255, 255, 255);
    }

    void drawVelocity() const {
        aalineRGBA(ren, pos.x, pos.y, pos.x + velocity.x * 20.0f, pos.y + velocity.y * 20.0f, 255, 0, 0, 255);
    }

    void drawAcceleration() const {
        aalineRGBA(ren, pos.x, pos.y, pos.x + acceleration.x * 300.0f, pos.y + acceleration.y * 300.0f, 0, 255, 0, 255);
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

class Debris { // Rendered as triangle
private:
    float sideLength;
public:
    Point v1, v2, v3; // vertices
    Point pos;
    Vector2 velocity;
    SDL_Color color;

    void draw() const {
        filledTrigonRGBA(ren, v1.x, v1.y, v2.x, v2.y, v3.x, v3.y,
                         color.r, color.g, color.b, color.a);
    }

    void update() {
        v1 = v1 + velocity;
        v2 = v2 + velocity;
        v3 = v3 + velocity;

        color.a -= 3;
    }

    Debris(Point _pos, float rotation, Vector2 _velocity, float _size, SDL_Color _color) {
        v1 = rotate_point(_pos.x, _pos.y, rotation, (_pos + Point {0, _size*2}));
        v2 = rotate_point(_pos.x, _pos.y, rotation, (_pos + Point {SQRT3*_size , -_size}));
        v3 = rotate_point(_pos.x, _pos.y, rotation, (_pos + Point {-SQRT3*_size , -_size}));

        pos = _pos;

        velocity    = _velocity;
        color       = _color;

        sideLength = 2 * _size * SQRT3;
    }
};

#endif //GRAVSIM_OBJECT_H