`timescale 1 ns / 100 ps

// Macro to help test
`define assert(expression) if (!expression) begin $display("ASSERTION FAILED in %m"); $finish; end


module tb();

    reg clock;

    reg [7:0] data;
    reg f_cs;
    reg f_reset;
    reg f_ack;
    reg f_a1;
    reg f_rw;
    wire [7:0] f_data = f_rw ? 8'bz : data;

    wire f_irq;
    wire f_drq;
    wire f_bus_dir;

    reg [7:0] avr_data;
    wire [7:0] a_data = a_bus_dir ? avr_data : 8'bz;
    reg a_cs;
    reg a_bus_dir;
    //reg a_cd;
    //reg a_cont;
    wire a_cmd;
    wire a_int;
    reg a_extra;
    reg a_extra2;
    reg a_ready;

    // What the AVR will set the extra data write byte to
    reg [7:0] extra_data_write = 8'b00100000;

    wire avr_mosi;
    wire avr_miso;
    wire avr_sck;
    wire mosi;
    wire miso;
    wire sck;
    wire led0;
    wire led1;
    wire a_clock = 0;
    wire a_reset = 1;

    assign avr_miso = 1'bz;
    assign mosi = 0;
    assign sck = 0;
    
    wire drq = ~f_drq;
    wire irq = ~f_irq;

    reg [7:0] testdata = 0;

    hdd hdd_model(
	    clock,
	    // Atari signals
	    f_data, f_cs, f_bus_dir, f_reset, f_ack, f_a1, f_rw, f_irq, f_drq,
	    // AVR signals
	    a_reset, a_data, a_cs, a_bus_dir, a_cmd, a_int, a_extra, a_extra2, a_ready
    );

    always begin
        #1 clock = ~clock;
    end

    initial begin
        #10000; // Wait a long time
        $display("Ended by timeout");
        $finish;
    end

    initial begin
        $display($time, " << Starting Simulation >> ");
        $dumpfile("dump.lxt");
        $dumpvars(0, tb);

        clock = 0;

        // Atari pins at default state
        f_cs = 1;
        f_ack = 1;
        f_rw = 1;
        f_a1 = 1;
        data = 8'hff;

        // AVR pins at default state
        a_extra = 0;
        a_extra2 = 0;
        a_ready = 0;
        a_bus_dir = 0;
        a_cs = 0;
        avr_data = 8'hff;

        #15;


        // AVR sets up the ACSI ID mask to respond to ACSI ID 0
        a_extra2 = 1;
        avr_data = 1;
        a_bus_dir = 1;
        #1;
        a_cs = 1;
        #5;
        a_cs = 0;
        a_extra2 = 0;
        a_bus_dir = 0;
        #15;


        // Start a REQUEST SENSE command - byte 0
        data[7:5] = 3'b000; // ACSI_ID = 0
        data[4:0] = 5'b00011; // OPCODE = 0x03
        #5;
        f_rw = 0;
        f_a1 = 0;
        #5;
        f_cs = 0;
        #5;
        f_cs = 1;
        #5;
        f_rw = 1;
        f_a1 = 1;
        #1;

        // AVR picks it up
        wait (a_int) #5;
        a_cs = 1;
        #5;
        a_cs = 0;
        #1;

        // Byte 1 - 5
        repeat (5) begin

            // AVR asserts readiness for another byte
            a_ready = 1;
            #5;

            // Atari waits for IRQ
            wait (f_irq);

            // Atari strobes data in
            data = 0;
            f_rw = 0;
            #1;
            f_cs = 0;
            #5;
            f_cs = 1;
            #1;
            f_rw = 1;

            // FPGA automatically sets a_int

            // AVR Picks up the byte
            wait (a_int);
            a_ready = 0;
            #5;
            a_cs = 1;
            #5;
            a_cs = 0;
            #5;
        end

        #10;

        // Phase 2: AVR switches to data mode to send 4 bytes response data

        // Extra data write to set command mode off (i.e. data mode)
        a_extra = 1;
        a_bus_dir = 1;
        #1;
        extra_data_write[5] = 0;
        avr_data = extra_data_write;
        a_cs = 1;
        #5;
        a_cs = 0;
        #5;
        a_extra = 0;
        #5;

        // Atari expects to read data, so RW remains high

        // AVR is going to write data, so a_bus_dir remains high

        // A long ACK pulse to test the FPGA will handle it correctly:

        // AVR wants to send a data-mode byte. Wait for device ready
        wait (a_int) #5;

        // AVR strobes in a write
        a_bus_dir = 1;
        avr_data = 8'h55;
        a_cs = 1;
        #2;

        // Atari waits for /DRQ
        wait (~drq) #2;
        #1;
        // Atari acknowledges receipt of data byte on bus with ACK signal
        f_ack = 0;
        #1;

        // Meanwhile, AVR finishes its CS pulse from when the byte was written
        a_cs = 0;

        // A long delay where the Atari is still holding onto /ACK (it's on a fixed 250ns time)
        #10;

        // AVR wants to send the next byte and is waiting for a_int here
        `assert(a_int == 0);

        // And Atari finally finishes its ACK pulse, a_int should now go high
        f_ack = 1;

        repeat (3) begin
            // AVR wants to send a data-mode byte

            // Wait for device ready
            wait (a_int) #5;

            // AVR strobes in a write
            a_bus_dir = 1;
            avr_data = 8'h55;
            a_cs = 1;
            #2;

            // Atari waits for /DRQ and asserts ACK
            wait (~drq) #2;
            #1;
            // Atari acknowledges receipt of data byte on bus with ACK signal
            f_ack = 0;
            #1;

            // Meanwhile, AVR finishes its CS pulse
            a_cs = 0;
            #4;

            // And Atari finishes its ACK pulse
            f_ack = 1;

        end

        // Data phase over, AVR switches back to command mode
        a_extra = 1;
        a_bus_dir = 1;
        #1;
        extra_data_write[5] = 1;
        avr_data = extra_data_write;
        a_cs = 1;
        #5;
        a_cs = 0;
        #5;
        a_extra = 0;
        a_bus_dir = 0;
        #5;

        // STATUS phase. AVR writes one command-mode status byte
        #2;
        a_bus_dir = 1;
        avr_data = 8'h33;
        #2;

        wait (a_int) #1;
        a_cs = 1;
        #5;
        a_cs = 0;

        // Atari reads the byte
        wait (!irq) #5;
        f_cs = 0;
        #5;
        f_cs = 1;
        #5;

        // AVR manually ends the transfer by unselecting the device
        a_extra = 1;
        a_bus_dir = 1;
        #1;
        extra_data_write[6] = 1;
        avr_data = extra_data_write;
        a_cs = 1;
        #5;
        a_cs = 0;
        #5;
        a_extra = 0;
        a_bus_dir = 0;
        #5;
        extra_data_write[6] = 0;

        #100;





        // Atari pins at default state
        f_cs = 1;
        f_ack = 1;
        f_rw = 1;
        f_a1 = 1;
        data = 8'hff;

        // AVR pins at default state
        a_extra = 0;
        a_ready = 0;
        a_bus_dir = 0;
        a_cs = 0;
        avr_data = 8'hff;

        #15;

        // Start a short WRITE command - byte 0
        data[7:5] = 3'b0; // ACSI_ID = 0
        data[4:0] = 5'b01010; // OPCODE = 0x0a
        #5;
        f_rw = 0;
        f_a1 = 0;
        #5;
        f_cs = 0;
        #5;
        f_cs = 1;
        #5;
        f_rw = 1;
        f_a1 = 1;
        #1;

        // AVR picks it up
        wait (a_int) #5;
        a_cs = 1;
        #5;
        a_cs = 0;
        #1;

        // Byte 1 - 5
        repeat (5) begin

            // AVR asserts readiness for another byte
            a_ready = 1;
            #5;

            // Atari waits for IRQ
            wait (f_irq);

            // Atari strobes data in
            data = 0;
            f_rw = 0;
            #1;
            f_cs = 0;
            #5;
            f_cs = 1;
            #1;
            f_rw = 1;

            // FPGA automatically sets a_int

            // AVR Picks up the byte
            wait (a_int);
            a_ready = 0;
            #5;
            a_cs = 1;
            #5;
            a_cs = 0;
            #5;
        end

        #10;

        // Phase 2: AVR switches to data mode to receive 256 bytes response data

        // Extra data write to set command mode off (i.e. data mode)
        a_extra = 1;
        a_bus_dir = 1;
        #1;
        extra_data_write[5] = 0;
        avr_data = extra_data_write;
        a_cs = 1;
        #5;
        a_cs = 0;
        #5;
        a_extra = 0;
        #5;

        // Atari expects to write data, so RW goes low
        f_rw = 0;

        #5;

        // AVR expects to read data, so a_bus_dir goes low
        a_bus_dir = 0;
        #5

        // Test reading with a long ACK pulse

        // First byte...
        // AVR is ready to receive a data-mode byte
        a_ready = 1;

        // Atari waits for /DRQ and asserts /ACK
        wait (~drq) #1;
        #1;

        // This /ACK will stay on for a long time to test that the FPGA holds off until the signal clears.
        f_ack = 0;
        #1;

        // AVR waits for INT to tell it there's a byte ready
        wait (a_int) #1;

        // AVR cancels ready and strobes in the data 
        a_ready = 0;
        a_bus_dir = 0;
        a_cs = 1;
        #2;
        a_cs = 0;
        #2;

        // Now AVR requests the next byte with a_ready, although /ACK is still asserted from the previous byte
        a_ready = 1;

        // The FPGA shouldn't assert /DRQ until /ACK is clear (250ns according to the Implementation Guide)
        #6;

        // And Atari finishes its ACK pulse for the first byte
        f_ack = 1;
        #1;

        // ... the remaining X number of bytes (512 in reality)
        repeat (16) begin
            // AVR is ready to receive a data-mode byte
            a_ready = 1;

            // Atari waits for /DRQ and asserts /ACK
            wait (~drq) #2;
            #2;
            f_ack = 0;
            #2;

            // AVR waits for INT to tell it there's a byte ready
            wait (a_int) #1;

            // AVR cancels ready and strobes in the data 
            a_ready = 0;
            a_bus_dir = 0;
            a_cs = 1;
            #2;

            // AVR finishes overlapping CS pulse
            a_cs = 0;
            #4;

            // And Atari finishes its /ACK pulse
            f_ack = 1;
            #2;

        end

        // Atari has finished writing, so RW returns high
        f_rw = 1;
        #2;

        // Data phase over, AVR switches back to command mode
        a_extra = 1;
        a_bus_dir = 1;
        #1;
        extra_data_write[5] = 1;
        avr_data = extra_data_write;
        a_cs = 1;
        #5;
        a_cs = 0;
        #5;
        a_extra = 0;
        a_bus_dir = 0;
        #5;

        // STATUS phase. AVR writes one command-mode status byte
        #2;
        a_bus_dir = 1;
        avr_data = 8'h33;
        #2;

        wait (a_int) #1;
        a_cs = 1;
        #5;
        a_cs = 0;

        // Atari reads the byte
        wait (!irq) #5;
        f_cs = 0;
        #5;
        f_cs = 1;
        #5;

        // AVR manually ends the transfer by unselecting the device
        a_extra = 1;
        a_bus_dir = 1;
        #1;
        extra_data_write[6] = 1;
        avr_data = extra_data_write;
        a_cs = 1;
        #5;
        a_cs = 0;
        #5;
        a_extra = 0;
        a_bus_dir = 0;
        #5;
        extra_data_write[6] = 0;

        #100;








        // A sector READ (disk to Atari) in double-buffered mode

        // Atari pins at default state
        f_cs = 1;
        f_ack = 1;
        f_rw = 1;
        f_a1 = 1;
        data = 8'hff;

        // AVR pins at default state
        a_extra = 0;
        a_ready = 0;
        a_bus_dir = 0;
        a_cs = 0;
        avr_data = 8'hff;

        #15;

        // Start a short WRITE command - byte 0
        data[7:5] = 3'b0; // ACSI_ID = 0
        data[4:0] = 5'b00110; // OPCODE = 0x06
        #5;
        f_rw = 0;
        f_a1 = 0;
        #5;
        f_cs = 0;
        #5;
        f_cs = 1;
        #5;
        f_rw = 1;
        f_a1 = 1;
        #1;

        // AVR picks it up
        wait (a_int) #5;
        a_cs = 1;
        #5;
        a_cs = 0;
        #1;

        // Byte 1 - 5
        repeat (5) begin

            // AVR asserts readiness for another byte
            a_ready = 1;
            #5;

            // Atari waits for IRQ
            wait (f_irq);

            // Atari strobes data in
            data = 0;
            f_rw = 0;
            #1;
            f_cs = 0;
            #5;
            f_cs = 1;
            #1;
            f_rw = 1;

            // FPGA automatically sets a_int

            // AVR Picks up the byte
            wait (a_int);
            a_ready = 0;
            #5;
            a_cs = 1;
            #5;
            a_cs = 0;
            #5;
        end

        #10;

        // Phase 2: AVR switches to data mode to send 512 bytes sector data

        // Extra data write to set command mode off (i.e. data mode) and double-buffered mode ON
        a_extra = 1;
        a_bus_dir = 1;
        #1;
        extra_data_write[1] = 1;
        extra_data_write[5] = 0;
        avr_data = extra_data_write;
        a_cs = 1;
        #5;
        a_cs = 0;
        #5;
        a_extra = 0;
        #5;


        // Atari expects to read data, so RW remains high

        // AVR is going to write data, so a_bus_dir remains high

        // A long ACK pulse to test the FPGA will handle it correctly:


        // Because FPGA can hold an extra buffered byte, there will 2 bytes on the wire at once. Therefore,
        // the AVR will send one additional byte now, and then 15 in the loop instead of 16

        // AVR wants to send a data-mode byte. Wait for device ready
        wait (a_int) #5;

        // AVR strobes in a write
        testdata = 1;
        avr_data = testdata;
        a_cs = 1;
        #2;
        a_cs = 0;
        #2;

        testdata = testdata + 1;

        repeat (15) begin
            // AVR wants to send a data-mode byte. Wait for device ready
            wait (a_int) #5;

            // AVR strobes in a write
            avr_data = testdata;
            a_cs = 1;
            #2;

            // Atari waits for /DRQ
            wait (~drq) #2;
            #1;
            // Atari acknowledges receipt of data byte on bus with ACK signal
            f_ack = 0;
            #1;

            // Meanwhile, AVR finishes its CS pulse from when the byte was written
            a_cs = 0;

            // A long delay where the Atari is still holding onto /ACK (it's on a fixed 250ns time)
            #10;

            // AVR wants to send the next byte and is waiting for a_int here
            //`assert(a_int == 0);

            // And Atari finally finishes its ACK pulse, a_int should now go high
            f_ack = 1;

            testdata = testdata + 1;
        end

        // The last byte is received by the Atari because it's still in the FPGA's buffer.

        // Atari waits for /DRQ
        wait (~drq) #2;
        #1;

        // Atari acknowledges receipt of data byte on bus with ACK signal
        f_ack = 0;
        #5;
        f_ack = 1;        

        // Data phase over, AVR switches back to command mode
        a_extra = 1;
        a_bus_dir = 1;
        #1;
        extra_data_write[5] = 1;
        avr_data = extra_data_write;
        a_cs = 1;
        #5;
        a_cs = 0;
        #5;
        a_extra = 0;
        a_bus_dir = 0;
        #5;

        // STATUS phase. AVR writes one command-mode status byte
        #2;
        a_bus_dir = 1;
        avr_data = 8'h33;
        #2;

        wait (a_int) #1;
        a_cs = 1;
        #5;
        a_cs = 0;

        // Atari reads the byte
        wait (!irq) #5;
        f_cs = 0;
        #5;
        f_cs = 1;
        #5;

        // AVR manually ends the transfer by unselecting the device
        a_extra = 1;
        a_bus_dir = 1;
        #1;
        extra_data_write[6] = 1;
        avr_data = extra_data_write;
        a_cs = 1;
        #5;
        a_cs = 0;
        #5;
        a_extra = 0;
        a_bus_dir = 0;
        #5;
        extra_data_write[6] = 0;

        #100;




