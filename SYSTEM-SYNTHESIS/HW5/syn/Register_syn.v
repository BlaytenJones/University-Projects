
module dff ( data, clk, rst, en, q );
  input [3:0] data;
  output [3:0] q;
  input clk, rst, en;
  wire   n5, n10, n11, n12, n13;

  DFFARX2 \q_reg[3]  ( .D(n13), .CLK(clk), .RSTB(n5), .Q(q[3]) );
  DFFARX2 \q_reg[2]  ( .D(n12), .CLK(clk), .RSTB(n5), .Q(q[2]) );
  DFFARX2 \q_reg[1]  ( .D(n11), .CLK(clk), .RSTB(n5), .Q(q[1]) );
  DFFARX2 \q_reg[0]  ( .D(n10), .CLK(clk), .RSTB(n5), .Q(q[0]) );
  INVX4 U8 ( .IN(rst), .QN(n5) );
  MUX21X1 U9 ( .IN1(q[3]), .IN2(data[3]), .S(en), .Q(n13) );
  MUX21X1 U10 ( .IN1(q[2]), .IN2(data[2]), .S(en), .Q(n12) );
  MUX21X1 U11 ( .IN1(q[1]), .IN2(data[1]), .S(en), .Q(n11) );
  MUX21X1 U12 ( .IN1(q[0]), .IN2(data[0]), .S(en), .Q(n10) );
endmodule

