
module tristate ( en, a, y );
  input [3:0] a;
  output [3:0] y;
  input en;

  tri   [3:0] y;

  TNBUFFHX4 \y_tri[0]  ( .IN(a[0]), .ENB(en), .Q(y[0]) );
  TNBUFFHX4 \y_tri[1]  ( .IN(a[1]), .ENB(en), .Q(y[1]) );
  TNBUFFHX4 \y_tri[2]  ( .IN(a[2]), .ENB(en), .Q(y[2]) );
  TNBUFFHX4 \y_tri[3]  ( .IN(a[3]), .ENB(en), .Q(y[3]) );
endmodule

