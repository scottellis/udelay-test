## udelay-test

### Overview

A linux module to find the practical resolution of the Linux udelay() 
function on the Gumstix Overo and Duovero boards. Should work for other
boards, but you'll have to find a gpio pin.

The test requires an oscope to do the measurements. 

For a Duovero, uncomment the appropriate section in `udelay.c` to use
pin 9 of the Parlor board header.

### Customization

The `udelay.c` code toggles `GPIO_170` which is easily accessible on the
Gumstix expansion boards and by default is already mux'd as gpio.

For a Duovero, uncomment the appropriate section in `udelay.c` to use
`GPIO_127` pin 9 of the Parlor board header. You'll probably have to
mux the pin as gpio. By default it's setup for `one-wire` use.

One way is like this

    root@duovero:~# echo 3 > /sys/kernel/debug/omap_mux/hdq_sio


If you are building for a different machine, you'll have to choose
another gpio pin and mux it similarly.

### Build

There is a file you can source to set up the environment for building using
the Yocto built tools configured.

Modify `yocto-env.sh` appropriately for your build environment.

    $ git clone git://github.com/scottellis/udelay-test.git
    $ cd udelay-test
    $ export OETMP=<TMPDIR-from-your-local.conf>
    $ export MACHINE=<overo or duovero or whatever>
    $ source yocto-env.sh
    $ make
 

### Run
 
Copy the udelay.ko module to the board, insert it.

    root@overo:~# insmod udelay.ko

To run a test, write the micro second delay you want and the driver will
toggle pin 19 on/off for two cycles, pausing the delay you asked for on
each change.

The acceptable range is 1 to 500.

For example, this will run a 5 us test.

    root@overo:~# echo 5 > /dev/udelay


Set your scope to trigger on a rising signal.

