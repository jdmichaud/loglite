# Makefile for logging

CC	= g++
BOOST	= ../../../boost-latest/
SRC	= ../test/logging_test.cpp		\
	../test/logging_test_hello_world.cpp	\
	../test/logging_test_macro.cpp		\
	../test/logging_test_multithread.cpp	\
	../test/logging_test_qualifier.cpp	\
	../test/logging_test_xml_output.cpp	
SRC2	= ../test/logging_test_multi_file.cpp	\
	../test/logging_test_multi_file2.cpp
SRC3	= ../test/logging_test_iostream.cpp	\
	../test/zlib.cpp
SRC4	= ../test/logging_test_iostream_shmem.cpp \
	../test/mapped_file.cpp
CLIENT	= ../test/logging_test_iostream_client.cpp
SERVER	= ../test/logging_test_iostream_server.cpp
OBJ	= $(SRC:.cpp=.o)
EXE	= $(SRC:.cpp=)
OBJ2	= $(SRC2:.cpp=.o)
OBJ3	= $(SRC3:.cpp=.o)
OBJ4	= $(SRC4:.cpp=.o)
CLOBJ	= $(CLIENT:.cpp=.o)
SRVOBJ	= $(SERVER:.cpp=.o)
EXE2	= ../test/logging_test_multi_file
EXE3	= ../test/logging_test_iostream
EXE4	= ../test/logging_test_iostream_shmem
CLEXE	= ../test/logging_test_iostream_client
SRVEXE	= ../test/logging_test_iostream_server
INCLUDE	= -I../ -I../test -I$(BOOST)
LIBS	= -static -L$(BOOST)/stage/lib		\
	-lboost_thread-gcc40-mt-d-1_35		\
	-lboost_date_time-gcc40-mt-d-1_35	\
	-lboost_system-gcc40-mt-d-1_35	\
	-pthread -lz
CFLAGS	= -c -Wfatal-errors -ggdb3 $(INCLUDE)

all: $(EXE) $(EXE2) $(EXE3) $(CLEXE) $(SRVEXE) $(EXE4)

.o:
	$(CC) -o $@ $? $(LDFLAGS) $(LIBS)

$(OBJ): $(SRC)
	$(CC) $(CFLAGS) $*.cpp -o $@

$(EXE2): $(OBJ2)
	$(CC) $(OBJ2) $(LDFLAGS) $(LIBS) -o $@

$(OBJ2): $(SRC2)
	$(CC) $(CFLAGS) $*.cpp -o $@

$(EXE3): $(OBJ3)
	$(CC) $(OBJ3) $(LDFLAGS) $(LIBS) -o $@

$(OBJ3): $(SRC3)
	$(CC) $(CFLAGS) $*.cpp -o $@

$(EXE4): $(OBJ4)
	$(CC) $(OBJ4) $(LDFLAGS) $(LIBS) -o $@

$(OBJ4): $(SRC4)
	$(CC) $(CFLAGS) $*.cpp -o $@

$(CLEXE): $(CLOBJ)
	$(CC) $(CLOBJ) $(LDFLAGS) $(LIBS) -o $@

$(CLOBJ): $(CLIENT)
	$(CC) $(CFLAGS) $*.cpp -o $@

$(SRVEXE): $(SRVOBJ)
	$(CC) $(SRVOBJ) $(LDFLAGS) $(LIBS) -o $@

$(SRVOBJ): $(SRVSRC)
	$(CC) $(CFLAGS) $*.cpp -o $@


clean:
	cd .. && rm -fr *.o *~ *#
	cd ../test && rm -fr *.o *~ *#
	cd ../test && rm -fr $(EXE) $(EXE2) $(EXE3)

re: clean all

