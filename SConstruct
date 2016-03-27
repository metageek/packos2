def Bootable_method(env, target, source, libs = None):
    env2 = env.Clone(LINKFLAGS = '-melf_i386 --section-start .text=0x100000 --section-start .rodata=0x10b000 --section-start .data=0x10e000 --section-start .bss=0x10f000')
    if libs is None:
        libs = []
    return env2.Program(target, source, LIBS = ['boot'] + libs)

bootable = Builder(action = '$LINK $LFLAGS $_LIBDIRFLAGS --section-start .text=0x100000 --section-start .rodata=0x10b000 --section-start .data=0x10e000 --section-start .bss=0x10f000 $SOURCES -lboot $_LIBFLAGS -o $TARGET')

env = Environment(CCFLAGS = '-g -Wall -Werror -m32 -Iinclude',
                  ASFLAGS = '-m32',
                  LFLAGS = '-melf_i386',
                  LIBPATH = ['../../lib'],
                  LINK = 'ld')

env.AddMethod(Bootable_method, 'Bootable')

Export('env')

SConscript(['lib/SConscript',
            'samples/SConscript',
            'scripts/SConscript'])
