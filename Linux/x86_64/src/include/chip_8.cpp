// 3rd Party Libraries

// Platform Libraries

// Project Libraries
#include "chip_8.hpp"
#include "logger.hpp"
#include "rom.hpp"

// Standard Libraries

bool CHIP_8::run() {
    bool ret = false;
    ret = true;
    return ret;
}

void CHIP_8::reset() {
    ram.fill(0);
    stack.fill(0);
    keyboard.fill(0);
    fontset.fill(0);
    for (std::array<uint8_t, SCREEN_WIDTH> & row : screen) row.fill(0);
    key_pressed = false;
    delay_timer = 0;
    sound_timer = 0;
    running = false;
}

void CHIP_8::load_rom(std::unique_ptr<std::vector<uint8_t>> & _rom) {
    rom = std::move(_rom);
}
