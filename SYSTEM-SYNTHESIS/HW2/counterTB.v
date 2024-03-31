module stimulusCounter;
  reg [7:0] vals;
  wire [3:0] y;
  
  counter count(vals, y);
  
  initial begin
    vals = 8'b00000000;
    while (vals != 8'b11111111) begin
        #10 vals = vals + 1'b1;
    end
  end
endmodule