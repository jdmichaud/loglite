//  Boost general library logging_test_iostream_server.cpp file  -------------//

//  (C) Copyright Jean-Daniel Michaud 2007. Permission to copy, use, modify, 
//  sell and distribute this software is granted provided this copyright notice 
//  appears in all copies. This software is provided "as is" without express or 
//  implied warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org/LICENSE_1_0.txt for licensing.
//  See http://code.google.com/p/loglite/ for library home page.

#include <ostream>
#include <fstream>
#include <queue>
#include <string>

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/thread/condition.hpp>
#include <boost/bind.hpp>
#include <boost/ref.hpp>

using boost::asio::ip::tcp;

boost::mutex            m;
std::ofstream           out("log.txt");
int nbthread = 0;

void push_trace(const std::string &t)
{
  std::cout << "thread created. # of threads: " << ++nbthread << std::endl;
  boost::mutex::scoped_lock scoped_lock(m);
  out << t << std::flush;
  --nbthread;
}

void accept(tcp::acceptor &acceptor)
{
  tcp::iostream stream;
  acceptor.accept(*stream.rdbuf());
  boost::thread thrd(boost::bind(&accept, boost::ref(acceptor)));
  
  std::string line;
  boost::thread_group thrd_group;
  while (std::getline(stream, line, '\f'))
    //thrd_group.create_thread(boost::bind(&push_trace, line));
	push_trace(line);

  thrd_group.join_all();
  thrd.join();
}

int main(int argc, char **argv)
{
  boost::asio::io_service io_service;

  tcp::endpoint endpoint(tcp::v4(), 16013);
  tcp::acceptor acceptor(io_service, endpoint);

  boost::thread thrd(boost::bind(&accept, boost::ref(acceptor)));
  thrd.join();
  return 0;
}

