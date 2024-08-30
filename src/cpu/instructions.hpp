#ifndef OPCODE_HPP
#define OPCODE_HPP

class Opcode {
    public:
        Opcode();
        ~Opcode();
        void run();
    private:
        int8_t opcode;
}; 


#endif // OPCODE_HPP
