`timescale 1ns/1ns

module raceLightControllerTB;
  reg rst;
  reg start;
  reg clk;
  wire red;
  wire yellow;
  wire green;
  
  raceLightController controller(clk, start, rst, red, yellow, green);
  
  initial begin
    start = 0;
    clk = 0;
    rst = 1;
    #10 rst = 0;
    #10 start = 1;
  end
  
  always begin
    #10 clk = ~clk;
  end
endmodule