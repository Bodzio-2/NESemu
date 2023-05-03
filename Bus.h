#pragma once
#include <cstdint>
#include "olc6502.h"
#include "olc2C02.h"
#include "Cartridge.h"
#include<array>
#include <memory>

class Bus{
public:
    Bus();
    ~Bus();

public: //Devices attached to bus
    olc6502 cpu;

    // Picture Processing Unity
    olc2C02 ppu;

    // RAM array (for now)
    std::array<uint8_t, 2048> cpuRam;

    // The Cartridge
    std::shared_ptr<Cartridge> cart;

public: // Read/Write of Bus
    void cpuWrite(uint16_t addr, uint8_t data);
    uint8_t cpuRead(uint16_t addr, bool bReadOnly = false);

public: // System interface
    void insertCartridge(const std::shared_ptr<Cartridge>& cartridge);
    void reset();
    void clock();

private:
    // Clock count
    uint32_t nSystemClockCounter = 0;
};