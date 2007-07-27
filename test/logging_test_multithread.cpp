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
#include <boost/logging.hpp>

void thread_log_something()
{
  unsigned int i;
  for (i = 0; i < 20; ++i)
    BOOST_LOG(1, "loop index: " << i); 
}

int main(int argc, char **argv)
{
  BOOST_LOG_INIT(("[" >> boost::logging::level >> "],"
                      >> boost::logging::filename >> "("
                      >> boost::logging::line >> "),"
                      >> boost::logging::time >> ","
                      >> boost::logging::trace
                      >> boost::logging::eol),
                      2);

  BOOST_LOG_ADD_OUTPUT_STREAM(new std::ofstream("./output.log"));
  BOOST_LOG_ADD_OUTPUT_STREAM(&std::cout);

  boost::thread_group thrd;
  unsigned int i;
  for (i = 0; i < 10; ++i)
    thrd.create_thread(boost::bind(thread_log_something));

  thrd.join_all();
  return 0;
}
