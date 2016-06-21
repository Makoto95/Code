module testadd4;
  wire		[4:0] s;
  reg		[3:0] a, b;
  initial begin
	$monitor( "%t %b + %b = %b", $time, a, b, s);
	a <= 0;   b <= 0;
	#40	a <= 1;   b <= 3;
	#40	a <= 4;   b <= 8;
	#40	a <= $random;   b <= $random;
	#40	a <= $random;   b <= $random;
	#40
	$finish;
  end
  add4 add ( s,a,b );
endmodule
