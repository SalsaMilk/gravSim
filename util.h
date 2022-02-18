//
// Created by mitch on 11/9/2021.
//

#ifndef GRAVSIM_UTIL_H
#define GRAVSIM_UTIL_H

#define PI 3.14159265f
#define SQRT3 1.73205f

#define ZEROTOHALF (((float) rand() / (RAND_MAX))/2)

typedef struct {
    float x, y;
} Vector2, Point;

float distance(Point p1, Point p2) {
    return sqrtf((p2.x - p1.x)*(p2.x - p1.x) + (p2.y - p1.y)*(p2.y - p1.y));
}

Point operator+ (const Point &p1, const Point &p2) {
    return Point {p1.x + p2.x, p1.y + p2.y};
}

Point operator- (const Point &p1, const Point &p2) {
    return Point {p1.x - p2.x, p1.y - p2.y};
}
SDL_Point operator- (const SDL_Point &p1, const SDL_Point &p2) {
    return SDL_Point {p1.x - p2.x, p1.y - p2.y};
}

Point operator* (const Point &p1, const Point &p2) {
    return Point {p1.x * p2.x, p1.y * p2.y};
}

Point operator* (const Point &p1, const float &i) {
    return Point {p1.x * i, p1.y * i};
}

Point operator/ (const Point &p1, const Point &p2) {
    return Point {p1.x / p2.x, p1.y / p2.y};
}

Point operator/ (const Point &p1, const float &i) {
    return Point {p1.x / i, p1.y / i};
}

float vec_length(Vector2 vec) {
    return sqrtf(vec.x*vec.x + vec.y*vec.y);
}

Vector2 vec_normalize(Vector2 vec) {
    float  len = vec_length(vec);
    return Vector2 {vec.x/len, vec.y/len};
}

SDL_Point getLocalCursorPos() {
    POINT cursorPos;
    GetCursorPos(&cursorPos);

    SDL_Point windowPos;
    SDL_GetWindowPosition(win, &windowPos.x, &windowPos.y);

    return SDL_Point{cursorPos.x-windowPos.x, cursorPos.y-windowPos.y};
}

Point rotate_point(float cx, float cy, float angle, Point p) {
    float s = sin(angle);
    float c = cos(angle);

    // translate point back to origin:
    p.x -= cx;
    p.y -= cy;

    // rotate point
    float xnew = p.x * c - p.y * s;
    float ynew = p.x * s + p.y * c;

    // translate point back:
    p.x = xnew + cx;
    p.y = ynew + cy;
    return p;
}

inline int SDL_SetRenderDrawColor(SDL_Renderer *renderer, SDL_Color color) {
    return SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

void aaFilledCircle(SDL_Renderer* renderer, short x, short y, short rad,
                    BYTE r, BYTE g, BYTE b, BYTE a) {
    filledCircleRGBA(renderer, x, y, rad, r, g, b, a);
    aaellipseRGBA(renderer, x, y, (short)(rad+1), rad, r, g, b, a);
    aacircleRGBA(renderer, x, y, rad, r, g, b, a);
}

#endif //GRAVSIM_UTIL_H
