//  Boost general library logging_test.cpp header file  ----------------------//

//  (C) Copyright Jean-Daniel Michaud 2007. Permission to copy, use, modify, 
//  sell and distribute this software is granted provided this copyright notice 
//  appears in all copies. This software is provided "as is" without express or 
//  implied warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.
//  See http://www.boost.org/libs/logging/ for library home page.

#include <iostream>
#include <fstream>
#include <boost/shared_ptr.hpp>
#include <logging.hpp>

int main(int argc, char **argv)
{
  boost::logging::logger lgr;

  lgr.add_format("[" >> boost::logging::level >> "],"
		 >> boost::logging::filename >> "("
		 >> boost::logging::line >> "),"
		 >> boost::logging::time >> ","
		 >> boost::logging::trace
		 >> boost::logging::eol);
  
  lgr.add_sink(&std::cout);
  lgr.add_sink(new std::ofstream("./output.log"));

  lgr.trace(1, "something", __FILE__, __LINE__);
  lgr.trace(1, "something else", __FILE__, __LINE__);
  return 0;
}
