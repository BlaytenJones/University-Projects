
module BCD ( clk, rst, y );
  output [3:0] y;
  input clk, rst;
  wire   N9, N10, N11, n17, n7, n13, n14, n16, n18, n19, n20, n21;

  DFFARX1 \count_reg[3]  ( .D(N11), .CLK(clk), .RSTB(n7), .Q(y[3]), .QN(n14)
         );
  DFFASX1 \count_reg[2]  ( .D(N10), .CLK(clk), .SETB(n7), .Q(y[2]), .QN(n13)
         );
  DFFASX1 \count_reg[0]  ( .D(n17), .CLK(clk), .SETB(n7), .Q(y[0]), .QN(n17)
         );
  DFFARX1 \count_reg[1]  ( .D(N9), .CLK(clk), .RSTB(n7), .Q(y[1]) );
  INVX0 U12 ( .IN(rst), .QN(n7) );
  OA21X1 U13 ( .IN1(n16), .IN2(y[1]), .IN3(n18), .Q(N9) );
  OA21X1 U14 ( .IN1(y[2]), .IN2(n14), .IN3(y[0]), .Q(n16) );
  MUX21X1 U15 ( .IN1(n19), .IN2(n20), .S(n14), .Q(N11) );
  NOR2X0 U16 ( .IN1(n18), .IN2(n13), .QN(n20) );
  NAND2X0 U17 ( .IN1(n21), .IN2(y[0]), .QN(n19) );
  XOR2X1 U18 ( .IN1(y[1]), .IN2(n13), .Q(n21) );
  XOR2X1 U19 ( .IN1(n13), .IN2(n18), .Q(N10) );
  NAND2X0 U20 ( .IN1(y[1]), .IN2(y[0]), .QN(n18) );
endmodule

