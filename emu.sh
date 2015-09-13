#!/bin/sh
KERNEL=bin/kernel
gnome-terminal --geometry=80x25 -e "qemu-system-x86_64 -kernel $KERNEL -curses"

