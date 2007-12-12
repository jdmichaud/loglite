//  Boost general library logging_test.cpp header file  ----------------------//

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

boost::logging::logger lgr;

int main(int argc, char **argv)
{
  lgr.add_format("[" >> boost::logging::mask >> "],"
		 >> boost::logging::filename >> "("
		 >> boost::logging::line >> "),"
		 >> boost::logging::time >> ","
		 >> boost::logging::trace
		 >> boost::logging::eol);
  
  boost::logging::sink sink_cout(&std::cout);
  boost::logging::sink sink_file(new std::ofstream("./output.log"));

  sink_cout.attach_qualifier(boost::logging::log);
  sink_file.attach_qualifier(boost::logging::log);

  lgr.add_sink(sink_cout);
  lgr.add_sink(sink_file);
 
  lgr.trace(1, boost::logging::log, "something", __FILE__, __LINE__);
  lgr.trace(1, boost::logging::log, "something else", __FILE__, __LINE__);
  return 0;
}
