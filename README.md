# What is this

This small utility to control LEDs on keyboard under Linux. It uses /dev/input API.

## How to use

    control_kbd_led event_file c|n|s 0|1

Where:
* event_file is /dev/input/event* file
* led_num is 'c' for CAPS_LOCK, 'n' for NUM_LOCK and 's' for SCROLL_LOCK
* '1' to switch LED on, '0' to switch LED off.

## How to build

### Ubuntu Linux 10.04 LTS

```bash
gcc -I/usr/include -I/usr/src/linux-headers-`uname -r`/include/ -I/usr/src/linux-headers-`uname -r`/arch/x86/include -o control_kbd_led control_kbd_led.c
```

### OpenWRT 12.09-beta, r33312 (in buildroot)

1. Prepare OpenWRT buildroot.
   Here is a tutorial: http://wiki.openwrt.org/doc/howto/buildroot.exigence

   Also you will get toolchain within this buildroot.
2. Prepare build script with variables TOOLCHAIN_PATH and STAGING_DIR.
3. Build using gcc from toolchain.

My example of build script:

```bash
TOOLCHAIN_PATH=/fakeroot/OpenWRT/buildroot/attitude_adjustment/staging_dir/toolchain-mips_r2_gcc-4.6-linaro_uClibc-0.9.33.2
OPT_ROOT=/fakeroot/OpenWRT/buildroot/opt
export STAGING_DIR=$TOOLCHAIN_PATH
export CC=$TOOLCHAIN_PATH/bin/mips-openwrt-linux-gcc
export LD=$TOOLCHAIN_PATH/bin/mips-openwrt-linux-ld
export NM=$TOOLCHAIN_PATH/bin/mips-openwrt-linux-nm
export CXX=$TOOLCHAIN_PATH/bin/mips-openwrt-linux-c++
export AR=$TOOLCHAIN_PATH/bin/mips-openwrt-linux-ar
export OBJDUMP=$TOOLCHAIN_PATH/bin/mips-openwrt-linux-objdump
export RANLIB=$TOOLCHAIN_PATH/bin/mips-openwrt-linux-ranlib
export STRINGS=$TOOLCHAIN_PATH/bin/mips-openwrt-linux-strings
export STRIP=$TOOLCHAIN_PATH/bin/mips-openwrt-linux-strip
export AS=$TOOLCHAIN_PATH/bin/mips-openwrt-linux-as
export CFLAGS=-I$TOOLCHAIN_PATH/include
export LDFLAGS=-L$TOOLCHAIN_PATH/lib

$CC -I${TOOLCHAIN_PATH}/usr/include -o control_kbd_led control_kbd_led.c
$TOOLCHAIN_PATH/bin/mips-openwrt-linux-strip control_kbd_led
```

## How to find matching event file

See at /proc/bus/input/devices to find you input devices.

My example (find event file for certain keyboard (product_id=0x1603) from certain manufacterer (vendor_id=0x04d9)):
* grab all matching devices (more than one maybe)
* get all handler / LED info
* get only one 'handler' line before 'LED' line
* match info about event file

```bash
cat /proc/bus/input/devices | grep -A 10 'Vendor=04d9 Product=1603'| grep -E '^(H: Handlers=|B: LED=)'| grep -B1 -E '^B: LED='| grep '^H: Handlers='| grep -o -E 'event[0-9]+'
```
