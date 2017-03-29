
/*
**  main.c
**
**  Utilisation des fonctions du mini SGF.
**
**  04/04/2007
*/

#include <stdio.h>
#include <stdlib.h>

#include "sgf-disk.h"
#include "sgf-fat.h"
#include "sgf-dir.h"
#include "sgf-io.h"

int main() {
	//~ OFILE* file;
	int c,i;
	//~ 
	init_sgf();
	//~ 
	//~ printf("\nListing du disque\n\n");
	list_directory();
	//~ 
	//~ file = sgf_open("essai.txt", READ_MODE);
	//~ i=0;
	//~ while ((c = sgf_getc(file)) > 0) {
		//~ putchar(c);
		//~ sgf_seek(file,i);
		//~ i=i+8;
		//~ }
	//~ sgf_close(file);
	OFILE* file = sgf_open("essai.txt", WRITE_MODE);
	sgf_puts(file, "Ceci est un petit texte qui occupe\n");
	sgf_puts(file, "quelques blocs sur ce disque fictif.\n");
	sgf_puts(file, "Le bloc faisant 128 octets, il faut\n");
	sgf_puts(file, "que je remplisse pour utiliser\n");
	sgf_puts(file, "plusieurs blocs.\n");
	sgf_close(file);
	
	return (EXIT_SUCCESS);
}

