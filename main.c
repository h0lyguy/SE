
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
	OFILE* file;
	int c,i;
	
	init_sgf();
	
	printf("\nListing du disque\n\n");
	list_directory();
	
	file = sgf_open("essai.txt", READ_MODE);
	i=0;
	while ((c = sgf_getc(file)) > 0) {
		putchar(c);
		sgf_seek(file,i);
		i=i+8;
		}
	sgf_close(file);
	
	return (EXIT_SUCCESS);
	}

