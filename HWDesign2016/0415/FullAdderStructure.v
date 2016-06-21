module FullAdderStructure ( x, y, cin, cout, s );
  input x, y, cin;
  output cout, s;
  wire w1, w2, w3, w4, w5;

  and a1 ( w1, x, y );
  and a2 ( w2, y, cin );
  and a3 ( w3, cin, x );
  or o1 ( w4, w1, w2 );
  or o2 ( cout, w4, w3 );
  xor x1 ( w5, x, y );
  xor x2 ( s, w5, cin );
endmodule
