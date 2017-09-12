PROJECT_DIR = $(dir $(realpath $(firstword $(MAKEFILE_LIST))))
COMPILER_PREFIX := $(PROJECT_DIR)/buildroot/output/host/usr/bin/arm-buildroot-linux-musleabi-
SYSROOT := $(PROJECT_DIR)/buildroot/output/host/arm-buildroot-linux-musleabi/sysroot
PKGCONFIG := $(PROJECT_DIR)/buildroot/output/host/usr/bin/pkg-config
FLKTCONFIG := $(SYSROOT)/usr/bin/fltk-config
STRIP = $(COMPILER_PREFIX)strip
STRIPFLAGS = --strip-all
CXXFLAGS = -std=c++11 -pedantic -Wall -Wno-format -Wno-unused-result \
           -fmessage-length=0 -static -D DO_FLTK_EXAMPLE \
           -I $(PROJECT_DIR)/websocketpp -I $(PROJECT_DIR)/rapidjson/include \
           -I $(SYSROOT)/usr/include
OPTFLAGS = -O2
DBGFLAGS = -O0 -g -DDEBUG
LDFLAGS = -static
CXX := $(COMPILER_PREFIX)g++

OBJS := main.o #startwsserver.o
LIBS = $(SYSROOT)/lib/libc.a $(SYSROOT)/usr/lib/libstdc++.a \
       $(SYSROOT)/usr/lib/libfltk.a $(SYSROOT)/usr/lib/libfltk_forms.a \
       $(SYSROOT)/usr/lib/libfltk_images.a $(SYSROOT)/usr/lib/libXft.a \
       $(SYSROOT)/usr/lib/libfontconfig.a $(SYSROOT)/usr/lib/libfreetype.a \
       $(SYSROOT)/usr/lib/libexpat.a $(SYSROOT)/usr/lib/libX11.a \
       $(SYSROOT)/usr/lib/libxcb*.a $(SYSROOT)/usr/lib/libXcursor.a \
       $(SYSROOT)/usr/lib/libX11.a $(SYSROOT)/usr/lib/libXcomposite.a \
       $(SYSROOT)/usr/lib/libXdamage.a $(SYSROOT)/usr/lib/libXext.a \
       $(SYSROOT)/usr/lib/libXfixes.a $(SYSROOT)/usr/lib/libXinerama.a \
       $(SYSROOT)/usr/lib/libXrandr.a $(SYSROOT)/usr/lib/libXau.a\
       $(SYSROOT)/usr/lib/libXtst.a $(SYSROOT)/usr/lib/libXi.a \
       $(SYSROOT)/usr/lib/libXft.a  $(SYSROOT)/usr/lib/libXrender.a \
       $(SYSROOT)/usr/lib/libXfont.a $(SYSROOT)/usr/lib/libbz2.a \
       $(SYSROOT)/usr/lib/libXdmcp.a $(SYSROOT)/usr/lib/libfontconfig.a \
       $(SYSROOT)/usr/lib/libpng16.a $(SYSROOT)/usr/lib/libz.a

TARGET = intervalometer

all: release

debug: CXXFLAGS := $(DBGFLAGS) $(CXXFLAGS)
debug: $(TARGET)

release: CXXFLAGS := $(OPTFLAGS) $(CXXFLAGS)
release: $(TARGET)
	$(STRIP) $(STRIPFLAGS) $(TARGET)

$(TARGET): $(STRIP) $(PROJECT_DIR)/websocketpp/websocketpp/version.hpp $(PROJECT_DIR)/rapidjson/include/rapidjson/rapidjson.h $(OBJS)
	$(CXX) -o $(TARGET) $(LDFLAGS) $(OBJS) $(LIBS)

$(PROJECT_DIR)/websocketpp/websocketpp/version.hpp:
	$(PROJECT_DIR)/get-wspp.sh

$(PROJECT_DIR)/rapidjson/include/rapidjson/rapidjson.h:
	$(PROJECT_DIR)/get-rapidjson.sh

$(STRIP):
	$(PROJECT_DIR)/do-buildroot.sh

clean:
	rm -f $(OBJS) $(TARGET)
