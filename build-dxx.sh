#!/bin/sh
#export CXX=g++-7
#export CXXFLAGS=-Wno-uninitialized
export CXX=g++-10
export CXXFLAGS=-DDXX_USE_STEREOSCOPIC_RENDER=1
scons -j4 d1x=1 d2x=1 use_stereo_render=1
