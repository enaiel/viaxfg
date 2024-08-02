/*******************************************************************************

  pci.h: PCI interface to read and write PCI configuration space registers
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

#ifndef	__PCI_H_
#define	__PCI_H_

#include<stdlib.h>
#include<unistd.h>
#include<dos.h>

#include "types.h"

/* PCI Constants */
#define PCI_MAX_BUS		256
#define PCI_MAX_DEV		32
#define PCI_MAX_FUN		8
#define PCI_BASE_ADDR		0x80000000L
#define PCI_CONFIG_ADDR 	0xcf8
#define PCI_CONFIG_DATA 	0xcfc

typedef struct  
{
	u16 bus;
	u16 dev;
	u16 fun;
	u32 addr;
	u16 vendor_id;
	u16 device_id;		
} pci_device;

static inline u32 pci_get_addr(u16 bus, u16 dev, u16 fun, u16 reg)
{
	u32 addr;
	addr = (bus << 16)|(dev << 11)|(fun << 8)|(reg & 0xFC)|PCI_BASE_ADDR;
	return addr;
}

static inline void pci_read_cfg_int(u16 bus, u16 dev, u16 fun, u16 reg, u32 *val)
{
	u32 addr;
	addr = pci_get_addr(bus, dev, fun, reg);
	outportl(PCI_CONFIG_ADDR, addr);
	*val = inportl(PCI_CONFIG_DATA + (reg & 0x03));
}

static inline void pci_read_cfg_word(u16 bus, u16 dev, u16 fun, u16 reg, u16 *val)
{
	u32 addr;
	addr = pci_get_addr(bus, dev, fun, reg);
	outportl(PCI_CONFIG_ADDR, addr);
	*val = inportb(PCI_CONFIG_DATA + (reg & 0x03));
	reg += 1;
	addr = pci_get_addr(bus, dev, fun, reg);
	outportl(PCI_CONFIG_ADDR, addr);
	*val = *val + (inportb(PCI_CONFIG_DATA + (reg & 0x03)) << 8);
}

static inline void pci_read_cfg_byte(u16 bus, u16 dev, u16 fun, u16 reg, u8 *val)
{
	u32 addr; 
	addr = pci_get_addr(bus, dev, fun, reg);
	outportl(PCI_CONFIG_ADDR, addr);
	*val = inportb(PCI_CONFIG_DATA + (reg & 0x03));
}

static inline void pci_write_cfg_byte(u16 bus, u16 dev, u16 fun, u16 reg, u8 val)
{
	u32 addr; 
	addr = pci_get_addr(bus, dev, fun, reg);
	outportl(PCI_CONFIG_ADDR, addr);
	outportb(PCI_CONFIG_DATA + (reg & 0x03), val);
}

static inline void pci_write_cfg_word(u16 bus, u16 dev, u16 fun, u16 reg, u16 val)
{
	u32 addr; 
	addr = pci_get_addr(bus, dev, fun, reg);
	outportl(PCI_CONFIG_ADDR, addr);
	outportb(PCI_CONFIG_DATA + (reg & 0x03), val);
	reg += 1;
	addr = pci_get_addr(bus, dev, fun, reg);
	outportl(PCI_CONFIG_ADDR, addr);
	outportb(PCI_CONFIG_DATA + (reg & 0x03), val >> 8);
}

static inline void pci_write_cfg_int(u16 bus, u16 dev, u16 fun, u16 reg, u32 val)
{
	u32 addr; 
	addr = pci_get_addr(bus, dev, fun, reg);
	outportl(PCI_CONFIG_ADDR, addr);
	outportl(PCI_CONFIG_DATA + (reg & 0x03), val);
}

static inline bool pci_find_device(u16 vendor_id, u16 device_id, pci_device *device)
{
	u16 bus,dev,fun;
	u32 addr, val;
	u16 vendor_id_t, device_id_t;
	for(bus = 0; bus < PCI_MAX_BUS; bus++)
		for(dev = 0; dev < PCI_MAX_DEV; dev++)
			for(fun = 0; fun < PCI_MAX_FUN; fun++)
			{	
				pci_read_cfg_int(bus, dev, fun, 0, &val);
				if((val != 0xffffffff)&&(val != 0))
				{
					vendor_id_t = val & 0x0000ffff;
					device_id_t = (val & 0xffff0000) >> 16;
					if(vendor_id_t == vendor_id && device_id_t == device_id)
					{
						addr = pci_get_addr(bus, dev, fun, 0);
						device->bus = bus;
						device->dev = dev;
						device->fun = fun;
						device->addr = addr;
						device->vendor_id = vendor_id;
						device->device_id = device_id;
						return true;
					}
				}
			}
	return false;
}

#endif	//__PCI_H_
