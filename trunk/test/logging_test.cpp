//  loglite library logging_test.cpp header file  ----------------------------//

//  (C) Copyright Jean-Daniel Michaud 2007. Permission to copy, use, modify, 
//  sell and distribute this software is granted provided this copyright notice 
//  appears in all copies. This software is provided "as is" without express or 
//  implied warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org/LICENSE_1_0.txt for licensing.
//  See http://code.google.com/p/loglite/ for library home page.

#include <iostream>
#include <fstream>
#include <boost/shared_ptr.hpp>
#include <logging.hpp>

loglite::logger lgr;

int main(int argc, char **argv)
{
  lgr.add_format("[" >> loglite::mask >> "],"
		 >> loglite::filename >> "("
		 >> loglite::line >> "),"
		 >> loglite::time >> ","
		 >> loglite::trace
		 >> loglite::eol);
  
  loglite::sink sink_cout(&std::cout);
  loglite::sink sink_file(new std::ofstream("./test.log"));

  sink_cout.attach_qualifier(loglite::log);
  sink_file.attach_qualifier(loglite::log);

  lgr.add_sink(sink_cout);
  lgr.add_sink(sink_file);
 
  lgr.trace(1, loglite::log, "something", __FILE__, __LINE__);
  lgr.trace(1, loglite::log, "something else", __FILE__, __LINE__);
  return 0;
}
