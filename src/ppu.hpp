#include <SDL2/SDL.h>
#include <cstdio>

class PPU {

private:
  uint32_t frambuffer[160 * 144];

public:
  void updatebuffer();
};
