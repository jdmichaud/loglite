# Created on 11th December 2007 by Jean-Daniel Michaud
# 30May2009 - JDM: make possible to adapt to other boost version

PROJECT(loglite_test)
# SUBDIRS(c cpp)

INCLUDE_DIRECTORIES($ENV{BOOST_INCLUDE} ../ ../test/)
LINK_DIRECTORIES($ENV{BOOST_INCLUDE}/stage/lib)

ADD_EXECUTABLE(test ../test/logging_test.cpp)
TARGET_LINK_LIBRARIES(test boost_thread$ENV{BOOST_LIB_SUFFIX} boost_date_time$ENV{BOOST_LIB_SUFFIX} boost_system$ENV{BOOST_LIB_SUFFIX})

ADD_EXECUTABLE(test_hello_world ../test/logging_test_hello_world.cpp)
TARGET_LINK_LIBRARIES(test_hello_world boost_thread$ENV{BOOST_LIB_SUFFIX} boost_date_time$ENV{BOOST_LIB_SUFFIX} boost_system$ENV{BOOST_LIB_SUFFIX} pthread)

ADD_EXECUTABLE(test_macro ../test/logging_test_macro.cpp)
TARGET_LINK_LIBRARIES(test_macro boost_thread$ENV{BOOST_LIB_SUFFIX} boost_date_time$ENV{BOOST_LIB_SUFFIX} boost_system$ENV{BOOST_LIB_SUFFIX} pthread)

ADD_EXECUTABLE(test_iostream ../test/logging_test_iostream.cpp ../test/zlib.cpp)
TARGET_LINK_LIBRARIES(test_iostream boost_thread$ENV{BOOST_LIB_SUFFIX} boost_date_time$ENV{BOOST_LIB_SUFFIX} boost_system$ENV{BOOST_LIB_SUFFIX} boost_iostreams$ENV{BOOST_LIB_SUFFIX} pthread z)

ADD_EXECUTABLE(test_iostream_client ../test/logging_test_iostream_client.cpp)
TARGET_LINK_LIBRARIES(test_iostream_client boost_thread$ENV{BOOST_LIB_SUFFIX} boost_date_time$ENV{BOOST_LIB_SUFFIX} boost_system$ENV{BOOST_LIB_SUFFIX} pthread)

ADD_EXECUTABLE(test_iostream_server ../test/logging_test_iostream_server.cpp)
TARGET_LINK_LIBRARIES(test_iostream_server boost_thread$ENV{BOOST_LIB_SUFFIX} boost_date_time$ENV{BOOST_LIB_SUFFIX} boost_system$ENV{BOOST_LIB_SUFFIX} pthread)

ADD_EXECUTABLE(test_iostream_shmem ../test/logging_test_iostream_shmem.cpp ../test/mapped_file.cpp)
TARGET_LINK_LIBRARIES(test_iostream_shmem boost_thread$ENV{BOOST_LIB_SUFFIX} boost_date_time$ENV{BOOST_LIB_SUFFIX} boost_system$ENV{BOOST_LIB_SUFFIX} pthread)

ADD_EXECUTABLE(test_multithread ../test/logging_test_multithread.cpp)
TARGET_LINK_LIBRARIES(test_multithread boost_thread$ENV{BOOST_LIB_SUFFIX} boost_date_time$ENV{BOOST_LIB_SUFFIX} boost_system$ENV{BOOST_LIB_SUFFIX} pthread)

ADD_EXECUTABLE(test_multi_file ../test/logging_test_multi_file.cpp ../test/logging_test_multi_file2.cpp)
TARGET_LINK_LIBRARIES(test_multi_file boost_thread$ENV{BOOST_LIB_SUFFIX} boost_date_time$ENV{BOOST_LIB_SUFFIX} boost_system$ENV{BOOST_LIB_SUFFIX} pthread)

ADD_EXECUTABLE(test_qualifier ../test/logging_test_qualifier.cpp)
TARGET_LINK_LIBRARIES(test_qualifier boost_thread$ENV{BOOST_LIB_SUFFIX} boost_date_time$ENV{BOOST_LIB_SUFFIX} boost_system$ENV{BOOST_LIB_SUFFIX} pthread)

ADD_EXECUTABLE(test_scope ../test/logging_test_scope.cpp)
TARGET_LINK_LIBRARIES(test_scope boost_thread$ENV{BOOST_LIB_SUFFIX} boost_date_time$ENV{BOOST_LIB_SUFFIX} boost_system$ENV{BOOST_LIB_SUFFIX} pthread)

ADD_EXECUTABLE(test_xml_output ../test/logging_test_xml_output.cpp)
TARGET_LINK_LIBRARIES(test_xml_output boost_thread$ENV{BOOST_LIB_SUFFIX} boost_date_time$ENV{BOOST_LIB_SUFFIX} boost_system$ENV{BOOST_LIB_SUFFIX} pthread)

ADD_EXECUTABLE(test_format ../test/logging_test_format.cpp)
TARGET_LINK_LIBRARIES(test_format boost_thread$ENV{BOOST_LIB_SUFFIX} boost_date_time$ENV{BOOST_LIB_SUFFIX} boost_system$ENV{BOOST_LIB_SUFFIX} pthread)
