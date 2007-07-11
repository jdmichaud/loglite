//  Boost general library logging.hpp header file  ---------------------------//

//  (C) Copyright Jean-Daniel Michaud 2007. Permission to copy, use, modify, 
//  sell and distribute this software is granted provided this copyright notice 
//  appears in all copies. This software is provided "as is" without express or 
//  implied warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.
//  See http://www.boost.org/libs/logging/ for library home page.

#include <fstream>
#include <boost/logging.hpp>

int main(int argc, char **argv)
{
  BOOST_LOG_INIT(("[" >> boost::logging::level >> "],"
                      >> boost::logging::filename >> "("
                      >> boost::logging::line >> "),"
                      >> boost::logging::time >> ","
                      >> boost::logging::trace
                      >> boost::logging::eol), // log format
                      2);                      // log level


  BOOST_LOG_ADD_OUTPUT_STREAM(new std::ofstream("./output.log"));
  BOOST_LOG_ADD_OUTPUT_STREAM(&std::cout);
  BOOST_LOG(1, "something");
  BOOST_LOG(2, "something else");
  BOOST_LOG(3, "doesn't matter");
  char you_want[256] = "you want";
  BOOST_LOG(1, "Let's say " << you_want << " to display " << 2);
  return 0;
}
