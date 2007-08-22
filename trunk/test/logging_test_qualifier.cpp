//  Boost general library logging_qualifier.cpp file  ------------------------//

//  (C) Copyright Jean-Daniel Michaud 2007. Permission to copy, use, modify, 
//  sell and distribute this software is granted provided this copyright notice 
//  appears in all copies. This software is provided "as is" without express or 
//  implied warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org/LICENSE_1_0.txt for licensing.
//  See http://code.google.com/p/loglite/ for library home page.

#include <fstream>
#include <logging.hpp>

using namespace boost::logging;
namespace bl = boost::logging;

void overheat(int d)
{
  BOOST_LOG(2, bl::log, "overheat called");
  BOOST_LOG(1, bl::warning, "Warning: Tube overheat! Shutdown system immediatly");
  BOOST_LOG(1, bl::error, "ERROR: tube overheat: " << d << "d C. Shutdown should follow");
}

int do_something()
{
  BOOST_LOG(2, bl::log, "do_something called");
  BOOST_LOG(2, bl::notice, "do_something is performing some fancy processing (useless log don't you think?)");

  return 7;
}

int main(int argc, char **argv)
{
  boost::logging::logger *l = boost::logging::logger::get_instance();

  boost::logging::format display_format(boost::logging::trace >> boost::logging::eol);
  boost::logging::format file_format("[" >> boost::logging::level >> "],"
                                         >> boost::logging::filename >> "("
                                         >> boost::logging::line >> "),"
                                         >> boost::logging::time >> ","
                                         >> boost::logging::trace
                                         >> boost::logging::eol); // log format
  l->add_format(display_format);
  l->add_format(file_format);


  boost::logging::sink file_sink(new std::ofstream("./output.log"), 3);
  file_sink.attach_qualifier(bl::log);
  file_sink.attach_qualifier(bl::error);
  l->add_sink(file_sink, file_format);

  boost::logging::sink display_sink(&std::cout, 1);
  display_sink.attach_qualifier(bl::notice);
  display_sink.attach_qualifier(bl::warning);
  l->add_sink(display_sink, display_format);

  BOOST_LOG(1, bl::log, "Application starting");
  BOOST_LOG(1, bl::notice, "Application version 1.0.3 - Copyright(2007) World Company");

  do_something();
  overheat(87);
  return 0;
}
