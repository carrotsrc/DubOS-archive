/*
* Copyright (C) 2014,  Charlie Fyvie-Gauld (Carrotsrc.org)
*
*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "kernel/sys.h"
#include "boot/multiboot.h"
#include "memr/mstrc.h"

#include "boot/gdt.h"
#include "boot/idt.h"

#include "io/port_io.h"
#include "io/uint_print.h"

#include "kernel/kterm.h"
#include "kernel/kmem.h"

#if defined(__linux__)
#error "Build requires cross-compiler setting"
#endif

void kmain(struct mb_info *mbi)
{
	kprintf("DubO/S v%d.%d\n", SWK_MAJOR_V, SWK_MINOR_V);
	return;
/*	gdt_init();
	idt_init();
	pic_init(0x20, 0x28);

	kterm_init();

	kmem_profile_t *profile = NULL;
	mbd_mmap_t mbmap;

	mbmap.addr = mbi->mmap_addr;
	mbmap.len = mbi->mmap_length;
	kprintf("Generating memory profile...");
	kmem_genprofile(&mbmap, profile);
	kprintf("OK\n");
	kprintf("Memory: %d KB / %d KB \n", profile->av/1024, profile->total/1024);
	kmem_page_bitmap(profile, &mbmap);
	kprintf("Allocated memory profile\n");
	kprintf("Total Pages: %d\n", profile->page_total);
	kprintf("Generating page directory...");
	if(alloc_pd(profile) == TRUE)
		kprintf("OK\n");
	else
		kprintf("FAIL\n");
	return;
*/
}

