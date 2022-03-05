#ifndef GRAVSIM_OBJECT_H
#define GRAVSIM_OBJECT_H

class Object {
public:
    Point pos               = {10.0f, 10.0f};
    Vector2 velocity        = {0.0f, 0.0f};
    Vector2 acceleration    = {0.0f, 0.0f};
    SDL_Color color         = {100, 100, 100, 255};

    float mass = 1000.0f;
    float radius = 10;

    bool selected = false;

    void draw() const {
        aaFilledCircle(ren, (short)pos.x, (short)pos.y, (short)radius,
                       color.r, color.g, color.b, color.a);
    }

    void drawSelected() const {
        aaellipseRGBA(ren, (short)pos.x, (short)pos.y, (short)(radius + 4.0f), (short)(radius + 5.0f), 255, 255, 255, 255);
        aaellipseRGBA(ren, (short)pos.x, (short)pos.y, (short)(radius + 5.0f), (short)(radius + 4.0f), 255, 255, 255, 255);
        aacircleRGBA(ren, (short)pos.x, (short)pos.y, (short)(radius + 3.0f), 255, 255, 255, 255);
        aacircleRGBA(ren, (short)pos.x, (short)pos.y, (short)(radius + 5.0f), 255, 255, 255, 255);
    }

    void drawVelocity() const {
        aalineRGBA(ren, (short)pos.x, (short)pos.y,
                   (short)(pos.x + velocity.x * 20.0f), (short)(pos.y + velocity.y * 20.0f),
                   255, 0, 0, 255);
    }

    void drawAcceleration() const {
        aalineRGBA(ren, (short)pos.x, (short)pos.y,
                   (short)(pos.x + acceleration.x * 300.0f),(short)(pos.y + acceleration.y * 300.0f),
                   0, 255, 0, 255);
    }

    void printPos() const {
        std::cout << "(" << pos.x << "," << pos.y << ")";
    }

    void move() {
        velocity.x += acceleration.x;
        velocity.y += acceleration.y;
        acceleration = Vector2{0, 0};
        pos.x += velocity.x;
        pos.y += velocity.y;
    }
    
    Object() = default;
    
    Object(float px, float py, float _radius, float _mass, float vx, float vy) {
        pos         = Point{px, py};
        velocity    = Point{vx, vy};
        radius      = _radius;
        mass        = _mass;
    }

    Object(float px, float py, float _radius, float _mass, float vx, float vy,
           uchar _r, uchar _g, uchar _b, uchar _a) {
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
    Point v1{}, v2{}, v3{}; // vertices
    Point pos{};
    Vector2 velocity{};
    SDL_Color color{};

    void draw() const {
        filledTrigonRGBA(ren,
                         (short)v1.x, (short)v1.y,
                         (short)v2.x, (short)v2.y,
                         (short)v3.x, (short)v3.y,
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