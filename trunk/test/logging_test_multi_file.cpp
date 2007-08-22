//  Boost general library logging_test_multi_file.cpp header file  -----------//

//  (C) Copyright Jean-Daniel Michaud 2007. Permission to copy, use, modify, 
//  sell and distribute this software is granted provided this copyright notice 
//  appears in all copies. This software is provided "as is" without express or 
//  implied warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.
//  See http://www.boost.org/libs/logging/ for library home page.

#include <iostream>
#include <fstream>
#include <logging.hpp>
#include "logging_test_multi_file2.h"

int main()
{
  BOOST_LOG_INIT((boost::logging::trace >> boost::logging::eol)); // log format
  boost::logging::sink s(&std::cout, 1);
  s.attach_qualifier(boost::logging::log);

  BOOST_LOG_ADD_OUTPUT_STREAM(s);
  BOOST_LOG_(1, "Multi file test - call to foo:");
  
  foo();
  return 0;
}
