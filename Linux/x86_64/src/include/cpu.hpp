#ifndef CPU_HPP
#define CPU_HPP
// 3rd Party Libraries

// Platform Libraries

// Project Libraries
#include "chip_8.hpp"
#include "logger.hpp"

// Standard Libraries
#include <memory>

/**
 * @brief CHIP-8 Instruction Set: https://en.wikipedia.org/wiki/CHIP-8
 * NNN             : address
 * NN              : 8-bit constant
 * N               : 4-bit constant
 * X and Y         : 4-bit register identifier
 * program_counter : Program Counter
 * I               : 12-bit register (For memory address) (Similar to void pointer);
 * VN              : One of the 16 available variables. N may be 0 to F (hexadecimal);
 */
typedef enum OPCODE {
    _0NNN = 0, // 0NNN: Calls machine code routine at address NNN
    _00E0,     // 00E0: Clears the screen
    _00EE,     // 00EE: Return from subroutine
    _1NNN,     // 1NNN: Jump to address NNN
    _2NNN,     // 2NNN: Calls subroutine at NNN
    _3XNN,     // 3XNN: Cond    if (Vx == NN)    Skips the next instruction if VX equals NN
               // usually the next instruction is a jump to skip a code block).
    _4XNN,     // 4XNN: Cond    if (Vx != NN)    Skips the next instruction if VX does not equal
               // NN (usually the next instruction is a jump to skip a code block).
    _5XY0,     // 5XY0: Cond    if (Vx == Vy)    Skips the next instruction if VX equals VY
               // (usually the next instruction is a jump to skip a code block).
    _6XNN,     // 6XNN: Const   Vx = NN    Sets VX to NN.
    _7XNN,     // 7XNN: Const   Vx += NN    Adds NN to VX (carry flag is not changed).
    _8XY0,     // 8XY0: Assig   Vx = Vy    Sets VX to the value of VY.
    _8XY1,     // 8XY1: BitOp   Vx |= Vy    Sets VX to VX or VY. (bitwise OR operation).
    _8XY2,     // 8XY2: BitOp   Vx &= Vy    Sets VX to VX and VY. (bitwise AND operation).
    _8XY3,     // 8XY3: BitOp   Vx ^= Vy    Sets VX to VX xor VY.
    _8XY4,     // 8XY4: Math    Vx += Vy    Adds VY to VX. VF is set to 1 when there's an
               // overflow, and to 0 when there is not.
    _8XY5,     // 8XY5: Math    Vx -= Vy    VY is subtracted from VX. VF is set to 0 when
               // there's an underflow, and 1 when there is not. (i.e. VF set to 1 if VX >= VY
               // and 0 if not).
    _8XY6,     // 8XY6: BitOp   Vx >>= 1    Shifts VX to the right by 1, then stores the least
               // significant bit of VX prior to the shift into VF.[b]
    _8XY7,     // 8XY7: Math    Vx = Vy - Vx    Sets VX to VY minus VX. VF is set to 0 when
               // there's an underflow, and 1 when there is not. (i.e. VF set to 1 if VY >= VX).
    _8XYE,     // 8XYE: BitOp   Vx <<= 1    Shifts VX to the left by 1, then sets VF to 1 if the
               // most significant bit of VX prior to that shift was set, or to 0 if it was
               // unset.[b]
    _9XY0,     // 9XY0: Cond    if (Vx != Vy)    Skips the next instruction if VX does not equal
               // VY. (Usually the next instruction is a jump to skip a code block).
    _ANNN,     // ANNN: MEM     I = NNN    Sets I to the address NNN.
    _BNNN,     // BNNN: Flow    program_counter = V0 + NNN    Jumps to the address NNN plus V0.
    _CXNN,     // CXNN: Rand    Vx = rand() & NN    Sets VX to the result of a bitwise and
               // operation on a random number (Typically: 0 to 255) and NN.
    _DXYN,     // DXYN: Display draw(Vx, Vy, N)    Draws a sprite at coordinate (VX, VY) that
               // has a width of 8 pixels and a height of N pixels. Each row of 8 pixels is read
               // as bit-coded starting from memory location I; I value does not change after
               // the execution of this instruction. As described above, VF is set to 1 if any
               // screen pixels are flipped from set to unset when the sprite is drawn, and to 0
               // if that does not happen.
    _EX9E,     // EX9E: KeyOp   if (key() == Vx)    Skips the next instruction if the key stored
               // in VX(only consider the lowest nibble) is pressed (usually the next
               // instruction is a jump to skip a code block).
    _EXA1,     // EXA1: KeyOp   if (key() != Vx)    Skips the next instruction if the key stored
               // in VX(only consider the lowest nibble) is not pressed (usually the next
               // instruction is a jump to skip a code block).
    _FX07,     // FX07: Timer   Vx = get_delay()    Sets VX to the value of the delay timer.
    _FX0A,     // FX0A: KeyOp   Vx = get_key()    A key press is awaited, and then stored in VX
               // (blocking operation, all instruction halted until next key event, delay and
               // sound timers should continue processing).
    _FX15,     // FX15: Timer   delay_timer(Vx)    Sets the delay timer to VX.
    _FX18,     // FX18: Sound   sound_timer(Vx)    Sets the sound timer to VX.
    _FX1E,     // FX1E: MEM     I += Vx    Adds VX to I. VF is not affected.[c]
    _FX29,     // FX29: MEM     I = sprite_addr[Vx]    Sets I to the location of the sprite for
               // the character in VX(only consider the lowest nibble). Characters 0-F (in
               // hexadecimal) are represented by a 4x5 font.
    _FX33,     // FX33: BCD     set_BCD(Vx) *(I+0) = BCD(3); *(I+1) = BCD(2); *(I+2) = BCD(1);
               // Stores the binary-coded decimal representation of VX, with the hundreds digit
               // in memory at location in I, the tens digit at location I+1, and the ones digit
               // at location I+2.
    _FX55,     // FX55: MEM     reg_dump(Vx, &I)    Stores from V0 to VX (including VX) in
               // memory, starting at address I. The offset from I is increased by 1 for each
               // value written, but I itself is left unmodified.[d]
    _FX65,     // FX65: MEM     reg_load(Vx, &I)    Fills from V0 to VX (including VX) with
               // values from memory, starting at address I. The offset from I is increased by 1
               // for each value read, but I itself is left unmodified.[d]
    _NOOP,     // No operation
}OPCODE_t;

typedef enum REGISTER {
    V0 = 0,
    V1,
    V2,
    V3,
    V4,
    V5,
    V6,
    V7,
    V8,
    V9,
    VA,
    VB,
    VC,
    VD,
    VE,
    VF,
    TOTAL_REGISTERS,
}REGISTER;

class CPU {
public:
    static CPU & instance() {
        static CPU instance;
        return instance;
    }

    // registers
    std::array<uint8_t, TOTAL_REGISTERS> registers{}; // 16 registers
    uint16_t instruction_register = {_NOOP};
    uint16_t program_counter      = {PROGRAM_START_ADDR};
    uint16_t stack_pointer        = {PROGRAM_START_ADDR};
    std::shared_ptr<CHIP_8> machine{};

    void fetch();
    void decode();
    void execute();
    void reset();

private:
    // --- Private Constructor / Destructor ---
    CPU() = default;
    ~CPU() = default;

    // --- Disable copy and move ---
    CPU(const CPU &) = delete;
    CPU & operator=(const CPU &) = delete;
    CPU(CPU &&) = delete;
    CPU & operator=(CPU &&) = delete;
};
#endif // CPU_HPP
