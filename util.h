//
// Created by mitch on 11/9/2021.
//

#ifndef GRAVSIM_UTIL_H
#define GRAVSIM_UTIL_H

typedef struct {
    double x, y;
} Vector2, Point;

double distance(Point p1, Point p2) {
    return sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));
}

Point operator+ (const Point &p1, const Point &p2) {
    return Point {p1.x + p2.x, p1.y + p2.y};
}

Point operator- (const Point &p1, const Point &p2) {
    return Point {p1.x - p2.x, p1.y - p2.y};
}

Point operator* (const Point &p1, const Point &p2) {
    return Point {p1.x * p2.x, p1.y * p2.y};
}

Point operator* (const Point &p1, const double &i) {
    return Point {p1.x * i, p1.y * i};
}

Point operator/ (const Point &p1, const Point &p2) {
    return Point {p1.x / p2.x, p1.y / p2.y};
}

Point operator/ (const Point &p1, const double &i) {
    return Point {p1.x / i, p1.y / i};
}

double vec_length(Vector2 vec) {
    return sqrt(vec.x*vec.x + vec.y*vec.y);
}

Vector2 vec_normalize(Vector2 vec) {
    double  len = vec_length(vec);
    return Vector2 {vec.x/len, vec.y/len};
}

// thanks to https://stackoverflow.com/a/48291620/14160311
void DrawCircle(SDL_Renderer *renderer, int x, int y, int radius)
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


#endif //GRAVSIM_UTIL_H
