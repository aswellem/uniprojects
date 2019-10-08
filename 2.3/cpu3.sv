module encryptor (output logic [4:0] Y, [4:0] data,
					input logic [4:0] A, [4:0] key);
		assign  Y = data ^ key;
		`include "xoropcode.h"
		`define [4:0] key 5'b10101;
	always_comb
	unique casez (A)
	5'b00000  : data = `@;
	5'b00001  : data = `a;
	5'b00010  : data = `b;
	default: data = `@;
	endcase
endmodule
		
