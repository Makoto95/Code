module testsub4;
  wire		[4:0] s;
  reg		[3:0] a, b;
  initial begin
	$monitor( "%t %b - %b = %b", $time, b, a, s);
	a <= 0;   b <= 0;
	#40	a <= 1;   b <= 3;
	#40	a <= 4;   b <= 8;
	#40	a <= $random;   b <= $random;
	#40	a <= $random;   b <= $random;
	#40
	$finish;
  end
  sub4 sub ( s,a,b );
endmodule
