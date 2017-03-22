
#ifndef __DRIVER_DISK__
#define __DRIVER_DISK__


/************************************************************
 *
 *  ROUTINES DE BASE POUR LES E/S DE BLOCKS.
 *
 ***********************************************************/

/**********************************************************************
 *
 *  Définition d'un bloc. Le bloc est l'unité élémentaire
 *  d'entrée/sortie sur les supports. Le matériel est capable de
 *  lire et d'écrire un bloc sur l'unité.
 *
 *********************************************************************/
 
#define BLOCK_SIZE              (128)    /* 128 octets */

typedef char BLOCK[ BLOCK_SIZE ];


/************************************************************
 Récupérer la taille du disque (en blocs)
 ***********************************************************/
 
int get_disk_size();

/************************************************************
 Primitives de bas niveau pour lire ou écrire un bloc sur
 disque. ATTENTION: pour simplifier, les E/S sont synchrones.
 En d'autres termes, ces procédures ATTENDENT la fin de l'E/S
 ***********************************************************/
 
void read_block (int n, BLOCK* b);
void write_block (int n, BLOCK* b);

/************************************************************
 initialisation et découverte du disque
 ***********************************************************/
 
void init_sgf_disk (void);


/************************************************************
 Afficher le message d'erreur et stopper la simulation.
 ************************************************************/

void panic (const char *format, ...);

#endif
