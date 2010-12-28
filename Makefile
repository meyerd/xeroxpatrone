CXX = $(shell wx-config --cxx)

#WARN_FLAGS = -O0 -g -Wall -Wextra -Wabi -Wctor-dtor-privacy -Wnon-virtual-dtor -Wreorder -Wstrict-null-sentinel -Woverloaded-virtual -Wshadow -Wcast-align -Wpointer-arith -Wwrite-strings -Wundef -Wredundant-decls
DEBUG_FLAGS = -O0 -g

CXXFLAGS = $(shell wx-config --cxxflags) $(shell libusb-config --cflags)
LINKFLAGS = $(shell wx-config --libs) $(shell libusb-config --libs)

BIN = xeroxpatrone
OBJECTS = xeroxpatrone.o  mainpanel.o programmingadapter.o usbadapter.o \
	logger.o serialadapterv1.o serialadapterv2.o helpwindow.o usbadapterkernel.o

Debug: all
Release: all

all: $(BIN)

$(BIN): $(OBJECTS)
	$(CXX) -o $@ $(OBJECTS) $(DEBUG_FLAGS) $(WARN_FLAGS) $(LINKFLAGS)

%.o: %.cpp %.h targetver.h
	$(CXX) $(DEBUG_FLAGS) $(WARN_FLAGS) $(CXXFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -f *.o $(BIN)
