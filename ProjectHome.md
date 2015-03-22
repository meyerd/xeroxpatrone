# Xeroxpatrone #
This is a tool for resetting the chip that is included in the Xerox Phaser 6130 Printer Cartridges. Since Xerox wants you to buy a new toner cartridges even if you are able to refill them, they included a little EEPROM microchip to prevent self refilling.

Now you can reset this little microchip inside the cartridge yourself.

**Warning: I think using this will definitely void the warranty!**

**UPDATES: Beware, that there might be a bug in the new XeroxpatroneV2 tool when writing chips with the EasyPC adapter. Until that issue is resolved, you might fall back to using the old command line software which also supports the Easy Adapter**

**UPDATES: Updated version 2 of the [xeroxpatrone-tool](XeroxpatroneV2.md). Now has a graphical user interfaces and supports a new [usb-only adapter](UsbOnlyAdapter.md).**

**[German/Deutsch](UebersichtDeutsch.md)**

## Other Printers/Parts ##
[A list of pointers to pages describing flashing other parts](OtherParts.md)

## EEPROMs ##
[Available and missing EEPROM dumps](EepromList.md)

## Using other cartridges ##
Yes you can use 6130 cartridges in your 6140 printer and vice versa. Also Xerox Phaser 6125, Xerox Phaser 6128, Dell 1320c, Dell 2130cn and Dell 2135cn are more or less the same. [Read here how it works](CartridgeModification.md).

## For which Printers does it work? ##
It definitely works for the **Xerox Phaser 6130**. I think it should also work for the **Xerox Phaser 6125**, **Xerox Phaser 6128**, **Dell 1320c**, **Dell 2130cn** and the **Dell 2135cn**, since they use the same cartridges. It was just tried out: The **Xerox Phaser 6140** uses exactly the same EEPROMS. Dumps for that printer can be found in the download section.

## How to do it? ##
First of all you need to have a i2c adapter for your PC. Something like [this (recommended)](EasyPCAdapter.md) or [this (for the full i2c functionality)](http://www.shop.robotikhardware.de/shop/catalog/product_info.php?products_id=68). You also can download the schematics there and build it yourself. I think you will most likely have to do that since you need some sort of adapter to attach to the tiny pcb where the EEPROM Chip is located (try out something like [this](http://xeroxpatrone.googlecode.com/files/Chip_connector.pdf)).

Then either [download](http://xeroxpatrone.googlecode.com/files/xeroxpatrone_bin_rel-z-diode_r48.zip) a precompiled (sorry, only windows) version of the reset tool. Or build it yourself.

With that tool you can now read out the data on the chip and write the same or, more important, modified data back onto the EEPROM. Use `xeroxpatrone --help` for further usage instructions.

If you have binary dumps for printers which are not listed on the site please [send](http://code.google.com/p/xeroxpatrone/people/list) them.

## Easy PC Adapter ##
[Really easy and cheap to build PC adapter](EasyPCAdapter.md). You will need this executable to read/write your EEPROMS [download](http://xeroxpatrone.googlecode.com/files/xeroxpatrone_bin_rel-z-diode_r48.zip)

## What else? ##
For effectively resetting a cartridge, you will need some EEPROM image of a full, new cartridge. Maybe I will eventually upload one here, or you have to read one image out yourself.

## Missing DLLs on Windows ##
  * "MSVCR100.dll missing" or "MSVCP100.dll missing" (Windows)
> > You need to install the Microsoft Visual C++ Redistributable, available for [Windows x86](https://www.microsoft.com/downloads/en/details.aspx?FamilyID=a7b7a05e-6de6-4d3a-a423-37bf0912db84&displaylang=en) and [Windows x64](https://www.microsoft.com/downloads/en/details.aspx?familyid=BD512D9E-43C8-4655-81BF-9350143D5867&displaylang=en).

## Any technical details ##
Yes.

The EEPROM chip is simply a `M24C02` serial i2c bus eeprom with 2KBit. There are some pull up resistors around, but thats it. You can use any way to read and write on that you want. Just use this information [here](http://xeroxpatrone.googlecode.com/files/eeprom_chip.png).

Someone testing it reported the programming went just fine with "JDM-Programmer".

## EEPROM dumps ##
I read out some EEPROM dumps from cartridges I own myself ([download here](http://code.google.com/p/xeroxpatrone/wiki/EepromList)). Feel free to use them and if you find something out about the encoding or the contents, maybe how to write a dump-"generator" to generate arbitrary fillings, please contact me.

If you get hold of any new dumps that are missing in the downloads section, please send them to me.

[People listing for mail addresses](http://code.google.com/p/xeroxpatrone/people/list)