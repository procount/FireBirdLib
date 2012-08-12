PROJECT = libFireBird

DIRS = av compression debug elf ExtAttribs Firmware flash FontManager hdd ini InstructionSequences LogoManager main rec shutdown string StringDB tap tapcom time TMSOSDMenu TMSOSDKeyboard tmsvfd

BASE = $(shell cd /tapapi/TMS; pwd)
include ${BASE}/include/tool.mk

all: lib devutils

docs: HTMLDOCS

.PHONY: doc

doc:
	markdown README.md >README.html

lib:
	@for i in $(DIRS); \
	do \
	  cd $$i; \
	  make all; \
	  cd ..; \
	done
	-$(RM) $(PROJECT).a
	@for i in $(DIRS); \
	do \
	  $(AR) -r $(PROJECT).a $$i/*.o; \
	done
	@$(RANLIB) $(PROJECT).a

.PHONY:	devutils

devutils:
	cd $@; \
	make all;

clean:
	@for i in $(DIRS); do cd $$i; make clean; cd ..; done
	cd devutils; make clean; cd ..;
	rm -rf $(PROJECT).a HTMLDOCS README.html

.PHONY: HTMLDOCS

HTMLDOCS:
	rm -rf $@
	doxygen
