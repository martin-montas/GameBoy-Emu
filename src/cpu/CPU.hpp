#ifndef CPU_HPP
#define CPU_HPP

#include <cstdint>
#include <vector>
#include <functional>
#include <array>
#include <string>

//#include "registers.hpp" 

class CPU {

    public:
     CPU();
     ~CPU();

     void step();
     void read_rom();
     std::vector<uint8_t> load_rom(const std::string &filename);
     void read_rom(const std::vector<uint8_t> &romData);

     // opcodes
     void init_opcode_table();

     void load_bc_nn();
     void load_b_n();
     void nop();
     void nn();
     void load_bc_a();
     void load_a_bc();
     void load_c_n();
     void stop();
     void load_de_nn();

     void inc_bc();
     void inc_b(); 
     void dec_b(); 

     void rlca(); 
     void load_nn_sp();
     void add_hl_bc(); 

     void dec_bc();
     void inc_c(); 
     void dec_c();      
     void rrca();

     void load_de_a();
     void inc_de();        
     void inc_d();         
     void dec_d();         
     void load_d_n();       
     void rla();           
     void jr_n();          
     void add_hl_de();     
     void load_a_de();     
     void dec_de();     
     void inc_e();         
     void dec_e();         
     void load_e_n();
     void rra();            
     void jr_nz_n();        
     void load_hl_nn();     
     void load_hl_plus_a(); 
     void inc_hl();        
     void inc_h();         
     void dec_h();         
     void load_h_n(); 
     void daa(); 
     void jr_z_n(); 
     void add_hl_hl(); 
     void load_a_hl_plus(); 
     void dec_hl(); 
     void inc_l(); 
     void dec_l(); 
     void load_l_n(); 
     void cpl(); 

     const uint8_t opcode_cycle[256] = {
         4, 12, 8, 8, 4, 4, 8, 4, 20, 8, 8, 8, 4, 4, 8, 4,       // 0x0_
         4, 12, 8, 8, 4, 4, 8, 4,  12, 8, 8, 8, 4, 4, 8, 4,      // 0x1_
         0, 12, 8, 8, 4, 4, 8, 4,  0, 8, 8, 8, 4, 4, 8, 4,       // 0x2_
         0, 12, 8, 8, 12, 12, 12, 4,  0, 8, 8, 8, 4, 4, 8, 4,    // 0x3_
         4, 4, 4, 4, 4, 4, 8, 4,  4, 4, 4, 4, 4, 4, 8, 4,        // 0x4_
         4, 4, 4, 4, 4, 4, 8, 4,  4, 4, 4, 4, 4, 4, 8, 4,        // 0x5_
         4, 4, 4, 4, 4, 4, 8, 4,  4, 4, 4, 4, 4, 4, 8, 4,        // 0x6_
         8, 8, 8, 8, 8, 8, 4, 8,  4, 4, 4, 4, 4, 4, 8, 4,        // 0x7_
         4, 4, 4, 4, 4, 4, 8, 4,  4, 4, 4, 4, 4, 4, 8, 4,        // 0x8_
         4, 4, 4, 4, 4, 4, 8, 4,  4, 4, 4, 4, 4, 4, 8, 4,        // 0x9_
         4, 4, 4, 4, 4, 4, 8, 4,  4, 4, 4, 4, 4, 4, 8, 4,        // 0xa_
         4, 4, 4, 4, 4, 4, 8, 4,  4, 4, 4, 4, 4, 4, 8, 4,        // 0xb_
         0, 12, 0, 16, 0, 16, 8, 16,  0, 16, 0, 0, 0, 24, 8, 16, // 0xc_
         0, 12, 0, 0, 0, 16, 8, 16,  0, 16, 0, 0, 0, 0, 8, 16,   // 0xd_
         12, 12, 8, 0, 0, 16, 8, 16,  16, 4, 16, 0, 0, 0, 8, 16, // 0xe_
         12, 12, 8, 4, 0, 16, 8, 16,  12, 8, 16, 4, 0, 0, 8, 16  // 0xf_
        };
    private:
      // Registers *register;
      std::array<std::function<void()>, 256> opcode_table; 
      uint16_t pc;
      uint16_t sp;
      uint32_t cycle;
      uint32_t cycle_count;
};

#endif // CPU_HPP
