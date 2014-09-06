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
