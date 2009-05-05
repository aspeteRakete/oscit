INCLUDE_HEADERS=-Ioscpack -Iinclude
TESTING=-D_TESTING_
AR = ar rcu
RAGEL=ragel
LIBTOOL=libtool -static
TEST=test/*_test.h
OBJECTS=url.o list.o midi_message.o object.o root.o command.o osc_command.o zeroconf_browser.o zeroconf_registration.o value.o thread.o oscpack/liboscpack.a matrix.o
CFLAGS=-g -Wall $(TESTING) $(PLAT_FLAGS)

PLATS=macosx linux


ifeq ($(PLAT),"macosx")
  PLAT_FLAGS="-D_macosx_"
else
  ifeq ($(PLAT),"linux")
    PLAT_FLAGS="-D_linux_"
  else
    PLAT=none
  endif
endif

default: $(PLAT)

all: test

# without slow tests
test: test/runner
	./test/runner && rm test/runner

# all tests (zeroconf & co)
test_all: test/runner_all
	./test/runner_all && rm test/runner_all

	
# TODO: archive (ar ..)
liboscit.a: $(OBJECTS)
	$(LIBTOOL) $(OBJECTS) -o $@

oscpack/liboscpack.a:
	cd oscpack && make liboscpack.a

test/runner.cpp: $(TEST)
	./test/cxxtest/cxxtestgen.pl --error-printer -o test/runner.cpp $(TEST)

test/runner: test/runner.cpp liboscit.a
	$(CC) $(CFLAGS) $(LFLAGS) -Itest $(INCLUDE_HEADERS) -I. test/runner.cpp liboscit.a -lgcc -lstdc++ -o test/runner

test/runner_all.cpp: $(TEST) test/*_test_slow.h
	./test/cxxtest/cxxtestgen.pl --error-printer -o test/runner_all.cpp $(TEST) test/*_test_slow.h

test/runner_all: test/runner_all.cpp liboscit.a
	$(CC) $(CFLAGS) $(LFLAGS) -Itest $(INCLUDE_HEADERS) -I. test/runner_all.cpp liboscit.a -lgcc -lstdc++ -o test/runner_all

root.o: src/root.cpp include/oscit/root.h include/oscit/*_meta_method.h
	$(CC) $(CFLAGS) -c $(INCLUDE_HEADERS) $< -o $@

value.o: src/value.cpp include/oscit/value.h
	$(CC) $(CFLAGS) -c $(INCLUDE_HEADERS) $< -o $@

cxalloc.o: include/opencv/cxalloc.cpp
	$(CC) $(CFLAGS) -c $(INCLUDE_HEADERS) $< -o $@

cxsystem.o: include/opencv/cxsystem.cpp
	$(CC) $(CFLAGS) -c $(INCLUDE_HEADERS) $< -o $@

zeroconf_browser.o: src/$(PLAT)/zeroconf_browser.cpp include/oscit/zeroconf.h
	$(CC) $(CFLAGS) -c $(INCLUDE_HEADERS) $< -o $@

zeroconf_registration.o: src/$(PLAT)/zeroconf_registration.cpp include/oscit/zeroconf.h
	$(CC) $(CFLAGS) -c $(INCLUDE_HEADERS) $< -o $@

%.o: src/%.cpp include/oscit/%.h
	$(CC) $(CFLAGS) -c $(INCLUDE_HEADERS) $< -o $@

src/%.cpp: src/%.rl
	$(RAGEL) $< -o $@

clean:
	rm -rf *.o *.dSYM liboscit.a test/runner.cpp test/*.dSYM test/runner_all.cpp

# targets for different platforms

macosx:
	$(MAKE) all PLAT=macosx

linux:
	$(MAKE) all PLAT=linux

none:
	@echo "Please choose a platform from the list: $(PLATS)"


