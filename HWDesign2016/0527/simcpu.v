module simcpu;
reg CK, RST;
wire RW;
wire [15:0] IA, DA, DD;
reg [15:0] ID, DDi;
reg [15:0] IMEM [0:127], DMEM[0:127];

	CPU c(CK,RST,IA,ID,DA,DD,RW);

   assign DD = ((RW == 1) ? DDi : 'b Z);

initial begin
	$dumpfile("cpu.vcd");
	$dumpvars;
	CK = 0;
	RST = 0;
#5	RST = 1;
#100	RST = 0;

        #10000 $finish;
end

always @(negedge CK) begin
	ID = IMEM[IA];
end

always @(negedge CK) begin
   if( RW == 1 ) DDi = DMEM[DA];
	else DMEM[DA] = DD;
end

initial begin
   IMEM[0]=	'b 1100_0001_0000_0000;
   IMEM[1]=	'b 1100_0010_0000_0001;
   IMEM[2]=	'b 1100_0011_0000_0001;
   IMEM[3]=	'b 1100_0100_0000_1010;
   IMEM[4]=	'b 1100_0101_0000_1100;
   IMEM[5]=	'b 1100_0110_0000_0111;
   IMEM[6]=	'b 1100_0111_0000_0000;
	IMEM[7] = 'b 0000_0001_0001_0010;
	IMEM[8] = 'b 0000_0010_0010_0011;
	IMEM[9] = 'b 0001_0100_0100_0011;
	IMEM[10] = 'b 1001_0000_0000_0101;
	IMEM[11] = 'b 1000_0000_0000_0110;
	IMEM[12] = 'b 1010_0000_0001_0111;
end

always #10 CK = ~CK;

endmodule
