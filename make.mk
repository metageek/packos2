.PHONY:: all app clean depend libs lib tst check ro

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

ifneq ($(lib),)
libs:: lib
LIBFILE:=lib$(lib).a
lib: $(LIBFILE)
endif

ifneq ($(LIBFILE),)
$(LIBFILE): $(LIBOBJS)
	$(RM) $(LIBFILE)
	ar r $(LIBFILE) $(LIBOBJS)

all:: $(LIBFILE)
endif
