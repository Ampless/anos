#!/usr/bin/env python3

from sys import argv

def lookup_arch(arch):
    if arch == 'i386':
        return 0
    else:
        # TODO: read through the whole spec
        raise Exception('Does multiboot even support anything but i386?')

def make_checksum(magic, arch, length):
    return 0x100000000 - (magic + arch + length)

def dw(w):
    print('dw ' + str(w))

def dd(d):
    print('dd ' + str(d))

def tag(tag_type, flags, size):
    dw(tag_type)
    dw(flags)
    dd(size)

def end_tag():
    tag(0, 0, 8)

magic = 0xE85250D6
arch = lookup_arch(argv[1])
length = 16 # TODO: calc
checksum = make_checksum(magic, arch, length)

print('section .multiboot2')
print('mb2_start:')
dd(magic)
dd(arch)
dd(length)
dd(checksum)
end_tag()
print('mb2_end:')
