#ifndef CHIP_8_HPP
#define CHIP_8_HPP
// 3rd Party Libraries

// Platform Libraries

// Project Libraries
#include "logger.hpp"
#include "rom.hpp"

// Standard Libraries

constexpr static uint8_t  MAX_KEYBOARD_SIZE = 16;   // Byte units
constexpr static uint16_t MAX_RAM_SIZE      = 4096; // Byte units
constexpr static uint8_t  MAX_STACK_SIZE    = 16;   // Byte units
constexpr static uint8_t  MAX_FONTSET_SIZE  = 80;   // Byte units
constexpr static uint8_t  MAX_TIMER_VALUE   = 255;  // Byte units

constexpr static uint16_t CHIP8_RAM_START_ADDR = 0x000;
constexpr static uint16_t CHIP8_RAM_END_ADDR   = 0x1FF;
constexpr static uint16_t PROGRAM_START_ADDR   = 0x200;
constexpr static uint16_t PROGRAM_END_ADDR     = 0xFFF;

constexpr static uint8_t SCREEN_WIDTH  = 64;
constexpr static uint8_t SCREEN_HEIGHT = 32;

constexpr static uint8_t OP_BYTE_SIZE = 4;  // Bit units
constexpr static uint8_t ADDRESS_SIZE = 12; // Bit units

constexpr std::array<uint8_t, MAX_FONTSET_SIZE> FONTSET = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

class CHIP_8 {
public:
    static CHIP_8 & instance() {
        static CHIP_8 instance;
        return instance;
    }

    // Memory
    std::array<uint8_t, MAX_RAM_SIZE> ram{};     // fixed 4K memory
    std::array<uint8_t, MAX_STACK_SIZE> stack{};
    // Keyboard
    std::array<uint8_t, MAX_KEYBOARD_SIZE> keyboard{};
    bool key_pressed = false;
    std::array<uint8_t, MAX_FONTSET_SIZE> fontset{};   // built-in font data
    // Screen
    std::array<std::array<uint8_t, SCREEN_WIDTH>, SCREEN_HEIGHT> screen{};
    // Timers
    uint16_t delay_timer{};
    uint16_t sound_timer{};
    // Current state of the CHIP-8 emulated machine
    bool running = false;
    // Contents of the CHIP-8 ROM being emulated
    std::unique_ptr<std::vector<uint8_t>> rom{};

    void reset();
    void load_rom(std::unique_ptr<std::vector<uint8_t>> & _rom);
    bool run();

private:
    // --- Private Constructor / Destructor ---
    CHIP_8() = default;
    ~CHIP_8() = default;

    // --- Disable copy and move ---
    CHIP_8(const CHIP_8 &) = delete;
    CHIP_8 & operator=(const CHIP_8 &) = delete;
    CHIP_8(CHIP_8 &&) = delete;
    CHIP_8 & operator=(CHIP_8 &&) = delete;
};
#endif // CHIP_8_HPP