/*

        // Start a status check command for the wrong device - byte 0
        data = 8'b00000000; // OPCODE = 0x12, device = 0
        #5;
        f_rw = 0;
        f_a1 = 0;
        #5;
        f_cs = 0;
        #5;
        f_cs = 1;
        #5;
        f_rw = 1;
        f_a1 = 1;

        // AVR picks it up and decides not to set CONTINUE
        wait (a_int);
        #5;        
        a_cs = 1;
        #5;
        a_cs = 0;
        #5;

        // Byte 1 - 4
        repeat (4) begin
            // Someone else generates IRQ - not us
            #5;

            data = 0;
            #5;
            f_rw = 0;
            #5;
            f_cs = 0;
            #5;
            f_cs = 1;
            #5;
            f_rw = 1;

            // Someone else picks it up
            //wait (a_int);
            #5;
            a_cs = 1;
            #5;
            a_cs = 0;
            #5;
        end
*/

/*
Not finished

        // INQUIRY command

        a_bus_dir = 0;
        a_cs = 0;
        //a_cd = 1;
        avr_data = 8'hff;        

        #100;

        // Start an INQUIRY command - byte 0
        data = 8'b01011000; // OPCODE = 0x12, device = 2
        #5;
        f_rw = 0;
        f_a1 = 0;
        #5;
        f_cs = 0;
        #5;
        f_cs = 1;
        #5;
        f_rw = 1;
        f_a1 = 1;

        // AVR picks it up and sets CONTINUE
        wait (a_int);
        //a_cont = 1;
        a_cs = 1;
        #5;
        a_cs = 0;
        #5;

        // Byte 1 - 4
        repeat (4) begin
            // Wait for IRQ
            wait (!f_irq);

            data = 0;
            #5;
            f_rw = 0;
            #5;
            f_cs = 0;
            #5;
            f_cs = 1;
            #5;
            f_rw = 1;

            // AVR picks it up
            wait (a_int);
            a_cs = 1;
            #5;
            a_cs = 0;
            #5;
        end

        // AVR unsets CONTINUE before last byte
        //a_cont = 0;

        // Byte 5
        // Wait for IRQ
        wait (!f_irq);

        data = 0;
        #5;
        f_rw = 0;
        #5;
        f_cs = 0;
        #5;
        f_cs = 1;
        #5;
        f_rw = 1;

        // AVR picks it up
        wait (a_int);
        #5;
        a_cs = 1;
        #5;
        a_cs = 0;
        #5;

        // DATA mode

        #20;
        // Phase 2: AVR switches to data mode, write
        //a_cd = 0;
        #5;
        a_bus_dir = 1;
        
        #5;
        // AVR writes some bytes
        avr_data = 8'haa;
        repeat (4) begin

            // AVR sends a byte
            wait (a_int) #5;
            a_cs = 1;
            #5;

            // Atari starts picking it up while the AVR is still trying to write it...
            f_ack = 0;
            #5;
            f_ack = 1;

            // Then the AVR finishes
            #5;
            a_cs = 0;

            
            #5;
        end

        // STATUS phase
        //a_cd = 1;
        #5;
        a_bus_dir = 1;
        #5;

        #20;
        // AVR switches to command mode and writes one byte
        //a_cd = 1;

        wait (a_int) #5;
        a_cs = 1;
        #5;
        a_cs = 0;

        // Atari reads the byte
        wait (!f_irq) #5;
        f_cs = 0;
        #5;
        f_cs = 1;
        #5;

*/

        #1000;
        $display($time, "<< Simulation Complete >>");
        $finish;
    end



endmodule
