# Intervalometer for the "bulb" mode of Samsung NX500.

This app is to be installed into the modded camera.

See [NX500 NX1 modding project](https://github.com/ottokiksmaler/nx500_nx1_modding)

It uses buildroot to make a modern toolchain with C++14 support capable of
cross-compile to the camera OS. The final binary is statically linked so you
don't need to install any additional library in the camera.

If buildroot compiling fails at LuaJit with error asm/errno.h: No such file or directory
you can solve it by installing linux-libc-dev:i386 in your computer, for example
in ubuntu:

```
sudo apt install linux-libc-dev:i386
```

# Camera modding information

This info is a brief compilation of information from
[NX500 NX1 modding project](https://github.com/ottokiksmaler/nx500_nx1_modding)
and from
[NX remote controller mod project](https://github.com/mewlips/nx-remote-controller-mod).

## Get and set exposure

```
/usr/bin/st key push s1; /usr/bin/st key release s1; /usr/bin/st cap lockinfo
```

You should pay attention for a few first lines:

```
----------------------------------------------------------
* Av            :128, FnoNum       :160, FnoDen   :10 
* Tv            :85, ShSpdNum     : 1, ShSpdDen :40 
* DeltaEv       :-4 
```

```FnoNum``` and ```FnoDen``` are the f setting (f/16) in this case.

```ShSpdNum``` and ```ShSpdDen``` are the exposure time, 1/40 in this case.

```DeltaEv``` is the evaluated over/under exposure, 0 being a perfectly exposed
picure, negative an under exposed picture and positive is overexposing.

The scale used in ```DeltaEv``` is rather strange. The gauge in the screen goes
from -5 to 5. When the screen gauge is in the -5 mark, the ```DeltaEv``` param
shows a value of about -80. The example value of -4 has been got when the screen
gauge was in the 0 mark.

Unfortunately there is no mean of differentiate an exposure setting of 30 seconds
( ```ShSpdNum``` = 300 and ```ShSpdDen``` = 10 ) from the "Bulb" mode.

You can increase the exposure time by sending a wheel 1 counter-clockwise event:

```st key jog jog1_ccw```

and drecrease it sending a clockwise event:

```st key jog jog1_cw```

## Exposure table

This is the discrete exposure values that can be set in NX500:

* 1/6000 (0.0001667)
* 1/5000 (0.0002)
* 1/4000 (0.00025)
* 1/3200 (0.0003125)
* 1/2500 (0.0004)
* 1/2000 (0.0005)
* 1/1600 (0.000625)
* 1/1250 (0.0008)
* 1/1000 (0.001)
* 1/800 (0.00125)
* 1/640 (0.0015625)
* 1/500 (0.002)
* 1/400 (0.0025)
* 1/320 (0.003125)
* 1/250 (0.004)
* 1/200 (0.005)
* 1/160 (0.00625)
* 1/125 (0.008)
* 1/100 (0.01)
* 1/80 (0.0125)
* 1/60 (0.01667)
* 1/50 (0.02)
* 1/40 (0.025)
* 1/30 (0.0337)
* 1/25 (0.04)
* 1/20 (0.05)
* 1/15 (0.0667)
* 1/13 (0.076923)
* 1/10 (0.1)
* 1/8 (0.125)
* 1/6 (0.1667)
* 1/5 (0.2)
* 1/4 (0.25)
* 0.3
* 0.4
* 0.5
* 0.6
* 0.8
* 1.0
* 1.3
* 1.6
* 2.0
* 2.5
* 3
* 4
* 5
* 6
* 8
* 10
* 13
* 15
* 20
* 25
* 30
* Bulb

In "Bulb" mode, the exposure time is limited to 8 minutes (480 seconds). After
that time, the exposure is finished even if the shutter button is not released.

## Take a photo

When not in "Bulb" mode, you can take a photo by issuing the following command:

```st app nx capture single```

In "Bulb" mode you must do it by sending keystrokes:

```
/usr/bin/st key push s1 && /bin/sleep 0.3 && /usr/bin/st key push s2
/bin/sleep <Exposure time in seconds>
/usr/bin/st key release s2 && /bin/sleep 0.3 && /usr/bin/st key release s1
```

This sequence half-presses the shutter button (s1) then presses it completely (s2)
then, after the desired exposure time has been passed, first releases the
full press and then releases the half press.

## Camera screen resolution

The camera screen resolution of the NX500 model is 720x480 pixels.

## Shutter mode

You can fully disable the mechanical shutter, avoiding vibrations, shutter wearing
and noise.

### Disable mechanical shutter

```/usr/bin/st cap capdtm setusr 117 0x00750001```

This setting does not survive a reboot.

In the [ottokiksmaler page](https://github.com/ottokiksmaler/nx500_nx1_modding/blob/master/NX500%20Silent%20shooting.md)
you can read **"You can set any shutter speed you want, but if it's under 1/30s it
will be 1/30s - no long exposures"**. In my own researches I found that you can get
exposures down to 1/15s, but bellow that setting, there is no change in exposure.

In "Bulb" mode, the mechanical shutter is always used despite this setting.

### Enable mechanical shutter

```/usr/bin/st cap capdtm setusr 117 0x00750000```

### Check the mechanical shutter setting

```/usr/bin/st cap capdtm getusr 117``` returns ```UserData is ADJUSTSHUTTERTYPE_MECHA (0x750000)```
when mechanical shutter is enabled and ```UserData is ADJUSTSHUTTERTYPE_ROLLING (0x750001)```
when it is disabled.

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
