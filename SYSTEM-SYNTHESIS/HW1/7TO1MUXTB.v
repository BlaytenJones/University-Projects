module stimulus;
  reg a, b, c, d, e, f, g;
  reg[2:0] select;
  wire y;
  mux7to1 mux(a, b, c, d, e, f, g, select, y);
  initial begin
    select = 3'b000;
    a = 0; b = 1; c = 1; d = 0; e = 1; f = 0; g = 0;
    #10 select = 3'b001;
    #10 select = 3'b010;
    #10 select = 3'b011;
    #10 select = 3'b100;
    #10 select = 3'b101;
    #10 select = 3'b110;
    #10 select = 3'b111;
  end
endmodule