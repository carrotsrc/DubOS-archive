/*
* Copyright 2014, Charlie Fyvie-Gauld
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
* 
*     http://www.apache.org/licenses/LICENSE-2.0
* 
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>
#include <errno.h>
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
	printf("%d.\t", pid);

	chs = decode_chs(p, START_CHS);
	printf("0x%x\t%d\t", p->bf, p->id);
	printf("%d/%d/%d - ", chs->c, chs->h, chs->s);
	free(chs);

	chs = decode_chs(p, END_CHS);
	printf("%d/%d/%d", chs->c, chs->h, chs->s);
	printf("\t+%d sectors\t %d sectors\n", p->rel, p->total);
	free(chs);
}

void print_mbr(struct mbrecord *mbr)
{
	char uid[11];
	uint8_t i = 0;
	uintptr_t p = (uintptr_t)mbr+10;

	strncpy(uid, mbr->uid, 10);
	uid[10] = '\0';
	printf("DiskID: %s\n", (char*)&uid);
	for(i = 1; i <= 4; i++) {
		print_pentry(i, (struct pentry*)p);
		p += 16;
	}
}

uint8_t valid_bootsector(FILE *fp)
{
	uint16_t sig = 0x0000;
	fseek(fp, 0x1fe, SEEK_SET);
	fread(&sig, sizeof(uint16_t), 1, fp);
	rewind(fp);
	if(sig != 0xAA55)
		return 0;
	return 1;
}

uint8_t write_bootsector(struct pentry *partition, FILE *img, FILE *bin)
{
	size_t bytes = 0;
	uint32_t offset = partition->rel<<9;

	printf("+%d bytes (0x%x)\n", offset, offset);
	uint8_t *code = malloc(sizeof(uint8_t)*512);
	bytes = fread(code, sizeof(uint8_t), 512, bin);
	
	fseek(img, offset, SEEK_SET);
	bytes = fwrite(code, sizeof(uint8_t), bytes, img);

	free(code);
	code = NULL;

	if(bytes == 0)
		printf("Error occured: %d\n", ferror(img));
	else
		printf("Done! (%d bytes written)\n", (uint32_t)bytes);

}

uint8_t clear_bootsector(struct pentry *partition, FILE *img)
{
	uint32_t zero = 0x0;
	size_t bytes = 0;
	uint32_t offset = partition->rel<<9;

	printf("+%d bytes (0x%x)\n", offset, offset);
	fseek(img, offset, SEEK_SET);
	while(bytes != 512)
		bytes += fwrite(&zero, sizeof(uint32_t), 1, img);

	if(bytes == 0)
		printf("Error occured: %d\n", ferror(img));
	else
		printf("Done! (%d bytes zeroed)\n", (uint32_t)bytes);
}

void print_help()
{
	printf("1-4\tSelect partition\np\tPrint MBR\nw\tWrite bootsector to selected partition\nc\tZero bootsector of selected partition\nq\tExit program\n\n");
}

int main(unsigned int argc, char *argv[])
{
	char *image = NULL, *bin = NULL, buf[16];
	uintptr_t ptr;
	FILE *fp, *gp;
	struct mbrecord *mbr = NULL;
	uint8_t partition = 0;

	printf("VBR Writer\n");

	if(argc > 1)
		scan_arg(&image, &bin, argc, argv);


	if(image == NULL || bin == NULL) {
		printf("Usage:\n");
		printf("\t-b [PATH]\t\tPath to binary of bootstrap code\n");
		printf("\t-i [PATH]\t\tPath to hard disk image\n");
		exit(EXIT_FAILURE);
	}

	printf("Bootstrap code: %s\n", bin);
	gp = fopen(bin, "rb");
	if(gp == NULL) {
		printf("Failed to open bootstrap binary\n");
		exit(EXIT_FAILURE);
	}

	if(!valid_bootsector(gp)) {
		printf("Invalid boot sector signature\n");
		exit(EXIT_FAILURE);
	}

	fp = fopen(image, "r+b");
	if(fp == NULL) {
		printf("Failed to access image\n");
		exit(EXIT_FAILURE);
	}


	mbr = read_mbr(fp);
	print_mbr(mbr);
	
	buf[0] = '\0';

	printf("\n");
	while(buf[0] != 0x71 && buf[0] != 0x51) {
		if(partition == 0)
			printf("$ ");
		else
			printf("Partition %d $ ", partition);

		scanf("%s", (char*)&buf);
		if(buf[0] == 0x71 || buf[0] == 0x51) {
			printf("Exiting...\n");
			exit(EXIT_SUCCESS);
		} else
		if((buf[0] == 0x77 || buf[0] == 0x57) && partition > 0) {
			printf("Writing bootstrap code to partition %d...\n", partition);
			break;
		} else
		if((buf[0] == 0x63 || buf[0] == 0x43) && partition > 0) {
			ptr = (uintptr_t)mbr+10+(16*--partition);
			clear_bootsector((struct pentry*)ptr, fp);
		} else
		if((buf[0] == 0x50 || buf[0] == 0x70)) {
			print_mbr(mbr);
			printf("\n");
		} else
		if((buf[0] == 0x48 || buf[0] == 0x68)) {
			print_help();
		} else
		if(buf[0] >= 0x30 && buf[0] <= 0x34) {
			partition = atoi(&buf);
			printf("Selected partition %d\n", partition);
		} else
			printf("Unknown\n");
	}

	ptr = (uintptr_t)mbr+10+(16*--partition);
	write_bootsector((struct pentry*)ptr, fp, gp);

	fclose(fp);
	fclose(gp);

	free(mbr);
	free(image);
	free(bin);

	exit(EXIT_SUCCESS);
}
