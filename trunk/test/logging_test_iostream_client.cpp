//  Boost general library logging_test_iostream_client.cpp file  -------------//

//  (C) Copyright Jean-Daniel Michaud 2007. Permission to copy, use, modify, 
//  sell and distribute this software is granted provided this copyright notice 
//  appears in all copies. This software is provided "as is" without express or 
//  implied warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org/LICENSE_1_0.txt for licensing.
//  See http://code.google.com/p/loglite/ for library home page.

#include <ostream>
#include <logging.hpp>
#include <boost/asio.hpp>
#include <boost/thread/xtime.hpp>

using boost::asio::ip::tcp;
using namespace boost::logging;

int main(int argc, char **argv)
{
  if (argc != 3)
  {
    std::cerr << "usage: test_iostream_client.exe <host> <client_id>" << std::endl;
    return 0;
  }

  BOOST_LOG_INIT(((*new boost::logging::literal_element(argv[2])) >> (*new boost::logging::literal_element(" - ")) 
                   >> (*new boost::logging::literal_element("[")) >> boost::logging::level >> "],"
                   >> boost::logging::filename >> "("
                   >> boost::logging::line >> "),"
                   >> boost::logging::time >> ","
                   >> boost::logging::trace
                   >> boost::logging::eol
                   >> boost::logging::eot)); // end-of-trace used because networking in the game

  try
  {
    sink sink_network(new tcp::iostream(argv[1], "16013"), 2);
    sink_network.attach_qualifier(boost::logging::log);
    BOOST_LOG_ADD_OUTPUT_STREAM(sink_network);

    for (int count = 0; count < 1000; ++count)
    {
      BOOST_LOG_(1, "Strange women lying in ponds distributing swords");
      BOOST_LOG_(1, "is no basis for a system of government");
      BOOST_LOG_(1, "Supreme executive power derives from a mandate of the masses");
      BOOST_LOG_(1, "not from some farcical aquatic ceremony!");
    }
  }
  catch (std::exception& e)
  {
    std::cout << "Exception: " << e.what() << std::endl;
  }

  return 0;
}
