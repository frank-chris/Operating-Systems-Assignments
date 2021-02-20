
IDIR = ../include
ODIR = obj

CC = gcc
CFLAGS = -I$(IDIR)
LINKERFLAG = -lm

.PHONY = clean

SOURCES = $(wildcard *.c)
BINARY = shell
_DEPS = $(wildcard $(IDIR)/*.h)
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))
_OBJS = $(SOURCES:.c=.o)
OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))

$(ODIR)/%.o: %.c $(DEPS)
	@echo "Building"
	$(CC) -c -o $@ $< $(CFLAGS)

$(BINARY): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LINKERFLAG)

clean:
	@echo "Cleaning"
	rm -rvf $(ODIR)/*.o $(BINARY)
