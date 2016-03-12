.PHONY:: all bootable clean depend libs lib tst check ro

all::

ifneq ($(strip $(subdirs)),)
all clean depend::
	for dir in $(subdirs); do $(MAKE) -C $$dir $@ ; done
endif

clean::
	$(RM) *.o *.a *~ $(app) depend.mk.bak

INCLUDES:=-I$(depth)/include
CFLAGS+=$(INCLUDES) -g -Wall -m32
ASFLAGS+=$(INCLUDES) -m32
LFLAGS+=-L$(depth)/lib

ifneq ($(lib),)
libs:: lib
LIBFILE:=$(depth)/lib/lib$(lib).a
lib: $(LIBFILE)
endif

ifneq ($(LIBFILE),)
$(LIBFILE): $(LIBOBJS)
	$(RM) $(LIBFILE)
	ar r $(LIBFILE) $(LIBOBJS)

all:: $(LIBFILE)
endif

ifneq ($(runsInHost),yes)
CFLAGS+=-nostdinc -fno-stack-protector
LFLAGS+=-melf_i386
endif

ifneq ($(bootable),)
LIBS:=$(foreach lib,$(libs),-l$(lib))
bootable: $(bootable).exe
all:: bootable
clean::
	$(RM) $(bootable).exe
$(bootable).exe: libs $(bootable).o $(OBJS)
	$(LD) $(LFLAGS) --section-start .text=0x100000 --section-start .rodata=0x10b000 --section-start .data=0x10e000 --section-start .bss=0x10f000 $(bootable).o $(OBJS) -lboot $(LIBS) -o $(bootable).exe
endif
