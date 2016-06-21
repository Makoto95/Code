module testmul4;
  wire		[7:0] s;
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
  mul4 mul ( s,a,b );
endmodule
