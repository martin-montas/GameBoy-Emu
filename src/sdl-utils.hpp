// Copyright 2022 Robot Locomotion Group @CSAIL.All rights reserved.
// All components of this software are licensed under the GNU License.
// Author: Martin Montas, martinmontas1@gmail.com

#ifndef SRC_SDL_UTILS_HPP_
#define SRC_SDL_UTILS_HPP_

#include <SDL2/SDL.h>

/* sdl window dimentions */
#define WIDTH  160
#define HEIGHT 144

/*
 * @brief: sdl2 helper class. does rendering and
 * event type management.
 */
class SDL {
    SDL_Renderer* renderer; /* render to the screen */
    SDL_Surface*  screen;   /* screen to write to */
    SDL_Texture*  texture;  /* deals with textures */
    SDL_Window*   window;   /* window to display */

  public:
    SDL_Event event;
    void      init();
    /*
     * @brief: Updates frames per vblank
     * @param[in]: pointer  to frame buffer
     */
    void frame_step(uint32_t* frame_buff);
    bool display_running;

    ~SDL();
};

#endif // SRC_SDL_UTILS_HPP_
