module test;
   wire [4:0] s;
   reg [3:0]  a, b;
   initial begin
      $dumpfile("test.vcd");
      $dumpvars;
      $monitor ("%t  %b + %b = %b  (%d + %d = %d)", $time, a, b, s, a, b, s );
      a <= 0;
      b <= 0;
      #40  a<= 1; b <= 1;
      #40  a<= 4; b <= 8;
      #40  a<= $random; b <= $random;
      #40  a<= $random; b <= $random;
      #40  a<= $random; b <= $random;
      #40  $finish;
   end // initial begin
   add add(s,a,b);
endmodule // test

module add(s,a,b);
   output [4:0] s;
   input [3:0] 	a, b;
   assign s=a+b;
endmodule // add
