#include <openssl/aes.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

static const uint8_t key[] = "JmpwfGjcfsjpnfJmpwfGjcfsjpnf0000";

int main()
{
    int i;
    AES_KEY ctx;
    uint8_t in[13];
    uint8_t out[AES_BLOCK_SIZE];

    // READ MAC
    printf("Please input GPON MAC address: ");
    i = fread(in, sizeof(char), 12, stdin);
    if (i != 12)
    {
        fprintf(stderr, "Invalid MAC length!\n");
        return EINVAL;
    }
    in[13] = 0;

    // CHECK MAC
    for (i = 0; i < 12; i++)
    {
        char *c = (char *)(in + i);
        if (((*c >= 0x30) && (*c <= 0x39)) || ((*c >= 0x41) && (*c <= 0x46)))
        {
            continue; // 0-9 A-F
        }
        if ((*c >= 0x61) && (*c <= 0x66))
        {
            *c = (*c) - 0x20; // uppercase when a-f
        }
        else
        {
            fprintf(stderr, "\n\nInvalid input character!\nPlease input EPON MAC address like 112233AABBCC\n");
            return EINVAL;
        }
    }
    printf("\n\nInput MAC is %s\n", in);

    // ENCRYPT MAC
    AES_set_encrypt_key(key, 256, &ctx);
    uint8_t padded[AES_BLOCK_SIZE];
    memset(padded, 0, AES_BLOCK_SIZE);
    memcpy(padded, in, 12);
    AES_ecb_encrypt(padded, out, &ctx, AES_ENCRYPT);

    // PRINT PASSWORD
    printf("Password is ");
    for (i = 0; i < AES_BLOCK_SIZE; i++)
    {
        printf("%02X", out[i]);
    }
    putchar('\n');

    return 0;
}