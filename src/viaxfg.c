/*******************************************************************************

  viaxfg.c: Main program for VIAXFG
  VIAXFG - VIA Extended SB Config Tool

  Author: Enaiel <enaiel@gmail.com> (c) 2022

  This program is free software; you can redistribute it and/or modify it
  under the terms and conditions of the GNU General Public License,
  version 2, as published by the Free Software Foundation.

  This program is distributed in the hope it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
  more details.

  You should have received a copy of the GNU General Public License along with
  this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.

  The full GNU General Public License is included in this distribution in
  the file called "COPYING".

*******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "types.h"
#include "pci.h"

#define FNAME "VIAXFG"
#define VER "0.3.0"

/* VIA PCI IDs */
#define PCI_VENDOR_ID_VIA		0x1106
#define PCI_DEVICE_ID_VIA_82C686	0x3057
#define PCI_DEVICE_ID_VIA_8231		0x8235
#define PCI_DEVICE_ID_VIA_AC97 		0x3058

/* VIA AC97 Registers */
#define CFG_COMMAND         		0x04 
#define CFG_BAR0             		0x10 
#define CFG_FUNCTION_ENABLE  		0x42

// Register 0x42, Function Enable
// Bit 0 SoundBlaster Enable
//     1 MIDI Enable
//     2 FM Enable
//     3 Game Port Enable

pci_device *card = NULL;
u16 cardiobase;

char* to_str(bool val)
{
    	if(val)
    		return "Enabled";
	else
		return "Disabled";
}

bool is_sb_enabled()
{
	u8 reg;
	if (!card) return false;
	pci_read_cfg_byte(card->bus, card->dev, card->fun, CFG_FUNCTION_ENABLE, &reg);
	return reg & 1;
}

bool is_fm_enabled()
{
	u8 reg;
	if (!card) return false;
	pci_read_cfg_byte(card->bus, card->dev, card->fun, CFG_FUNCTION_ENABLE, &reg);
	return reg & 0x4;
}

bool is_midi_enabled()
{
	u8 reg;
	if (!card) return false;
	pci_read_cfg_byte(card->bus, card->dev, card->fun, CFG_FUNCTION_ENABLE, &reg);
	return reg & 0x2;
}

bool is_joy_enabled()
{
	uint8_t reg;
	if (!card) return false;
	pci_read_cfg_byte(card->bus, card->dev, card->fun, CFG_FUNCTION_ENABLE, &reg);
	return reg & 0x8;
}

void set_all(bool sb, bool fm, bool midi, bool joy)
{
	u8 reg;
	pci_read_cfg_byte(card->bus, card->dev, card->fun, CFG_FUNCTION_ENABLE, &reg);
	
	reg &= ~0x1F;
	
	if (sb) reg |= 0x1;
	if (fm)       reg |= 0x4;
	if (midi)     reg |= 0x2;
	if (joy) reg |= 0x8;
	
	pci_write_cfg_byte(card->bus, card->dev, card->fun, CFG_FUNCTION_ENABLE, reg);
}

void init_card()
{
	u16 command;
	u32 bar;

	card = (pci_device *)malloc(sizeof(pci_device));
	pci_device southbridge;

	printf("Detecting... ");

	if (pci_find_device(PCI_VENDOR_ID_VIA, PCI_DEVICE_ID_VIA_82C686, &southbridge)) 
	{
		printf("VIA VT82C686... ");
	}
	else if (pci_find_device(PCI_VENDOR_ID_VIA, PCI_DEVICE_ID_VIA_8231, &southbridge)) 
	{
		printf("VIA VT8231... ");
	}
	else
	{
		printf("ERROR\nVIA southbridge VT82C686/A/B or VT8231 not found!\n");
		exit(1);
	}

	if (pci_find_device(PCI_VENDOR_ID_VIA, PCI_DEVICE_ID_VIA_AC97, card)) 
	{
		printf("VIA AC97... ");
	}
	else
	{
		printf("ERROR\nVIA AC97 not found!\n");
		exit(1);
	}
	
	pci_read_cfg_word(card->bus, card->fun, card->dev, CFG_COMMAND, &command);
	if (command & 1) 
	{
		printf("IO access.. ");
	}
	else
	{
		printf("VIA AC97 is not initialized for IO access\n");
		exit(1);
	}
	
	pci_read_cfg_int(card->bus, card->fun, card->dev, CFG_BAR0, &bar);
	
	cardiobase = bar & 0xFFFC;

	printf("DONE\n");
}

void print_header()
{
	printf("VIAXFG v%s - VIA Extended SB Config Tool. (c) 2022 Enaiel <enaiel@gmail.com>\n", VER);
}

void print_usage()
{
	puts("For VIA southbridge chipsets VT82C686/A/B and VT8231");
	puts("\nUsage:\tVIAXFG [/E] [/D] [/E?] [/D?] [/H]");
	puts("Options:");
	puts("\t/E\tEnable SB, FM, MIDI and Joy");
	puts("\t/D\tDisable SB, FM, MIDI and Joy");
	puts("\t/ES\tEnable SB");
	puts("\t/DS\tDisable SB");
	puts("\t/EF\tEnable FM");
	puts("\t/DF\tDisable FM");
	puts("\t/EM\tEnable MIDI");
	puts("\t/DM\tDisable MIDI");
	puts("\t/EJ\tEnable Joy");
	puts("\t/DJ\tDisable Joy");
	puts("\t/S\tShow status screen");
	puts("\t/H /?\tPrint this screen");
}

int main(int argc, const char *argv[]) 
{
	bool sb;
	bool fm;
	bool midi;
	bool joy;

    	int i;
	const char* action;

	print_header();

	if (argc == 1)
	{
		print_usage();
		return 0;
	}

	init_card();
	sb = is_sb_enabled();
	fm = is_fm_enabled();
	joy = is_joy_enabled();
	midi = is_midi_enabled();
	for(i=1; i<argc; i++)
	{
		action = argv[i];
		if(strcasecmp(action,"/E") == 0)
		{
			sb = true;
			fm = true;
			midi = true;
			joy = true;
		}
		else if(strcasecmp(action,"/D") == 0)
		{
			sb = false;
			fm = false;
			midi = false;
			joy = false;
		}
		else if(strcasecmp(action,"/ES") == 0)
			sb = true;
		else if(strcasecmp(action,"/DS") == 0)
			sb = false;
		else if(strcasecmp(action,"/EF") == 0)
			fm = true;
		else if(strcasecmp(action,"/DF") == 0)
			fm = false;
		else if(strcasecmp(action,"/EM") == 0)
			midi = true;
		else if(strcasecmp(action,"/DM") == 0)
			midi = false;
		else if(strcasecmp(action,"/EJ") == 0)
			joy = true;
		else if(strcasecmp(action,"/DJ") == 0)
			joy = false;
		else if(strcasecmp(action,"/S") == 0)
		{
		}
		else 
		{
		    print_usage();
		    return 0;
		}
	}
	set_all(sb, fm, midi, joy);
	printf("SB: %s",to_str(is_sb_enabled()));
	printf("\tFM: %s",to_str(is_fm_enabled()));
	printf("\tMIDI: %s",to_str(is_midi_enabled()));
	printf("\tJoy: %s",to_str(is_joy_enabled()));
	printf("\n");
	return 0;
}
