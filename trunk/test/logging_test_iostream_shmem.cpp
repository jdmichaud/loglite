//  Boost general library logging_test_iostream_shmem.cpp file  -------------//

//  (C) Copyright Jean-Daniel Michaud 2007. Permission to copy, use, modify, 
//  sell and distribute this software is granted provided this copyright notice 
//  appears in all copies. This software is provided "as is" without express or 
//  implied warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org/LICENSE_1_0.txt for licensing.
//  See http://code.google.com/p/loglite/ for library home page.

#include <iostream>
#include <fstream>
#include <logging.hpp>
#include <boost/iostreams/device/mapped_file.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>

using namespace boost::logging;

void server_thread(const std::string &mem_file_name)
{
  std::ofstream out("log.txt");
  bool done = false;

  while (!done)
  {
    try
    {
      boost::xtime xt;
      boost::xtime_get(&xt, boost::TIME_UTC);
      xt.sec += 1;
      boost::thread::sleep(xt);

      boost::iostreams::stream
        <
          boost::iostreams::mapped_file_source
        > in(mem_file_name);

      done = true;

      std::string line;
      while (std::getline(in, line, '\f'))
        out << line;
    }
    catch (const std::exception &e) {}
  }
}

int main(int argc, char **argv)
{
  std::string memory_file_name("segment");
  boost::thread thrd(boost::bind(&server_thread, memory_file_name));

  BOOST_LOG_INIT(filename >> (*new literal_element("("))
                          >> line >> "),"
                          >> boost::logging::time >> ","
                          >> trace
                          >> eol
                          >> eot); // end-of-trace used because shared memory in the game

  try
  { 
    boost::iostreams::stream <boost::iostreams::mapped_file_sink> 
      *out  = new boost::iostreams::stream <boost::iostreams::mapped_file_sink>();

    boost::iostreams::mapped_file_params p(memory_file_name);
    p.new_file_size = 1024 * sizeof (char);
    
    out->open(boost::iostreams::mapped_file_sink(p));

    sink sink_file(out, 2);
    sink_file.attach_qualifier(boost::logging::log);
    BOOST_LOG_ADD_OUTPUT_STREAM(sink_file);

    BOOST_LOG_(1, "Strange women lying in ponds distributing swords");
    BOOST_LOG_(1, "is no basis for a system of government");
    BOOST_LOG_(1, "Supreme executive power derives from a mandate of the masses");
    BOOST_LOG_(1, "not from some farcical aquatic ceremony!");

    out->close();
    thrd.join();
  }
  catch (std::exception& e)
  {
    std::cout << "Exception: " << e.what() << std::endl;
  }
 
  return 0;
}
