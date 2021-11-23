#!/bin/sh

if (test -z $OVMF_PATH); then
    if (test -f /usr/share/edk2-ovmf/x64/OVMF.fd); then
        OVMF_PATH=/usr/share/edk2-ovmf/x64
    elif (test -f /usr/share/qemu/OVMF.fd); then
        OVMF_PATH=/usr/share/qemu
    else
        OVMF_PATH=/
    fi
fi

mkdir -p /tmp/efi
sudo mount /tmp/efi.disk /tmp/efi
sudo cp wolfboot.efi /tmp/efi
sudo umount /tmp/efi

qemu-system-x86_64 \
    -m 256M \
    -net none \
    -bios ${OVMF_PATH}/OVMF.fd \
    -drive file=/tmp/efi.disk,index=0,media=disk,format=raw \
    -vga none \
    -serial stdio \
    -display none
