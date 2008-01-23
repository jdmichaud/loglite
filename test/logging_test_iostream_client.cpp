//  loglite library logging_test_iostream_client.cpp file  -------------------//

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
using namespace loglite;

int main(int argc, char **argv)
{
  if (argc != 3)
  {
    std::cerr << "usage: test_iostream_client.exe <host> <client_id>" << std::endl;
    return 0;
  }

  LOGLITE_INIT(((*new loglite::literal_element(argv[2])) >> (*new loglite::literal_element(" - ")) 
                  >> (*new loglite::literal_element("[")) >> loglite::mask >> "],"
                  >> loglite::filename >> "("
                  >> loglite::line >> "),"
                  >> loglite::time >> ","
                  >> loglite::trace
                  >> loglite::eol
                  >> loglite::eot)); // end-of-trace used because networking in the game

  try
  {
    sink sink_network(new tcp::iostream(argv[1], "16013"), LOGLITE_MASK_LEVEL_2);
    sink_network.attach_qualifier(loglite::log);
    LOGLITE_ADD_OUTPUT_STREAM(sink_network);

    for (int count = 0; count < 100; ++count)
    {
      boost::xtime xt;
      boost::xtime_get(&xt, boost::TIME_UTC);
      xt.sec += 1;
      boost::thread::sleep(xt);
      
      std::cout << "client: " << argv[2] << " trace: " << count << std::endl;
      LOGLITE_LOG_(1, "Strange women lying in ponds distributing swords");
      LOGLITE_LOG_(1, "is no basis for a system of government");
      LOGLITE_LOG_(1, "Supreme executive power derives from a mandate of the masses");
      LOGLITE_LOG_(1, "not from some farcical aquatic ceremony!");
    }
  }
  catch (std::exception& e)
  {
    std::cout << "Exception: " << e.what() << std::endl;
  }

  return 0;
}
