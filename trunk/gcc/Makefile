# Makefile for Other kernel

CC	= g++
BOOST	= ../../boost_1_34_1/
SRC	= ../test/logging_test.cpp		
#	../test/logging_test_hello_world.cpp	\
#	../test/logging_test_macro.cpp		\
#	../test/logging_test_multi_file2.cpp	\
#	../test/logging_test_multi_file.cpp	\
#	../test/logging_test_multithread.cpp	\
#	../test/logging_test_xml_output.cpp
OBJ	= $(SRC:.cpp=.o)
EXE	= $(SRC:.cpp=)
INCLUDE	= -I../ -I../test -I$(BOOST)
LIBS	= -L$(BOOST)/stage/lib
CFLAGS	= -c -Wfatal-errors -ggdb3 $(INCLUDE)

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) -o $@ $(LDFLAGS) $? $(LIBS)

.cpp.o:
	$(CC) $(CFLAGS) -o $@ $?

clean:
	cd .. && rm -fr *.o *~ *#
	cd ../test && rm -fr *.o *~ *#

re: clean all

