# operating-system
A small side project where I explore some concepts and the very basics of operating system kernels.

This is mainly written in C++ and runs on an x86-64 CPU.  It can be loaded by GRUB via multiboot, and it should
work when run in QEMU, see emu.sh (you may need to adapt that script to your system).

Right now, the main function of the kernel does not much more than some silly stuff and some screen output before halting.

For building, it is recommended that you use an i686-elf-gcc cross-compiler.

