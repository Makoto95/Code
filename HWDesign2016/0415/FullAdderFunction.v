module FullAdderFunction ( x, y, cin, cout, s );
  input x, y, cin;
  output cout, s;
  assign cout = (x & y ) | ( x & cin ) | (y & cin ) ;
  assign s = x ^ y ^ cin ;
endmodule
