# Xeroxpatrone #
Hier finden Sie ein Tool zum resetten ihrer Xerox Phaser 6130 Tonerkartuschen.

**Warnung: Es besteht danach wahrscheinlich kein Garantieanspruch mehr. Ich denke vor allem nicht mehr auf die Kartuschen selbst.**

## Welche Drucker werden unterstützt? ##
Es sollte definitv mit dem Xerox Phaser 6130 funktionieren. Xerox Phaser 6125 und Dell 1320c sollten auch funktionieren, da diese die gleichen Kartuschen verwenden.

Xerox Phaser 6140 funktionert auch. EEPROM Dumps befinden sich bei den Downloads. Über fehlende Dumps per Mail freue ich mich natürlich.

## Andere Kartuschen in meinem Drucker verwenden ##
Man kann auch Xerox 6130 Kartuschen in seinem 6140 Drucker verwenden, oder umgekehrt. Kartuschen für den Xerox Phaser 6125, Xerox Phaser 6128, Dell 1320c, Dell 2130cn und den Dell 2135cn funktionieren auch. Das ganze aber nur nach einer kleinen [Hardwaremodifikation](KartuschenModifikationen.md).

## Wie geht das? ##
Man benötigt für das Resetten einen i2c Adapter für den PC. Zum Beispiel so einen [so einen](http://www.shop.robotikhardware.de/shop/catalog/product_info.php?products_id=68). Auf der Seite kann man auch die Schaltpläne herunterladen und sich selbst einen bauen.

Alternativ kann man auch diesen sehr einfachten Adapter bauen (was ich empfehler würde):
[Really easy and cheap to build PC adapter](EasyPCAdapter.md)

Für die Verbindung zwischen Adapter und Chip-Platine kann man zum Beispiel den Stecker Lumberg 302299 04 K01 verwenden. [Hier](http://xeroxpatrone.googlecode.com/files/Chip_Stecker.pdf) gibt es dazu noch Details und Bilder zum Nachlesen.

Dann die [Software](http://xeroxpatrone.googlecode.com/files/xeroxpatrone_bin_rel-z-diode_r48.zip) herunterladen (binär aktuell nur für Windows). Oder selbst kompilieren. Oder für ein eigenes Tool aus dem Sourcecode klauen.

Mit dem Tool kann man nun den EEPROM auf dem Chip in der Kartusche beschreiben und auslesen.

## Das ist ja viel zu kompliziert. ##
Ja. Leider gehts nicht einfacher. Eventuell habe ich Lust Ihnen zu helfen und resette die Chips für sie. Kontaktieren sie mich einfach per EMail (Adresse finden sie unter den Kontaktdetails (Project Home->People->Owner)).

## Technische Details? ##
Der EEPROM ist ein `M24C02` serieller i2c mit 2Kbit Speicher. Um sich selbst was zu hacken kann man [hierauf](http://xeroxpatrone.googlecode.com/files/eeprom_chip.png) zurückgreifen.

Es wurde auch berichtet, dass der "JDM-Programmer" für Lesen und Schreiben des EEPROM gut funktioniert.

## EEPROM roms ##
[Hier](http://code.google.com/p/xeroxpatrone/wiki/EepromList) gibt es einige EEPROM dumps, die ich ausgelesen habe. Diese können entweder verwendet werden um alte Kartuschen wieder "aufzufrischen" oder um weiter Forensik bezüglich des Inhalts zu betreiben. Sollten sich Informationen bezüglich des Inhalts und der Kodierung der Dumps ergeben, freue ich mich natürlich über jede Email. Auch natürlich für den Fall, dass sie ein EEPROM zur Hand haben, die noch nicht bei den Downloads gelistet ist.

[Email Adressen](http://code.google.com/p/xeroxpatrone/people/list)