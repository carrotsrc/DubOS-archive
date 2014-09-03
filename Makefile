target = i586-elf
bindir = ~/development/xgcc/bin

as = $(bindir)/$(target)-as
gcc = $(bindir)/$(target)-gcc
ld = $(bindir)/$(target)-ld

incx = 'header'

gccflg=-std=gnu99 -ffreestanding -O2 -Wall -Wextra -I $(incx)
lnkflg=-ffreestanding -O3 -nostdlib -lgcc
lnkflg_ld= -O3 -nostdlib --verbose -r
objdir=obj
binout=bin
configdir=config
# this is for the boot loader
dbs:
	$(as) boot/ibe/dbsec.s -Iboot/ibe -o $(objdir)/ibedbs.o
	$(ld) --oformat binary --Ttext 0x7C00 -o $(binout)/dbsec.bin $(objdir)/ibedbs.o 

dbsinst:
	make dbs
	@./scripts/dbsloop
	@./scripts/dbsdd
	@./scripts/dbsuloop

dbe:
	$(as) boot/ebe/dbebe.s -Iboot/ebe -o $(objdir)/ebedbs.o
	$(ld) --oformat binary --Ttext 0x7F00 -o $(binout)/dbebe.bin $(objdir)/ebedbs.o 

dbeinst:
	make dbe
	@./scripts/dbsloop
	@./scripts/ebedd
	@./scripts/dbsuloop
	

boot.o:
	$(as) boot/boot.s -o $(objdir)/boot.o
	$(as) io/port_io.s -o $(objdir)/port_io.o

gdt.o:
	$(gcc) -c boot/gdt.c -o $(objdir)/gdt.o $(gccflg)


idt.o:
	$(as) boot/bisr.s -o $(objdir)/bisr.o
	$(gcc) -c boot/bisr.c -o $(objdir)/bisrc.o $(gccflg)
	$(gcc) -c boot/idt.c -o $(objdir)/idt.o $(gccflg)

sys_init.o: boot.o gdt.o idt.o
	$(ld) \
	$(objdir)/boot.o \
	$(objdir)/port_io.o \
	$(objdir)/gdt.o \
	$(objdir)/idt.o \
	$(objdir)/bisr.o \
	$(objdir)/bisrc.o \
	-o $(objdir)/sys_init.o $(lnkflg_ld)

bio_vbuf.o:
	$(as) -c kernel/as_vbuf.s -o $(objdir)/as_vbuf.o


io_print.o: kterm.o
	$(as) -c io/uint_print.s -o $(objdir)/uint_print.o
	$(ld) $(objdir)/kterm.o $(objdir)/uint_print.o -o $(objdir)/io_print.o $(lnkflg_ld)

kterm.o: bio_vbuf.o
	$(as) -c kernel/as_kterm.s -o $(objdir)/as_kterm.o
	$(gcc) -c kernel/kterm.c -o $(objdir)/c_kterm.o $(gccflg)
	$(ld) $(objdir)/c_kterm.o $(objdir)/as_kterm.o -o $(objdir)/kterm.o $(lnkflg_ld)

kmem.o: 
	$(gcc) -c kernel/kmem.c -o $(objdir)/c_kmem.o $(gccflg)

kernel.o: kmem.o
	$(gcc) -c kernel/kernel.c -o $(objdir)/kernel.o $(gccflg) 

libstring.o:
	$(gcc) -c lib/string.c -o $(objdir)/libstring.o $(gccflg) 

linkme:
	$(gcc) -T linker.ld -o sub.bin $(lnkflg) 

all: sys_init.o io_print.o libstring.o kernel.o
	$(gcc) -T $(configdir)/linker.ld -o sub.bin \
					$(objdir)/sys_init.o \
					$(objdir)/as_vbuf.o \
					$(objdir)/io_print.o \
					$(objdir)/libstring.o \
					$(objdir)/c_kmem.o \
					$(objdir)/kernel.o \
					$(lnkflg) 

clean:
	rm obj/*.o
	make all

clr:
	rm obj/*.o

install:
	./lloop
	sudo cp sub.bin /mnt/user/vfs/boot/sub.bin
	./uloop

full:
	make clr
	make all
	make install

localinst:
	sudo mount /dev/sda1 /boot
	sudo cp sub.bin /boot
	sudo umount /boot


# Tools are here

vbrw.o:
	gcc -std=c99 -ggdb -c tools/vbrw/entry.c -o tools/vbrw/obj/vbrw.o
vbrw: vbrw.o
	gcc tools/vbrw/obj/vbrw.o -o tools/bin/vbrw
