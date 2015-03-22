# Introduction #

This is the information page about the new Xeroxpatrone v2 program.

In addition to the previous versions it supports:
  * [EasyPCAdapter](EasyPCAdapter.md) and the old COM-Port PC Adapter at the same time (Windows only)
  * The [new USB Adapter](UsbOnlyAdapter.md) via libusb and the i2c-tiny-usb kernel driver
  * Graphical User Interface based on wxWidgets

It can be downloaded here for [Windows](https://xeroxpatrone.googlecode.com/files/xeroxpatronev2_win_x86.zip) (x86) and [Linux](https://xeroxpatrone.googlecode.com/files/xeroxpatrone_linux_x64_dynamic.zip) (x64, dynamically linked).
The sourcecode is available in the SVN repository, to compile it you will need the wxWidgets and libusb libraries.

# Usage #

The usage should be pretty intuitive. On linux it may be neccessary to start the probram with root permissions, or to adjust the device access rights when using the usb kernel driver.

If you started the program and plugged in the usb device afterwards, it's not automatically detected. Just select "File->Scan for devices again ..."

# FAQ #

  * "MSVCR100.dll missing" or "MSVCP100.dll missing" (Windows)
> > You need to install the Microsoft Visual C++ Redistributable, available for [Windows x86](https://www.microsoft.com/downloads/en/details.aspx?FamilyID=a7b7a05e-6de6-4d3a-a423-37bf0912db84&displaylang=en) and [Windows x64](https://www.microsoft.com/downloads/en/details.aspx?familyid=BD512D9E-43C8-4655-81BF-9350143D5867&displaylang=en).
  * "libusb0.dll missing" (Windows)
> > You need to install the [driver](https://xeroxpatrone.googlecode.com/files/i2c-tiny-usb_driver.zip) for the tinyusb device.