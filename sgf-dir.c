
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>

#include "sgf-disk.h"
#include "sgf-fat.h"
#include "sgf-data.h"
#include "sgf-dir.h"


static int directory_first_block = -1;


/**********************************************************************
 rechercher et renvoyer l'adresse du descripteur d'un fichier.
 Cette fonction renvoie -1 en cas d'erreur.
 *********************************************************************/

int find_inode(const char* name)
    {
    int adr;
    TBLOCK b;
    int j;
    
    if (directory_first_block == -1) {
        read_block(ADR_BLOCK_DEF, & b.data);
        directory_first_block = b.super.adr_dir;
        }

    adr = directory_first_block;
    
    while (adr != FAT_EOF)
        {
        read_block(adr, &b.data);
        for(j = 0; j < BLOCK_DIR_SIZE; j++)
            if (b.dir[j].inode > 0)
                if (strcmp(b.dir[j].name, name) == 0)
                    return (b.dir[j].inode);
        adr = get_fat(adr);
        }
    
    return (-1);
    }


/**********************************************************************
 Ajouter un couple <name,inode> au répertoire. Si un couple existe déjà,
 la fonction renvoie l'adresse du descripteur et -1 dans le cas
 contraire.
 *********************************************************************/

int add_inode (const char* name, int inode)
    {
    int  oldinode, padr, adr, nadr;
    int j, nj;
    TBLOCK  b;
    
    if ((strlen(name) + 1) > LONG_FILENAME) {
        return (-1);
        }
    
    if (directory_first_block == -1) {
        read_block(ADR_BLOCK_DEF, & b.data);
        directory_first_block = b.super.adr_dir;
        }

    adr = directory_first_block;
    
    nadr = nj = -1;
    while (adr != FAT_EOF)
        {
        read_block(adr, &b.data);
        for(j = 0; j < BLOCK_DIR_SIZE; j++)
            if (b.dir[j].inode > 0)
                if (strcmp(b.dir[j].name, name) == 0)
                    {
                    oldinode = b.dir[j].inode;
                    b.dir[j].inode = inode;
                    write_block(adr, & b.data);
                    return (oldinode);
                    }
                else ;
            else nadr = adr, nj = j;
        padr = adr;
        adr = get_fat(adr);
        }

    if (nadr != -1) {
        read_block(nadr, & b.data);
        b.dir[nj].inode = inode;
        strcpy(b.dir[nj].name, name);
        write_block(nadr, & b.data);
        return (-1);
        }

    /** Allouer un nouveau bloc pour le répertoire **/
    adr = alloc_block();
    if (adr < 0) return (-1);
    
    /** Initialiser ce nouveau bloc **/
    for(j = 0; j < BLOCK_DIR_SIZE; j++)
        b.dir[j].inode = 0;

    /** Utiliser la 1ere entree de ce bloc **/
    b.dir[0].inode = inode;
    strcpy(b.dir[0].name, name);
    write_block(adr, & b.data);
    
    /** Mettre à jour la FAT **/
    set_fat(adr, FAT_EOF);
    set_fat(padr, adr);
    
    return (-1);
    }


/**********************************************************************
 Effacer un couple <name,inode> au répertoire.
 *********************************************************************/

void delete_inode (const char* name)
    {
    int adr;
    TBLOCK b;
    int j;
    
    if (directory_first_block == -1)
        {
        read_block(ADR_BLOCK_DEF, & b.data);
        directory_first_block = b.super.adr_dir;
        }

    adr = directory_first_block;
    
    while (adr != FAT_EOF)
        {
        read_block(adr, & b.data);
        for(j = 0; j < BLOCK_DIR_SIZE; j++)
            if (b.dir[j].inode > 0)
                if (strcmp(b.dir[j].name, name) == 0)
                    {
                    b.dir[j].inode = 0;
                    write_block(adr, & b.data);
                    return ;
                    }
        adr = get_fat(adr);
        }
    }


/**********************************************************************
 Formater le disque et créer un répertoire vide.
 *********************************************************************/

void create_empty_directory ()
    {
    int adr_repertoire;
    TBLOCK b;
    int j;
    
    /* lire le super bloc */
    read_block(0, &b.data);
    directory_first_block = adr_repertoire = b.super.adr_dir;
    
    /* vider le 1er bloc du répertoire et le sauver */
    for(j = 0; j < BLOCK_DIR_SIZE; j++) b.dir[j].inode = 0;
    write_block(adr_repertoire, & b.data);

    printf("create empty directory (block %d)\n", directory_first_block);
    }


/**********************************************************************
 Lister les fichiers du répertoire avec leur taille.
 *********************************************************************/

void list_directory (void)
{
	TBLOCK b, b2;
    int adr;
    int j;
	
	if (directory_first_block == -1) {
        read_block(ADR_BLOCK_DEF, & b.data);
        directory_first_block = b.super.adr_dir;
        }

    adr = directory_first_block;
    
    while (adr != FAT_EOF)
	{
        read_block(adr, &b.data);
        for(j = 0; j < BLOCK_DIR_SIZE; j++){
			if (b.dir[j].inode > 0)
			{
				read_block(b.dir[j].inode,&b2.data);
				printf("%s : Taille : %d\n",b.dir[j].name,b2.inode.length);	
			}
		}
        adr = get_fat(adr);
	}
}


