// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Author: Martin Montas, martinmontas1@gmail.com

#ifndef SRC_SDL_UTILS_HPP_
#define SRC_SDL_UTILS_HPP_

#include <SDL2/SDL.h>

#define WIDTH  160
#define HEIGHT 144

#define WHITE      0xFFFFFFFF
#define LIGHT_GRAY 0xFFAAAAAA
#define DARK_GRAY  0xFF555555
#define BLACK      0xFF000000
#define SCALE      4

class SDL {
  private:
    SDL_Renderer* renderer;
    SDL_Surface*  screen;
    SDL_Texture*  texture;
    SDL_Window*   window;
    bool          sdl_running;

  public:
    void     init();
    uint32_t frame_buff[WIDTH * HEIGHT];
    void     update_frame_buff(size_t y_pos, size_t x_pos, uint32_t hex);
    void     frame_step();
    ~SDL();
};

#endif // SRC_SDL_UTILS_HPP_
