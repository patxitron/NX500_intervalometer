# Intervalometer for the "bulb" mode of Samsung NX500.

This app is to be installed into the modded camera.

See https://github.com/ottokiksmaler/nx500_nx1_modding

It uses buildroot to make a modern toolchain with C++14 support capable of
cross-compile to the camera OS. The final binary is statically linked so you
don't need to install any library in the camera.

If buildroot compiling fails at LuaJit with error asm/errno.h: No such file or directory
you can solve it by installing linux-libc-dev:i386 in your computer, for example
in ubuntu:

```
sudo apt install linux-libc-dev:i386
```

## NX Key mapping to xdo_send_keysequence_window* functions:

|NX KEY|key name||
|---|---|---|
|UP|KP_Up||
|LEFT|KP_Left||
|RIGHT|KP_Right||
|DOWN|KP_Down||
|DEL|KP_Delete||
|DEPTH|Henkan_Mode||
|METER|Hiragana_Katakana||
|OK|KP_Enter||
|PWON|XF86AudioRaiseVolume||
|PWOFF|XF86PowerOff||
|RESET|XF86PowerOff||
|S1|Super_L||
|S2|Super_R||
|MENU|Menu||
|AEL|XF86Favorites||
|REC|XF86WebCam||
|FN|XF86HomePage||
|EV|XF86WWW|NX1|
|EV|XF86Reload|NX500,NX300|
|PB|XF86Tools||
|AF_MODE|XF86TaskPane||
|WB|XF86Launch6||
|ISO|XF86Launch7||
|AF_ON|XF86Launch9||
|LIGHT|XF86TouchpadToggle||
|MF_ZOOM|XF86TouchpadOff||
|WIFI|XF86Mail||
|JOG1_CW|XF86ScrollUp||
|JOG1_CCW|XF86ScrollDown||
|JOG2_CW|parenleft||
|JOG2_CCW|parenright||
|JOG_CW|XF86AudioNext||
|JOG_CCW|XF86AudioPrev||
|MODE_WIFI|F3|NX300|
|MODE_SCENE_GET|F4||
|MODE_SCENE|XF86Send||
|MODE_SMART_GET|F6||
|MODE_SMART|XF86Reply||
|MODE_P_GET|F7||
|MODE_P|XF86MailForward||
|MODE_A_GET|F8||
|MODE_A|XF86Save||
|MODE_S_GET|F9||
|MODE_S|XF86Documents||
|MODE_M_GET|F10||
|MODE_M|XF86Battery||
|MODE_CUSTOM1_GET|KP_Home||
|MODE_CUSTOM1|XF86WLAN||
|MODE_CUSTOM2_GET|Scroll_Lock||
|MODE_CUSTOM2|XF86Bluetooth||
|MODE_SAS_GET|F1||
|MODE_SAS|XF86KbdBrightnessDown||
|WHEEL_CW|Left||
|WHEEL_CCW|Right||
|DRIVE_SINGLE|XF86Search||
|DRIVE_CONTI_N|XF86Go||
|DRIVE_CONTI_H|XF86Finance||
|DRIVE_TIMER|XF86Game||
|DRIVE_BRACKET|XF86Shop||
