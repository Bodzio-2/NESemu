#include <cstdint>
#include <memory>
#include "Cartridge.h"
class olc2C02{
public:    
    olc2C02();
    ~olc2C02();

private:
    // VRAM
    uint8_t tblName[2][1024];
    // RAM - palette information
    uint8_t tblPalette[32];
    // Pattern memory (not used in this project, but might be used in the future)
    uint8_t tblPattern[2][4096];

public:
    // Comms with Main Bus
    uint8_t cpuRead(uint16_t addr, bool rdonly=false);
    void cpuWrite(uint16_t addr, uint8_t data);

    // Comms with PPU Bus
    uint8_t ppuRead(uint16_t addr, bool rdonly=false);
    void ppuWrite(uint16_t addr, uint8_t data);

private:
    // The Cartridge
    std::shared_ptr<Cartridge> cart;

public:
    // Interface
    void ConnectCartridge(const std::shared_ptr<Cartridge>& cartridge);
    void clock();
};