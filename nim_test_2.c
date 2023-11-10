#define _CRT_SECURE_NO_WARNINGS

#include "nim.h"
#include "nim_io.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

int main()
{
	int tab[7] = { 19, 10, 14, 13, 4, 20, 7 };
	int matrice[20][8];

	construire_mat_binaire(tab, 7, matrice);
}

/*
Fonction qui inverse les "nb_bits/2" premières valeurs d'un tableau de bits:
le premier bit devient la dernier (et inversement),
le deuxième devient l'avant dernier, etc.
Cette fonction est utilisée par la fonction "codage_dec2bin".
Retour de 1 si l’opération a été effectuée et 0 sinon ("nb_bits" trop gros).
*/
int inverser_tab_bits(int tab_bits[], int nb_bits)
{
	int tampon, i, j = nb_bits/2;

	printf("\n");
	for (i = 0; i < nb_bits; i++)
		printf("%d\t", tab_bits[i]);
	printf("\n");

	if (nb_bits <= CODAGE_NB_BITS)
	{
		for (i = 0; i < (nb_bits / 4); i++)
		{
			tampon = tab_bits[i];
			tab_bits[i] = tab_bits[j];
			tab_bits[j] = tampon;
			j--;
		}
		
		for (i = 0; i < nb_bits; i++)
			printf("%d\t", tab_bits[i]);

		return 1;
	}

	else
	{
		for (i = 0; i < nb_bits; i++)
			printf("%d\t", tab_bits[i]);
		return 0;
	}
}

/*
Traduit un nombre décimal en binaire.
Le résultat est stocké dans le tableau resultat et le nombre de bits utilisés est renvoyé.
Le codage du nombre décimal doit se faire en un maximum de CODAGE_NB_BITS (fixée à 8).
La fonction renvoie le nombre de bits qui a été nécessaire pour coder le nombre en binaire.
Si le nombre requiert plus que CODAGE_NB_BITS, la fonction renvoie 0.
*/
int codage_dec2bin(int nombre, int resultat[])
{
	int i;

	if (nombre >= (pow(2, CODAGE_NB_BITS))) // 2^8 et plus
		return 0;
	else
	{
		for (i = 0; nombre || i <= CODAGE_NB_BITS; i++)
		{
			resultat[i] = (nombre % 2);
			nombre /= 2;
		}

		return i;
	}
}

/*Traduit un tableau de bits, représentant un nombre en binaire,
vers sa représentation décimale. La valeur décimale est retournée
par la fonction. */
int codage_bin2dec(const int tab_bits[])
{
	int i;
	int nb = 0;

	for (i = 0; i < CODAGE_NB_BITS; i++)
		nb += (pow(2, i) * tab_bits[i]);

	return nb;
}

/*Fonction d'une matrice binaire qui aide l'ordinateur à choisir comment jouer.
Chaque ligne de la matrice correspond à une colonne du plateau de jeu et
indique combien de pièces sont dans cette colonne en utilisant des 0 et des 1.
Le résultat est stocké dans le tableau "matrice".*/ 
void construire_mat_binaire(const int plateau[], int nb_colonnes, int matrice[][CODAGE_NB_BITS]) {
	// Pour chaque colonne sur le plateau
	int i;
	int tab_bits[CODAGE_NB_BITS]; //Ligne du tableau à copier dans la matrice
	int nb_bits; //Retour de codage_dec2bin

	for (i = 0; i < nb_colonnes; i++) {
		clrscr();

		nb_bits = codage_dec2bin(plateau[i],tab_bits);

		if (nb_bits == 0)
		{
			clrscr();
			printf("Erreur! Le nb de bits est plus grand que 8.");
		}

		strcpy(matrice[i], tab_bits);
		afficher_tab_bits(matrice, CODAGE_NB_BITS);
		printf("\n");
	}
}

void afficher_tab_bits(const int tab_bits[], int nb_bits){

	int i = 0;
	while (i<nb_bits){
		printf("%d ", tab_bits[i]);
		i++;
	}
}

/*
Calcule les sommes des colonnes d'une matrice binaire.
La matrice contient "nb_lignes" lignes et chaque ligne représente
une colonne du plateau de jeu sous forme binaire (0 et 1).
Les résultats sont stockés dans le tableau "sommes".
*/
void sommes_mat_binaire(const int matrice[][CODAGE_NB_BITS], int nb_lignes, int sommes[]) {
	// Pour chaque colonne (bit) de la matrice
	for (int colonne = 0; colonne < CODAGE_NB_BITS; colonne++) {
		int somme = 0;

		// Pour chaque ligne (colonne du plateau)
		for (int ligne = 0; ligne < nb_lignes; ligne++) {
			// Ajoute la valeur de la case correspondante à la somme
			somme += matrice[ligne][colonne];
		}

		// Stocke la somme dans le tableau "somme"
		sommes[colonne] = somme;
	}
}

/*La fonction position_premier_impaire trouve la première valeur impaire dans un tableau `tab` d'entiers. 
Si aucune valeur impaire n'est trouvée, elle renvoie -1. 
Utilisée dans le module NIM pour l'analyse de l'état du jeu.*/
int position_premier_impaire(const int tab[]) {
	for (int indice = 0; tab[indice] != -1; indice++) {
		// Vérifie si la valeur à l'indice actuel est impaire
		if (tab[indice] % 2 != 0) {
			return indice; // Retourne l'indice de la première valeur impaire
		}
	}

	return -1; // Si aucune valeur impaire n'est trouvée, retourne -1
}