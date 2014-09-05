#ifndef DATA_H
#define DATA_H

struct chs {
	uint16_t c;
	uint8_t h;
	uint8_t s;
};

#define START_CHS 1
#define END_CHS 2

struct pentry {
	uint8_t bf;

	uint8_t sh;
	uint8_t ss;
	uint8_t sc;

	uint8_t id;

	uint8_t eh;
	uint8_t es;
	uint8_t ec;

	uint32_t rel;
	uint32_t total; 
} __attribute__((packed));

struct mbrecord {
	char uid[10];
	struct pentry p1;
	struct pentry p2;
	struct pentry p3;
	struct pentry p4;
} __attribute__((packed));
#endif
