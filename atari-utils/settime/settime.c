
#include <gemdos.h>
#include <libc.h>
#include <xbios.h>

#include "../acsi.h"

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

smonson_disk_datetime_t datetime;

int settime_command() {

    uint8_t cmdbuf[11];
    uint8_t dmabuf[16];

    uint8_t controller = 0;

    memset(cmdbuf, 0, sizeof(cmdbuf));
    memset(dmabuf, 0x99, sizeof(dmabuf));

    cmdbuf[0] = (controller << 5) | ICD_ESCAPE_CMD;
    cmdbuf[1] = SMONSON_DISK_ACSI_CONFIG_CMD;
    cmdbuf[2] = SMONSON_SETTIME;

    memcpy(dmabuf, &datetime, sizeof(smonson_disk_datetime_t));
    
    // ACSI send 1 sector which will only end up being 16 bytes
    uint16_t acsi_result = acsi_command(cmdbuf, 11, dmabuf, 1, 1);

    return acsi_result;
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

    if (cli_len != 19) {
        printf("Date format: YYYY-MM-DD HH:MM:SSplz\n");
    } else {
        datetime.year = getnumber(_basepage + 129, 4);
        datetime.month = getnumber(_basepage + 129 + 5, 2);
        datetime.day = getnumber(_basepage + 129 + 8, 2);

        datetime.hour = getnumber(_basepage + 129 + 11, 2);
        datetime.minute = getnumber(_basepage + 129 + 14, 2);
        datetime.second = getnumber(_basepage + 129 + 17, 2);

        printf("Going to set date: %04d-%02d-%02d %02d:%02d:%02d\n", 
            datetime.year, datetime.month, datetime.day,
            datetime.hour, datetime.minute, datetime.second);

        int result = xbios_supexec(settime_command);
        if (result) {
            printf("Error setting RTC in device!\n");
            Cconin();
            exit(1);
        }
    }

    printf("--- any key to exit ---");
    Cconin();

    return 0;
}
