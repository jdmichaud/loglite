//  loglite library logging_test_multithread.hpp file  -----------------------//

//  (C) Copyright Jean-Daniel Michaud 2007. Permission to copy, use, modify, 
//  sell and distribute this software is granted provided this copyright notice 
//  appears in all copies. This software is provided "as is" without express or 
//  implied warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org/LICENSE_1_0.txt for licensing.
//  See http://code.google.com/p/loglite/ for library home page.

#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <fstream>
#include <logging.hpp>

int foo(int i)
{
  LOGLITE_LOG_L1("foo called");
  return i;
}

void thread_log_something()
{
  unsigned int i;
  for (i = 0; i < 100; ++i)
    LOGLITE_LOG_L1("loop index: " << foo(i)); 
}

int main(int argc, char **argv)
{
  LOGLITE_INIT(("[" >> loglite::mask >> "],"
                    >> loglite::filename >> "("
                    >> loglite::line >> "),"
                    >> loglite::time >> ","
                    >> loglite::trace
                    >> loglite::eol));

  loglite::sink sink_file(new std::ofstream("./test_multithread.log"), LOGLITE_MASK_LEVEL_2);
  sink_file.attach_qualifier(loglite::log);
  LOGLITE_ADD_OUTPUT_STREAM(sink_file);

  loglite::sink sink_cout(&std::cout, LOGLITE_MASK_LEVEL_2);
  sink_cout.attach_qualifier(loglite::log);
  LOGLITE_ADD_OUTPUT_STREAM(sink_cout);

  boost::thread_group thrd;
  unsigned int i;
  for (i = 0; i < 20; ++i)
    thrd.create_thread(boost::bind(thread_log_something));

  thrd.join_all();
  return 0;
}
