module  alutest;
reg	[7:0]	A, B;
reg	[3:0]	CTR;
reg	ck;
wire	[7:0] O0;
initial begin
	ck=0;
	$dumpfile("alu.vcd");
	$dumpvars;
	$monitor( "%t\tA=%d, B=%d, CTR=%b, OUT=%d", $time, A, B, CTR, O );
   #1000
     $display( "OK\n" );
   $finish;
end
alu	ALU(A , B , O0 , CTR , ck);
always	#10	ck = ~ck;
always @(negedge ck) begin
	A = $random;
	B = $random;
	CTR  = $random;
end

   reg	[7:0]	INA, INB, OUT, O;
reg	[3:0]	C, CC;

always @(negedge ck) begin
   if( !(O == O0 || ( (CC == 'b0010 || CC== 'b0011 || CC == 'b0100 || CC == 'b0101 || CC=='b0110 || CC == 'b0111) && O0 == 8'b0)) ) $finish;
end



always @(posedge ck) begin
	C <= CTR;
   CC <= C;
	INA <= A;
	INB <= B;
	O <= OUT;
end

always @(C or INA or INB) begin
	case  (C)
	'b0000 : OUT <= INA + INB;
	'b0001 : OUT <= INA - INB;
	'b1000 : OUT <= INA & INB;
	'b1001 : OUT <= INA | INB;
	'b1010 : OUT <= INA ^ INB;
	'b1011 : OUT <= ~INA;
	'b1100 : OUT <= INA>>1;
	'b1110 : OUT <= {INA[0], INA[7:1]};
	'b1111 : OUT <= {INA[6:0], INA[7]};
//	  default : OUT <= 0;
	endcase
end


endmodule
