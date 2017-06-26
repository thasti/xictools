//
// Copyright (c) 1999 Steven Wilson (stevew@home.com)
//
//    This source code is free software; you can redistribute it
//    and/or modify it in source code form under the terms of the GNU
//    General Public License as published by the Free Software
//    Foundation; either version 2 of the License, or (at your option)
//    any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
//
//  SDW - Validate and gate vector
//


module main;

reg globvar;

wire [15:0] out;
reg [15:0] a,b, rslt;
reg error;

// The test gate goes HERE!

and foo [15:0] (out,a,b);

always @(a or b)
  rslt = a & b;

initial
  begin  // {
    error = 0;
    # 1;
    for(a = 16'h1; a != 16'hffff; a = (a << 1) | 1)
      begin  // {
        for(b = 16'hffff; b !== 16'h0; b = b >> 1)
          begin  // {
            #1 ;
            if(out !== rslt)
              begin  // {
                 $display("FAILED - GA And a=%x,b=%x,expct=%x - rcvd=%x",
                           a,b,rslt,out);
                 error = 1; 
              end // } 
          end // } 
      end // } 
    if( error == 0)
      $display("PASSED");
  end // } 
endmodule // main