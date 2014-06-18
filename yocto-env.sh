# You need two environment variables defined before you source this
# OETMP to point to the Yocto local.conf TMPDIR
# MACHINE for finding the kernel source dir, you probably want overo
# or duovero

if [[ -z "${OETMP}" ]]; then
	echo "Need an OETMP defined!"
	exit 1
fi

if [[ -z "${MACHINE}" ]]; then
	echo "Need a MACHINE defined!"
	exit 1
fi

SYSROOTSDIR=${OETMP}/sysroots
STAGEDIR=${SYSROOTSDIR}/`uname -m`-linux/usr

export KERNELDIR=${SYSROOTSDIR}/${MACHINE}/usr/src/kernel

# The directory to add to your PATH depends on your build
# options: hard vs soft float, overo or duovero 
# Look at your system and choose appropriately

# overo or duovero, soft-float
PATH=${PATH}:${STAGEDIR}/bin:${STAGEDIR}/bin/armv7a-vfp-neon-poky-linux-gnueabi

# overo, hard-float
#PATH=${PATH}:${STAGEDIR}/bin:${STAGEDIR}/bin/cortexa8hf-vfp-neon-poky-linux-gnueabi

# duovero, hard-float
#PATH=${PATH}:${STAGEDIR}/bin:${STAGEDIR}/bin/cortexa9hf-vfp-neon-poky-linux-gnueabi

unset CFLAGS CPPFLAGS CXXFLAGS LDFLAGS MACHINE

export ARCH="arm"
export CROSS_COMPILE="arm-poky-linux-gnueabi-"
export CC="arm-poky-linux-gnueabi-gcc"
export LD="arm-poky-linux-gnueabi-ld"
export STRIP="arm-poky-linux-gnueabi-strip"
export AR="arm-poky-linux-gnueabi-ar"

