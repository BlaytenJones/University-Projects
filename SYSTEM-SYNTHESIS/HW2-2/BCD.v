//MADE BY: BLAYTEN JONES
//DATE: 9/24/2023
//FUNCTION: CREATES A BCD COUNTER
//PROJECT 2

module BCD(clk, rst, y);
  input clk;
  input rst;
  output [3:0] y;
  reg [3:0] count;
  
  always @(posedge clk or posedge rst) begin
    if (rst) begin
      count <= 4'b0101;
    end else begin
      if (count == 4'b1001) begin
        count <= 4'b0000; //assumes that it goes to 0
      end else begin
        count <= count + 1;
      end
    end
  end
  
  assign y = count;
  
endmodule