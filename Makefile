$(shell mkdir -p build >/dev/null)
$(shell rm -f build/main.o)
PROJECT_DIR = $(dir $(realpath $(firstword $(MAKEFILE_LIST))))
COMPILER_PREFIX := $(PROJECT_DIR)/buildroot/output/host/usr/bin/arm-buildroot-linux-musleabi-
SYSROOT := $(PROJECT_DIR)/buildroot/output/host/arm-buildroot-linux-musleabi/sysroot
PKGCONFIG := $(PROJECT_DIR)/buildroot/output/host/usr/bin/pkg-config
FLKTCONFIG := $(SYSROOT)/usr/bin/fltk-config
STRIP = $(COMPILER_PREFIX)strip
STRIPFLAGS = --strip-all
CXXFLAGS = -std=c++14 -pedantic -Wall -Wno-format -fmessage-length=0 -static $(DEFINES) \
           -I $(PROJECT_DIR)/xdotool -I $(PROJECT_DIR)/beast/include \
           -I$(PROJECT_DIR)/json/src -I $(SYSROOT)/usr/include
OPTFLAGS = -Os
DBGFLAGS = -O0 -g -DDEBUG
DEPFLAGS = -MT $@ -MMD -MP -MF build/$*.Td
LDFLAGS = -static
CXX := $(COMPILER_PREFIX)g++
APP_BUILD_TIME := $(shell date -u +'%Y-%m-%d %H:%M:%S UTC')
APP_BUILD_UUID := $(shell uuid)
APP_GIT_COMMIT := $(shell git rev-parse --short HEAD)

TARGET = intervalometer
APP_VERSION := "2.0a"

DEFINES = -D_BSD_SOURCE -DAPP_NAME="\"$(TARGET)\"" \
          -DAPP_BUILD_UUID="\"$(APP_BUILD_UUID)\"" \
          -DAPP_BUILD_TIME="\"$(APP_BUILD_TIME)\"" \
          -DAPP_VERSION="\"$(APP_VERSION)\"" \
          -DAPP_GIT_COMMIT="\"$(APP_GIT_COMMIT)\""

SRCFILES := main.cpp spinner.cpp uintspinner.cpp intervalometer-ui.cpp shutter.cpp \
            daemon.cpp setup-ui.cpp
SOURCES := $(patsubst %.cpp,src/%.cpp,$(SRCFILES))
OBJS := $(patsubst %.cpp,build/%.o,$(SRCFILES))
LIBS = xdotool/libxdo.a $(SYSROOT)/lib/libc.a $(SYSROOT)/usr/lib/libstdc++.a \
       $(SYSROOT)/usr/lib/libfltk.a $(SYSROOT)/usr/lib/libfltk_forms.a \
       $(SYSROOT)/usr/lib/libfltk_images.a $(SYSROOT)/usr/lib/libXft.a \
       $(SYSROOT)/usr/lib/libfontconfig.a $(SYSROOT)/usr/lib/libfreetype.a \
       $(SYSROOT)/usr/lib/libexpat.a $(SYSROOT)/usr/lib/libX11.a \
       $(SYSROOT)/usr/lib/libxkbcommon.a $(SYSROOT)/usr/lib/libxcb*.a \
       $(SYSROOT)/usr/lib/libXcursor.a $(SYSROOT)/usr/lib/libX11.a \
       $(SYSROOT)/usr/lib/libXcomposite.a $(SYSROOT)/usr/lib/libXdamage.a \
       $(SYSROOT)/usr/lib/libXext.a $(SYSROOT)/usr/lib/libXfixes.a \
       $(SYSROOT)/usr/lib/libXinerama.a $(SYSROOT)/usr/lib/libXrandr.a \
       $(SYSROOT)/usr/lib/libXau.a $(SYSROOT)/usr/lib/libXtst.a \
       $(SYSROOT)/usr/lib/libXi.a $(SYSROOT)/usr/lib/libXft.a  \
       $(SYSROOT)/usr/lib/libXrender.a $(SYSROOT)/usr/lib/libXfont.a \
       $(SYSROOT)/usr/lib/libbz2.a $(SYSROOT)/usr/lib/libXdmcp.a \
       $(SYSROOT)/usr/lib/libfontconfig.a $(SYSROOT)/usr/lib/libpng16.a \
       $(SYSROOT)/usr/lib/libz.a $(SYSROOT)/usr/lib/libboost_system.a \
       $(SYSROOT)/usr/lib/libboost_regex.a


all: release

debug: CXXFLAGS := $(DBGFLAGS) $(CXXFLAGS)
debug: $(TARGET)

release: CXXFLAGS := $(OPTFLAGS) $(CXXFLAGS)
release: $(TARGET)
	$(STRIP) $(STRIPFLAGS) $(TARGET)

build/%.o: src/%.cpp
build/%.o: src/%.cpp build/%.d
	$(CXX) $(DEPFLAGS) $(CXXFLAGS) -c $< -o $@
	@mv -f build/$*.Td build/$*.d && touch $@

$(TARGET): $(STRIP) $(PROJECT_DIR)/xdotool/libxdo.a $(PROJECT_DIR)/beast/include/boost/beast.hpp $(PROJECT_DIR)/json/src/json.hpp $(OBJS)
	$(CXX) -o $(TARGET) $(LDFLAGS) $(OBJS) $(LIBS)

$(PROJECT_DIR)/xdotool/libxdo.a:
	$(PROJECT_DIR)/do-submodule.sh xdotool

$(PROJECT_DIR)/beast/include/boost/beast.hpp:
	$(PROJECT_DIR)/do-submodule.sh beast

$(PROJECT_DIR)/json/src/json.hpp:
	$(PROJECT_DIR)/do-submodule.sh json

$(STRIP):
	$(PROJECT_DIR)/do-submodule.sh buildroot

build/%.d: ;
.PRECIOUS: build/%.d

clean:
	rm -f build/* $(TARGET) *~ src/*~ core

-include $(OBJS:.o=.d)
