//  Boost general library logging_test_iostream_server.cpp file  -------------//

//  (C) Copyright Jean-Daniel Michaud 2007. Permission to copy, use, modify, 
//  sell and distribute this software is granted provided this copyright notice 
//  appears in all copies. This software is provided "as is" without express or 
//  implied warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org/LICENSE_1_0.txt for licensing.
//  See http://code.google.com/p/loglite/ for library home page.

#include <ostream>
#include <logging.hpp>
#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/gzip.hpp>

using namespace boost::logging;

int main(int argc, char **argv)
{
  BOOST_LOG_INIT(trace >> eol);

  boost::iostreams::filtering_ostream *out = new boost::iostreams::filtering_ostream();
  out->push(boost::iostreams::gzip_compressor());
  out->push(boost::iostreams::file_sink("log.gz"));

  sink sink_file(out, BOOST_LOG_MASK_LEVEL_2);
  sink_file.attach_qualifier(boost::logging::log);
  BOOST_LOG_ADD_OUTPUT_STREAM(sink_file);

  BOOST_LOG_(1, "Strange women lying in ponds distributing swords");
  BOOST_LOG_(1, "is no basis for a system of government");
  BOOST_LOG_(1, "Supreme executive power derives from a mandate of the masses");
  BOOST_LOG_(1, "not from some farcical aquatic ceremony!");
  return 0;
}
