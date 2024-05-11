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
 
