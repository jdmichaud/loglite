//  Boost general library logging_test_macro.cpp file  ------------------------//

//  (C) Copyright Jean-Daniel Michaud 2007. Permission to copy, use, modify, 
//  sell and distribute this software is granted provided this copyright notice 
//  appears in all copies. This software is provided "as is" without express or 
//  implied warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org/LICENSE_1_0.txt for licensing.
//  See http://code.google.com/p/loglite/ for library home page.

#include <fstream>
#include <logging.hpp>

namespace bl = boost::logging;

int main(int argc, char **argv)
{
  BOOST_LOG_INIT(("[" >> bl::mask >> "],"
                      >> bl::filename >> "("
                      >> bl::line >> "),"
                      >> (*new bl::date_element("%d/%m/%Y")) >> ","
                      >> bl::trace
                      >> bl::eol)); // log format

  bl::sink s1(&std::cout, BOOST_LOG_MASK_LEVEL_2);
  s1.attach_qualifier(bl::log);
  BOOST_LOG_ADD_OUTPUT_STREAM(s1);

  BOOST_LOG_(BOOST_LOG_LEVEL_1, "Hello, I'm H.G. Pennypacker");
  BOOST_LOG_(BOOST_LOG_LEVEL_2, "wealthy american industrialist");
  BOOST_LOG_(BOOST_LOG_LEVEL_2, "philantropist and ...");
  BOOST_LOG_(BOOST_LOG_LEVEL_1, "bicyclist!");

  return 0;
}
