PROJECT = libFireBird

DIRS = av compression debug elf EPG ExtAttribs Firmware flash FontManager hdd ini InstructionSequences LogoManager main rec shutdown string StringDB tap tapcom time TMSOSDMenu TMSOSDKeyboard tmsvfd

BASE = $(shell cd /tapapi/TMS; pwd)
include ${BASE}/include/tool.mk

all: lib devutils readme

.PHONY: readme devutils

lib:
	@for i in $(DIRS); \
	do \
	  cd $$i; \
	  $(MAKE) all; \
	  cd ..; \
	done
	-$(RM) $(PROJECT).a
	@for i in $(DIRS); \
	do \
	  $(AR) -r $(PROJECT).a $$i/*.o; \
	done
	@$(RANLIB) $(PROJECT).a

devutils:
	cd $@; \
	$(MAKE) all;

docs:
	rm -rf $@
	doxygen

readme:
	markdown README.md >README.html

clean:
	@for i in $(DIRS); do cd $$i; make clean; cd ..; done
	cd devutils; make clean; cd ..;
	rm -rf $(PROJECT).a HTMLDOCS README.html
