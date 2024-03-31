module dff(data, clk, rst, en, q);
  input clk, rst, en;
  input [3:0] data;
  output reg [3:0] q;
  always @(posedge clk or posedge rst)
    begin
      if(rst)
        q <= 4'b0000;
      else if(en)
        q <= data;
    end
endmodule