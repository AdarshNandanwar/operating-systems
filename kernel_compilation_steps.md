# STEPS OF COMPILATION

1. Fresh install ubuntu virtual machine
2. Download latest kernel from kernel.org and setup

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
    ```
3. Recompilation of modified kernel
    ``` shell
    $ # makes default configuration file
    $ make defconfig
    $ make kvm_guest.config
    $ make -j$(nproc)
    $ make modules_install
    $ make modules_install install
    ```
4.  Get current kernel version and reboot
    ``` shell
    $ # get current kernel
    $ uname -r
    $ # reboot
    $ init 6
    ```