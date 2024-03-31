//MADE BY: BLAYTEN JONES
//DATE: 9/18/2023
//FUNCTION: CREATES A 7-TO-1 MULTIPLEXER
//PROJECT 1

module mux7to1 (a, b, c, d, e, f, g, select, y);
output y;
input a, b, c, d, e, f, g;
input [2:0] select;
reg y;

always @(select or a or b or c or d or e or f or g)
case(select)
  3'b000: y = a;
  3'b001: y = b;
  3'b010: y = c;
  3'b011: y = d;
  3'b100: y = e;
  3'b101: y = f;
  3'b110: y = g;
  default: y = 0;
endcase
endmodule
