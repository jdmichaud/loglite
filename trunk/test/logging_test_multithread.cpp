//  Boost general library logging_test_multithread.hpp file  -----------------//

//  (C) Copyright Jean-Daniel Michaud 2007. Permission to copy, use, modify, 
//  sell and distribute this software is granted provided this copyright notice 
//  appears in all copies. This software is provided "as is" without express or 
//  implied warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.
//  See http://www.boost.org/libs/logging/ for library home page.

#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <fstream>
#include <logging.hpp>

void thread_log_something()
{
  unsigned int i;
  for (i = 0; i < 20; ++i)
    BOOST_LOG_(1, "loop index: " << i); 
}

int main(int argc, char **argv)
{
  BOOST_LOG_INIT(("[" >> boost::logging::level >> "],"
                      >> boost::logging::filename >> "("
                      >> boost::logging::line >> "),"
                      >> boost::logging::time >> ","
                      >> boost::logging::trace
                      >> boost::logging::eol));

  boost::logging::sink sink_file(new std::ofstream("./output.log"), 2);
  sink_file.attach_qualifier(boost::logging::log);
  BOOST_LOG_ADD_OUTPUT_STREAM(sink_file);

  boost::logging::sink sink_cout(&std::cout, 2);
  sink_cout.attach_qualifier(boost::logging::log);
  BOOST_LOG_ADD_OUTPUT_STREAM(sink_cout);

  boost::thread_group thrd;
  unsigned int i;
  for (i = 0; i < 10; ++i)
    thrd.create_thread(boost::bind(thread_log_something));

  thrd.join_all();
  return 0;
}
