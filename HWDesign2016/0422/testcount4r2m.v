module testcount4r2m;
  wire		[7:0] out;
  reg		ck, res;
  initial begin
	$monitor( "%t %b %b %b", $time, ck, res, out);
	ck<=0;
	res<=0;
	#40
	res <= 1;
	#350
	$finish;
  end
  always #10  ck <= ~ck;
  count4r2m  cnt2m ( out, ck, res );
endmodule
