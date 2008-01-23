//  Boost general library logging_test_hello_world.cpp header file  ----------//

//  (C) Copyright Jean-Daniel Michaud 2007. Permission to copy, use, modify, 
//  sell and distribute this software is granted provided this copyright notice 
//  appears in all copies. This software is provided "as is" without express or 
//  implied warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org/LICENSE_1_0.txt for licensing.
//  See http://code.google.com/p/loglite/ for library home page.

#include <iostream>
#include <fstream>
#include <logging.hpp>

int bar()
{
  loglite::scope_item bar_scope("bar()");
  LOGLITE_LOG_L1("Doing some processing in bar");
}

int foo(int i)
{
  loglite::scope_item foo_bar("foo()");
  LOGLITE_LOG_L1("Doing some processing in foo");

  switch (i)
  {
  case 0:
  {
    loglite::scope_item case1("case 0");
    break;
  }
  case 1:
  {
    loglite::scope_item case1("case 1");
    break;
  }
  default:
  {
    loglite::scope_item case1("default");
  }
  }

  bar();
}

int macro_bar()
{
  LOGLITE_LOG_SCOPE(LOGLITE_LEVEL_1, bar_scope);
  LOGLITE_LOG_L1("Doing some processing in macro_bar");
}

int macro_foo()
{
  LOGLITE_LOG_SCOPE(LOGLITE_LEVEL_1, foo_scope);
  LOGLITE_LOG_L1("Doing some processing in macro_foo");

  macro_bar();
}

int main()
{
  loglite::logger_p l = loglite::logger::get_instance();

  loglite::format display_format(loglite::trace >> loglite::eol);
  loglite::format file_format("[" >> loglite::mask >> "],"
                                         >> loglite::filename >> "("
                                         >> loglite::line >> "),"
                                         >> loglite::time >> ", "
                                         >> loglite::scope_stack >> ", "
                                         >> loglite::trace
                                         >> loglite::eol); // log format

  l->add_format(display_format);
  l->add_format(file_format);
   
  loglite::sink sink_cout(&std::cout);
  loglite::sink sink_file(new std::ofstream("./test_scope.log"));
  sink_cout.attach_qualifier(loglite::log);
  sink_file.attach_qualifier(loglite::log);
  sink_file.attach_qualifier(loglite::scope);

  l->add_sink(sink_cout, display_format);
  l->add_sink(sink_file, file_format);

  LOGLITE_LOG_L1("No stack here");

  foo(2);
  bar();

  macro_foo();
  macro_bar();
  return 0;
}
