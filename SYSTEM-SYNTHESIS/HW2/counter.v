//MADE BY: BLAYTEN JONES
//DATE: 9/23/2023
//FUNCTION: CREATES A 1'S COUNTER
//PROJECT 2

module counter(vals, y);
  input [7:0] vals;
  output [3:0] y;
  
  assign y = vals[0] + vals[1] + vals[2] + vals[3] + vals[4] + vals[5] + vals[6] + vals[7];
endmodule