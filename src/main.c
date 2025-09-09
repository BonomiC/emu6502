#include <stdio.h>
#include "cpu.h"
#include "rom.h"
#include "pager.h"

#include <SDL3/SDL.h>

int	 usleep(useconds_t) __DARWIN_ALIAS_C(usleep);

#define ADD_INSTRUCTION(...) add_instruction((uint8_t[]){__VA_ARGS__}, sizeof((uint8_t[]){__VA_ARGS__}))
#define ADD_PROGRAM(addr,...) add_program(addr, (uint8_t[]){__VA_ARGS__}, sizeof((uint8_t[]){__VA_ARGS__}))

#define PIXEL_SIZE 10
#define ROWS 32
#define COLS 32

void add_instruction(uint8_t *instructions, uint16_t size)
{
    static uint16_t ram_pos = 0x0600;
    memcpy(mainMemory + ram_pos, instructions, size);
    ram_pos += size;
}

void add_program(uint16_t addr, uint8_t *program, uint16_t size)
{
    memcpy(mainMemory + addr, program, size);
}

int main(int argc, char** argv)
{
    initialize();

    context.pc = 0x0600;

    if (argc > 1)
    {
        uint16_t romSize = read_rom(argv[1]);
        if (romSize == 0)
        {
            return 1;
        }
        printf("RomSize: %d\n", romSize);

        add_program(0x600, romBuffer, romSize);
    }
    else
    {
        ADD_INSTRUCTION(0xa2, 0x08, 0xca, 0x8e, 0x00, 0x02, 0xe0, 0x03, 0xd0, 0xf8, 0x8e, 0x01, 0x02, 0xCB);
    }

    // Initialize SDL
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Create a window
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_CreateWindowAndRenderer("Simple SDL3 Example", 320, 320, 0, &window, &renderer);
    if (window == NULL) {
        SDL_Log("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_FRect pixels[ROWS*COLS];
    for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c < COLS; c++)
        {
            pixels[r * ROWS + c] = (SDL_FRect){.x=c*PIXEL_SIZE, .y=r*PIXEL_SIZE, .w=10, .h=10};
        }
    }

    // Event loop
    SDL_Event e;
    int quit = 0;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_EVENT_QUIT) {
                quit = 1;
            }
            else if (e.type == SDL_EVENT_KEY_DOWN)
            {
                switch (e.key.key)
                {
                case SDLK_W:
                    mainMemory[0xFF] = 0x77;
                    break;
                case SDLK_A:
                    mainMemory[0xFF] = 0x61;
                    break;
                case SDLK_S:
                    mainMemory[0xFF] = 0x73;
                    break;
                case SDLK_D:
                    mainMemory[0xFF] = 0x64;
                    break;
                }
            }
        }

        mainMemory[0xFE] = rand();

        // Have an instruction to execute, so step the CPU
        if (mainMemory[context.pc] != 0x00)
        {
            step();
            step_pager();
            print_context();
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        for (int r = 0; r < ROWS; r++)
        {
            for (int c = 0; c < COLS; c++)
            {
                uint8_t color = mainMemory[0x0200 + (r*ROWS+c)];
                uint8_t red = color > 0 ? 255 : 0;

                SDL_SetRenderDrawColor(renderer, red, 0, 0, SDL_ALPHA_OPAQUE);
                SDL_RenderFillRect(renderer, &pixels[r*ROWS+c]);
            }
        }

        SDL_RenderPresent(renderer);

        // getchar();
    }

    // Destroy window and quit SDL
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

