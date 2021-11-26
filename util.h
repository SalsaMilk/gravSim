//
// Created by mitch on 11/9/2021.
//

#ifndef GRAVSIM_UTIL_H
#define GRAVSIM_UTIL_H

typedef struct {
    float x, y;
} Vector2, Point;

float distance(Point p1, Point p2) {
    auto retVal = (float)sqrt((p2.x - p1.x)*(p2.x - p1.x) + (p2.y - p1.y)*(p2.y - p1.y));
    return retVal;
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
    return sqrt(vec.x*vec.x + vec.y*vec.y);
}

Vector2 vec_normalize(Vector2 vec) {
    float  len = vec_length(vec);
    return Vector2 {vec.x/len, vec.y/len};
}

SDL_Point getLocalMousePos() {
    SDL_Point tempPoint;
    GetCursorPos(reinterpret_cast<LPPOINT>(&tempPoint));
    return tempPoint-windowPos;
}

inline int SDL_SetRenderDrawColor(SDL_Renderer *renderer, SDL_Color color) {
    return SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}


// thanks to https://stackoverflow.com/a/48291620/14160311
void DrawFilledCircle(SDL_Renderer *renderer, int x, int y, int radius)
{
    for (int w = 0; w < radius * 2; w++)
    {
        for (int h = 0; h < radius * 2; h++)
        {
            int dx = radius - w; // horizontal offset
            int dy = radius - h; // vertical offset
            if ((dx*dx + dy*dy) <= (radius * radius))
                SDL_RenderDrawPoint(renderer, x + dx, y + dy);
        }
    }
}

// https://stackoverflow.com/a/48291620/14160311
void DrawCircle(SDL_Renderer *renderer, int32_t cx, int32_t cy, int32_t radius)
{
    const int32_t diameter = (radius * 2);

    int32_t x = (radius - 1);
    int32_t y = 0;
    int32_t tx = 1;
    int32_t ty = 1;
    int32_t error = (tx - diameter);

    while (x >= y)
    {
        //  Each of the following renders an octant of the circle
        SDL_RenderDrawPoint(renderer, cx + x, cy - y);
        SDL_RenderDrawPoint(renderer, cx + x, cy + y);
        SDL_RenderDrawPoint(renderer, cx - x, cy - y);
        SDL_RenderDrawPoint(renderer, cx - x, cy + y);
        SDL_RenderDrawPoint(renderer, cx + y, cy - x);
        SDL_RenderDrawPoint(renderer, cx + y, cy + x);
        SDL_RenderDrawPoint(renderer, cx - y, cy - x);
        SDL_RenderDrawPoint(renderer, cx - y, cy + x);

        if (error <= 0)
        {
            ++y;
            error += ty;
            ty += 2;
        }

        if (error > 0)
        {
            --x;
            tx += 2;
            error += (tx - diameter);
        }
    }
}

#endif //GRAVSIM_UTIL_H
