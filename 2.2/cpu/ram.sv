/////////////////////////////////////////////////////////////////////
// Design unit: RAM
//            :
// File name  : ram.sv
//            :
// Description: Synchronous RAM for basic processor
//            :
// Limitations: None
//            : 
// System     : SystemVerilog IEEE 1800-2005
//            :
// Author     : Mark Zwolinski
//            : School of Electronics and Computer Science
//            : University of Southampton
//            : Southampton SO17 1BJ, UK
//            : mz@ecs.soton.ac.uk
//
// Revision   : Version 1.0 05/08/08
//            : Version 1.1 17/12/13
//            : Version 1.2 12/12/14 Only map top half of address range
/////////////////////////////////////////////////////////////////////

module RAM #(parameter WORD_W = 8, OP_W = 3)
               (input logic clock, n_reset, MDR_bus, load_MDR, load_MAR, CS, R_NW,
                inout wire [WORD_W-1:0] sysbus);

`include "opcodes.h"
		
logic [WORD_W-1:0] mdr;
logic [WORD_W-OP_W-1:0] mar;
logic [WORD_W-1:0] mem [0:(1<<(WORD_W-OP_W-1))-1]; //top half of address range
//logic [WORD_W-1:0] mem [0:11]; //top half of address range
int i;

assign sysbus = (MDR_bus & mar[WORD_W-OP_W-1]) ? mdr : {WORD_W{1'bZ}};

always_ff @(posedge clock, negedge n_reset)
  begin
  if (~n_reset)
    begin 
    mdr <= 0;
    mar <= 0;
    end
  else
    if (load_MAR)
      mar <= sysbus[WORD_W-OP_W-1:0]; 
    else if (load_MDR)
      mdr <= sysbus;
    else if (CS & mar[WORD_W-OP_W-1])
      if (R_NW)
        mdr <= mem[mar[WORD_W-OP_W-2:0]];
      else
        mem[mar[WORD_W-OP_W-2:0]] <= mdr;
  end


endmodule

module register #(parameter WORD_W = 8, OP_W = 3, N = 4)
               (output logic [7:0] data_out, input logic clock, n_reset, MDR_bus, load_MDR, load_MAR, CS, R_NW,[7:0] data_in,
                inout wire [WORD_W-1:0] sysbus);

logic [WORD_W-1:0] mdr;
logic [WORD_W-OP_W-1:0] mar;
logic [WORD_W-1:0] mem [0:(1<<(WORD_W-OP_W-1))-1]; //top half of address range
							   							   
always_ff @(posedge clock, negedge n_reset)
	begin
		if (~n_reset)
		begin
		mdr <= 0;
		mar <= 0;
		end
	else
		if (load_MAR)
		mar <= sysbus[WORD_W-OP_W-1:0];
		else if (load_MDR)
			mdr <= sysbus;
		else if (CS & (mar == 31)) // look for address 31
			if (!R_NW) // Writing, so this signal needs to be low
			data_out <= mdr;
		end

endmodule

module buffer #(parameter WORD_W = 8, OP_W = 3, N = 4)
               (output logic data, input logic clock, n_reset, MDR_bus, load_MDR, load_MAR, CS, R_NW, [7:0] data_in,
                inout wire [WORD_W-1:0] sysbus);
								
			
logic [WORD_W-1:0] mdr;
logic [WORD_W-OP_W-1:0] mar;
logic [WORD_W-1:0] mem [0:(1<<(WORD_W-OP_W-1))-1];

assign sysbus = (MDR_bus & (mar == 30)) ? data_in : {WORD_W{1'bZ}};
always_ff @(posedge clock, negedge n_reset)
	begin
		if (~n_reset)
		begin
			mar <= 0;
		end
		else
		if (load_MAR)
			mar <= sysbus[WORD_W-OP_W-1:0];
		end

endmodule

module switch #(parameter WORD_W = 8, OP_W = 3, N = 4)
               (output logic [7:0] sw, 
 		input logic clock, n_reset, MDR_bus, load_MDR, load_MAR, CS, R_NW,
                inout wire [WORD_W-1:0] sysbus);

//`include "opcodes.h"
		

logic [WORD_W-OP_W-1:0] mar;
logic [WORD_W-1:0] mdr;


assign sysbus = (MDR_bus & ~mar[WORD_W-OP_W-1]) ? mdr : {WORD_W{1'bZ}};

always_ff @(posedge clock, negedge n_reset)
  begin
  if (~n_reset)
    begin 
    mar <= 0;
    end
  else
    if (load_MAR)
      mar <= sysbus[WORD_W-OP_W-1:0];
  end

endmodule 