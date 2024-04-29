
#include <gemdos.h>
#include <libc.h>
#include <xbios.h>

#include "../acsi.h"

/* Get the time and date from the hard drive's RTC and set the local date/time from it. */

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

int gettime_command() {

    uint8_t cmdbuf[11];
    uint8_t dmabuf[16];

    uint8_t controller = 0;

    memset(cmdbuf, 0, sizeof(cmdbuf));
    cmdbuf[0] = (controller << 5) | ICD_ESCAPE_CMD;
    cmdbuf[1] = SMONSON_DISK_ACSI_CONFIG_CMD;
    cmdbuf[2] = SMONSON_GETTIME;

    memset(dmabuf, 0, sizeof(dmabuf));

    // ACSI receive 1 sector which will only end up being 16 bytes
    uint16_t acsi_result = acsi_command(cmdbuf, 11, dmabuf, 1, 0);
    if (acsi_result != 0) {
        return 1;
    }

    memcpy(&datetime, dmabuf, sizeof(smonson_disk_datetime_t));

    return 0;
}

int main(int argc, char **argv)
{
    int result = xbios_supexec(gettime_command);

    if (result) {
        printf("Smonson Disk local time - error accessing RTC on device.\n");
        exit(1);
    }

    printf("Smonson Disk local time: %04d-%02d-%02d %02d:%02d:%02d\n",
        datetime.year, datetime.month, datetime.day, datetime.hour, datetime.minute, datetime.second);

    uint16_t date;
    uint16_t time;

    date = (datetime.year - 1980) << 9;
    date |= datetime.month << 5;
    date |= datetime.day;
    Tsetdate(date);

    time = datetime.hour << 11;
    time |= datetime.minute << 5;
    time |= datetime.second << 0;
    Tsettime(time);

    return 0;
}
