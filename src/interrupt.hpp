// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Author: Martin Montas, martinmontas1@gmail.com
//
#ifndef SRC_INTERRUPT_HPP_
#define SRC_INTERRUPT_HPP_

#include <stdint.h>
#include <stdio.h>
#include "interface-interrupt.hpp"
#include "cpu.hpp"

/* interrupt addresses */
#define IE 0xFFFF
#define IF 0xFF0F

#define VECTOR_VBLANK   0x0040
#define VECTOR_LDC_STAT 0x0048
#define VECTOR_TIMER    0x0050
#define VECTOR_SERIAL   0x0058
#define VECTOR_JOYPAD   0x0050

/*
 * @brief: implementation of the IInterrupt class.
 * deals with handling of states and execution of
 * the interrupt subsystem of the GameBoy.
 *
 */
class Cpu;
class Interrupt : public IInterrupt {
  public:
    uint8_t         _IF;            /* pointer to interrupt flag reg */
    uint8_t         _IE;            /* pointer to interrupt enable reg */
    Cpu*            _cpu;           /* pointer to cpu object */
    Interrupt_Flags _flags;         /* interrupt with higher priority */
    Interrupt() : _IF(0), _IE(0) {} /* interrupt contructor */

    /*
     * @brief: override of the interrupt interface.
     * sets the interrupt flag registers bits on.
     * @param[in]: bits to be inserted in the interrupt
     * flag.
     */
    inline void request_exec_interrupt(Interrupt_Flags flag) override {
        _IF |= flag;
        if (!(pending_interrupt())) {
            return;
        }
        // TODO: interrupt should happen here instantly
        if ((_IF & INTERRUPT_VBLANK) && (_IE & INTERRUPT_VBLANK)) {
            _cpu->_instruction->interrupt_handler(VECTOR_VBLANK);
        } else if ((_IF & INTERRUPT_LCD) && (_IE & INTERRUPT_LCD)) {
            _cpu->_instruction->interrupt_handler(VECTOR_LDC_STAT);

        } else if ((_IF & INTERRUPT_TIMER) && (_IE & INTERRUPT_TIMER)) {

            _cpu->_instruction->interrupt_handler(VECTOR_TIMER);
        } else if ((_IF & INTERRUPT_SERIAL) && (_IE & INTERRUPT_SERIAL)) {

            _cpu->_instruction->interrupt_handler(VECTOR_SERIAL);
        } else if ((_IF & INTERRUPT_SERIAL) && (_IE & INTERRUPT_JOYPAD)) {
            _cpu->_instruction->interrupt_handler(VECTOR_JOYPAD);
        } else {
            return;
        }
    }

    /*
     * @brief: reads either the IF and the Ie
     * registers. Its an override of the
     * interrupt interface.
     * @param[in]: address to read.
     */
    inline uint8_t read(uint16_t addr) override {
        if (addr == 0xFFFF) {
            return _IE;
        }
        return _IF;
    }
    /*
     * @brief: writes to either the IF and the Ie
     * registers. Its an override of the
     * interrupt interface.
     * @param[in]: address to write.
     * @param[in]: value being written.
     */
    inline void write(uint16_t addr, uint8_t value) override {
        if (addr == 0xFFFF)
            _IE = value & 0x1F;

        if (addr == 0xFF0F)
            _IF = value & 0x1F;
    }

    inline void attach(Cpu* cpu) {
        _cpu = cpu;
    }

    /*
     * @brief: tells its its handler if there is an interrupt
     * at the current call time.
     * @return: boolean, pending, true, not pending, false.
     */
    inline bool pending_interrupt() override {
        if (((_IF & _IE) & 0x1F) != 0) {
            return true;
        }
        return false;
    }

    /*
     * @brief gets the vector for the type of interrupt
     * needed.
     * @return: unsigned 16 bit value for the vector.
     */
    inline uint16_t get_interrupt_vector() override {
        if (_flags == INTERRUPT_VBLANK) {
            _IF &= ~INTERRUPT_VBLANK;
            printf("vlblank fired\n");
            return 0x0040;
        } else if (_flags == INTERRUPT_LCD) {
            _IF &= ~INTERRUPT_LCD;
            printf("lcd fired\n");
            return 0x0048;
        } else if (_flags == INTERRUPT_TIMER) {
            _IF &= ~INTERRUPT_TIMER;
            printf("timer fired\n");
            return 0x0050;
        } else if (_flags == INTERRUPT_SERIAL) {
            _IF &= ~INTERRUPT_SERIAL;
            printf("serial fired\n");
            return 0x0058;
        } else {
            return 0x0060;
        }
    }
};

#endif // SRC_INTERRUPT_HPP_
