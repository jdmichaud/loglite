//  loglite library logging_qualifier.cpp file  ------------------------------//

//  (C) Copyright Jean-Daniel Michaud 2007. Permission to copy, use, modify, 
//  sell and distribute this software is granted provided this copyright notice 
//  appears in all copies. This software is provided "as is" without express or 
//  implied warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org/LICENSE_1_0.txt for licensing.
//  See http://code.google.com/p/loglite/ for library home page.

#include <fstream>
#include <logging.hpp>

using namespace loglite;
namespace l = loglite;

void overheat(int d)
{
  LOGLITE_LOG(2, l::log, "overheat called");
  LOGLITE_LOG(1, l::warning, "Warning: Tube overheat! Shutdown system immediatly");
  LOGLITE_LOG(1, l::error, "ERROR: tube overheat: " << d << "d C. Shutdown should follow");
}

int do_something()
{
  LOGLITE_LOG(2, l::log, "do_something called");
  LOGLITE_LOG(2, l::notice, "do_something is performing some fancy processing (useless log don't you think?)");

  return 7;
}

int main(int argc, char **argv)
{
  loglite::logger_p l = loglite::logger::get_instance();

  loglite::format display_format(loglite::trace >> loglite::eol);
  loglite::format file_format("[" >> loglite::mask >> "],"
                                         >> loglite::filename >> "("
                                         >> loglite::line >> "),"
                                         >> loglite::time >> ","
                                         >> loglite::trace
                                         >> loglite::eol); // log format
  l->add_format(display_format);
  l->add_format(file_format);


  loglite::sink file_sink(new std::ofstream("./test_qualifier.log"), LOGLITE_MASK_LEVEL_3);
  file_sink.attach_qualifier(l::log);
  file_sink.attach_qualifier(l::error);
  l->add_sink(file_sink, file_format);

  loglite::sink display_sink(&std::cout, LOGLITE_MASK_LEVEL_1);
  display_sink.attach_qualifier(l::notice);
  display_sink.attach_qualifier(l::warning);
  l->add_sink(display_sink, display_format);

  LOGLITE_LOG(LOGLITE_LEVEL_1, l::log, "Application starting");
  LOGLITE_LOG(LOGLITE_LEVEL_1, l::notice, "Application version 1.0.3 - Copyright(2007) World Company");

  LOGLITE_LOG(LOGLITE_LEVEL_1, l::log, "do_something returned: " << do_something());
  overheat(87);
  return 0;
}
