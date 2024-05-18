#!/bin/sh

LLVM=`llvm-config --version`
if [ $LLVM != "11.0.0" ]
then
	./fixup-llvm-11.sh
fi

cd mesa
if [ ! -d build ]
then
	mkdir build
	cd build
	meson
else
	cd build
	ninja
	sudo ninja install
fi

