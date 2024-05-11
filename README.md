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
boards which featured “mode X” for accessing all 4 pages of 64KB video memory. This special VGA
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

