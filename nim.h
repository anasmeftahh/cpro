/*******************************************************************************/
/*  NIM.H                                                                      */
/*  Ce module contient les fonctions necéssaires à l'implémantation du jeu NIM */
/*******************************************************************************/
#ifndef MANIP_PLATEAU_H_
#define MANIP_PLATEAU_H_

#include "codage_numerique.h"
#include "m_distributions.h"

#define PLATEAU_MAX_COLONNES 20		//Nombre maximal de colonnes sur le plateau
#define PLATEAU_MAX_PIECES 35		//Nombre maximal de pièces par colonne
#define CODAGE_NB_BITS 8            //Nombre maximal de bits par ligne de la matrice
#define TRUE   1
#define FALSE  0

/*******************************************************************************/
/*                   DÉCLARATION DES FONCTIONS PUBLIQUES                       */
/*******************************************************************************/

int lire_entier(int min, int max);

void plateau_init(int plateau[], int nb_colonnes);

int nim_jouer_tour(int plateau[], int nb_colonnes, int colonne, int nb_pieces);

void plateau_supprimer_colonne(int plateau[], int nb_colonnes, int col_a_supprimer);

int plateau_defragmenter(int plateau[], int nb_colonnes);

void nim_choix_ia_aleatoire(const int plateau[], int nb_colonnes, int* choix_colonne, int* choix_nb_pieces);

void plateau_afficher(const int plateau[], int nb_colonnes, int col_select);

void nim_choix_ia(const int plateau[], int nb_colonnes, int niveau,
    int* choix_colonne, int* choix_nb_pieces);

static int choisir_colonne(int plateau[], int nb_colonnes);

void tour_humain(int plateau[], int nb_colonnes);

void tour_ia(int plateau[], int nb_colonnes, int difficulte);

void demarrer_jeu(int niveau);


/*******************************************************************************/
/*                                TEST 2                                       */
/*******************************************************************************/

int inverser_tab_bits(int tab_bits[], int nb_bits);

int codage_dec2bin(int nombre, int resultat[]);

void afficher_tab_bits(const int tab_bits[], int nb_bits);

int codage_bin2dec(const int tab_bits[]);

void construire_mat_binaire(const int plateau[], int nb_colonnes,
    int matrice[][CODAGE_NB_BITS]);

void sommes_mat_binaire(const int matrice[][CODAGE_NB_BITS],
    int nb_lignes, int sommes[]);

int position_premier_impaire(const int tab[]);

#endif