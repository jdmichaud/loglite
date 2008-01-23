//  loglite library logging_test_multi_file.cpp header file  -----------------//

//  (C) Copyright Jean-Daniel Michaud 2007. Permission to copy, use, modify, 
//  sell and distribute this software is granted provided this copyright notice 
//  appears in all copies. This software is provided "as is" without express or 
//  implied warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org/LICENSE_1_0.txt for licensing.
//  See http://code.google.com/p/loglite/ for library home page.

#include <iostream>
#include <fstream>
#include <logging.hpp>
#include "logging_test_multi_file2.h"

int main()
{
  LOGLITE_INIT((loglite::trace >> loglite::eol)); // log format
  loglite::sink s(&std::cout, LOGLITE_MASK_LEVEL_1);
  s.attach_qualifier(loglite::log);

  loglite::sink file(new std::ofstream("./test_multi_file.log"), LOGLITE_MASK_LEVEL_1);
  file.attach_qualifier(loglite::log);

  LOGLITE_ADD_OUTPUT_STREAM(s);
  LOGLITE_ADD_OUTPUT_STREAM(file);

  LOGLITE_LOG_(1, "Multi file test - call to foo:");
  
  foo();
  return 0;
}
