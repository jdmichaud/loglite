//  Boost general library logging_test_macro.cpp file  ------------------------//

//  (C) Copyright Jean-Daniel Michaud 2007. Permission to copy, use, modify, 
//  sell and distribute this software is granted provided this copyright notice 
//  appears in all copies. This software is provided "as is" without express or 
//  implied warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org/LICENSE_1_0.txt for licensing.
//  See http://code.google.com/p/loglite/ for library home page.

#include <fstream>
#include <logging.hpp>

int infinite_loop()
{
  while (1) ;
}

int foo()
{
  BOOST_LOG_(1, "foo called");
  return 7;
}

int main(int argc, char **argv)
{
  BOOST_LOG_INIT(("[" >> boost::logging::level >> "],"
                      >> boost::logging::filename >> "("
                      >> boost::logging::line >> "),"
                      >> boost::logging::time >> ","
                      >> boost::logging::trace
                      >> boost::logging::eol)); // log format


  boost::logging::sink s1(new std::ofstream("./output.log"), 2);
  s1.attach_qualifier(boost::logging::log);
  BOOST_LOG_ADD_OUTPUT_STREAM(s1);

  boost::logging::sink s2(&std::cout, 2);
  s2.attach_qualifier(boost::logging::log);
  BOOST_LOG_ADD_OUTPUT_STREAM(s2);

  BOOST_LOG_(1, "something");
  BOOST_LOG_(2, "something else");
  BOOST_LOG_(3, "If you evaluate me you die!" << infinite_loop());
  char you_want[256] = "you want";
  BOOST_LOG_(1, "Let's say " << you_want << " to display " << 2);
  BOOST_LOG_(1, "foo will be evaluated: " << foo());
  return 0;
}
