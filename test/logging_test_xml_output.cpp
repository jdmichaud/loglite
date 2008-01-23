//  loglite library logging_test_xml_output.cpp file  ------------------------//

//  (C) Copyright Jean-Daniel Michaud 2007. Permission to copy, use, modify, 
//  sell and distribute this software is granted provided this copyright notice 
//  appears in all copies. This software is provided "as is" without express or 
//  implied warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org/LICENSE_1_0.txt for licensing.
//  See http://code.google.com/p/loglite/ for library home page.

#include <fstream>
#include <logging.hpp>

using namespace loglite;

int main(int argc, char **argv)
{
  LOGLITE_INIT(("\t<log>" >> eol >>
      "\t\t<mask>" >> mask >> "</mask>" >> eol >>
      "\t\t<function_name>" >> function_name >> "</function_name>" >> eol >>
      "\t\t<function_signature>" >> function_signature >> "</function_signature>" >> eol >>
      "\t\t<filename>" >> filename >> "</filename>" >> eol >>
      "\t\t<line>" >> line >> "</line>" >> eol >>
      "\t\t<time>" >> loglite::time >> "</time>" >> eol >>
      "\t\t<trace>" >> eol >> "\t\t\t" >>
        trace >> eol >>
      "\t\t</trace>" >> eol >>
    "\t</log>" >> eol));

  sink sink_file(new std::ofstream("./test_xml_output.xml"), LOGLITE_MASK_LEVEL_2);
  sink_file.attach_qualifier(loglite::log);
  LOGLITE_ADD_OUTPUT_STREAM(sink_file);

  sink sink_cout(&std::cout, LOGLITE_MASK_LEVEL_2);
  sink_cout.attach_qualifier(loglite::log);
  LOGLITE_ADD_OUTPUT_STREAM(sink_cout);

  boost::gregorian::date d(boost::gregorian::day_clock::local_day());
  std::string date = boost::gregorian::to_iso_extended_string(d);

  LOGLITE_LOG_UNFORMATTED(1, loglite::log, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl);
  LOGLITE_LOG_UNFORMATTED(1, loglite::log, "<log_session date=\"" << date << "\">" << std::endl);

  LOGLITE_LOG_(1, "Strange women lying in ponds distributing swords");
  LOGLITE_LOG_(1, "is no basis for a system of government");
  LOGLITE_LOG_(1, "Supreme executive power derives from a mandate of the masses");
  LOGLITE_LOG_(1, "not from some farcical aquatic ceremony!");

  LOGLITE_LOG_UNFORMATTED(1, loglite::log, "</log_session>");
  
  return 0;
}
