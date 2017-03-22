
#ifndef __SGF_FAT__
#define __SGF_FAT__


/**********************************************************************
 *
 *  ROUTINES DE MANIPULATION DE LA FAT (File Allocation Table)
 *
 *********************************************************************/

#define FAT_FREE                (-1)
#define FAT_RESERVED            (-2)
#define FAT_INODE               (-3)
#define FAT_EOF                 (-4)

/**********************************************************************
 Rechercher un bloc libre sur le disque en parcourant la FAT. Cette
 fonction renvoie 0 en cas d'erreur.
 *********************************************************************/

    int alloc_block (void);

/**********************************************************************
 Lire/Ecrire l'entrée numéro "n" dans la FAT du disque.
 Ces fonctions ne génèrent aucune erreur.
 *********************************************************************/

    int get_fat (int n);
    void set_fat (int n, int valeur);

/**********************************************************************
 Charger la FAT d'un disque en mémoire pour que ce disque soit
 utilisable (monté).
 *********************************************************************/

    void init_sgf_fat (void);

/**********************************************************************
 Formater le disque en écrivant une FAT vide sur disque.
 Ces fonctions ne génère aucune erreur.
 *********************************************************************/

    void create_empty_fat (void);


#endif
