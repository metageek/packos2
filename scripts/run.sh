#!/bin/bash

foundSelf=`type -path "$0"`
scriptDir=`dirname "$foundSelf"`
toplevel=`dirname "$scriptDir"`

imgsDir="$toplevel"/imgs

tmpImg=`mktemp --tmpdir="$imgsDir"`

cp "$imgsDir"/floppy.img "$tmpImg"

e2cp "$1" "$tmpImg":/kernel

qemu-system-i386 -s -d int,pcall -fda "$tmpImg" -hda "$imgsDir"/hd-grub.img -boot c

rm "$tmpImg"
