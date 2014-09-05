#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#include "data.h"

void scan_arg(char **image, char **bin, unsigned int argc, char *argv[])
{
	unsigned short i = 1;
	for(;i < argc; i++) {
		if(argv[i][0] == '-') {
			switch(argv[i][1]) {
			case 'b':
				*(bin) = malloc(sizeof(char)*64);
				strcpy(*(bin), argv[++i]);
			break;

			case 'i':
				*(image) = malloc(sizeof(char)*64);
				strcpy(*(image), argv[++i]);
			break;
			}
		}
	}
}

uint8_t bit_reverse(uint8_t value)
{
	uint8_t rev = 0;
	for(int8_t s = 7; s >= 0; s--) {
		rev ^= (value&1)<<s;
		value >>= 1;
	}
	return rev;
}

struct chs *decode_chs(struct pentry *entry, uint8_t soe)
{
	struct chs *chs = (struct chs*)malloc(sizeof(struct chs));
	if(soe == START_CHS) {
		chs->c = entry->sc;
		chs->h = entry->sh;
		chs->s = entry->ss;
	} else {
		chs->c = entry->ec;
		chs->h = entry->eh;
		chs->s = entry->es;
	}

	chs->c ^= (chs->s&0xc0)<<2;
	chs->s &= 0x3f;
	chs->s--;
	return chs;
}

struct mbrecord *read_mbr(FILE *fp)
{
	fseek(fp, 0x1b4, SEEK_SET);
	struct mbrecord *mbr = malloc(sizeof(struct mbrecord));
	fread(mbr, sizeof(struct mbrecord), 1, fp);
	return mbr;
}

void print_pentry(uint8_t pid, struct pentry *p)
{
	if(p->id == 0x0)
		return;

	struct chs *chs = NULL;
	printf("P%d\t", pid);

	chs = decode_chs(p, START_CHS);
	printf("0x%x\t%d\t", p->bf, p->id);
	printf("%d/%d/%d - ", chs->c, chs->h, chs->s);
	chs = decode_chs(p, END_CHS);
	printf("%d/%d/%d", chs->c, chs->h, chs->s);
	printf("\t+%d sectors\t %d sectors\n", p->rel, p->total);
}

void print_mbr(struct mbrecord *mbr)
{
	char uid[11];
	uint8_t i = 0;
	uintptr_t p = (uintptr_t)mbr+10;

	strncpy(uid, mbr->uid, 10);
	uid[10] = '\0';
	printf("DiskID: %s\n", (char*)&uid);
	for(i = 0; i < 4; i++) {
		print_pentry(i, (struct pentry*)p);
		p += 16;
	}
}

int main(unsigned int argc, char *argv[])
{
	char *image = NULL, *bin = NULL;
	struct mbrecord *mbr = NULL;

	printf("VBR Writer\n");

	if(argc > 1)
		scan_arg(&image, &bin, argc, argv);

	if(image == NULL || bin == NULL) {
		printf("Usage:\n");
		printf("\t-b [PATH]\t\tPath to binary of bootstrap code\n");
		printf("\t-i [PATH]\t\tPath to hard disk image\n");
		exit(EXIT_FAILURE);
	}

	FILE *fp = fopen(image, "rb");
	mbr = read_mbr(fp);
	print_mbr(mbr);
	exit(EXIT_SUCCESS);
}
