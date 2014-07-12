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
#include "kernel/kterm.h"
#include "kernel/kmem.h"

// memory dimensions and vitals
#define _KMEM_KB 1024
#define KMEM_PAGE 4096
#define KMEM_UPPER 0x100000
#define PBLOCK_SIZE 1

// macros for conversion
#define m_addr_toblock(a) ( a/KMEM_PAGE/(8*PBLOCK_SIZE) ) // physical address to block
#define m_addr_topage(a) ( a/KMEM_PAGE ) // pysical address to page number

#define m_block_toaddr(a) ( a*KMEM_PAGE*(8*PBLOCK_SIZE) ) // block to physical address
#define m_block_topage(a) ( a*(8*PBLOCK_SIZE) ) // block to page number

#define m_page_toblock(a) ( a/(8*PBLOCK_SIZE) ) // page number to block
#define m_page_toaddr(a) ( a*KMEM_PAGE ) // page number to physical address

void _kmem_allocate_profile(mbd_mmap_t *mbmap, kmem_profile_t *profile);
void _set_page_table(uint8_t v, uintptr_t *start, uint32_t length);
void _allocate_kernel(kmem_profile_t *profile);
uint32_t _loc_freeblock(kmem_profile_t *profile, uint32_t offset);
uint32_t _loc_freepage(kmem_profile_t* profile, uint32_t block, uint8_t off);
uint8_t _check_size(uint32_t wb, uint8_t wbt, uint32_t npages, kmem_profile_t *profile);
uint32_t *_loc_scan(kmem_profile_t *profile, uint32_t npages);
uint8_t _scan_freepage(uint32_t *block, uint8_t *page, kmem_profile_t *profile);
void _allocate_pages(uint32_t block, uint8_t page, uint32_t npage, kmem_profile_t *profile);
void _deallocate_pages(uint32_t block, uint8_t page, uint32_t npage, kmem_profile_t *profile);
void _check_pt(uintptr_t *ptr);

uint32_t _generate_pte(uint32_t pte, uint32_t addr);
uintptr_t *generate_pt(kmem_profile_t *profile, uint32_t startp, uint32_t nump, uint32_t flags);

void printMemSectTypes(uintptr_t map_addr, uint32_t map_len)
{
	struct _mmap_entry *mmap;
	mmap = map_addr;

	while((uint32_t) mmap < ( (uint32_t) map_addr + map_len)) {
		kprintf("[%d] ", mmap->type);
		kprintf("0x%x ", mmap->base_lo);
		kprintf("%d bytes\n\n", mmap->length_lo);
		mmap = ((uint32_t)mmap + mmap->size) + sizeof(uint32_t);
	}
}

void kmem_genprofile(mbd_mmap_t *mbmap, kmem_profile_t *profile)
{
	profile->av = 0;
	profile->total = 0;
	profile->page_total = 0;
	profile->map_addr = (uintptr_t) &_kspace_eaddr;

	struct _mmap_entry *mmap;
	mmap = mbmap->addr;
	uint32_t msz = (uint32_t) mbmap->addr + mbmap->len;
	while( (uint32_t) mmap < msz) {
		if(mmap->type == 1)
			profile->av += mmap->length_lo;

		profile->total += mmap->length_lo;

		mmap = ((uint32_t)mmap + mmap->size) + sizeof(uint32_t);
	}
}

