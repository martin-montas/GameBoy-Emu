#ifndef CPU_HPP
#define CPU_HPP

#include <cstdint>
#include <vector>
#include <functional>
#include <array>
#include <string>

// #include "../registers.hpp" 

class CPU {

    public:
        CPU();
        ~CPU();

        void step();
        void read_rom();
        std::vector<uint8_t> load_rom(const std::string &filename);
        uint32_t execute_opcode(uint8_t opcode);
        void emulate_cycles(uint32_t cyclesToRun);
        void init_opcode_table();
        std::vector<uint8_t> romData;

        // opcodes
        void load_sp_nn();
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
        void hl_mem();
        void load_a_hl_minus();
        void dec_bc();
        void inc_c(); 
        void dec_c();      
        void rrca();
        void ret_c();
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
        void jr_c_n();

        void load_b_b();
        void load_b_c();
        void load_b_d();
        void load_b_e();
        void load_b_h();

        void load_b_l(); 
        void load_b_hl_mem();
        void load_b_a();
        void load_c_b();
        void load_c_c();

        void load_c_d();
        void load_c_e();
        void load_c_h();
        void load_c_l();
        void load_c_hl_mem();
        void load_c_a(); 

        void load_d_b();
        void load_d_c();
        void load_d_d();
        void load_d_e();
        void load_d_h();

        void load_d_l();
        void load_d_hl_mem();
        void load_d_a(); 
        void load_e_b(); 
        void load_e_c(); 

        void load_e_d();
        void load_e_e();
        void load_e_h();
        void load_e_l();
        void load_e_hl_mem();
        void load_e_a(); 
        void load_h_b();
        void load_h_c();
        void load_h_d();
        void load_h_l();
        void load_h_e();
        void load_h_h();

        void load_h_hl_mem(); 
        void load_h_a(); 
        void load_l_b(); 
        void load_l_c(); 
        void load_l_d(); 
        void load_l_e();
        void load_l_h();
        void load_l_l();
        void load_l_hl_mem();
        void load_l_a(); 

        void  load_hl_mem_b();
        void  load_hl_mem_c();
        void  load_hl_mem_d();
        void  load_hl_mem_e();
        void  load_hl_mem_h();
        void  load_hl_mem_l();

        void halt();
        void load_hl_mem_a(); 
        void load_a_b(); 
        void load_a_c(); 
        void load_a_d(); 

        void jr_nc_n();
        void load_hl_minus_a();

        void inc_sp();
        void inc_hl_mem();
        void dec_hl_mem();
        void load_hl_n();
        void scf();

        void add_hl_sp();
        void dec_sp();
        void inc_a();           
        void dec_a();                
        void load_a_n();
        void ccf(); 

        void load_a_e();
        void load_a_h();
        void load_a_l();
        void load_a_hl_mem();
        void load_a_a();

        void add_a_b();
        void add_a_c();
        void add_a_d();
        void add_a_e();
        void add_a_h();
        void add_a_l();
        void add_a_hl_mem();
        void add_a_a();
        void adc_a_b();
        void adc_a_c();
        void adc_a_d();
        void adc_a_e();
        void adc_a_h();
        void adc_a_l();


        void sub_a_b(); 
        void sub_a_c(); 
        void sub_a_d(); 
        void sub_a_e(); 
        void sub_a_h(); 
        void sub_a_l(); 
        void sub_a_hl_mem();
        void sub_a_a(); 
        void sbc_a_b(); 
        void sbc_a_c(); 
        void sbc_a_d(); 
        void sbc_a_e(); 
        void sbc_a_h(); 
        void sbc_a_l(); 
        void sbc_a_hl_mem();
        void sbc_a_a(); 
        void adc_a_hl_mem();

        void and_a_b();
        void and_a_c();
        void and_a_d();
        void and_a_e();
        void and_a_h();
        void and_a_l();
        void and_a_hl_mem();
        void and_a_a();
        void xor_a_b();
        void xor_a_c();
        void xor_a_d();
        void xor_a_e();
        void xor_a_h();
        void xor_a_l();
        void xor_a_hl_mem();
        void xor_a_a();
        void or_a_b();

