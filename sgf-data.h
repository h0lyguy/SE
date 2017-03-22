
#ifndef __SGF_DATA__
#define __SGF_DATA__


#define ADR_BLOCK_DEF           (0)
#define ADR_BLOCK_FAT           (1)


/**********************************************************************
 *
 *  Structure de donnée pour représenter le répertoire.
 *
 *********************************************************************/

#define LONG_FILENAME           (32 - sizeof(int))
#define BLOCK_DIR_SIZE          (BLOCK_SIZE / sizeof(DIR_ENTRY))

typedef struct DIR_ENTRY        /* Une entrée de répertoire         */
    {                           /* -------------------------------- */
    char name [LONG_FILENAME];  /* nom du fichier                   */
    int  inode;                 /* adresse du descripteur           */
    }
    DIR_ENTRY;

typedef  DIR_ENTRY       BLOCK_DIR [ BLOCK_DIR_SIZE ];


/**********************************************************************
 *
 *  Structure de donnée pour représenter les descripteurs.
 *
 *********************************************************************/

typedef struct INODE            /* INODE: Descripteur de fichiers   */
    {                           /* -------------------------------- */
    int  length;                /* taille du fichier (en octets)    */
    int  first;                 /* adresse du premier bloc logique  */
    int  last;                  /* adresse du dernier bloc logique  */
    }
    INODE;


/**********************************************************************
 *
 *  Définition d'un Super Bloc (Bloc de définition du système
 *  de fichiers).
 *
 *********************************************************************/

#define SIGNATURE_SUPER_BLOCK   (0xAA88FF33)

typedef struct SUPER_BLOCK      /* Bloc d'un <<super bloc>>         */
    {                           /* -------------------------------- */
    int  signature;             /* signature du système de fichiers */
    int  adr_dir;               /* adr du 1er bloc du répertoire    */
    }
    SUPER_BLOCK;


/**********************************************************************
 *
 *  Définition d'un bloc typé qui est soit un super-bloc, soit un bloc
 *  répertoire, soit un INODE, soit un bloc de données.
 *
 *********************************************************************/

typedef union
    {
    SUPER_BLOCK super;
    BLOCK_DIR   dir;
    INODE       inode;
    BLOCK       data;
    }
    TBLOCK;



#endif
