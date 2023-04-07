#pragma once
#include <cstdint>
#include "olc6502.h"
#include<array>

class Bus{
public:
    Bus();
    ~Bus();

public: //Devices attached to bus
    olc6502 cpu;

    // RAM array (for now)
    std::array<uint8_t, 64 * 1024> ram;

public: // Read/Write of Bus
    void write(uint16_t addr, uint8_t data);
    uint8_t read(uint16_t addr, bool bReadOnly = false);
};