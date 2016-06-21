module  alutest;
reg	[7:0]	A, B;
reg	[3:0]	CTR;
reg	ck;
wire	[7:0] O;
initial begin
	ck=0;
	$dumpfile("alu.vcd");
	$dumpvars;
	$monitor( "%t\tA=%d, B=%d, CTR=%b, OUT=%d", $time, A, B, CTR, O );
	#400  $finish;
end
alu alu(A , B , O , CTR , ck);
always	#40	ck <= ~ck;
always @(negedge ck) begin
	A <= $random;
	B <= $random;
	CTR  <= 0;
end
endmodule
