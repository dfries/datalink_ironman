CXXFLAGS  = -g -Wall `gtkmm-config --cflags`
LDFLAGS = `gtkmm-config --libs`

ifdef efence
LDFLAGS += -lstdc++-2-libc6.1-1-2.9.0 -lefence
endif

all: fun signaldump

signaldump: signaldump.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

fun: fun.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f *.o fun
