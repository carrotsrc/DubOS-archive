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

struct mbr *read_mbr(FILE *fp)
{
	fseek(fp, 0x1b4, SEEK_SET);
	struct mbrecord *mbr = malloc(sizeof(struct mbrecord));
	fread(mbr, sizeof(struct mbrecord), 1, fp);
}

int main(unsigned int argc, char *argv[])
{
	char *image = NULL, *bin = NULL;
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
	read_mbr(fp);
	exit(EXIT_SUCCESS);
}
