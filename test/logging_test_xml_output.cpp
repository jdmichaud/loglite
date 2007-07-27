//  Boost general library logging_test_xml_output.cpp file  ------------------//

//  (C) Copyright Jean-Daniel Michaud 2007. Permission to copy, use, modify, 
//  sell and distribute this software is granted provided this copyright notice 
//  appears in all copies. This software is provided "as is" without express or 
//  implied warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.
//  See http://www.boost.org/libs/logging/ for library home page.

#include <fstream>
#include <boost/logging.hpp>

int main(int argc, char **argv)
{
  BOOST_LOG_INIT(("\t<log>" >> boost::logging::eol >>
      "\t\t<level>" >> boost::logging::level >> "</level>" >> boost::logging::eol >>
      "\t\t<filename>" >> boost::logging::filename >> "</filename>" >> boost::logging::eol >>
      "\t\t<line>" >> boost::logging::line >> "</line>" >> boost::logging::eol >>
      "\t\t<time>" >> boost::logging::time >> "</time>" >> boost::logging::eol >>
      "\t\t<trace>" >> boost::logging::eol >> "\t\t\t" >>
        boost::logging::trace >> boost::logging::eol >>
      "\t\t</trace>" >> boost::logging::eol >>
    "\t</log>" >> boost::logging::eol),
    2); // Max log level
  
  BOOST_LOG_ADD_OUTPUT_STREAM(new std::ofstream("./output.xml"));
  BOOST_LOG_ADD_OUTPUT_STREAM(&std::cout);

  boost::gregorian::date d(boost::gregorian::day_clock::local_day());
  std::string date = boost::gregorian::to_iso_extended_string(d);

  BOOST_LOG_UNFORMATTED(1, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl);
  BOOST_LOG_UNFORMATTED(1, "<log_session date=\"" << date << "\">" << std::endl);

  BOOST_LOG(1, "Strange women lying in ponds distributing swords");
  BOOST_LOG(1, "is no basis for a system of government");
  BOOST_LOG(1, "Supreme executive power derives from a mandate of the masses");
  BOOST_LOG(1, "not from some farcical aquatic ceremony!");

  BOOST_LOG_UNFORMATTED(1, "</log_session>");
  
  return 0;
}
