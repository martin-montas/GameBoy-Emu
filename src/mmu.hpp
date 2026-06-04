// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Programmer: Martin Montas, martinmontas1@gmail.com
//
#ifndef SRC_MMU_HPP_
#define SRC_MMU_HPP_

// #include "IO.hpp"
#include "MBC.hpp"
#include "system-bus.hpp"
#include "serial.hpp"
#include "timer.hpp"

#include <memory>
#include <stdint.h>
#include <string>
#include <vector>

#define HRAM_SIZE 0x7F
#define IRAM_SIZE 8192
#define OAM_SIZE  0xA0
#define VRAM_SIZE 0x2000
#define WRAM_SIZE 0x2000
#define IO_SIZE   0x80

using namespace std;

class MMU : public SystemBus {
  public:
    std::vector<uint8_t> romData;
    MMU(std::string filename, Timer* timer, Serial* serial);

    bool ram_enabled = false;
    /*
     * @brief: Reads based the 16 bit memory value
     * from ROM/RAM.
     *
     */
    uint8_t  read8(uint16_t addr) override;
    uint16_t read16(uint16_t addr) override;
    void     write8(uint16_t addr, uint8_t value) override;
    void     write16(uint16_t addr, uint16_t value) override;

    /*
     * @brief: Loads the rom file into the
     * romData vector.
     */
    void load_rom(const std::string& filename);

    uint8_t getIE() override {
        return 0;
    }
    uint8_t getIF() override {
        return 0;
    }

    void setIF(uint8_t value) override {
        return;
    }

  private:
    std::unique_ptr<MBC> mbc;
    Timer*               timer;
    Serial*              serial;

    /*
     * @brief: Based on the 0x147 byte of the rom
     * file the gameboy goes in different  different
     * mode where diffent type of RAM memory gets
     * allocated and more.
     */
    void    check_rom_type();
    uint8_t HRAM[HRAM_SIZE]       = {};
    uint8_t IRAM[IRAM_SIZE]       = {};
    uint8_t VRAM[VRAM_SIZE]       = {};
    uint8_t WRAM[WRAM_SIZE]       = {};
    uint8_t OAM[OAM_SIZE]         = {};
    uint8_t IO_REGISTERS[IO_SIZE] = {};
    uint8_t EXTERNAL_RAM[8192]    = {};
    uint8_t INTERRUPT[1]          = {};
    uint8_t rom_bank              = 1;

    // auto InterruptEnabled;
};

#endif // SRC_MMU_HPP_
