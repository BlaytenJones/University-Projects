module busdriver(r0_in, r1_in, r0_out, r1_out, clk, rst, y);
  input r0_in, r1_in, r0_out, r1_out, clk, rst;
  output wire [3:0] y;
  wire [3:0] q0, q1;
  dff reg0(y, clk, rst, r0_in, q0);
  dff reg1(y, clk, rst, r1_in, q1);
  tristate buffer0(r0_out, q0, y);
  tristate buffer1(r1_out, q1, y);
endmodule