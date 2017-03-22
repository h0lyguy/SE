
#ifndef __SGF_IO__
#define __SGF_IO__


/**********************************************************************
 *
 *  La structure OFILE décrit un fichier ouvert.
 *
 *  Cette structure comporte des informations sur l'implantation
 *  physique du fichier ainsi que des champs destinés à décrire
 *  l'état du fichier.
 *
 *********************************************************************/

#define READ_MODE       (0)
#define WRITE_MODE      (1)

struct OFILE            /* "Un fichier ouvert"                  */
    {                   /* ------------------------------------ */
    int   length;       /* taille du fichier (en octets)        */
    int   first;        /* adresse du premier bloc logique      */
    int   last;         /* adresse du dernier bloc logique      */
    int   inode;        /* adresse de l'INODE (descripteur)     */
    int   ptr;          /* n° logique du prochain caractère     */

    int   mode;         /* READ_MODE ou WRITE_MODE              */
    BLOCK buffer;       /* buffer contenant le bloc courant     */
    };

typedef struct OFILE OFILE;

/**********************************************************************
 *
 *  ROUTINES DE GESTION DES E/S VERS DES FICHIERS OUVERTS (OFILE)
 *
 *********************************************************************/

/************************************************************
 *  Ecrire un caractère/une chaîne sur un fichier ouvert en
 *  écriture.
 ************************************************************/

    void sgf_puts (OFILE* f, char *s);
    void sgf_putc (OFILE* f, char  c);

/************************************************************
 *  Lire un caractère sur un fichier ouvert en lecture.
 *  renvoyer -1 en cas de fin de fichier.
 ************************************************************/

    int sgf_getc (OFILE* f);

/************************************************************
 *  Ouvrir/Fermer/Partager un fichier.
 ************************************************************/

    OFILE* sgf_open  (const char *nom, int mode);
    void   sgf_close (OFILE* f);

/**********************************************************************
 * Initialiser le Système de Gestion de Fichiers.
 *********************************************************************/

    void init_sgf ();

/***********************************************************************/
	int sgf_seek (OFILE* f, int pos);
#endif
