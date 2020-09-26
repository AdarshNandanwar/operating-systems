# STEPS OF COMPILATION

1. Fresh install ubuntu virtual machine
2. download latest kernel from kernel.org
3. 
    ``` shell
    $ sudo su
    $ mv Downloads/linux-5.8.12.tar.xz /usr/src/
    $ cd /usr/src/
    $ tar -xvf linux-5.8.12.tar.xz
    $ sudo apt install libncurses5-dev libssl-dev ccache flex bison libelf-dev
    $ sudo apt install build-essential fakeroot
    $ sudo apt install qemu qemu-system
    $ cd linux-5.8.12
    $ # menuconfig not really needed, just for fun
    $ make menuconfig 
    $ # makes default configuration file
    $ make defconfig
    $ make kvm_guest.config
    $ make -j$(nproc)
    $ make modules_install
    $ make modules_install install
    $ # get current kernel
    $ uname -r
    $ # reboot
    $ init 6
    ```