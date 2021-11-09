//
// Created by Salsa on 11/8/2021.
//

#ifndef GRAVSIM_OBJECT_H
#define GRAVSIM_OBJECT_H

typedef struct {
    int x, y;
} Vector2, Point;

class object {
private:

public:
    SDL_Point pos = {0, 0};

};

#endif //GRAVSIM_OBJECT_H
