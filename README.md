# Intervalometer with web interface for Samsung NX500.

This app is for installing into the camera.

See https://github.com/ottokiksmaler/nx500_nx1_modding

It uses buildroot to make a modern toolchain with C++11 support capable of
cross-compile to the camera OS. The final binary is statically linked so no
libraries must be installed on camera.

If buildroot compiling fails at LuaJit with error asm/errno.h: No such file or directory
you can solve it by installing linux-libc-dev:i386 in your computer, for example
in ubuntu:

```
sudo apt install linux-libc-dev:i386
```
