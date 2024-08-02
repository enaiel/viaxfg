
ifeq ($(DJGPP),)
$(error ERROR: DJGPP not defined! ***)
endif

RM=del
CP=cp
ZIP=zip -r
DPMI=cwsdpmi.exe

PROG=viaxfg
VER=0.3.0
TARGET=$(PROG).exe
TARGETTXT=$(PROG).txt
TARGETZIP=$(PROG)-$(VER).zip
TARGETDISTZIP=$(PROG)_dist-$(VER).zip

all:
	-$(MAKE) -C src all

clean:
	-$(MAKE) -C src clean

dist: 
	-$(CP) src\$(TARGET) .
	-$(RM) src\$(TARGET)
	-$(RM) dist\$(TARGETZIP)
	-$(ZIP) dist\$(TARGETZIP) $(TARGET) $(TARGETTXT) $(DPMI)
	-$(RM) dist\$(TARGETDISTZIP)
	-$(ZIP) dist\$(TARGETDISTZIP) * -x bak* dist* *.o

.PHONY:	dist
