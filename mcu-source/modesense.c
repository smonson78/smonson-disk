#include "acsi.h"
#include "fpga_comm.h"
#include "sdcard.h"

// These appear not to do anything with the ICD Tools drivers.
uint16_t tracks = 5000;
uint8_t sectors = 100;
uint8_t heads = 10;

void do_mode_sense_page_3() {
    set_data_out();
    set_data_mode();

    write_byte(0x03);  // Page code = 3
    write_byte(0x16); // Page length

    write_byte(tracks >> 8); // Tracks per zone
    write_byte(tracks & 0xff);
    write_byte(0); // Alternate sectors per zone MSB
    write_byte(0);
    write_byte(0); // Alternate tracks per zone
    write_byte(0);
    write_byte(0); // Alternate tracks per volume
    write_byte(0);

    write_byte(0); // Sectors per track MSB
    write_byte(sectors);

    write_byte(2); // Bytes per physical sector MSB (512)
    write_byte(0); // LSB

    write_byte(1); // Interleave
    write_byte(0);
    write_byte(0); // Track skew
    write_byte(0);
    write_byte(0); // Cylinder skew
    write_byte(0);
    write_byte(0); // Drive type etc
    write_byte(0);
    write_byte(0);
    write_byte(0);
}

void do_mode_sense_page_1() {
    set_data_out();
    set_data_mode();

    write_byte(0x01); // Page code = 1
    write_byte(0x10); // Page length

    write_byte(0x07); // Various bits - disable all recovery options
    write_byte(0); // Read retry count
    write_byte(0); // Obsolete
    write_byte(0); // Obsolete
    write_byte(0); // Obsolete
    write_byte(0); // Reserved/restricted
    write_byte(0); // Write retry count
    write_byte(0); // Reserved
    write_byte(0); // Recovery time limit MSB
    write_byte(0); // Recovery time limit LSB

    // TODO: on data error, the block address should be returned in the sense data
}

void do_mode_sense_page_4() {
    // Rigid Drive Geometry page
    set_data_mode();
    set_data_out();

    write_byte(0x4);  // Page code = 4
    write_byte(0x16); // Page length

    write_byte(0); // Cylinders MSB
    write_byte(tracks >> 8); // Cylinders
    write_byte(tracks & 0xff); // Cylinders LSB
    
    write_byte(heads); // Heads
    
    write_byte(0); // Write Precompensation MSB
    write_byte(0); 
    write_byte(0); // LSB
    write_byte(0); // Starting cyinder MSB
    write_byte(0); 
    write_byte(0); // LSB
    write_byte(0); // Step rate MSB
    write_byte(0); // Step rate LSB
    write_byte(0); // Landing Zone MSB
    write_byte(0);
    write_byte(0); // LSB
    write_byte(0);
    write_byte(0);
    write_byte(0);
    write_byte(0);
    write_byte(0);
    write_byte(0);
    write_byte(0);
}

void do_mode_sense_page_0() {
    // Atari's ACSI specific page
    set_data_mode();
    set_data_out();

    write_byte(0x0);  // Dummy 0
    write_byte(0x0);  // Dummy 0
    write_byte(0x0);  // Dummy 0
    write_byte(0x8);  // Dummy 0 this seems to be a magic number

    write_byte(0x0);  // Dummy 1
    write_byte(0x0);  // Dummy 1
    write_byte(0x0);  // Dummy 1
    write_byte(0x0);  // Dummy 1

    write_byte(0x0);  // Sector size 512. ACHI still thinks it's 2048 though
    write_byte(0x0);  // Sector size
    write_byte(0x2);  // Sector size
    write_byte(0x0);  // Sector size

    write_byte(0x1);  // Format code

    write_byte(tracks >> 8); // Cylinders
    write_byte(tracks & 0xff); // Cylinders LSB
    
    write_byte(heads); // Heads
    
    write_byte(0x0);  // Reduced current thing
    write_byte(0x0);  // Reduced current thing

    write_byte(0); // Write Precompensation MSB
    write_byte(0); // LSB

    write_byte(0); // Landing Zone

    write_byte(0); // Step rate
    // SOFT/CHNG=0, FIX=4, HARD=8 apparently
    write_byte(8); // Type

    write_byte(sectors); // Sectors
}
