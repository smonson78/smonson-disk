
#include <gemdos.h>
#include <libc.h>
#include <xbios.h>

#include "acsi.h"

/* This is a simple time and date setting program */

// This is set up by crt0:
extern uint8_t *_basepage;

int year;
int month;
int day;
int hour;
int minute;
int second;

typedef struct {
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
} smonson_disk_datetime_t;

int settime_command() {

    uint8_t cmdbuf[11];
    uint8_t dmabuf[16];

    uint8_t controller = 0;
    uint8_t command = ICD_ESCAPE_CMD;

    memset(cmdbuf, 0, sizeof(cmdbuf));
    memset(dmabuf, 0, sizeof(dmabuf));

    cmdbuf[0] = (controller << 5) | command;
    cmdbuf[1] = SMONSON_DISK_ACSI_CONFIG_CMD;

    smonson_disk_datetime_t datetime;

    memcpy(dmabuf, &datetime, sizeof(smonson_disk_datetime_t));
    
    // ACSI send 1 sector which will only end up being 16 bytes
    acsi_command(cmdbuf, 11, dmabuf, 1, 1);

    return 0;
}

int getnumber(uint8_t *string, int length) {
    int acc = 0;

    while (length > 0) {
        acc *= 10;
        int digit = *string - '0';
        acc += digit;
        string++;
        length--;
    }

    return acc;
}


int main(int argc, char **argv)
{
    uint8_t cli_len = *(_basepage + 128);

    printf("Smonson Disk set clock\n");

    if (cli_len != 10) {
        printf("CLI length: %d\n", cli_len);
        printf("Date format: YYYY-MM-DD plz\n");
    } else {
        year = getnumber(_basepage + 129, 4);
        month = getnumber(_basepage + 129 + 5, 2);
        day = getnumber(_basepage + 129 + 8, 2);

        printf("Going to set date: %d-%d-%d\n", year, month, day);

        //xbios_supexec(test_inquiry_command);
        xbios_supexec(settime_command);
    }

    printf("--- any key to exit ---");
    Cconin();

    return 0;
}