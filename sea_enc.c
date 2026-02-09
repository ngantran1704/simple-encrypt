#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("USAGE: ./sea_enc KEY_IN_HEX\n");
        return 1;
    }

    uint16_t N = (uint16_t) strtol(argv[1], NULL, 16);

    const char hex[] = "0123456789ABCDEF";

    unsigned char buf[1024];
    ssize_t n;

    int count = 0;   // encrypted bytes printed on current line
    int any = 0;     // printed at least one byte

    while ((n = read(0, buf, sizeof(buf))) > 0) {
        for (ssize_t i = 0; i < n; i++) {
            uint8_t encrypted = (uint8_t)((N ^ buf[i]) & 0xFF);

            putchar(hex[encrypted >> 4]);
            putchar(hex[encrypted & 0x0F]);

            any = 1;
            count++;
            if (count == 40) {
                putchar('\n');
                count = 0;
            }

            // rotate left 1 bit (16-bit)
            N = (uint16_t)((N << 1) | (N >> 15));
            // multiply by 257, keep lower 16 bits
            N = (uint16_t)((N << 8) + N);
        }
    }

    // output always ends with newline; empty input prints only newline
    if (!any) {
        putchar('\n');
    } else if (count != 0) {
        putchar('\n');
    }

    return 0;
}
