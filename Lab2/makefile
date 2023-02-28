# Ahmaad Ansari    100785574
# Hanzalah Patel   100785622
# Waddah Saleh     100785692
# Huzaifa Zia      100779087

CC = gcc
CFLAGS = -Wall
LFLAGS = 
LIBS = 
SOURCES = myshell.c
OBJECTS = $(subst .c,.o,$(SOURCES))
EXE = myshell
.PHONY: clean help

$(EXE) : $(OBJECTS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@ 

%.o : %.c
	$(CC) $(CFLAGS) -c $< 

all : $(EXE)

clean:
	rm -f $(OBJECTS) $(EXE) *~

cleanup:
	rm -f $(OBJECTS) *~

help:
	@echo "Valid targets:"
	@echo "  all:    generates all binary files"
	@echo "  clean:  removes .o and .exe files"
