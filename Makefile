
CFLAGS=-ansi -Wall
CSRC=$(wildcard sgf*.c)
OBJ=$(CSRC:.c=.o)
HDR=$(CSRC:.c=.h)
EXE=sgf

all : $(EXE)
	@test -e Makefile2 && make -f Makefile2 all || true

clean:
	@rm -vf $(OBJ) $(EXE)
	@test -e Makefile2 && make -f Makefile2 clean || true

$(EXE): $(OBJ) main.c
	@echo "Assemblage de $(EXE)"
	@$(CC) -o $(EXE) main.c $(OBJ)

%.o: %.c $(HDR)
	$(CC) $(CFLAGS) -c -o $@ $<

disk:
	@echo "Creation du disque disk0"
	@dd if=/dev/zero of=disk0 bs=128 count=1000

zip:
	@rm -f $(OBJ)
	@rm -f ../mini-sgf.zip
	@cd .. ; zip -r mini-sgf.zip mini-sgf/

