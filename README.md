
VIAXFG v0.3.0 - VIA Extended SB Config Tool 
===========================================
For VIA southbridge chipsets VT82C686/A/B and VT8231

~~~
Usage:	VIAXFG [/E] [/D] [/E?] [/D?] [/H]
Options:
	/E	Enable SB, FM, MIDI and Joy
	/D	Disable SB, FM, MIDI and Joy
	/ES	Enable SB
	/DS	Disable SB
	/EF	Enable FM
	/DF	Disable FM
	/EM	Enable MIDI
	/DM	Disable MIDI
	/EJ	Enable Joy
	/DJ	Disable Joy
	/S	Show status screen
	/H /?	Print this screen
~~~

FEATURES
--------
* Allows enabling/disabling VIA Sound Blaster features without a restart.
* Runs on a 386 or later x86 processor.
* Compatible with DOS, with or without EMM386.
* Compatible with Windows 95, 98 and ME.
* Requires DOS4GW.exe or a compatible DPMI host.
* Can be run from a batch file, or even from autoexec.bat.

DISCLAIMER
----------
WARNING: USE AT YOUR OWN RISK! This software should not be used for mission
critical applications. Author is not responsible for any damage caused to your
computer from using this software.

ALTERNATIVES
------------
VIASBCFG by Vogons member @JazeFox.
VIAS by Vogons member @Javispedro1.
Official VIAUDIO utility.

CREDITS
-------
Author: Enaiel <enaiel@gmail.com> (c) 2022. 

Thanks to:
* Vogons.org member @JazeFox for VIASBCFG which this tool supplements.
* Vogons.org member @Javispedro1 for releasing the source to VIAS. 

SOURCE
------
Source is now available at https://github.com/enaiel/viaxfg.

License is GPL due to my use of VIAS code. 

Built with DJGPP. You can obtain your copy from http://www.delorie.com/djgpp.

TESTED
------
Motherboard               | Southbridge  
--------------------------|--------------
MSI MS-6369     	  | VT82C686/A/B 

HISTORY
-------
2022-11-17: v0.1.0
* Initial version with support for VIA chipset VT82C686/A/B. 

2024-07-20: v0.1.1
* Fixes bug where all features were disabled by default.

2024-08-01: v0.2.0
* Complete re-write to simplify the code.

2024-08-01: v0.3.0
* Initial Github release.
