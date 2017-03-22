
/*
**  sgf-fat.c
**
**  Gestion de la FAT disque
**
*/


#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "sgf-disk.h"
#include "sgf-data.h"
#include "sgf-fat.h"


#define PAR_EXCES(n,d)          (((n) + (d) - 1) / (d))


/**********************************************************************
 *
 *  Définition de la FAT en mémoire centrale.
 *
 *********************************************************************/

typedef struct FAT
    {
    int    in_memory;           /* la FAT est-elle en mémoire ?         */
    int    fat_size_in_blocks;  /* taille de la FAT en blocs            */
    int    disk_size;           /* taille du disque en blocs            */
    int*   tab;                 /* la FAT en mémoire                    */
    BLOCK* blocks;              /* la FAT vu comme un tab. de blocs     */
    int*   modif;               /* pour chaque bloc un bit de modif     */
    }
    FAT;

static FAT  fat = {0, 0, 0, NULL, NULL, NULL};


/**********************************************************************
 *
 *  Création d'une FAT vide
 *
 *********************************************************************/

static void create_memory_fat (void)
    {
    int fat_size_in_bytes;
    int k;

    if (fat.tab != NULL)
        {
        free(fat.tab);
        free(fat.modif);
        fat.in_memory = 0;
        }
    
    fat.disk_size = get_disk_size();
    fat_size_in_bytes = (fat.disk_size * sizeof(int));
    fat.fat_size_in_blocks = PAR_EXCES(fat_size_in_bytes, BLOCK_SIZE);
    fat_size_in_bytes = (fat.fat_size_in_blocks * BLOCK_SIZE);
    
    fat.tab = malloc(fat_size_in_bytes);
    fat.blocks = (BLOCK*) fat.tab;
    fat.modif = malloc(fat.fat_size_in_blocks * sizeof(int));
    if (fat.tab == NULL || fat.modif == NULL)
        panic("impossible d'allouer la FAT en mémoire.");
    
    /* le bloc physique 0 est réservé pour le bloc de définition */
    fat.tab[0] = FAT_RESERVED;

    for(k = 0; (k < fat.fat_size_in_blocks); k++)
        {
        fat.tab[k + ADR_BLOCK_FAT] = FAT_RESERVED;
        fat.modif[k] = 1;
        }
    
    k = fat.fat_size_in_blocks + ADR_BLOCK_FAT;
    for(; (k < fat.disk_size); k++)
        fat.tab[k] = FAT_FREE;
    }


/**********************************************************************
 *
 *  Chargement de la FAT en mémoire centrale.
 *
 *********************************************************************/

void init_sgf_fat (void)
    {
    TBLOCK block;
    int k;
    
    create_memory_fat();
    
    /* verifier la signature */
    read_block(0, &block.data);
    if (block.super.signature != SIGNATURE_SUPER_BLOCK) {
        panic("Le disque n'est pas formatté.");
        }
    
    for(k = 0; (k < fat.fat_size_in_blocks); k++)
        {
        read_block(k + ADR_BLOCK_FAT, & fat.blocks[k]);
        fat.modif[k] = 0;
        }
    
    fat.in_memory = 1;
    }


/**********************************************************************
 *
 *  Sauvegarde des blocs modifiés de la FAT sur disque
 *
 *********************************************************************/

void save_fat (void)
    {
    int k;
    
    for(k = 0; (k < fat.fat_size_in_blocks); k++)
        if (fat.modif[k])
            {
            write_block(k + ADR_BLOCK_FAT, & fat.blocks[k]);
            fat.modif[k] = 0;
            }
    }


/**********************************************************************
 *
 *  Lire une entrée de la FAT
 *
 *********************************************************************/

int get_fat (int n)
    {
    if (!fat.in_memory)
        panic("La FAT n'est pas initialisée.");
    
    if (n < 0  ||  n >= fat.disk_size)
        panic("Utilisation de <<get_fat>> incorrecte.");
    
    return fat.tab[ n ];
    }


/**********************************************************************
 *
 *  Changer la valeur d'une entrée de la FAT et la sauver
 *  sur disque.
 *
 *********************************************************************/

void set_fat (int n, int valeur)
    {
    if (!fat.in_memory)
        panic("La FAT n'est pas initialisée.");
    
    if (n < 0  ||  n >= fat.disk_size)
        panic("Utilisation de <<get_fat>> incorrecte.");
    
    assert(
        ((valeur) == FAT_FREE) ||
        ((valeur) == FAT_INODE) ||
        ((valeur) == FAT_EOF) ||
        ((valeur) >= 0 && (valeur) < fat.disk_size)
    );
    
    fat.tab[ n ] = valeur;
    fat.modif[ n / (BLOCK_SIZE / sizeof(int)) ] = 1;
    save_fat ();
    }


/**********************************************************************
 *
 *  Rechercher un bloc physique libre en parcourant la FAT.
 *  (en cas d'erreur cette fonction renvoie -1).
 *
 *********************************************************************/

int alloc_block (void)
    {
    int k;
    
    if (!fat.in_memory)
        panic("La FAT n'est pas initialisée.");
    
    for(k = 0; k < fat.disk_size; k++)
        if (fat.tab[k] == FAT_FREE)
            return (k);

    return (-1);
    }


/**********************************************************************
 *
 *  Initialiser le disque avec une FAT vide.
 *
 *********************************************************************/

void create_empty_fat ()
    {
    int fat_size_in_bytes;
    int fat_size_in_blocks;
    TBLOCK super_bloc;
    BLOCK *blocks;
    int *tab;
    int k;
    int adr_rep;
    int disk_size = get_disk_size();
    
    fat_size_in_bytes  = (disk_size * sizeof(int));
    fat_size_in_blocks = PAR_EXCES(fat_size_in_bytes, BLOCK_SIZE);
    fat_size_in_bytes  = (fat_size_in_blocks * BLOCK_SIZE);
    
    tab = malloc(fat_size_in_bytes);
    blocks = (BLOCK*) tab;
    if (tab == NULL)
        panic("FAT: create_empty_fat: impossible d'allouer la FAT en mémoire.");
    
    /* le bloc physique 0 est réservé pour le Super Bloc */
    /* ------------------------------------------------- */

    tab[0] = FAT_RESERVED;

    for(k = 0; (k < fat_size_in_blocks); k++)
        {
        tab[k + ADR_BLOCK_FAT] = FAT_RESERVED;
        }
    
    k = fat_size_in_blocks + ADR_BLOCK_FAT;
    for(; (k < disk_size); k++)
        {
        tab[k] = FAT_FREE;
        }
    
    /* Rechercher un bloc libre et le prendre pour le répertoire */
    /* --------------------------------------------------------- */

    for(k = 0; (tab[k] != FAT_FREE); k++) ;
    adr_rep = (k);
    tab[k] = FAT_EOF ;
    
    /* Ecrire la FAT sur le disque */
    /* --------------------------- */

    for(k = 0; (k < fat_size_in_blocks); k++)
        {
        write_block(k + ADR_BLOCK_FAT, & blocks[k]);
        }

    /* Préparer et écrire le Super Bloc sur le disque */
    /* ---------------------------------------------- */
    
    super_bloc.super.signature = SIGNATURE_SUPER_BLOCK;
    super_bloc.super.adr_dir = adr_rep;
    write_block(0, & super_bloc.data);
    
    /* Liberer la FAT en mémoire */
    /* ------------------------- */
    
    printf("writing empty FAT done (block %d to %d)\n", 1, adr_rep - 1);

    free(tab);
    }

