CXXFLAGS  = -g -Wall
LDFLAGS =
GDKFLAGS = `gtkmm-config --cflags`
GDKLDFLAGS = `gtkmm-config --libs`

ifdef efence
LDFLAGS += -lstdc++-2-libc6.1-1-2.9.0 -lefence
endif

all: fun signaldump

signaldump: signaldump.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

fun: fun.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(GDKFLAGS) $(GDKLDFLAGS ) -o $@ $^ $(LDFLAGS)

clean:
	rm -f *.o fun signaldump
