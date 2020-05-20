.PHONY : clean

CXX=g++
CXXFLAGS= -fPIC -Wall -O2 -std=c++0x --shared -lboost_regex
LDFLAGS= -shared

SOURCES = src/locationtools.cc src/liblocation.cc src/UTM.cc
HEADERS = src/liblocation.h
OBJECTS=$(SOURCES:.cc=.o)

LIBLOCATION_LIBDIR=lib

TARGET=$(LIBLOCATION_LIBDIR)/liblocation.so

all: $(TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET)

$(TARGET) : $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(TARGET)
