
module counter ( vals, y );
  input [7:0] vals;
  output [3:0] y;
  wire   n15, n16, n17, n18, n19, n20, n21, n22, n23, n24, n25, n26, n27, n28,
         n29;

  NOR2X0 U19 ( .IN1(n15), .IN2(n16), .QN(y[3]) );
  XOR2X1 U20 ( .IN1(n16), .IN2(n15), .Q(y[2]) );
  AOI21X1 U21 ( .IN1(n17), .IN2(n18), .IN3(n19), .QN(n15) );
  NAND2X0 U22 ( .IN1(n20), .IN2(n21), .QN(n16) );
  XOR2X1 U23 ( .IN1(n20), .IN2(n21), .Q(y[1]) );
  AND2X1 U24 ( .IN1(n22), .IN2(n23), .Q(n21) );
  AOI21X1 U25 ( .IN1(n24), .IN2(n25), .IN3(n19), .QN(n20) );
  NOR2X0 U26 ( .IN1(n25), .IN2(n24), .QN(n19) );
  XNOR2X1 U27 ( .IN1(n18), .IN2(n17), .Q(n25) );
  AO22X1 U28 ( .IN1(vals[3]), .IN2(vals[4]), .IN3(vals[5]), .IN4(n26), .Q(n17)
         );
  AO22X1 U29 ( .IN1(vals[0]), .IN2(vals[1]), .IN3(vals[2]), .IN4(n27), .Q(n18)
         );
  AOI22X1 U30 ( .IN1(vals[6]), .IN2(vals[7]), .IN3(n28), .IN4(n29), .QN(n24)
         );
  XOR2X1 U31 ( .IN1(n23), .IN2(n22), .Q(y[0]) );
  XOR2X1 U32 ( .IN1(n29), .IN2(n28), .Q(n22) );
  XOR2X1 U33 ( .IN1(vals[6]), .IN2(vals[7]), .Q(n28) );
  XOR2X1 U34 ( .IN1(vals[2]), .IN2(n27), .Q(n29) );
  XOR2X1 U35 ( .IN1(vals[0]), .IN2(vals[1]), .Q(n27) );
  XOR2X1 U36 ( .IN1(vals[5]), .IN2(n26), .Q(n23) );
  XOR2X1 U37 ( .IN1(vals[3]), .IN2(vals[4]), .Q(n26) );
endmodule