// create a bitmap for pages of memory
void kmem_page_bitmap(kmem_profile_t *profile, mbd_mmap_t *mbmap)
{
	uint32_t pages = profile->total/KMEM_PAGE;
	uint32_t ui8_pages = pages/(sizeof(uint8_t)*8);
	profile->page_total = pages;
	profile->page_ui8_total = ui8_pages;

	_kmem_allocate_profile(mbmap, profile);
}
// allocate the pages of the current memory profile
void _kmem_allocate_profile(mbd_mmap_t *mbmap, kmem_profile_t *profile)
{
	uint8_t alt, cbt, wi, wb;
	uint32_t als, ale, cpg, tpg, baddr, sz, ci;
	struct _mmap_entry *mmap;

	als = ale = ci =0;
	mmap = mbmap->addr;

	baddr = (uint32_t) mmap->base_lo;
	sz = (uint32_t) mbmap->addr + mbmap->len;
	alt = 0;

	_set_page_table(0xff, profile->map_addr, profile->page_ui8_total);
	while((uint32_t)mmap < sz) {

		als = (( (uint32_t) mmap->base_lo)/KMEM_PAGE); // page number start

		if(als > (profile->page_ui8_total*8))
			break;


		cpg = 0;  // current page
		tpg = (mmap->length_lo)/KMEM_PAGE; // length in pages
		if(mmap->type == 1)
			alt = 0x0;
		else
			alt = 0x1;

		ci = als/(sizeof(uint8_t)*8);
		while(cpg < tpg) {
		// local page count
			cbt = 7; // current bit is 0
			wi = 0; // working int
			while(cbt > 0) {
				
				if(cpg == tpg)
					break;

				wb = alt<<cbt;
				wi |= wb;

				cpg++;
				cbt--;
			}

			if(cbt == 0) {
				wi |= alt;
				cpg++;
			}

			profile->map_addr[ci] = wi; // set current 8 page block to working int
			ci++; // increment to next integer
		}
		mmap = ((uint32_t)mmap + mmap->size) + sizeof(uint32_t);
	}

	_allocate_kernel(profile);
}

void _set_page_table(uint8_t v, uintptr_t *start, uint32_t length)
{
	uint32_t i = 0;
	while(i < length)
		start[i++] = v;
}

void kmem_check_profile_allocation(kmem_profile_t *profile)
{
	uint32_t i = 0;
	uint32_t c = 256;
	uint32_t addr = 0;
	while(i < profile->page_ui8_total) {
		if(profile->map_addr[i] != c) {
			c = profile->map_addr[i];
			addr = m_block_toaddr(i);
			if(c == 0)
				kprintf("Free Block [0x%x]\n", addr, i*8);
			else
				kprintf("Allocated Block [0x%x]\n", addr, i);
		}
		i++;
	}
	
}

void _allocate_kernel(kmem_profile_t *profile)
{
	uint8_t cbt, wb, wi;
	uint32_t sizeb, sizep, cp, t, ci;

	ci = m_addr_toblock(KMEM_UPPER);
	sizeb = profile->map_addr + profile->page_ui8_total;

	// round up
	sizep = 1+((sizeb - KMEM_UPPER)-1)/KMEM_PAGE;
	cp = 0;
	/*  TODO:
	 *  Not flowing into next page
	 *  block correctly
	 */
	while(cp < sizep) {
	// local page count
		cbt = 7; // current bit is 0
		wi = 0; // working int
		while(cbt > 0) {
			
			if(cp == sizep)
				break;

			wb = 0x1<<cbt;
			wi |= wb;

			cp++;
			cbt--;
		}

		if(cbt == 0) {
			wi |= 0x1;
			cp++;
		}

		profile->map_addr[ci++] = wi; // set current 8 page block to working int
	}
}

uint32_t kmem_avpUpper(kmem_profile_t *profile)
{
	uint8_t pblock, i;
	uint32_t addr, tp;
	addr = m_addr_toblock(KMEM_UPPER);
	tp = 0;
	while(addr < profile->page_ui8_total) {
		if(profile->map_addr[addr] == 0)
			tp += 8;


		else {
			pblock = profile->map_addr[addr];
			i = 0;
			while(i++ < 8) {
				if((pblock & 0x1) == 1)
					tp++;

				pblock >>= 1;
			}
		}

		addr++;
	}

	return tp;

}

uintptr_t *kpalloc(kmem_profile_t *profile, uint32_t npage)
{
	return _loc_scan(profile, npage);
}
/*
 *  Notes:
 *  if a working integer is 0xff then it is full
 */
uintptr_t *_loc_scan(kmem_profile_t *profile, uint32_t npages)
{
	uint32_t wb, wi, ci, np;
	uint8_t wbt, cbt, chk;
	wi = m_addr_toblock(KMEM_UPPER);
	wbt = 7;

	// find the free block and page
	if(_scan_freepage(&wi, &wbt, profile) == FALSE)
		return NULL;

	// at this point- found a free page

	if(npages == 1) {
		// it's a single page so it's fine
		profile->map_addr[wi] |= 0x1<<wbt;
		return m_page_toaddr(((wi*8)+(7-wbt)));
	}

	// check the size
	np = npages-1; // first page is already checked
	cbt = wbt-1; // first page is already checked
	ci = wi;
	while(np > 0) {
		wb = profile->map_addr[ci];
		chk = 0;

		while(cbt < 255 && np > 0) {
			// there isn't space in
			// this array of free pages
			if((wb&(0x1<<cbt)) > 0) {
				break;
			}

			cbt--;
			np--;
		}

		if(np != 0 && cbt < 255) {
			// not enough space at this point in memory
			if(_scan_freepage(&ci, &cbt, profile) == FALSE)
				return NULL;

			// we have a fresh starting point
			wi = ci;
			wbt = cbt;
			np = npages-1;
		}
		else
		if(np > 0){
			// we haven't run out of space yet
			// so move onto the next block
			ci++;
			cbt= 7;
		}
	}
	
	_allocate_pages(wi, wbt, npages, profile);
	return (uintptr_t*)m_page_toaddr(((wi*8)+(7-wbt)));
}

