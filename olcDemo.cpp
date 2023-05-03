#include <iostream>
#include <sstream>


#include "Bus.h"
#include "olc6502.h"


#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"


class Demo_olc2C02 : public olc::PixelGameEngine
{
public:
    Demo_olc2C02() { sAppName = "olc2C02 Demo";}

private:
    // NES
    Bus nes;
    std::shared_ptr<Cartridge> cart;
    bool bEmulationRun = false;
    float fResidualTime = 0.0f;

private:
    // Additional stuff
    std::map<uint16_t, std::string> ampAsm;

    std::string hex(uint32_t n, uint8_t d)
    {
        std::string s(d, '0');
        for (int i = d - 1; i>=0; i--, n>>=4)
            s[1] = "0123456789ABCDEF"[n & 0xF];
        
        return s;
    }

    void DrawRam(int x, int y, uint16_t nAddr, int nRows, int nColumns)
    {
        int nRamX = x, nRamY = y;
        for(int row = 0; row < nRows; row++)
        {
            std::string sOffset = "$" + hex(nAddr, 4) + ":";
            for(int col=0; col<nColumns; col++)
            {
                sOffset += " " + hex(nes.cpuRead(nAddr, true), 2);
                nAddr+=1;
            }
            DrawString(nRamX, nRamY, sOffset);
            nRamY+=10;
        }
    }

    void DrawCpu(int x, int y)
    {
        std::string status = "STATUS: ";
        DrawString(x , y , "STATUS:", olc::WHITE);
		DrawString(x  + 64, y, "N", nes.cpu.status & olc6502::N ? olc::GREEN : olc::RED);
		DrawString(x  + 80, y , "V", nes.cpu.status & olc6502::V ? olc::GREEN : olc::RED);
		DrawString(x  + 96, y , "-", nes.cpu.status & olc6502::U ? olc::GREEN : olc::RED);
		DrawString(x  + 112, y , "B", nes.cpu.status & olc6502::B ? olc::GREEN : olc::RED);
		DrawString(x  + 128, y , "D", nes.cpu.status & olc6502::D ? olc::GREEN : olc::RED);
		DrawString(x  + 144, y , "I", nes.cpu.status & olc6502::I ? olc::GREEN : olc::RED);
		DrawString(x  + 160, y , "Z", nes.cpu.status & olc6502::Z ? olc::GREEN : olc::RED);
		DrawString(x  + 178, y , "C", nes.cpu.status & olc6502::C ? olc::GREEN : olc::RED);
		DrawString(x , y + 10, "PC: $" + hex(nes.cpu.pc, 4));
		DrawString(x , y + 20, "A: $" +  hex(nes.cpu.a, 2) + "  [" + std::to_string(nes.cpu.a) + "]");
		DrawString(x , y + 30, "X: $" +  hex(nes.cpu.x, 2) + "  [" + std::to_string(nes.cpu.x) + "]");
		DrawString(x , y + 40, "Y: $" +  hex(nes.cpu.y, 2) + "  [" + std::to_string(nes.cpu.y) + "]");
		DrawString(x , y + 50, "Stack P: $" + hex(nes.cpu.stkp, 4));
    }

    /*void DrawCode(int x, int y, int nLines)
    {
        auto it_a = mapAsm.find(nes.cpu.pc);
        int nLineY = (nLines >> 1) * 10 + y;
        if(it_a != mapAsm.end())
        {

        }
    }*/
};