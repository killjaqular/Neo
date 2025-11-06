// 3rd Party Libraries

// Platform Libraries

// Project Libraries
#include "include/chip_8.hpp"
#include "include/cpu.hpp"
#include "include/logger.hpp"
#include "include/rom.hpp"

// Standard Libraries
#include <cstdbool>
#include <cstdlib>
#include <filesystem>
#include <memory>
#include <string.h>
#include <string>

const char USAGE[] = {
"\nUsage: program [-help] -rom <some/path/to/chip-8.rom>\n"
"CHIP-8 Emulator for Linux on x86_64\n"
"    -help...........................Display this help menu.\n"
"    -rom str::string....REQUIRED....The CHIP-8 binary ROM to emulate.\n"
};

class Options {
public:
    std::unique_ptr<std::string> rom_file_path;

    // --- Access Singleton Instance ---
    static Options & instance() {
        static Options instance;
        return instance;
    }

    [[nodiscard("Ensure rom_file_path is set")]]
    bool has_rom_path() const noexcept {return rom_file_path && !rom_file_path->empty();}

    [[nodiscard("Ensure file exists")]]
    bool file_exists() const noexcept {
        bool ret = false;

        if (!has_rom_path()) goto has_rom_path_fail;

        try {
            ret = std::filesystem::exists(* rom_file_path);
            if (!ret) goto exists_failed;
        } catch (...) {
            goto exception;
        }

        ret = true;

    exception:
    exists_failed:
    has_rom_path_fail:
        return ret;
    }

    void set_rom_path(const char * path) {
        rom_file_path = std::make_unique<std::string>(path);
    }

private:
    // --- Private Constructor / Destructor ---
    Options() = default;
    ~Options() = default;

    // --- Disable copy and move ---
    Options(const Options &) = delete;
    Options & operator=(const Options &) = delete;
    Options(Options &&) = delete;
    Options & operator=(Options &&) = delete;
};

bool parse_cli(int argc, char * argv[]) {
    bool ret = false;

    Options & options = Options::instance();

    for (int i = 0; i < argc; i++) {
        if (!strcmp(argv[i], "-help")) {
            goto help;

        } else if (!strcmp(argv[i], "-rom")) {
            if (i + 1 >= argc) {
                fprintf(stdout, "Invalid syntax: -rom <path>\n");
                goto invalid_rom_option_syntax;
            } else {
                options.set_rom_path(argv[++i]);
            }

        } else {
            fprintf(stdout, "Unknown option=<%s>\n", argv[i]);
            goto unknown_option;
        }
    }

    if (!options.has_rom_path()) {
        fprintf(stdout, "Missing: -rom <path>\n");
        goto missing_rom;
    }

    if (!options.file_exists()) {
        fprintf(stdout, "%s does not exist on the file system\n",
                options.rom_file_path->c_str());
        goto missing_rom;
    }

    ret = true;

missing_rom:
unknown_option:
invalid_rom_option_syntax:
help:
    return ret;
}

int main(int argc, char * argv[]) {
    int ret = EXIT_FAILURE;

    CHIP_8 & chip_8_machine = CHIP_8::instance();
    Options & options = Options::instance();
    ROM & rom_reader = ROM::instance();

    if (!parse_cli(argc - 1, argv + 1)) {
        puts(USAGE);
        goto parse_cli_fail;
    }

    rom_reader.read_rom_from_file(*options.rom_file_path);

    if (!rom_reader.is_valid_chip_8_rom()) {
        fprintf(stdout, "Invalid CHIP-8 ROM binary format");
        goto is_valid_chip_8_rom_fail;
    }

    chip_8_machine.load_rom(rom_reader.rom);

    INFO(stdout, "Starting...");
    OKAY(stdout, "Emulating %s", Options::instance().rom_file_path->c_str());

    if (!chip_8_machine.run()) {
        ERRO(stdout, "CHIP-8 Virtual machine did not exit gracefully");
        goto run_fail;
    }

    ret = EXIT_SUCCESS;
    INFO(stdout, "Stopping...");

run_fail:
is_valid_chip_8_rom_fail:
parse_cli_fail:
    return ret;
}
