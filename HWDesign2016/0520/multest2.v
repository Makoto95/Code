module  multest;
reg	[7:0]	A, B;
reg	ck;
reg	start;
reg	[3:0] st;
wire	[16:0] O;
reg	[16:0] OR;
initial begin
	ck=0;
	start=0;
	st=0;
	$dumpfile("mul.vcd");
	$dumpvars;
	$monitor( "%t\tA=%h, B=%h, (OUT=%h) OUT=%h", $time, A, B, O, OR );
	A=0;
	B=0;
end
mul	MUL(A , B , O ,  ck, start, fin);
always	#10	ck = ~ck;
always @(negedge ck) begin
	if( st == 0 ) begin
		start <= 1;
	end
	else start <= 0;
	if( fin == 1 ) begin
	    OR <= O;
	    {B,A} <= {B,A} + 1;
	   st <= 0;
            if( O == A*B ) begin
            end
            if( A == 'h f && B == 'h f ) begin
               $finish;
            end
	end else  st <= st+1;
end
endmodule
