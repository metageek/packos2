all::

ifneq ($(strip $(subdirs)),)
all clean depend::
	for dir in $(subdirs); do $(MAKE) -C $$dir $@ ; done
endif

INCLUDES:=-I$(depth)/include
CFLAGS+=$(INCLUDES) -g -Wall -m32
ASFLAGS+=$(INCLUDES) -m32

ifneq ($(LIBFILE),)
$(LIBFILE): $(LIBOBJS)
	$(RM) $(LIBFILE)
	ar r $(LIBFILE) $(LIBOBJS)

all:: $(LIBFILE)
endif
