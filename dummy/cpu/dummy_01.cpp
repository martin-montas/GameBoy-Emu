#include <cstdio>
#include <cstdint>

using namespace std;

int main() {

    uint16_t _whole = 0x2343;
    printf("whole %X\n", _whole);

    uint8_t _lw = (0xf >> 8 | (_whole & 0xff));
    printf("immediate %X\n", _lw);
}
