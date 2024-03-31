module tristate(en, a, y);
  input [3:0] a;
  input en;
  output [3:0] y;
  assign y = en ? a : 4'bzzzz;
endmodule