/*
 *  Find a free page in the memory 
 */
uint8_t _scan_freepage(uint32_t *block, uint8_t *page, kmem_profile_t *profile)
{
	uint32_t wi;
	uint8_t wbt, t;
	wi = *block;
	wbt = *page;
	while(wi < profile->page_ui8_total) {

		while(wi < profile->page_ui8_total) {
			if(profile->map_addr[wi] < 0xff) {
				break;
			}
			wi++;
		}

		// find first free wbt
		// searching hi-lo order
		while(wbt < 0xff) {
			if((profile->map_addr[wi]&(0x1<<wbt)) == 0) {
				*block = wi;
				*page = wbt;
				return TRUE; // found a free wbt
			}
			wbt--;
		}

		if(wbt == 0xff) {
			// skip onto the next wi
			wi++;
			wbt = 7;
		}

	}

	return FALSE;
}

void _allocate_pages(uint32_t block, uint8_t page, uint32_t npage, kmem_profile_t *profile)
{
	while(npage > 0) {

		while(page < 0xff) {
			if(npage == 0)
				break;

			profile->map_addr[block] |= (0x1<<page);
			page--;
			npage--;
		}

		page = 7;
		block++;
	}
}

void _deallocate_pages(uint32_t block, uint8_t page, uint32_t npage, kmem_profile_t *profile)
{
	page = 7-page;
	while(npage > 0) {

		while(page < 0xff) {
			if(npage == 0)
				break;

			profile->map_addr[block] &= ~(0x1<<page);
			page--;
			npage--;
		}

		page = 7;
		block++;
	}
}

void kpfree(kmem_profile_t *profile, uintptr_t *addr, uint32_t npage)
{
	uint32_t wp = ((uint32_t)addr)/4096;
	uint32_t wb = wp/8;
	uint8_t p = wp-(wb*8);

	_deallocate_pages(wb, p, npage, profile);
}

uint8_t alloc_pd(kmem_profile_t *profile)
{
	// lower memory 0x0 - 0x100000 set as read-only
	// since lots of bios stuff below here
	if(generate_pt(profile, 0, 256, PTE_BLANK) == NULL)
		return FALSE;

	return TRUE;
}

uintptr_t *generate_pt(kmem_profile_t *profile, uint32_t startp, uint32_t nump, uint32_t flags)
{

	if(nump > 1024) // more than a table's worth
		return  NULL;
	uint32_t diff = 1024-nump;
	uintptr_t *pt = kpalloc(profile,1); // allocate for table

	uint32_t i = m_page_toblock(startp); // block index of start page
	uint8_t off = 0;
	if((i<<3)<startp)
		off = startp-(i<<3);

	uint32_t bp = NULL; // block page number
	uint8_t cb = NULL;  // current block
	uint8_t cbt = NULL; // current bit
	uint32_t addr = 0x0;
	uint32_t pte_dirty = flags | PTE_ACCESSED | PTE_DIRTY;
	uint32_t pte_clean = flags | PTE_ACCESSED;

	// loop through the blocks
	while(nump > 0) {

		addr = m_block_toaddr(i); // physical address of current block
		bp = i<<3; // block's first page's number
		cbt = 0+off;

		if(profile->map_addr[i] == 0xff) { // block is fully allocated
			cbt = 0;
			while(cbt < 8) {
				// current block in in pages+current page in block
				pt[bp+cbt] = _generate_pte(pte_dirty, addr+(cbt*KMEM_PAGE));
				cbt++;
				nump--;
			}
		}
		else {
			cb = profile->map_addr[i];
			if(cb == 0) { // block has no allocation
				while(cbt < 8) {
					// current block in in pages+current page in block
					pt[bp+cbt] = _generate_pte(pte_clean, addr+(cbt*KMEM_PAGE));
					cbt++;
					nump--;
				}
			}
			else { // block is semi-allocated
				while(cbt < 8) {
					if((cb&(0x1<<cbt)) > 0) // allocated
						pt[bp+cbt] = _generate_pte(pte_dirty, addr+(cbt*KMEM_PAGE));
					else // not allocated
						pt[bp+cbt] = _generate_pte(pte_clean, addr+(cbt*KMEM_PAGE));
					cbt++;
					nump--;
				}

			}
		}
		off = 0;
		i++;
	}

	if(diff > 0) {
		cb = bp+cbt;
		while(diff-- > 0)
			pt[cb++] = _generate_pte(PTE_BLANK,NULL);
	}

	return pt;
}

