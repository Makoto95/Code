module simFullAdderStructure;
wire	cout, s ;
reg		x, y, cin;
reg		ck;
initial begin
$monitor( "%t %b + %b + %b = %b%b", $time, cin, x, y, cout, s);
$dumpfile("simFullAdderStructure.vcd");
$dumpvars;
x <= 0;
y <= 0;
cin <= 0;
ck <= 0;
end
always  #10 ck <= ~ck;
always @(negedge ck) begin
if( {cin,x,y} == 7 ) begin
$finish;
end
end
always @(posedge ck) begin
{cin, x, y} <= {cin, x, y} + 1;
end
FullAdderStructure FAS ( x, y, cin, cout, s );
endmodule

//hwdesign2016@vdec.u-tokyo.ac.jp
//subject HWdesign20160415
