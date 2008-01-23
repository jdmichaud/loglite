//  loglite library logging_test_macro.cpp file  -----------------------------//

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
  LOGLITE_LOG_L1("oops...");
  while (1) ;
}

int foo()
{
  LOGLITE_LOG_(1, "foo called");
  return 7;
}

int main(int argc, char **argv)
{
  LOGLITE_INIT(("[" >> loglite::mask >> "],"
                    >> loglite::filename >> "("
                    >> loglite::line >> "),"
                    >> loglite::time >> ","
                    >> loglite::trace
                    >> loglite::eol)); // log format

  loglite::sink s1(new std::ofstream("./test_macro.log"), LOGLITE_MASK_LEVEL_2);
  s1.attach_qualifier(loglite::log);
  LOGLITE_ADD_OUTPUT_STREAM(s1);

  loglite::sink s2(&std::cout, LOGLITE_MASK_LEVEL_2);
  s2.attach_qualifier(loglite::log);
  LOGLITE_ADD_OUTPUT_STREAM(s2);

  LOGLITE_LOG(LOGLITE_LEVEL_1, loglite::log, "something");
  LOGLITE_LOG_L2("something else"); // A log of level 2 (L2)
  LOGLITE_LOG_(LOGLITE_LEVEL_3, "If you evaluate me you die!" << infinite_loop());
  char you_want[256] = "you want";
  LOGLITE_LOG_(LOGLITE_LEVEL_1, "Let's say " << you_want << " to display " << 2);
  LOGLITE_LOG_(LOGLITE_LEVEL_1, "foo will be evaluated: " << foo());
  return 0;
}
