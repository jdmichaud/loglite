//  loglite library logging_test_iostream_shmem.cpp file  --------------------//

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

using namespace loglite;

void server_thread(const std::string &mem_file_name)
{
  std::ofstream out("test_iostream_shmem.txt", std::ios::app);
  bool done = false;

  while (!done)
  {
    try
    {
      boost::iostreams::stream
        <
          boost::iostreams::mapped_file_source
        > in(mem_file_name);

      done = true;

      std::string line;
      while (std::getline(in, line, '\f'))
        if (!in.eof() && !in.fail() && !in.bad())
          out << line;
    }
    catch (const std::exception &e) 
    {
      std::cerr << e.what() << std::endl;
      boost::xtime xt;
      boost::xtime_get(&xt, boost::TIME_UTC);
      xt.sec += 1;
      boost::thread::sleep(xt); // Sleep      
    }
  }
}

int main(int argc, char **argv)
{
  std::string memory_file_name("boost.logging.shared.memory.segment");
  boost::thread thrd(boost::bind(&server_thread, memory_file_name));

  LOGLITE_INIT(filename >> (*new literal_element("("))
                        >> line >> "),"
                        >> loglite::time >> ","
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

    sink sink_file(out, LOGLITE_MASK_LEVEL_2);
    sink_file.attach_qualifier(loglite::log);
    LOGLITE_ADD_OUTPUT_STREAM(sink_file);

    LOGLITE_LOG_(1, "This test the shared memory streaming");
    LOGLITE_LOG_(1, "Now you can log from a process and dump the log to a file from another process");

    out->close();
    thrd.join();
  }
  catch (std::exception& e)
  {
    std::cout << "Exception: " << e.what() << std::endl;
  }
 
  return 0;
}
