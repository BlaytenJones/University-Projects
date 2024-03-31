
module raceLightController ( clk, start, rst, red, yellow, green );
  input clk, start, rst;
  output red, yellow, green;
  wire   n5, n20, n21, n22, n23, n24, n25, n26, n27, n28, n29, n30, n31, n32;
  wire   [3:0] pres_state;
  wire   [3:0] next_state;

  DFFNARX1 \pres_state_reg[3]  ( .D(next_state[3]), .CLK(clk), .RSTB(n5), .Q(
        pres_state[3]), .QN(n22) );
  DFFNARX1 \pres_state_reg[0]  ( .D(next_state[0]), .CLK(clk), .RSTB(n5), .Q(
        pres_state[0]), .QN(n21) );
  DFFNARX1 \pres_state_reg[1]  ( .D(next_state[1]), .CLK(clk), .RSTB(n5), .Q(
        pres_state[1]), .QN(n20) );
  DFFNARX1 \pres_state_reg[2]  ( .D(next_state[2]), .CLK(clk), .RSTB(n5), .Q(
        pres_state[2]), .QN(n23) );
  ISOLANDX1 U24 ( .D(n24), .ISO(n25), .Q(yellow) );
  OA21X1 U25 ( .IN1(n26), .IN2(n24), .IN3(n27), .Q(red) );
  NAND2X0 U26 ( .IN1(n22), .IN2(n28), .QN(next_state[3]) );
  NAND3X0 U27 ( .IN1(pres_state[0]), .IN2(pres_state[2]), .IN3(pres_state[1]), 
        .QN(n28) );
  MUX21X1 U28 ( .IN1(pres_state[2]), .IN2(n25), .S(n29), .Q(next_state[2]) );
  NOR2X0 U29 ( .IN1(n21), .IN2(n20), .QN(n29) );
  NOR2X0 U30 ( .IN1(n30), .IN2(n31), .QN(next_state[1]) );
  XNOR2X1 U31 ( .IN1(pres_state[0]), .IN2(pres_state[1]), .Q(n31) );
  AO21X1 U32 ( .IN1(n32), .IN2(n21), .IN3(n30), .Q(next_state[0]) );
  INVX0 U33 ( .IN(n26), .QN(n30) );
  NAND2X0 U34 ( .IN1(pres_state[3]), .IN2(pres_state[2]), .QN(n26) );
  NAND3X0 U35 ( .IN1(n27), .IN2(n20), .IN3(start), .QN(n32) );
  OA21X1 U36 ( .IN1(n22), .IN2(pres_state[2]), .IN3(n25), .Q(n27) );
  NAND2X0 U37 ( .IN1(pres_state[2]), .IN2(n22), .QN(n25) );
  INVX0 U38 ( .IN(rst), .QN(n5) );
  AND3X1 U39 ( .IN1(pres_state[3]), .IN2(n23), .IN3(n24), .Q(green) );
  NAND2X0 U40 ( .IN1(n20), .IN2(n21), .QN(n24) );
endmodule

