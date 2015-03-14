# A small C++ logging library

## Getting Started

You will need the boost library (at least 1.34.0) you can find it here.

For help on how to compile boost: [Getting Started](http://www.boost.org/more/getting_started/index.html).

Once boost is compiled, you can give it a try by compiling the examples
* Download the loglite using svn.
* Go into the folder gcc and follow the instructions supplied in the README file.

You're done.

Much more is possible thanks to Boost.IOStreams and Boost.Asio.

* Look at [logging_test_iostream.cpp](https://github.com/jdmichaud/loglite/blob/master/test/logging_test_iostream.cpp) for an example of log compressed on the fly
* Look at [logging_test_iostream_client.cpp](https://github.com/jdmichaud/loglite/blob/master/test/logging_test_iostream_client.cpp) and [logging_test_iostream_server.cpp](https://github.com/jdmichaud/loglite/blob/master/test/logging_test_iostream_server.cpp) for logging through a network.

More examples in the trunk

## Documentation

http://loglite.googlecode.com/svn/trunk/doc/index.html

## An example

###Source code
```
//  loglite library logging_test_macro.cpp file  -----------------------------//

//  (C) Copyright Jean-Daniel Michaud 2007. Permission to copy, use, modify,
//  sell and distribute this software is granted provided this copyright notice
//  appears in all copies. This software is provided "as is" without express or
//  implied warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org/LICENSE_1_0.txt for licensing.
//  See http://code.google.com/p/loglite/ for library home page.

#include <fstream>
#include <logging.hpp>

int infinite_loop()
{
  LOGLITE_LOG_L1("oops...");
  while (1) ;
}

int foo()
{
  LOGLITE_LOG_(1, "foo called");
  return 7;
}

int main(int argc, char **argv)
{
  // Define the format of your log
  LOGLITE_INIT(("[" >> loglite::mask >> "],"
                    >> loglite::filename >> "("
                    >> loglite::line >> "),"
                    >> loglite::time >> ","
                    >> loglite::trace
                    >> loglite::eol)); // log format

  // All the logs below level 3 should sink in ./test_macro.log
  loglite::sink s1(new std::ofstream("./test_macro.log"), LOGLITE_MASK_LEVEL_2);
  // The sink will only handle the log qualifier
  s1.attach_qualifier(loglite::log);
  LOGLITE_ADD_OUTPUT_STREAM(s1);

  // All the logs below 3 should sink on the standard output
  loglite::sink s2(&std::cout, LOGLITE_MASK_LEVEL_2);
  // The sink will only handle the log qualifier
  s2.attach_qualifier(loglite::log);
  LOGLITE_ADD_OUTPUT_STREAM(s2);

  LOGLITE_LOG(LOGLITE_LEVEL_1, loglite::log, "something");
  LOGLITE_LOG_L2("something else"); // A log of level 2 (L2)
  // Logs are not evaluated if no sink is configured to use them
  LOGLITE_LOG_(LOGLITE_LEVEL_3, "If you evaluate me you die!" << infinite_loop());
  char you_want[256] = "you want";
  LOGLITE_LOG_(LOGLITE_LEVEL_1, "Let's say " << you_want << " to display " << 2);
  LOGLITE_LOG_(LOGLITE_LEVEL_1, "foo will be evaluated: " << foo());
  return 0;
}
```
### Output
```
[1],logging_test_macro.cpp(43),2007-Aug-24 07:36:33.858844,something
[2],logging_test_macro.cpp(44),2007-Aug-24 07:36:33.858844,something else
[1],logging_test_macro.cpp(47),2007-Aug-24 07:36:33.858844,Let's say you want to display 2
[1],logging_test_macro.cpp(21),2007-Aug-24 07:36:33.874469,foo called
[1],logging_test_macro.cpp(48),2007-Aug-24 07:36:33.874469,foo will be evaluated: 7                                              
```
