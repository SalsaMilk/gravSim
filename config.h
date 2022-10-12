#ifndef GRAVSIM_CONFIG_H
#define GRAVSIM_CONFIG_H

// everything that should be configurable in the menu
namespace cfg {
    SDL_Color bg = {0, 0, 0, 255};      // Background color
    float framerate = 60.0f;       // Framerate limit
    bool show_grid = true;
    float grid_scale = 20.0f;
}

#endif //GRAVSIM_CONFIG_H
