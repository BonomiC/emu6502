#include "rom.h"

uint8_t romBuffer[MAX_ROM_SIZE];
FILE *romFile;

uint16_t read_rom(const char *filename)
{
    romFile = fopen(filename, "rb");
    if (romFile == NULL)
    {
        perror("Error opening file");
        return 0;
    }

    uint16_t bytesRead = fread(romBuffer, sizeof(char), MAX_ROM_SIZE, romFile);
    if (ferror(romFile))
    {
        perror("Error reading file");
        bytesRead = 0;
    }

    fclose(romFile);

    return bytesRead;
}
