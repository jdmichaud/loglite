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
  boost::logging::scope_item bar_scope("bar()");
  BOOST_LOG_L1("Doing some processing in bar");
}

int foo(int i)
{
  boost::logging::scope_item foo_bar("foo()");
  BOOST_LOG_L1("Doing some processing in foo");

  switch (i)
  {
  case 0:
  {
    boost::logging::scope_item case1("case 0");
    break;
  }
  case 1:
  {
    boost::logging::scope_item case1("case 1");
    break;
  }
  default:
  {
    boost::logging::scope_item case1("default");
  }
  }

  bar();
}

int macro_bar()
{
  BOOST_LOG_SCOPE(BOOST_LOG_LEVEL_1, bar_scope);
  BOOST_LOG_L1("Doing some processing in macro_bar");
}

int macro_foo()
{
  BOOST_LOG_SCOPE(BOOST_LOG_LEVEL_1, foo_scope);
  BOOST_LOG_L1("Doing some processing in macro_foo");

  macro_bar();
}

int main()
{
  boost::logging::logger_p l = boost::logging::logger::get_instance();

  boost::logging::format display_format(boost::logging::trace >> boost::logging::eol);
  boost::logging::format file_format("[" >> boost::logging::mask >> "],"
                                         >> boost::logging::filename >> "("
                                         >> boost::logging::line >> "),"
                                         >> boost::logging::time >> ", "
                                         >> boost::logging::scope_stack >> ", "
                                         >> boost::logging::trace
                                         >> boost::logging::eol); // log format

  l->add_format(display_format);
  l->add_format(file_format);
   
  boost::logging::sink sink_cout(&std::cout);
  boost::logging::sink sink_file(new std::ofstream("./output.log"));
  sink_cout.attach_qualifier(boost::logging::log);
  sink_file.attach_qualifier(boost::logging::log);
  sink_file.attach_qualifier(boost::logging::scope);

  l->add_sink(sink_cout, display_format);
  l->add_sink(sink_file, file_format);

  BOOST_LOG_L1("No stack here");

  foo(2);
  bar();

  macro_foo();
  macro_bar();
  return 0;
}
