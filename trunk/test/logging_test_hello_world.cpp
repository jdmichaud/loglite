//  loglite library logging_test_hello_world.cpp header file  ----------------//

//  (C) Copyright Jean-Daniel Michaud 2007. Permission to copy, use, modify, 
//  sell and distribute this software is granted provided this copyright notice 
//  appears in all copies. This software is provided "as is" without express or 
//  implied warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org/LICENSE_1_0.txt for licensing.
//  See http://code.google.com/p/loglite/ for library home page.

#include <iostream>
#include <fstream>
#include <logging.hpp>

int main()
{
  LOGLITE_INIT( (loglite::trace >> loglite::eol) ); //log format
   
  loglite::sink sink_cout(&std::cout);
  loglite::sink sink_file(new std::ofstream("./test_hello_world.log"));
  sink_cout.attach_qualifier(loglite::log);
  sink_file.attach_qualifier(loglite::log);
  
  LOGLITE_ADD_OUTPUT_STREAM(sink_cout);
  LOGLITE_ADD_OUTPUT_STREAM(sink_file);

  LOGLITE_LOG_(1, "Hello World!");
  return 0;
}
