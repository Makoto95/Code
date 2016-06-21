module top;
   reg[7:0] Din;
   wire [7:0] Dout;
   reg ck, rst, Wen, Ren, flag;
   wire Fempty, Ffull;

   fifo fifo( Din, Dout, Wen, Ren, rst, ck, Fempty, Ffull );

   initial begin
	ck = 0;
	rst = 1;
	$dumpfile( "fifo.vcd" );
	$dumpvars;
	#20 rst = 0;
	#50 rst = 1;
	#10000
      $finish;
   end

   always @(negedge ck)begin
     flag = $random;
     if( flag == 1 )begin
	Wen <= 1;
	Ren <= 0;
	Din = $random;
     end else begin
	Wen <= 0;
	Ren <= 1;
     end
   end

   always #10 ck <= ~ck;

endmodule
