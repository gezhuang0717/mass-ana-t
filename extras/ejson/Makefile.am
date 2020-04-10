noinst_PROGRAMS = ejson-test
ejson_test_SOURCES = ejson_test.c
ejson_test_LDADD = libejson.la
ejson_test_CFLAGS = -std=c99

noinst_LTLIBRARIES = libejson.la
libejson_la_SOURCES = ejson.c
libejson_la_CFLAGS = -std=c99

BUILT_SOURCES = ejson.c

ejson.c: ejson.rl ejson.h
	ragel -G1 -e $(srcdir)/ejson.rl

# library_la_LDADD