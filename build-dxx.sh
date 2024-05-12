#!/bin/sh

# Mac OS X or Linux?
UNAME=`uname`
if [ $UNAME == "Darwin" ]
then
	CPUS=`sysctl -n hw.logicalcpu`
	JOBS="-j$((CPUS))"
	OPTS="macos_add_frameworks=false sdl2=true sconf_cxx20=1 use_stereo_render=1"
	export CXX=g++
	export CXXFLAGS=-Wno-uninitialized
else
	CPUS=`nproc`
	JOBS="-j$((CPUS))"
	OPTS="use_stereo_render=1"
	export CXX=g++-10
	export CXXFLAGS=-DDXX_USE_STEREOSCOPIC_RENDER=1
fi

echo "kernel=$UNAME, cpus=$CPUS, scons=$OPTS"
scons $JOBS $OPTS
