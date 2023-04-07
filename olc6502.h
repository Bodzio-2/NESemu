#pragma once
#include <string>
#include <vector>

class Bus;

class olc6502{
public:
    olc6502();
    ~olc6502();

public:
    enum FLAGS6502{ //Flags in 6502 are set by setting a bit at a designated position (that's why we use the shift operation)
        C = (1 << 0), //Carry bit
        Z = (1 << 1), //Zero
        I = (1 << 2), //Disable interrupts
        D = (1 << 3), //Decimal Mode (unused)
        B = (1 << 4), //Break
        U = (1 << 5), //Unused
        V = (1 << 6), //Overflow
        N = (1 << 7), //Negative
    };

    //Various variables stored in the 6502 processor
    uint8_t a = 0x00; // Accumulator register
    uint8_t x = 0x00; // X Register
    uint8_t y = 0x00; // Y Register
    uint8_t stkp = 0x00; // Stack Pointer (pointer to a location on bus)
    uint16_t pc = 0x0000; // Program Counter
    uint8_t status = 0x00; // Status Register

    //Function used to connect the bus to the CPU
    void ConnectBus(Bus *n) { bus = n; }

    //Both addressing modes and opcodes return either 1 or 0 depending on if an additional cycle is required
    
    // Addressing Modes "where to look for data"
    uint8_t IMP();  uint8_t IMM();
    uint8_t ZP0();  uint8_t ZPX();
    uint8_t ZPY();  uint8_t REL();
    uint8_t ABS();  uint8_t ABX();
    uint8_t ABY();  uint8_t IND();
    uint8_t IZX();  uint8_t IZY();


    // Opcodes "What to do with the data"
    uint8_t ADC();	uint8_t AND();	uint8_t ASL();	uint8_t BCC();
	uint8_t BCS();	uint8_t BEQ();	uint8_t BIT();	uint8_t BMI();
	uint8_t BNE();	uint8_t BPL();	uint8_t BRK();	uint8_t BVC();
	uint8_t BVS();	uint8_t CLC();	uint8_t CLD();	uint8_t CLI();
	uint8_t CLV();	uint8_t CMP();	uint8_t CPX();	uint8_t CPY();
	uint8_t DEC();	uint8_t DEX();	uint8_t DEY();	uint8_t EOR();
	uint8_t INC();	uint8_t INX();	uint8_t INY();	uint8_t JMP();
	uint8_t JSR();	uint8_t LDA();	uint8_t LDX();	uint8_t LDY();
	uint8_t LSR();	uint8_t NOP();	uint8_t ORA();	uint8_t PHA();
	uint8_t PHP();	uint8_t PLA();	uint8_t PLP();	uint8_t ROL();
	uint8_t ROR();	uint8_t RTI();	uint8_t RTS();	uint8_t SBC();
	uint8_t SEC();	uint8_t SED();	uint8_t SEI();	uint8_t STA();
	uint8_t STX();	uint8_t STY();	uint8_t TAX();	uint8_t TAY();
	uint8_t TSX();	uint8_t TXA();	uint8_t TXS();	uint8_t TYA();

    //NOTE: when an illegal opcode is called, we'll use this function.
    //That differs from the way 6502 worked, but in this use case it doesn't matter
    uint8_t XXX();

    //A function called when we want "one clock cycle" to occur
    void clock();
    
    //All of these signals can occur at any time, that's why they need to be able to be called asynchronously from the clock
    //They will interrupt the processor, but the current instruction will be executed
    void reset();   //Reset signal
    void irq();     //Interrupt request signal - can be ignored if the "non interrupt" mask is enabled
    void nmi();     //Non-maskable interrupt request - ignores the above mask

    //Functions implemented to facilitate emulation
    uint8_t fetch(); //Function used to fetch data for an instruction
    uint8_t fetched = 0x00; //where the above is stored

    uint16_t addr_abs = 0x0000;  // if we need to fetch data from a location, it will be stored here
    uint16_t addr_rel = 0x00;   // relative address (used in branch instructions for jumping)
    uint8_t opcode = 0x00;  // opcode of the instruction
    uint8_t cycles = 0; // numbers of cycles left in the duration of that instruction (in 6502 instructions last very precise clock signal amounts, modern processors can do all calculations quicker, so we need to arbitrarily wait a number of cycles to match designed specifications)
private:
    //Pointer to the bus (so the CPU can access it)
    Bus *bus = nullptr;
    //Analogical functions for R/W but for the CPU, it just calls R/W functions of the assigned bus
    uint8_t read(uint16_t a);
    void write(uint16_t a, uint8_t d);

    //Functions for an easier way of accessing the status register
    uint8_t GetFlag(FLAGS6502 f);
    void SetFlag(FLAGS6502 f, bool v);

    struct INSTRUCTION{ //struct containing pointers to both opcode and address mode functions, as well as required amount of clock cycles 
        std::string name;
        uint8_t(olc6502::*operate)(void) = nullptr;
        uint8_t(olc6502::*addrmode)(void) = nullptr;
        uint8_t cycles = 0;
    };

    std::vector<INSTRUCTION> lookup; //a lookup table for all instructions
};