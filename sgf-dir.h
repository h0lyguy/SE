
#ifndef __SGF_REP_H__
#define __SGF_REP_H__


/**********************************************************************
 Rechercher l'adresse d'un inode à partir d'un nom de fichier.
 cette fonction renvoie -1 en cas d'erreur.
 *********************************************************************/

int find_inode (const char* nom);

/**********************************************************************
 Ajouter un couple <nom,inode> au répertoire. Si il existe déjà un
 couple <nom,inode'> la fonction renvoie inode' sinon elle renvoie -1.
 *********************************************************************/

int add_inode (const char* nom, int desc);

/**********************************************************************
 Effacer un couple <nom,desc> du répertoire.
 *********************************************************************/

void delete_inode (const char* nom);

/**********************************************************************
 Formater un disque et créer un répertoire vide.
 *********************************************************************/

void create_empty_directory (void);

/**********************************************************************
 Lister les fichiers du répertoire avec leur taille.
 *********************************************************************/

void list_directory (void);


#endif
