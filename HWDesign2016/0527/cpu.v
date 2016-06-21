module CPU(CK,RST,IA,ID,DA,DD,RW);
input CK,RST;
input [15:0] ID;
output 	RW;
output [15:0] IA,DA;
inout [15:0]  DD;
reg [15:0] 	 PC, INST, FUA,
		FUB, LSUA, LSUB, FUC, LSUC, PCC, PCI;
reg [15:0] 	 RF[0:14];
reg [1:0] 	 STAGE;
reg 		 FLAG,RW;
wire [15:0] 	 ABUS, BBUS, CBUS;
wire [3:0] 	 OPCODE, OPR1,
		OPR2, OPR3;
wire [7:0] 	 IMM;
wire [15:0] 	 PCn;
assign PCn = PC + 1;

assign OPCODE = INST[15:12];
assign OPR1 = INST[11:8];
assign OPR2 = INST[7:4];
assign OPR3 = INST[3:0];
assign IMM = INST[7:0];
assign ABUS = (OPR2 == 0 ? 0 : RF[OPR2]);
assign BBUS = (OPR3 == 0 ? 0 : RF[OPR3]);
assign DA = LSUB;
assign IA = PC;
assign DD =((RW==0)? LSUA : 16'b Z);
assign CBUS = (OPCODE[3]==0 ? FUC : (OPCODE[3:1]=='b 101 ? LSUC :
		(OPCODE=='b 1100 ? {8'b 0,IMM} : OPCODE=='b 1000 ? PCC : 'b z)));

always @(posedge CK) begin
if( RST == 1 ) begin
	PC <= 0;
	STAGE <= 0;
	RW<=1;
end else if( STAGE == 0 ) begin
	RW <= 1;
	INST <= ID;
	STAGE <= 1;
end else if( STAGE == 1 ) begin
	if( (OPCODE[3:0] == 'b 1000) || (OPCODE[3:0] == 'b 1001 && FLAG == 1 ) ) PCI <= BBUS;
	else PCI <= PCn;
	if( OPCODE[3] == 0 ) begin
      FUA <= ABUS;
      FUB <= BBUS;
	end else if( OPCODE[2:1] == 'b 01) begin
		RW <= OPCODE[0];
		LSUA <= ABUS;
		LSUB <= BBUS;
	end
	STAGE <= 2;
end else if( STAGE == 2 ) begin
	if( OPCODE[3] == 0 ) begin
		case(OPCODE[2:0])
         'b 000: FUC<=FUA+FUB;
			'b 001: FUC<=FUA-FUB;
			'b 010: FUC<=FUA>>FUB;
			'b 011: FUC<=FUA<<FUB;
			'b 100: FUC<=FUA|FUB;
         'b 101: FUC<=FUA&FUB;
			'b 110: FUC<=~FUA;
			'b 111: FUC<=FUA^FUB;
		endcase
	end else if( OPCODE[3:1] == 'b 101 ) begin
		if( OPCODE[0] == 1 ) begin
         RW <= 1;
         LSUC <= DD;
		end else begin
         RW <= 0;
      end
      end else if( OPCODE[3:0] == 'b 1000 ) PCC <= PCn;
	STAGE <= 3;
end else if( STAGE == 3 ) begin
	RW <= 1;
	if( OPCODE[3] == 0 ) begin
		if( CBUS == 0 ) FLAG <= 1;
      else FLAG <= 0;
	end
   RF[OPR1] <= CBUS;
   PC <= PCI;
	STAGE <= 0;
end
end
endmodule
