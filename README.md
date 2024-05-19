**Descent Stereo Revisited**

The following notes chronicle steps which retrace adding stereo support for the Descent video game.
This repo is an overview meant to wrap other git repos containing their respective source code.  

**Background History**

During the 1990s there were multiple attempts by multiple parties at adding stereoscopic display support
to various 3D games and apps. One stereoscopic device by StereoGraphics called the “sync doubler”
input a display image rendered in stereo above/below format and output sequential left/right image fields 
for viewing with stereoscopic eyewear through LCS liquid crystal shutters. This sync doubler device was
modeled after the fullscreen stereo display mode in above/below format on many low-end SGI graphics
workstations.

StereoGraphics also offered an inexpensive pair of eyewear which depended on alternating left/right
display fields in page-flipping mode. Game apps hosted on MS-DOS at the time used VGA graphics
boards which featured “mode X” for accessing all 4 planes of 64KB video memory. This special VGA
video memory configuration allowed support for quad-buffered stereoscopic page-flipping modes, 
as supported via OpenGL on higher-end SGI graphics workstations. 

Graphics chipset and board vendors at the time were competing to release 3D accelerators for
rendering graphics, so many DOS/4GW-hosted 3D games like Descent were ported to use graphics
libraries for each respective accelerator. Some graphics chipset vendors like Rendition included
stereo page-flipping support in their hardware display controllers, since they employed former
engineers who previously worked for SGI. Other startup vendors like nVidia opted to support
stereo above/below format, though was not as successful getting acceptance. 
 
**Descent 1.x**

The original Descent game by Parallax Software was distributed by Interplay Productions who
made OEM versions of their games bundled with various graphics board vendors. This included
various stereoscopic eyewear vendors too like StereoGraphics. While the retail version of Descent 
would work with any VGA graphics boards, special OEM bundled versions were ported to S3,
Rendition, and 3DFX graphics accelerator chipsets.

Stereo display support had already been implemented by others, but with inferior stereo viewing
effects. StereoGraphics consultant Bob Akka fixed the stereo viewing support with the preferred
parallel-projection method, and enlisted me to assist with VGA graphics mode support. Another
stereo page-flip driver called LCDBIOS had been developed independently by Don Sawdai and
included on retail releases. OEM versions of Descent used external library functions for stereo 
page-flipping support, like SVRDOS32 for S3 Virge, or hardware page-flip registers for Rendition Verite.

Someone from Parallax Software posted the Descent 1.5 source with stereo support options
intact with external LCDBIOS driver. I re-added support for SVRDOS32 library as an additional
stereo support option `—simuleyes` alternative to LCDBIOS. The SVRDOS32 library uses
the LCDBIOS interrupt handlers internally, though additionally allows support for VESA
graphics modes instead of VGA, including SciTech UniVBE VESA BIOS extensions for
linear framebuffer modes and refresh rate control.

Descent 1.5 legacy only runs on DOS/4GW hosts so gets built with Watcom C compiler.
A batch file `maked1.bat` is used for building instead of the buggy makefile.

```
cd descent
maked1
```

**Descent Rebirth**

The Descent Rebirth project ‘dxx-rebirth’ has upgraded the Descent1 & Descent 2 sources
to use C++ extensively for code re-organization and OpenGL for rendering output.

This looked like a great place to re-port the stereo display support from Descent 1.5 source,
and use GL_STEREO quad-buffering for stereo rendering on capable graphics boards like Radeon.
This option is limited to select stereo-capable hardware and only gets exposed through the
Mesa / GL stack when a stereo-capable visual gets selected via glxChooseVisual(). Such
stereo conditionals are enabled via command line option `-gl_stereo`.

```
./d1x-rebirth/d1x-rebirth -gl_stereo
```

Adding stereo viewport formats for above/below sync-doubler and side/by/side HMDs was not
that much extra effort for initial submission. The stereo viewport options were selected by a
separate command line option `-gl_stereoview <n>`, where ’n’ selects the above/below or
side/by/side viewport format enumerations.

```
./d2x-rebirth/d2x-rebirth -gl_stereoview 4
```

However since the original Descent legacy code was setup to switch video modes between 
game play mode vs menu mode or automap mode, the stereo viewport format modes were not
immediately accessible outside of normal game play mode without mangling menus or automaps.
So additional code modifications were submitted just to support stereo viewports with popup
menus and automaps for consistant viewing experience. This additional code is a bit messier and
reminiscent for similar challenges for UI support on graphics workstations which only supported
above/below stereo format.

When building on MacBook Pro for Catalina:
Needed g++10, Python 3.11, scons 4.5, SDL 2.x.

```
brew install scons
brew install sdl2 sdl2_image sdl2_mixer physfs
brew bundle
..
cd dxx-rebirth
export CXXFLAGS=-Wno-uninitialized
scons -j8 macos_add_frameworks=false sdl2=true sconf_cxx20=1 use_stereo_render=1
```

When building on Linux Ubuntu 20.04:
Needed g++10.5, Python 3.8.10, scons 3.1.2

```
apt install g++10 python3 scons
apt install libsdl1.2-dev libsdl-image1.2-dev libsdl-mixer1.2-dev libphysfs-dev
..
cd dxx-rebirth
export CXX=g++10
export CXXFLAGS=-DDXX_USE_STEREOSCOPIC_RENDER=1
scons -j4 d1x=1 d2x=1 use_stereo_render=1
```

**Mesa GL Support**

Building Mesa is fairly straightforward by following the instructions.
Needed to add library package dependencies to complete `meson` configuration.
Compiler is LLVM and needed v11.

```
apt install meson
apt install llvm-11
apt install libelf-dev
apt install wayland wayland-protocols waylandpp-dev
apt install libwayland-egl libwayland-egl1-mesa libwayland-egl-backend
apt install xcb libxcb-glx0-dev
```

Needed to fixup symlinks to llvm-11 default, since llvm-10 had been installed previously.

```
./fixup-llvm-11.sh
llvm-config —version
```

```
cd mesa
mkdir build
cd build
meson
ninja
sudo ninja install
```

Recompiled Mesa libraries will have been deployed to /usr/local/lib, so overide LD_LIBRARY_PATH when launching GL apps.
DRI drivers for Radeon graphics chipsets are in /dri subdirectory.

```
ls -ltr /usr/local/lib
ls -ltr /usr/local/lib/x86_64-linux-gnu
ls -ltr /usr/local/lib/x86_64-linux-gnu/dri
```
```
export LD_LIBRARY_PATH=/usr/local/lib/x86_64-linux-gnu/
glxinfo | grep OpenGL
```

