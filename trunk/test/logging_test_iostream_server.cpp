//  Boost general library logging_test_xml_output.cpp file  ------------------//

//  (C) Copyright Jean-Daniel Michaud 2007. Permission to copy, use, modify, 
//  sell and distribute this software is granted provided this copyright notice 
//  appears in all copies. This software is provided "as is" without express or 
//  implied warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org/LICENSE_1_0.txt for licensing.
//  See http://code.google.com/p/loglite/ for library home page.

#include <ostream>
#include <fstream>
#include <logging.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
using namespace boost::logging;

int main(int argc, char **argv)
{
  boost::asio::io_service io_service;

  tcp::endpoint endpoint(tcp::v4(), 13);
  tcp::acceptor acceptor(io_service, endpoint);

  for (;;)
  {
    tcp::iostream stream;
    acceptor.accept(*stream.rdbuf());
    while (!stream.eof())
    {
      char c;
      stream.get(c);
      if (!stream.eof())
        std::cout << c;
    }
  }

  return 0;
}

