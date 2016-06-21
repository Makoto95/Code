module stm(ck,rst,x);
input	ck, rst;
output	[3:0] x;
reg [3:0] x;
reg	st;
always @(posedge ck) begin
	if( rst == 1 ) begin
		st <= 0;
		x <= 0;
	end else begin
		if( st == 0 ) begin
			if( x == 0 ) st <= 1;
			else x <= x-1;
		end else begin
			if( x == 15 ) st <= 0;
			else x <= x+1;
		end
	end
end
endmodule
