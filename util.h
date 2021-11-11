//
// Created by mitch on 11/9/2021.
//

#ifndef GRAVSIM_UTIL_H
#define GRAVSIM_UTIL_H

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
            {
                SDL_RenderDrawPoint(renderer, x + dx, y + dy);
            }
        }
    }
}


#endif //GRAVSIM_UTIL_H