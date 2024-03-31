module raceLightController(clk, start, rst, red, yellow, green);
  input clk, start, rst;
  output reg red, yellow, green;
  reg[3:0] pres_state, next_state;
  parameter init = 4'b0000, r1 = 4'b0001, r2 = 4'b0010, r3 = 4'b0011, dt1 = 4'b0100, y1 = 4'b0101, y2 = 4'b0110,
  y3 = 4'b0111, dt2 = 4'b1000, g1 = 4'b1001, g2 = 4'b1010, g3 = 4'b1011, dt3 = 4'b1100, final = 4'b1101;
  
  always @(negedge clk or posedge rst) begin
    if(rst) begin
      pres_state <= init;
    end else begin
      pres_state <= next_state;
    end
  end
  
  always @(pres_state or start) begin
    red <= 1'b0; yellow <= 1'b0; green <= 1'b0;
    case(pres_state)
      init: begin
        if(start) begin
          next_state <= init;
        end else begin
          next_state <= r1;
        end
        red <= 1'b1; yellow <= 1'b0; green <= 1'b0;
      end
      r1: begin
        next_state <= r2;
        red <= 1'b1; yellow <= 1'b0; green <= 1'b0;
      end
      r2: begin
        next_state <= r3;
        red <= 1'b1; yellow <= 1'b0; green <= 1'b0;
      end
      r3: begin
        next_state <= dt1;
        red <= 1'b1; yellow <= 1'b0; green <= 1'b0;
      end
      dt1: begin
        next_state <= y1;
        red <= 1'b0; yellow <= 1'b0; green <= 1'b0;
      end
      y1: begin
        next_state <= y2;
        red <= 1'b0; yellow <= 1'b1; green <= 1'b0;
      end
      y2: begin
        next_state <= y3;
        red <= 1'b0; yellow <= 1'b1; green <= 1'b0;
      end
      y3: begin
        next_state <= dt2;
        red <= 1'b0; yellow <= 1'b1; green <= 1'b0;
      end
      dt2: begin
        next_state <= g1;
        red <= 1'b0; yellow <= 1'b0; green <= 1'b0;
      end
      g1: begin
        next_state <= g2;
        red <= 1'b0; yellow <= 1'b0; green <= 1'b1;
      end
      g2: begin
        next_state <= g3;
        red <= 1'b0; yellow <= 1'b0; green <= 1'b1;
      end
      g3: begin
        next_state <= dt3;
        red <= 1'b0; yellow <= 1'b0; green <= 1'b1;
      end
      dt3: begin
        next_state <= final;
        red <= 1'b0; yellow <= 1'b0; green <= 1'b0;
      end
      final: begin
        next_state <= final;
        red <= 1'b1; yellow <= 1'b0; green <= 1'b0;
      end
      default: begin
        next_state <= final;
        red <= 1'b1; yellow <= 1'b0; green <= 1'b0;
      end
    endcase 
  end
endmodule