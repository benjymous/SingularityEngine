CC=zcc
AS=zcc
TARGET=+zx
VERBOSITY=-vn
CRT=31
EXDIR = ../../../examples
SRCDIR = src
DATDIR = data
OBJDIR = build

PRAGMA_FILE=$(SRCDIR)/zpragma.h

C_OPT_FLAGS=-SO3 -I../../../include -I../include

OUT_FLAGS = --c-code-in-asm --list

APP_FLAGS =

CFLAGS=$(TARGET) $(VERBOSITY) -c $(C_OPT_FLAGS) -clib=new -pragma-include:$(PRAGMA_FILE) --c-code-in-asm 
LDFLAGS=$(TARGET) $(VERBOSITY) -clib=new -pragma-include:$(PRAGMA_FILE)  --fverbose-asm -m 
ASFLAGS=$(TARGET) $(VERBOSITY) -c

EXEC_OUTPUT=tetronimo
EXEC=$(EXEC_OUTPUT).tap

all : $(EXEC)

OBJECTS = 	$(OBJDIR)/$(EXEC_OUTPUT).o \
			$(OBJDIR)/zxmain.o \
			$(OBJDIR)/assets.o

$(OBJDIR)/$(EXEC_OUTPUT).o: $(EXDIR)/$(EXEC_OUTPUT)/$(EXEC_OUTPUT).c $(PRAGMA_FILE)
	$(CC) $(CFLAGS) -o $@ $<

$(OBJDIR)/zxmain.o: ../zxmain.c $(PRAGMA_FILE)
	$(CC) $(CFLAGS) -o $@ $<

$(OBJDIR)/%.o: $(DATDIR)/%.asm
	$(AS) $(ASFLAGS) -o $@ $<

$(EXEC) : $(OBJECTS) 
	 $(CC) $(LDFLAGS) -startup=$(CRT) $(OBJECTS) -o $(EXEC_OUTPUT) $(OUT_FLAGS) -create-app $(APP_FLAGS)

.PHONY: clean
clean:
	-rm -f *.o *.bin *.tap *.map zcc_opt.def *~ /tmp/tmpXX*