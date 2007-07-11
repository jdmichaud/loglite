//  Boost general library logging.hpp header file  ---------------------------//

//  (C) Copyright Jean-Daniel Michaud 2007. Permission to copy, use, modify, 
//  sell and distribute this software is granted provided this copyright notice 
//  appears in all copies. This software is provided "as is" without express or 
//  implied warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.
//  See http://www.boost.org/libs/logging/ for library home page.

#include <iostream>
#include <fstream>
#include <boost/shared_ptr.hpp>
#include <boost/logging.hpp>

int main(int argc, char **argv)
{
  boost::logging::logger lgr("[" >> boost::logging::level >> "],"
                             >> boost::logging::filename >> "("
                             >> boost::logging::line >> "),"
                             >> boost::logging::time >> ","
                             >> boost::logging::trace
                             >> boost::logging::eol);
  
  boost::logging::stream_list_t stream_list;
  stream_list.push_back(boost::shared_ptr<std::ostream>(&std::cout, boost::logging::null_deleter()));
  stream_list.push_back(boost::shared_ptr<std::ostream> (new std::ofstream("./output.log")));
  lgr.set_output_streams(stream_list);
  lgr.trace(1, "something", __FILE__, __LINE__);
  lgr.trace(1, "something else", __FILE__, __LINE__);
  return 0;
}
