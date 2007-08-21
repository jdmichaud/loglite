# Makefile for logging

CC	= g++
BOOST	= ../../../boost_1_34_1/
SRC	= ../test/logging_test.cpp		\
	../test/logging_test_hello_world.cpp	\
	../test/logging_test_macro.cpp		\
	../test/logging_test_multithread.cpp	\
	../test/logging_test_xml_output.cpp
SRC2	= ../test/logging_test_multi_file.cpp	\
	../test/logging_test_multi_file2.cpp
OBJ	= $(SRC:.cpp=.o)
EXE	= $(SRC:.cpp=)
OBJ2	= $(SRC2:.cpp=.o)
EXE2	= ../test/logging_test_multi_file
INCLUDE	= -I../ -I../test -I$(BOOST)
LIBS	= -static -L$(BOOST)/stage/lib		\
	-lboost_thread-gcc40-mt-d-1_34_1	\
	-lboost_date_time-gcc40-mt-d-1_34_1	\
	-pthread
CFLAGS	= -c -Wfatal-errors -ggdb3 $(INCLUDE)

all: $(EXE) $(EXE2)

.o:
	$(CC) -o $@ $? $(LDFLAGS) $(LIBS)

$(OBJ): $(SRC)
	$(CC) $(CFLAGS) $*.cpp -o $@

$(EXE2): $(OBJ2)
	$(CC) $(OBJ2) $(LDFLAGS) $(LIBS) -o $@

$(OBJ2): $(SRC2)
	$(CC) $(CFLAGS) $*.cpp -o $@

clean:
	cd .. && rm -fr *.o *~ *#
	cd ../test && rm -fr *.o *~ *#
	cd ../test && rm -fr $(EXE) $(EXE2)

re: clean all

