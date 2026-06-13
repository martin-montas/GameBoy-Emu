// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Author: Martin Montas, martinmontas1@gmail.com
#ifndef SRC_DEVICE_HPP_
#define SRC_DEVICE_HPP_

#include <stdint.h>
#include <stddef.h>

/*
 * @brief: This class deals with all members that
 * read and write to registers in memory.
 */
class Device {

  public:
    /*
     * @brief: ticks for every cycle;
     * @param[in]: current cycle to update.
     */
    virtual void cycle(int cycle) {}

    /*
     * @brief: virtual method implemented by
     * by all the subsystems that deal with
     * read to memory.
     * @param[in]: address to read.
     */
    virtual uint8_t read(uint16_t addr) = 0;

    /*
     * @brief: virtual method implemented by
     * by all the subsystems that deal with
     * write to memory.
     * @param[in]: address to read.
     * @param[in]: value to write to memory.
     */
    virtual void write(uint16_t addr, uint8_t value) = 0;
};

#endif // SRC_DEVICE_HPP_