        void or_a_c();
        void or_a_d();
        void or_a_e();
        void or_a_h();
        void or_a_l();
        void or_a_hl_mem();
        void or_a_a();
        void cp_a_b();
        void cp_a_c();
        void cp_a_d();
        void cp_a_e();
        void cp_a_h();
        void cp_a_l();
        void cp_a_hl_mem();
        void cp_a_a();
        void adc_a_a();
        void ret_nz();
        void pop_bc();
        void jp_nz_nn(); 
        void jp_nn(); 

        void call_nz_nn();
        void push_bc();
        void add_a_n();
        void rst_00h();
        void ret_z();
        void ret();
        void jp_z_nn();
        void cb_prefix();
        void call_z_nn();
        void call_nn();
        void adc_a_n();
        void rst_08h();

        void ret_nc();
        void pop_de();
        void jp_nc_nn();
        void call_nc_nn();
        void push_de();
        void sub_a_n();
        void rst_10h();

        void reti();
        void jp_c_nn();
        void in_a_n();
        void call_c_nn();
        void sbc_a_n();
        void rst_18h();

        void ld_ff_a();
        void pop_hl();
        void ld_ff_c();
        void push_hl();
        void and_n();
        void rst_20h();
        void add_sp_n();
        void jp_hl();
        void ld_nn_a();
        void prefix_cb();
        void no_operation();

        void push_af();
        void or_n();
        void rst_30h();
        void ld_hl_sp_n();
        void ld_sp_hl();
        void ld_a_nn();
        void ei();

        void ed_prefix(); 
        void xor_n(); 
        void rst_28h(); 
        void ld_a_ff00_n(); 
        void pop_af(); 
        void ld_a_ff00_c();
        void di();

        void fd_prefix(); 
        void cp_n(); 
        void rst_38h(); 

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

        const int opcode_cycles[256] = {
            /* 0x00 - 0x0F */ 4, 12, 8, 8, 4, 4, 8, 4, 20, 8, 8, 8, 4, 4, 8, 4,
            /* 0x10 - 0x1F */ 8, 12, 8, 8, 4, 4, 8, 4, 12, 8, 8, 8, 4, 4, 8, 4,
            /* 0x20 - 0x2F */ 12, 12, 8, 8, 4, 4, 8, 4, 12, 8, 8, 8, 4, 4, 8, 4,
            /* 0x30 - 0x3F */ 12, 12, 8, 8, 4, 4, 8, 4, 12, 8, 8, 8, 4, 4, 8, 4,
            /* 0x40 - 0x4F */ 4, 4, 4, 4, 4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 8, 4,
            /* 0x50 - 0x5F */ 4, 4, 4, 4, 4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 8, 4,
            /* 0x60 - 0x6F */ 4, 4, 4, 4, 4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 8, 4,
            /* 0x70 - 0x7F */ 4, 4, 4, 4, 4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 8, 4,
            /* 0x80 - 0x8F */ 4, 4, 4, 4, 4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 8, 4,
            /* 0x90 - 0x9F */ 4, 4, 4, 4, 4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 8, 4,
            /* 0xA0 - 0xAF */ 4, 4, 4, 4, 4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 8, 4,
            /* 0xB0 - 0xBF */ 4, 4, 4, 4, 4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 8, 4,
            /* 0xC0 - 0xCF */ 20, 12, 16, 12, 12, 16, 8, 16, 20, 16, 16, 12, 12, 24, 8, 16,
            /* 0xD0 - 0xDF */ 20, 12, 16, 0, 12, 16, 8, 16, 20, 16, 16, 0, 12, 0, 8, 16,
            /* 0xE0 - 0xEF */ 12, 12, 8, 0, 12, 16, 8, 16, 16, 4, 16, 0, 12, 0, 8, 16,
            /* 0xF0 - 0xFF */ 12, 12, 8, 4, 12, 16, 8, 16, 12, 8, 16, 4, 12, 0, 8, 16
        };

        std::array<std::function<void()>, 256> opcode_table; 
        uint16_t pc;
        uint16_t sp;
        uint32_t cycle;
        uint32_t cycle_count;
        uint32_t globalCycles;
};

#endif // CPU_HPP
