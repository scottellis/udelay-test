  udelay-test
=============

Overview
-------

A linux module to find the practical resolution of the Linux udelay() 
function on an OMAP3 Gumstix board.

The test requires an oscope to do the measurements. 

I used a convenient pin on the Gumstix Overo Tobi expansion header, pin 19.
This pin shouldn't require any mux'ing on a default kernel.


Build
-------

There is a file you can source to set up the environment for building using
the Yocto built tools configured.

    $ git clone git://github.com/scottellis/udelay-test.git
    $ cd udelay-test
    $ export OETMP=<TMPDIR-from-your-local.conf>
    $ source yocto-env.sh
    $ make
 

Run
-------
 
Copy the udelay.ko module to the board, insert it.

    root@overo:~# insmod udelay.ko

To run a test, write the micro second delay you want and the driver will
toggle pin 19 on/off for two cycles, pausing the delay you asked for on
each change.

The acceptable range is 1 to 500.

For example, this will run a 5 us test.

    root@overo:~# echo 5 > /dev/udelay


Set your scope to trigger on a rising signal.

