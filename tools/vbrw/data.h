#ifndef DATA_H
#define DATA_H

struct pentry {
	uint8_t bf;

	uint8_t sh;
	uint8_t sc;
	uint8_t ss;

	uint8_t id;

	uint8_t eh;
	uint8_t ec;
	uint8_t es;

	uint32_t rel;
	uint32_t total; 
};

struct mbrecord {
	char uid[10];
	struct pentry p1;
	struct pentry p2;
	struct pentry p3;
	struct pentry p4;
};
#endif
