PROJECT_DIR = $(dir $(realpath $(firstword $(MAKEFILE_LIST))))
COMPILER_PREFIX := $(PROJECT_DIR)/buildroot/output/host/usr/bin/arm-buildroot-linux-musleabi-
SYSROOT := $(PROJECT_DIR)/buildroot/output/host/arm-buildroot-linux-musleabi/sysroot
STRIP = $(COMPILER_PREFIX)strip
STRIPFLAGS = --strip-all
CXXFLAGS = -std=c++11 -pedantic -Wall -Wno-format -Wno-unused-result -fmessage-length=0 -static -I $(PROJECT_DIR)/websocketpp -I $(PROJECT_DIR)/rapidjson
OPTFLAGS = -O2
DBGFLAGS = -O0 -g -DDEBUG
LDFLAGS = -static
CXX := $(COMPILER_PREFIX)g++

OBJS := main.o wsserver.o

LIBS = $(SYSROOT)/lib/libc.a $(SYSROOT)/usr/lib/libstdc++.a $(SYSROOT)/usr/lib/libX11.a

TARGET = intervalometer

all: release

debug: CXXFLAGS := $(DBGFLAGS) $(CXXFLAGS)
debug: $(TARGET)

release: CXXFLAGS := $(OPTFLAGS) $(CXXFLAGS)
release: $(TARGET)
	$(STRIP) $(STRIPFLAGS) $(TARGET)

$(TARGET): $(STRIP) $(PROJECT_DIR)/websocketpp/websocketpp/version.hpp $(OBJS)
	$(CXX) -o $(TARGET) $(LDFLAGS) $(OBJS) $(LIBS)

$(PROJECT_DIR)/websocketpp/websocketpp/version.hpp:
	$(PROJECT_DIR)/get-wspp.sh

$(STRIP):
	$(PROJECT_DIR)/do-buildroot.sh

clean:
	rm -f $(OBJS) $(TARGET)
