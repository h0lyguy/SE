
/*
**  hardware.c
**
**  Simulation du matériel.
**
*/


#include <stdio.h>
#include <setjmp.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "sgf-disk.h"


/*****************************************************************
 *
 *  Fonctions de gestion du disque virtuel.
 *
 ****************************************************************/

int trace_sgf_disk = 0;


static struct HARD_DISK {
    FILE*   file;
    int     size;
    int     exist;
    int     scaned;
    char    nom[32];
    }
    dd = {NULL, 0, 0, 0, ""};


#define DISK_OK(n)              (((n) >= 0) && ((n) < 4))
#define DISK_EXIST(n)           (hdisk[(n)].exist != 0)
#define DISK_SCANED(n)          (hdisk[(n)].scaned != 0)
#define NU_BLOC_OK(n)           (((n) >= 0) && ((n) < dd.size))


/*****************************************************************
 lire un bloc physique a partir du disque.
 ****************************************************************/

void read_block(int n, BLOCK* bloc)
    {
    if (!dd.exist) init_sgf_disk();
    
    if (!NU_BLOC_OK(n))
        {
        panic("sgf-disk: read_block: n° de bloc incorrect.");
        }
        
    if (fseek(dd.file, (n * BLOCK_SIZE), SEEK_SET) == 0)
        if (BLOCK_SIZE == fread(bloc, 1, BLOCK_SIZE, dd.file))
            {
            if (trace_sgf_disk)
                {
                fprintf(stderr, "read block %d\n", n);
                }
            return ;
            }
    
    panic("sgf-disk: read_block: impossible de lire le bloc %d\n", n);
    exit(EXIT_FAILURE);
    }


/************************************************************
 ecrire un bloc physique sur disque.
 ************************************************************/

void write_block(int n, BLOCK* b)
    {
    if (!dd.exist) init_sgf_disk();
    
    if (n < 0  ||  n >= dd.size)
        {
        panic("sgf-disk: write_block: n° de bloc incorrect.");
        }
        
    if (fseek(dd.file, (n * BLOCK_SIZE), SEEK_SET) == 0)
        if (BLOCK_SIZE == fwrite(b, 1, BLOCK_SIZE, dd.file))
            {
            fflush(dd.file);
            if (trace_sgf_disk)
                {
                fprintf(stderr, "write block %d\n", n);
                }
            return ;
            }
    
    panic("sgf-disk: impossible d'écrire le bloc %d\n", n);
    exit(EXIT_FAILURE);
    }


/************************************************************
 renvoyer la taille du disque en blocs.
 ************************************************************/

int get_disk_size()
    {
    if (!dd.exist) init_sgf_disk();
    
    return (dd.size);
    }


/************************************************************
 tester la presence d'un disque virtuel concretiser par
 un fichier dont le nom est passe en parametre.
 ************************************************************/

int test_disk(char* name)
    {
    long size;
    FILE* file;
    
    dd.file = NULL;
    dd.exist = 0;
    dd.size = 0;
    dd.scaned = 0;
    strcpy(dd.nom, "");
    
    file = fopen(name, "r+b");
    if (file == NULL) return (0);
    
    if (fseek(file, 0, SEEK_END) != 0) {
        fclose(file);
        return (0);
        }
    
    size = (ftell(file) / BLOCK_SIZE);
    if (size <= 0) {
        fclose(file);
        return (0);
        }
    
    if (size > 10000) {
        fclose(file);
	panic("sgf-disk: test_disk: disque %s trop important.", name);
        return (0);
        }
    
    dd.file = file;
    dd.exist = 1;
    dd.size = size;
    dd.scaned = 1;
    strcpy(dd.nom, name);
    
    return (1);
    }


/************************************************************
 afficher un message d'erreur et arreter la simulation.
 ************************************************************/

void panic (const char *format, ...)
    {
    va_list ap;
    
    fprintf(stderr, "Panique: ");
    va_start(ap, format);
    vfprintf(stderr, format, ap);
    va_end(ap);
    fprintf(stderr, "\n");
    exit(EXIT_FAILURE);
    /* ARRET DE LA SIMULATION */
    }


/************************************************************
 initialiser le gestionnaire de disque.
 ************************************************************/

void init_sgf_disk() {
	/* tester les quatre disques */
	if (test_disk("disk0")) return ;
	if (test_disk("disk1")) return ;
	if (test_disk("disk2")) return ;
	if (test_disk("disk3")) return ;
	
	panic("sgf-disk: init_sgf_disk: impossible de trouver un disque");
}


