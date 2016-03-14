depth:=.
subdirs:=lib samples tests

include make.mk

.PHONY:: test

test: tests/test_main.exe scripts/run
	./scripts/run.py tests/test_main.exe
