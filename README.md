# DubOS
-------

### Notes
---------

DubOS is an exploration into kernel development.
It's long term aim is to become an environment for dedicated audio signal processing; whether it will ever reach that is another story! For now it's interesting just to venture into the world of hardware programming.

Mainly tested on bochs, but early builds were tested straight on the metal. Includes bash scripts for compiling, installing and loading in bochs.

### Current Task
------------

MBR relocates itself to 0x0600, and loads up the second sector of the disk which has the EBE- this is close to being in a state for chainloading!

Swapped everything over from floppy image to hard disk image. Next need to partition the hard disk image properly.

### Points of Interest
------------------

* Started work on boot loader- currently loads second sector of floppy disk  and prints messages to text buffer
  
* Protected mode switch

* kprintf function that prints dec and hex numbers correctly- written assembly  routine that converts uint32 to hex. This function can display correct memory  addresses. kprintf is written in assembly as well. Whether it 'beats the compiler'  is not tested -- probably doesn't -- but is interesting none the less.

* C routines for managing page allocation - this has not been tested on metal! only  in bochs.

* Programmable Interrupt Controller initialized correctly

* Basic interrupt service routines. Not thoroughly tested.

* Fairly thorough comments on GDT, IDT. Start of comments on page tables.
