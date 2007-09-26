//  Boost general library logging_test_iostream_client.cpp file  ------------//

//  (C) Copyright Jean-Daniel Michaud 2007. Permission to copy, use, modify, 
//  sell and distribute this software is granted provided this copyright notice 
//  appears in all copies. This software is provided "as is" without express or 
//  implied warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org/LICENSE_1_0.txt for licensing.
//  See http://code.google.com/p/loglite/ for library home page.

#include <ostream>
#include <logging.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
using namespace boost::logging;

int main(int argc, char **argv)
{
  if (argc != 2)
  {
    std::cerr << "usage: test_iostream_client.exe <host>" << std::endl;
    return 0;
  }

  BOOST_LOG_INIT(("\t<log>" >> eol >>
      "\t\t<level>" >> level >> "</level>" >> eol >>
      "\t\t<filename>" >> filename >> "</filename>" >> eol >>
      "\t\t<line>" >> line >> "</line>" >> eol >>
      "\t\t<time>" >> boost::logging::time >> "</time>" >> eol >>
      "\t\t<trace>" >> eol >> "\t\t\t" >>
        trace >> eol >>
      "\t\t</trace>" >> eol >>
    "\t</log>" >> eol));

  try
  {
    sink sink_file(new tcp::iostream("localhost", "daytime"), 2);
    sink_file.attach_qualifier(boost::logging::log);
    BOOST_LOG_ADD_OUTPUT_STREAM(sink_file);

    BOOST_LOG_UNFORMATTED(1, boost::logging::log, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl);
    BOOST_LOG_UNFORMATTED(1, boost::logging::log, "<log_session>" << std::endl);

    BOOST_LOG_(1, "Strange women lying in ponds distributing swords");
    BOOST_LOG_(1, "is no basis for a system of government");
    BOOST_LOG_(1, "Supreme executive power derives from a mandate of the masses");
    BOOST_LOG_(1, "not from some farcical aquatic ceremony!");

    BOOST_LOG_UNFORMATTED(1, boost::logging::log, "</log_session>");
  }
  catch (std::exception& e)
  {
    std::cout << "Exception: " << e.what() << std::endl;
  }

  return 0;
}
