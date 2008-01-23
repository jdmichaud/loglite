//  loglite library logging_test_macro.cpp file  -----------------------------//

//  (C) Copyright Jean-Daniel Michaud 2007. Permission to copy, use, modify, 
//  sell and distribute this software is granted provided this copyright notice 
//  appears in all copies. This software is provided "as is" without express or 
//  implied warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org/LICENSE_1_0.txt for licensing.
//  See http://code.google.com/p/loglite/ for library home page.

#include <fstream>
#include <logging.hpp>

namespace l = loglite;

int main(int argc, char **argv)
{
  LOGLITE_INIT((" L" >> l::mask >> ","
                     >> (*new l::filename_element(l::filename_element::filename_only)) >> "("
                     >> l::line >> "),"
                     >> (*new l::date_element("%d/%m/%Y")) >> " "
                     >> (*new l::time_element("%H:%M:%S%F")) >> ","
                     >> l::trace
                     >> l::eol)); // log format

  l::sink s1(&std::cout, LOGLITE_MASK_LEVEL_2);
  s1.attach_qualifier(l::log);
  LOGLITE_ADD_OUTPUT_STREAM(s1);

  l::sink s2(new std::ofstream("./test_format.log"), LOGLITE_MASK_LEVEL_2);
  s2.attach_qualifier(l::log);
  LOGLITE_ADD_OUTPUT_STREAM(s2);

  LOGLITE_LOG_(LOGLITE_LEVEL_1, "Hello, I'm H.G. Pennypacker");
  LOGLITE_LOG_(LOGLITE_LEVEL_2, "wealthy american industrialist");
  LOGLITE_LOG_(LOGLITE_LEVEL_2, "philantropist and ...");
  LOGLITE_LOG_(LOGLITE_LEVEL_1, "bicyclist!");

  return 0;
}
