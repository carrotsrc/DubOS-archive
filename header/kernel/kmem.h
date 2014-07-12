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
#ifndef __KMEM_H
#define __KMEM_H

#define KMEM_UPPER 0x100000

typedef struct _kmem_profile {
	uint32_t av;
	uint32_t total;
	uint32_t page_total;
	uint32_t page_ui8_total;
	uintptr_t *map_addr;
} kmem_profile_t;

typedef struct _kmem_mbd_mmap {
	uintptr_t addr;
	uint32_t len;
} mbd_mmap_t;

typedef struct _kmem_pte {
	uint32_t page[1024];
} kmem_pte_t;

typedef struct _kmem_pd {
	kmem_pte_t pde[1024];
} kmem_pd_t;


extern uint32_t _kspace_saddr;
extern uint32_t _kspace_eaddr;

void printMemSectTypes(uintptr_t map_addr, uint32_t map_len);
void kmem_genprofile(mbd_mmap_t *mbmap, kmem_profile_t *profile);
void kmem_page_bitmap(kmem_profile_t *profile, mbd_mmap_t *mbmap);
void kmem_check_profile_allocation(kmem_profile_t *profile);
uintptr_t *kpalloc(kmem_profile_t *profile, uint32_t nblock);
void kpfree(kmem_profile_t *profile, uintptr_t *addr, uint32_t npage);
uint32_t kmem_avpUpper(kmem_profile_t *profile);
uint8_t alloc_pd(kmem_profile_t *profile);

#define PTE_BLANK 0
#define PTE_PRESENT 1
#define PTE_RW 2
#define PTE_USER 4
#define PTE_PWT 8
#define PTE_PCD 16
#define PTE_ACCESSED 32
#define PTE_DIRTY 64
#define PTE_PAT 128
#define PTE_GLOBAL 256

#endif
