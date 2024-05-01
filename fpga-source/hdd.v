
module hdd (
	clock,
	// Atari signals
	f_data, f_cs, f_bus_dir, f_reset, f_ack, f_a1, f_rw, f_irq, f_drq,
	// AVR signals
	a_reset, a_data, a_cs, a_bus_dir, a_cmd, a_int,
	a_extra, a_extra_2, a_ready
);

	input clock;
	inout [7:0] f_data;
	output f_bus_dir;
	input f_cs;             // Latch a byte in
	input f_reset;
	input f_ack;
	input f_a1;
	input f_rw;
	output f_irq;
	output f_drq;

	inout [7:0] a_data;
	output a_cmd;           // This bit is high when the byte received from the Atari was a command byte (it's effectively an extra data line)
	output a_int;           // asserted to indicate to AVR that a byte has been received or could be sent
	input a_bus_dir;        // low = FPGA writes to bus; high = AVR writes to bus
	input a_cs;             // strobe high to latch input/output
    
	// AVR<-->FPGA comms
	output a_reset;
	// Read/write "extra data" when set
	input a_extra;
    input a_extra_2;
	// AVR is waiting to receive the next byte if this signal is raised
	input a_ready;
	 
	// Drive "RESET" to zero when the Atari does. Otherwise tristate it. This is here so the device could be 
	// reset from the ST.
	wire a_reset = f_reset ? 1'bz : 1'b0;

	// Device config/status registers

	// Which ACSI IDs does this device respond to?
	// This is a bitmap with MSB indicating ACSI ID 7 and so on.
	reg [7:0] acsi_ids;

    // NOTE: Bits 0 and 6 are read directly from the bus and not stored in the register
    // Bit 0   - clear buffers if written to 1
    // Bit 1   - double-buffered send mode
    // Bit 2-3 - unused
    // Bit 5   - command/data mode (command high, data low)
    // Bit 6   - explicit unselect device if written to 1
    // Bit 7   - unused
	reg [7:0] extra_data_write;

    wire double_buffer_mode = extra_data_write[1];
	// Command/data phase control
	wire in_command_mode = extra_data_write[5];
	wire in_data_mode = ~extra_data_write[5];
	
    wire extra_data_write_condition = a_cs_rising && a_bus_dir && a_extra && !a_extra_2;
    
	initial begin
        // Respond to no ACSI IDs by default
		acsi_ids = 8'b0;
    
		// Command mode
		extra_data_write = 8'b00100000;
	end
		
	// Writes to the two internal registers - extra_data and acsi_ids
	always @(posedge clock) begin
		if (!f_reset) begin
			extra_data_write <= 8'b00100000;
			acsi_ids <= 0;
		end
		else if (extra_data_write_condition) begin
            // Get extra data byte from AVR
			extra_data_write <= a_data;
        end else if (a_cs_rising && a_bus_dir && !a_extra && a_extra_2) begin
            // Get ACSI IDs from AVR
            acsi_ids <= a_data;
		end
	end
	
	// ACSI bus state - this lasts until the AVR switches it off with an extra data write, or until
	// another ACSI command comes in for an ID we don't respond to.
	reg selected;
	
	initial begin
		selected = 0;
	end
	
	// ACSI command protocol
	reg [7:0] acsi_byte;
	reg acsi_byte_available;
	reg is_first_command_byte;
	
	reg [1:0] f_cs_edge_detect;
	// 1 = drive the bus
	assign f_bus_dir = selected & f_rw;
	assign f_data = (selected && f_rw) ? avr_byte : 8'bz;
	
	initial begin
		acsi_byte_available = 0;
		is_first_command_byte = 0;
		
		f_cs_edge_detect = 0;
	end
	
	// Edge detectors
	wire f_cs_falling = f_cs_edge_detect == 2'b10;
    wire f_cs_rising = f_cs_edge_detect == 2'b01;
	always @(posedge clock) begin
		f_cs_edge_detect <= {f_cs_edge_detect[0], f_cs};
	end

	always @(posedge clock) begin
		if (!f_reset) begin
			acsi_byte_available <= 0;
		end else if (in_command_mode) begin
			if (f_cs_falling && !f_rw && !f_a1) begin
				acsi_byte <= f_data;
				acsi_byte_available <= 1'b1;
				is_first_command_byte <= 1'b1;
			end else if (selected && f_cs_falling && !f_rw) begin
				// Subsequent command byte arrives from Atari
				acsi_byte <= f_data;
				acsi_byte_available <= 1'b1;
				is_first_command_byte <= 1'b0;
			end else if (selected && acsi_byte_available && a_cs_rising) begin
				// Byte is picked up by AVR in response to A_INT
				acsi_byte_available <= 1'b0;
			end
		end else begin
			// Data mode
			if (selected && f_ack_falling && !f_rw) begin
				// Atari writes a data byte
				acsi_byte <= f_data;
				acsi_byte_available <= 1'b1;
				is_first_command_byte <= 1'b0;
			end else if (selected && acsi_byte_available && a_cs_rising) begin
				// Byte is picked up by AVR in response to A_INT
				acsi_byte_available <= 1'b0;
			end
		end
	end
	
	// In command mode, assert IRQ if:
	// - AVR is indicating that it wants a byte and no byte is already in the buffer,
	// - OR AVR has written a byte that hasn't been picked up.

	// IRQ is open-collector and thus has inverted voltage logic
	// IRQ is active low, but inverted means the below wire is active high
	wire irq_signal = (a_ready & ~acsi_byte_available) | avr_byte_available;
	assign f_irq = (selected && in_command_mode) ? (irq_signal & f_cs) : 1'b0;

	// DRQ is also active low and thus inverted.
	wire drq_signal = (a_ready & ~acsi_byte_available) | avr_byte_available;
	assign f_drq = (selected && in_data_mode) ? (drq_signal & f_ack) : 1'b0;
	
	// Always respond to Command byte 0
	always @(posedge clock) begin
		if (!f_reset) begin
			selected <= 0;
		end else if (extra_data_write_condition && a_data[6]) begin
			// Force device out of selected state if 6th bit of extra_data_write is set.
            selected <= 0;
		end	else if (f_cs_falling && !f_rw && !f_a1) begin
			// On receipt of a command packet, select if it's for this device or unselect if it's not.
			if ((1 << f_data[7:5]) & acsi_ids) begin
				selected <= 1'b1;
			end else begin
				selected <= 0;
			end
		end
	end
	
	// AVR protocol
	assign a_data = a_bus_dir ? 8'bz : acsi_byte;

    // Buffered mode: a_int will be high if buffer can hold another byte (2 total)
    // Non-Buffered mode: a_int will be high if Atari is indicating readiness to receive a byte
    wire buffered_int_signal = double_buffer_mode ? (~avr_byte_buf_available) : (~avr_byte_available & f_ack);
    
    // a_int high means AVR can write a byte
	assign a_int = selected & (
		a_bus_dir ?
		// AVR ---> Atari: clear to send another byte
		buffered_int_signal
		// AVR <--- Atari: byte available for pickup
		: acsi_byte_available
	);
	assign a_cmd = selected & is_first_command_byte;
	
	// Edge detectors
	reg [1:0] a_cs_edge_detect;
	reg [1:0] f_ack_edge_detect;
	initial begin
		a_cs_edge_detect = 0;
		f_ack_edge_detect = 0;
	end
	wire a_cs_rising = a_cs_edge_detect == 2'b01;
	wire f_ack_falling = f_ack_edge_detect == 2'b10;
    wire f_ack_rising = f_ack_edge_detect == 2'b01;
	always @(posedge clock) begin
		a_cs_edge_detect <= {a_cs_edge_detect[0], a_cs};
		f_ack_edge_detect <= {f_ack_edge_detect[0], f_ack};
	end	

	// Command mode data transfer is handled by acsi_byte register and a_int pin

	// Bytes written by AVR for transfer to Atari
	reg [7:0] avr_byte;
    reg [7:0] avr_byte_buf;
	reg avr_byte_available;
    reg avr_byte_buf_available;

	initial begin
		avr_byte_available = 0;
        avr_byte_buf_available = 0;
	end

	always @(posedge clock) begin
		if (!f_reset) begin
			avr_byte_available <= 0;
            avr_byte_buf_available <= 0;
		end else if (extra_data_write_condition && a_data[0]) begin
			avr_byte_available <= 0;
            avr_byte_buf_available <= 0;
		end else if (!a_extra && !a_extra_2 && a_bus_dir && a_cs_rising) begin
			// AVR writes a byte
            if (avr_byte_available) begin
                avr_byte_buf <= a_data;
                avr_byte_buf_available <= 1;
            end else begin
                avr_byte <= a_data;
                avr_byte_available <= 1;
            end
		end else if (in_command_mode && f_rw && f_cs_rising) begin
			// Atari receives a byte in command mode
			avr_byte_available <= avr_byte_buf_available;
            avr_byte <= avr_byte_buf;
            avr_byte_buf_available <= 0;
		end else if (in_data_mode && f_rw && f_ack_rising) begin
			// Atari receives a byte in data mode
			avr_byte_available <= avr_byte_buf_available;
            avr_byte <= avr_byte_buf;
            avr_byte_buf_available <= 0;
		end
	end
     
endmodule
