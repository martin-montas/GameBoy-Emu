// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Author: Martin Montas, martinmontas1@gmail.com
//
#ifndef SRC_MMU_HPP_
#define SRC_MMU_HPP_

// #include "IO.hpp"
#include "MBC.hpp"
#include "system-bus.hpp"
#include "serial.hpp"
#include "timer.hpp"
#include "ppu.hpp"
#include "./interface-interrupt.hpp"

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

class Ppu;

class MMU : public SystemBus {
  public:
    MMU(const std::string file, Timer* timer, IInterrupt* interrupt)
        : _timer(timer), _interrupt(interrupt) {
        load_rom(file);
        // check_rom_type();
    }
    /* @brief: holds rom data. the ROM Can have extra memory
     * given by its type which can be MBC0, MBC1 etc.
     */
    std::vector<uint8_t> romData;

    bool ram_enabled = false;
    /*
     * @brief: Reads based the 16 bit memory value
     * from ROM/RAM.
     *
     */

    void attach(Ppu* ppu) {
        _ppu = ppu;
    }

    /* @brief: methods made for memory operations:
     * read8, write8, read16, and write16.
     * @param[in]: address to be written/read/
     * @param[in]: value value to be written at address
     * specified.
     */
    uint8_t  read8(uint16_t addr) override;
    uint16_t read16(uint16_t addr) override;
    void     write8(uint16_t addr, uint8_t value) override;
    void     write16(uint16_t addr, uint16_t value) override;

    /*
     * @brief: The follow methods are  setters and getters
     * for the interrupt registers: currently not implemented.
     */
    uint8_t getIE() override {
        return 0;
    }
    uint8_t getIF() override {
        return 0;
    }
    void setIF(uint8_t value) override {
        return;
    }

    /*
     * @brief: Loads the rom file into the
     * romData vector
     * @param[in]: File name that will be used as rom.
     */
    void load_rom(const std::string& filename);

  private:
    std::unique_ptr<MBC> mbc;

    /* @brief: These 2 objects are io registers that either synchronize
     * the timing of each game or prints the  serial data.
     */
    Serial      serial;
    Ppu*        _ppu       = nullptr;
    Timer*      _timer     = nullptr;
    IInterrupt* _interrupt = nullptr;

    uint8_t rom_bank = 1;

    /*
     * @brief: Based on the 0x147 byte of the rom
     * file the gameboy goes in different  different
     * mode where diffent type of RAM memory gets
     * allocated and more.
     */
    void check_rom_type();

    /* @brief: memory arrays initialize with zeroes
     * each of them hold the amount of memory specified
     * in the official pandocs website.
     */
    uint8_t HRAM[HRAM_SIZE]       = {};
    uint8_t IRAM[IRAM_SIZE]       = {};
    uint8_t VRAM[VRAM_SIZE]       = {};
    uint8_t WRAM[WRAM_SIZE]       = {};
    uint8_t OAM[OAM_SIZE]         = {};
    uint8_t IO_REGISTERS[IO_SIZE] = {};
    uint8_t EXTERNAL_RAM[8192]    = {};
    uint8_t INTERRUPT[1]          = {};
};

#endif // SRC_MMU_HPP_
