module  alu(A,B,O,CTR,ck);
input	[7:0]	A, B;
input	[3:0]	CTR;
input	ck;
output	[7:0] O;
reg	[7:0]	INA, INB, OUT, O;
reg	[3:0]	C;

always @(posedge ck) begin
	C <= CTR;
	INA <= A;
	INB <= B;
	case  (C)
	'b0000 : O <= INA + INB;
	'b0001 : O <= INA - INB;
	'b1000 : O <= INA & INB;
	'b1001 : O <= INA | INB;
	'b1010 : O <= INA ^ INB;
	'b1011 : O <= ~INA;
	'b1100 : O <= INA>>1;
	'b1101 : O <= INA<<1;
	'b1110 : O <= {INA[0], INA[7:1]};
	'b1111 : O <= {INA[6:0], INA[7]};
	endcase
end

endmodule
