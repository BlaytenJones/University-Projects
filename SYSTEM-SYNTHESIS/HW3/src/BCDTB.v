`timescale 1ns/1ns

module BCDTB;
  reg clk;
  reg rst;
  wire [3:0] y;
  
  BCD counter(clk, rst, y);
  
  initial begin
    clk = 0;
    rst = 1;
    #10 rst = 0;
  end
  
  always begin
    #10 clk = ~clk;
  end
endmodule