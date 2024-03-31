`timescale 1ns/1ns
module toplevelTB;
  reg r0_in, r1_in, r0_out, r1_out, clk, rst;
  wire [3:0] data;
  busdriver bus(r0_in, r1_in, r0_out, r1_out, clk, rst, data);
  initial begin
    rst = 1; clk = 0;
    r0_in = 0; r1_in = 0; r0_out = 0; r1_out = 0;
    #20 rst = 0;
    #40 r0_out = 1;
    #20 r1_in = 1;
    #10 r1_in = 0;
    #40 r0_out = 0;
    #40 r1_out = 1;
    #20 r0_in = 1;
    #10 r0_in = 0;
    #40 r1_out = 0;
  end
  
  always begin
    #10 clk = ~clk;
  end
endmodule