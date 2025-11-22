#ifndef ROM_HPP
#define ROM_HPP
// 3rd Party Libraries

// Platform Libraries

// Project Libraries
#include "logger.hpp"

// Standard Libraries
#include <cstring>
#include <fstream>
#include <memory>
#include <vector>

class ROM {
public:
    // --- Access Singleton Instance ---
    static ROM & instance() {
        static ROM instance;
        return instance;
    }

    std::unique_ptr<std::vector<uint8_t>> rom;

    [[nodiscard("Ensure ROM File was read in from filesystem")]]
    bool read_rom_from_file(const std::string & rom_file_path) {
        bool ret = false;

        std::streampos size;
        std::unique_ptr<std::vector<uint8_t>> buffer;
        std::unique_ptr<std::ifstream> temp;

        if (rom_file_path.empty()) {
            ERRO(stdout, "rom_file_path.empty()");
            goto rom_file_path_empty;
        }

        temp = std::make_unique<std::ifstream>(rom_file_path, std::ios::in | std::ios::binary);

        if (!temp || !temp->is_open()) {
            ERRO(stdout, "!temp || !temp->is_open()");
            goto temp_is_open_fail;
        }

        temp->seekg(0, std::ios::end);
        size = temp->tellg();
        if (size <= 0) {
            ERRO(stdout, "size <= 0");
            goto tellg_fail;
        }
        temp->seekg(0, std::ios::beg);

        rom = std::make_unique<std::vector<uint8_t>>(static_cast<size_t>(size));
        if (!temp->read(reinterpret_cast<char*>(rom->data()), size)) {
            ERRO(stdout, "!temp->read(reinterpret_cast<char*>(rom->data()), size)");
            goto read_fail;
        }

        INFO(stdout,
             "Loaded %s CHIP-8 ROM (%lld bytes)",
             rom_file_path.c_str(),
             static_cast<long long>(size));

        ret = true;

    read_fail:
    tellg_fail:
    temp_is_open_fail:
    rom_file_path_empty:
        return ret;
    }

    /**
     * TODO: Implement checks to validate ROM file
     * NOTE: There is no CHIP-8 ROM "header", therefore there are no "magic bytes" to check for.
     *       Instead, I will implement my own huristic to give a best judgement if a file is a
     *       valid CHIP-8 ROM format or not.
     *       For now, assume the given file is a valid CHIP-8 ROM binary file.
     * 1. Check file size
     * 2. Even byte count
    */
    [[nodiscard("Ensure ROM is a valid ROM file")]]
    bool is_valid_chip_8_rom() {
        bool ret = false;

        ret = true;

        return ret;
    }

private:
    // --- Private Constructor / Destructor ---
    ROM() = default;
    ~ROM() = default;

    // --- Disable copy and move ---
    ROM(const ROM &) = delete;
    ROM & operator=(const ROM &) = delete;
    ROM(ROM &&) = delete;
    ROM & operator=(ROM &&) = delete;
};
#endif // ROM_HPP
