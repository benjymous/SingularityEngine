# provide TITLE_NAME via commandline
# e.g.
# > make TITLE_NAME=tetronimo

CC=zcc
AS=zcc
TARGET=+zx
VERBOSITY=-vn
CRT=31

ROOTDIR = ../../..
EXDIR = $(ROOTDIR)/examples/$(TITLE_NAME)
PLATDIR = ..
SRCDIR = src
DATDIR = data
OBJDIR = build

PRAGMA_FILE=$(SRCDIR)/zpragma.h

C_OPT_FLAGS=-SO3 -I$(ROOTDIR)/include -I$(PLATDIR)/include

OUT_FLAGS = --c-code-in-asm --list

APP_FLAGS =

CFLAGS=$(TARGET) $(VERBOSITY) -c $(C_OPT_FLAGS) -clib=new -pragma-include:$(PRAGMA_FILE) --c-code-in-asm 
LDFLAGS=$(TARGET) $(VERBOSITY) -clib=new -pragma-include:$(PRAGMA_FILE)  --fverbose-asm -m 
ASFLAGS=$(TARGET) $(VERBOSITY) -c


EXEC=$(TITLE_NAME).tap

all : $(EXEC)

OBJECTS = 	$(OBJDIR)/$(TITLE_NAME).o \
			$(OBJDIR)/zxmain.o \
			$(OBJDIR)/int.o \
			$(OBJDIR)/graphics.o

# .c files in the platforms/zxspectrum/<name>/src/ folder
$(OBJDIR)/%.o: $(SRCDIR)/%.c $(PRAGMA_FILE)
	$(CC) $(CFLAGS) -o $@ $<

# .c files in the Examples/<name>/ folder
$(OBJDIR)/%.o: $(EXDIR)/%.c $(PRAGMA_FILE)
	$(CC) $(CFLAGS) -o $@ $<

# .c files in the platforms/zxspectrum/ folder
$(OBJDIR)/%.o: $(PLATDIR)/%.c $(PRAGMA_FILE)
	$(CC) $(CFLAGS) -o $@ $<

# data asm files
$(OBJDIR)/%.o: $(DATDIR)/%.asm
	$(AS) $(ASFLAGS) -o $@ $<

# final link
$(EXEC) : $(OBJECTS) 
	 $(CC) $(LDFLAGS) -startup=$(CRT) $(OBJECTS) -o $(TITLE_NAME) $(OUT_FLAGS) -create-app $(APP_FLAGS)

.PHONY: clean
clean:
	-rm -f *.o *.bin *.tap *.map zcc_opt.def *~ /tmp/tmpXX*