/*
 * 4KB Page Table Entry
 * Intel Manual Reference (lo to hi)
 * Chapter 4 Dev Manual combined
 *====================================
 *
 * bit 00-00: p		--	present
 * bit 01-01: rw	--	read/write (0 = read only)
 * bit 02-02: u		--	user (or supervisor) (1 = user)
 * bit 03-03: pwt	--	page-level write-through
 * bit 04-04: pcd	--	page-level cache (1 = disabled)
 * bit 05-05: a		--	accessed (1 = software has accessed)
 * bit 06-06: d		--	dirty (1 = software has written to page)
 * bit 07-07: pat	--	PAT support?
 * bit 08-08: g		--	global translation (1 = global translation)
 * bit 11-09: --	--	ignored
 * bit 31-12: addr	--	physical address of page (upper 20 bits of physical addr)
 *
 * --------------------------------------------------------------------------------------------
 * |           addr            | ignore |  g  | pat |  d  |  a  | pcd | pwt |  u  | r w |  p  |
 * --------------------------------------------------------------------------------------------
 * 31                         12       09    08    07    06    05    04    03    02    01    00
 */
/*
uint32_t _generate_pte(uint8_t p, uint8_t rw, uint8_t u,
		uint8_t pwt, uint8_t pcd, uint8_t a, uint8_t d,
		uint8_t pat, uint8_t g, uint32_t addr)
{
	uint32_t pte;

	pte = p;
	if(p == 0)
		return pte;

	pte |= rw << 1;
	pte |= u<<2;
	pte |= pwt<<3;
	pte |= pcd<<4;
	pte |= a<<5;
	pte |= d<<6;
	pte |= pat<<7;
	pte |= g<<8;

	pte |= (addr&&0xFFFFF000);


	return pte;
}
*/
uint32_t _generate_pte(uint32_t pte, uint32_t addr)
{
	pte |= (addr&&0xFFFFF000);
	return pte;
}

/*
 * 4KB Page Directory Entry
 * Intel Manual Reference (lo to hi)
 * Chapter 4 Dev Manual combined
 *====================================
 *
 * bit 00-00: p		--	present
 * bit 01-01: rw	--	read/write (0 = read/write)
 * bit 02-02: u		--	user (or supervisor) (1 = user)
 * bit 03-03: pwt	--	page-level write-through
 * bit 04-04: pcd	--	page-level cache (1 = disabled)
 * bit 05-05: a		--	accessed (1 = software has accessed)
 * bit 06-06: --	--	Ignored
 * bit 07-07: ps	--	if CR4.PSE is 1 then it should be 0 / other wise ignored
 * bit 11-08: --	--	Ignored
 * bit 31-12: addr	--	physical address of page (upper 20 bits of physical addr)
 *
 * --------------------------------------------------------------------------------------------
 * |           addr            |    ignore    | ps  |  -  |  a  | pcd | pwt |  u  | r w |  p  |
 * --------------------------------------------------------------------------------------------
 * 31                         12             08    07    06    05    04    03    02    01    00
 */
uint32_t _generate_pde(uint8_t p, uint8_t rw, uint8_t u,
		uint8_t pwt, uint8_t pcd, uint8_t a, uint8_t ps,
		uint32_t addr)
{
	uint32_t pde;
	pde = p;
	pde |= rw<<1;
	pde |= u<<2;
	pde |= pwt<<3;
	pde |= pcd<<5;
	pde |= a<<5;
	pde |= ps<<7;
	pde |= addr<<12;
}
