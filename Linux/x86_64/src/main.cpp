// 3rd Party Libraries

// Platform Libraries

// Project Libraries
#include "include/chip_8.hpp"
#include "include/cpu.hpp"
#include "include/logger.hpp"
#include "include/renderer.hpp"
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
            ret = std::filesystem::exists(*rom_file_path);
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

    Options & options = Options::instance();
    ROM & rom = ROM::instance();
    CHIP_8 & chip_8 = CHIP_8::instance();
    Renderer & renderer = Renderer::instance();

    // Parse given CLI
    if (!parse_cli(argc - 1, argv + 1)) {
        puts(USAGE);
        goto parse_cli_fail;
    }

    // Load in ROM file from disk
    if (!rom.read_rom_from_file(*options.rom_file_path)) {
        ERRO(stdout, "rom_reader.read_rom_from_file() fail");
        goto read_rom_from_file_fail;
    }
    // Validate ROM file
    if (!rom.is_valid_chip_8_rom()) {
        ERRO(stdout, "rom_reader.is_valid_chip_8_rom() fail");
        goto is_valid_chip_8_rom_fail;
    }

    // Load the ROM into the CHIP-8 virtual machine's memory
    chip_8.load_rom(rom.rom);
    if (!chip_8.rom) {
        ERRO(stdout, "chip_8.load_rom() fail");
        goto chip_8_load_rom_fail;
    }

    // Start the Renderer
    renderer.NEO_SDL_CreateWindow();
    if (!renderer.window) {
        ERRO(stdout, "renderer.NEO_SDL_CreateWindow() fail");
        goto renderer_NEO_SDL_CreateWindow_fail;
    }
    renderer.NEO_SDL_CreateRenderer();
    if (!renderer.renderer) {
        ERRO(stdout, "renderer.NEO_SDL_CreateRenderer() fail");
        goto renderer_NEO_SDL_CreateRenderer_fail;
    }

    INFO(stdout, "Starting...");
    OKAY(stdout, "Emulating %s", Options::instance().rom_file_path->c_str());

    // Run the CHIP-8 virtual machine
    if (!chip_8.run()) {
        ERRO(stdout, "CHIP-8 Virtual machine did not exit gracefully");
        goto run_fail;
    }

    ret = EXIT_SUCCESS;
    INFO(stdout, "Stopping...");

run_fail:
renderer_NEO_SDL_CreateRenderer_fail:
renderer_NEO_SDL_CreateWindow_fail:
chip_8_load_rom_fail:
is_valid_chip_8_rom_fail:
read_rom_from_file_fail:
parse_cli_fail:
    exit(ret);
}
