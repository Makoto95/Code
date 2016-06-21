module simstm;
reg ck, rst;
wire [3:0] x;
initial begin
	$dumpvars;
	$dumpfile("stm.vcd");
	$monitor( "ck = %b, st = %b: x = %x", s.ck, s.st, s.x );
	ck=0; rst=0;
	#20 rst=1;
	#60 rst=0;
	#800
	  if( x == 7 )  $display("OK\n");
   $finish;
end
always #10	ck<=~ck;

stm	s(ck,rst,x);

endmodule
