# Cartridges for Ricoh Afico Printers #

The cartridges have some small pcb mounted. Excerpt of a mail describing the findings:

```
  The cartridge has a counter PCB. Mounted to the PCB is a M24C01WP 
  EEPROM from STMicroelectronics, wired for chip address 1 (A0 
  connected to VCC, A1 and A2 connected to GND).
  A connector with 2 mm pin spacing is used to connect the PCB 
  to the printer electronics. 

  The connections are:

  Pin   Signal
  --------------
   1    GND
   2    SDA
   3    SCL
   4    +5V
   5    GND
   6    GND
```

<img src='http://xeroxpatrone.googlecode.com/files/Ricoh_Aficio_AP610N_black_connection_pins01.jpg' border='0' width='50%'>

Please note, when flashing the cartridges, there seems to be a serial number encoded into it. You may have to change this. For example for the eeprom dump available <a href='http://xeroxpatrone.googlecode.com/files/Ricoh_Aficio_AP610N_black.bin'>here</a> has the serial number <code>A7270102061</code>.<br>
<br>
A hexdump of it looks like this:<br>
<br>
<pre><code> 00000000:  30 01 01 03 14 01 01 00  64 00 ff ff ff ff ff ff  0.......d.......<br>
 00000010:  32 30 31 30 30 31 20 41  37 32 37 30 32 30 36 31  201001 A72702061<br>
 00000020:  ff ff ff ff ff ff ff ff  ff ff ff ff 64 ff ff ff  ............d...<br>
 00000030:  ff ff ff ff ff ff ff ff  ff ff ff ff ff ff ff ff  ................<br>
 00000040:  ff ff ff ff ff ff ff ff  ff ff ff ff ff ff ff ff  ................<br>
 00000050:  ff ff ff ff ff ff ff ff  ff ff ff ff ff ff ff ff  ................<br>
 00000060:  ff ff ff ff ff ff ff ff  ff ff ff ff ff ff ff ff  ................<br>
 00000070:  ff ff ff ff ff ff ff ff  ff ff ff ff ff ff ff ff  ................<br>
</code></pre>