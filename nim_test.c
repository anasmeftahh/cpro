#define _CRT_SECURE_NO_WARNINGS

#include "nim.h"
#include "nim_io.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


int main()
{
	int choix = 0;

	while (choix != 4) //Le jeu recommence tant que l'utilisateur ne quitte pas
	{
		printf("MENU:\n-------\n"); //Affiche le menu
		printf("(1) Jeu niveau FACILE\n");
		printf("(2) Jeu niveau MOYEN\n");
		printf("(3) Jeu niveau DIFFICILE\n");
		printf("(4) Quitter\n");
		printf("Saisir une valeur entre 1 et 4: ");
		choix = lire_entier(1, 4); //Trouve le choix
		if (choix == 4)
			break;
		demarrer_jeu(choix); //D�marre le jeu
	}
}

/****************************************************
					 PARTIE 1
****************************************************/

/* Demande � l'usager de saisir un entier entre les bornes
"min" et "max" (inclusivement). La fonction doit valider
la saisie et redemander � l'usager de saisir une valeur
jusqu'� l'obtention d'une valeur satisfaisante. */
int lire_entier(int min, int max)
{
	int saisie;

	scanf("%d", &saisie);


	while (saisie < min || saisie > max) {
		printf("\nValeur incorrecte, recommencez svp.\n");
		printf("Saisir une valeur entre %d et %d: ", min, max);
		scanf("%d", &saisie);
	}

	return saisie;
}

/* Initialise/G�n�re le plateau de jeu en remplissant
les "nb_colonnes" d'un nombre al�atoire de pi�ces entre
1 et PLATEAU_MAX_PIECES = 35. */
void plateau_init(int plateau[], int nb_colonnes)
{
	int i;

	srand(time(NULL));
	for (i = 0; i < nb_colonnes; i++)
	{
		plateau[i] = rand() % PLATEAU_MAX_PIECES;
	}
}

/* Applique des changements � la configuration du plateau de jeu
en retirant "nb_pieces" de la colonne "colonne" du plateau.

La fonction s'assure que le jeu est valide et renvoie Vrai
si le jeu d�sir� a pu s'appliquer � la configuration actuelle du jeu.
Pour �tre valide, la colonne doit exister et nombre de pi�ces
retir�es doit �tre <= au nombre de pi�ces actuellement pr�sentes
dans la colonne en question. */
int nim_jouer_tour(int plateau[], int nb_colonnes, int colonne, int nb_pieces) {
	/* colonne >= nb_colonnes = v�rifie si le nb_pieces qu'on veut retirer
	est plus grand que le nb_pieces actuel dans plateau[colonne]) ou non 	*/
	if (colonne < 0 || colonne >= nb_colonnes || nb_pieces < 0 || nb_pieces > plateau[colonne])
		return FALSE;	//Le coup ne marche pas

	else
	{
		plateau[colonne] -= nb_pieces;
		return TRUE;	//Le coup fonctionne
	}
}

/* Supprime la colonne "col_a_supprimer" du plateau.
On devra donc copier les valeurs de toutes les colonnes
suivantes du plateau d�une position vers la gauche. */
void plateau_supprimer_colonne(int plateau[], int nb_colonnes, int col_a_supprimer)
{
	int i;

	for (i = col_a_supprimer; i < nb_colonnes - 1; i++) {
		plateau[i] = plateau[i + 1];
	}
}

/* Fonction qui supprime les colonnes vides du tableau
en utilisant la fonction "plateau_supprimer_colonne".
Le nombre de colonnes restant est retourn�. */
int plateau_defragmenter(int plateau[], int nb_colonnes)
{
	int i; // compteur pour la boucle for
	int nb_col_left = nb_colonnes; // nb de colonnes restantes

	for (i = 0; i < nb_col_left; i++) // analyse d'une colonne � la fois
	{
		if (plateau[i] == 0) // si la colonne est vide...
		{
			plateau_supprimer_colonne(plateau, nb_colonnes, i); // on supprime la colonne...
			nb_col_left--; // ...et on indique qu'il y a une colonne restante en moins
		}
	}

	// remet toutes les colonnes suivantes � 0
	for (i = nb_col_left; i < PLATEAU_MAX_COLONNES; i++)
	{
		plateau[i] = 0;
	}

	return nb_col_left;
}

/* Fonction qui effectue un jeu al�atoire en choisissant
au hasard une colonne, puis au hasard le nombre de pi�ces
� jouer de cette colonne. */
void nim_choix_ia_aleatoire(const int plateau[], int nb_colonnes, 
							int* choix_colonne, int* choix_nb_pieces)
{
	srand(time(NULL));
	*choix_colonne = (rand() % nb_colonnes); // choisit une colonne au hasard
	*choix_nb_pieces = (rand() % (plateau[*choix_colonne]))+1; // choisit un nb de pi�ces au hasard
}

/* Affiche la configuration du plateau � l'�cran.
Elle affiche chaque colonne en mettant une pi�ce (�toile) par ligne,
selon le nombre de pi�ces pr�sentes dans la colonne en question.
Les num�ros de colonnes ainsi qu�une colonne de chiffres � gauche
indiquant le nombre de pi�ces sont �galement affich�s. */
void plateau_afficher(const int plateau[], int nb_colonnes, int col_select)
{
	int i, j; // Compteurs pour les boucles
	// Parcours de la grille en commen�ant par la ligne sup�rieure
	for (j = PLATEAU_MAX_PIECES; j > 0; j--)
	{
		printf("%2d  ", j);
				
		for (i = 0; i < nb_colonnes; i++)
		{
			if (plateau[i] < j)
			{
				printf("   "); // Affiche trois espaces pour aligner les colonnes
			}
			else
			{
				if (i == col_select)
				{
					textbackground(RED); // Si c'est la colonne s�lectionn�e, met le fond en RED
					printf("*");
					textbackground(BLACK); //...mais juste les *
					printf("  ");
				}
				else
				{
					textbackground(BLACK); // Sinon, met le fond BLACK
					printf("*  "); // Affiche une �toile suivie d'un espace pour les colonnes avec des pi�ces
				}
				
			}
			textbackground(BLACK);

		}
		printf("\n"); // Passe � la ligne suivante apr�s chaque rang�e
	}

	// Affichage des num�ros de colonnes
	printf("\n\n   ");
	for (i = 0; i < nb_colonnes; i++)
	{
		if (i == col_select)
		{
			textbackground(RED);
			printf("%2d ", i);
		}
		textbackground(BLACK);
		printf("%2d ", i);
	}
	printf("\n");

	textbackground(BLACK); // R�initialise la couleur de l'arri�re-plan � noir pour �viter des probl�mes
}

/*
Fonction qui d�termine quel doit �tre le jeu de l'ordinateur.
Cette fonction impl�mente l'algorithme intelligent d�crit dans la partie 2 du TP.
Le choix de l'ordinateur sera stock� dans les deux r�f�rences
"choix_colonne" et "choix_nb_pieces".
Si une erreur se produit, la fonction stocke la valeur aberrante -1
dans les deux r�f�rences "choix_colonne" et "choix_nb_pieces". 
*/
void nim_choix_ia(const int plateau[], int nb_colonnes, int niveau,
	int* choix_colonne, int* choix_nb_pieces)
{
	int matrice[PLATEAU_MAX_COLONNES][CODAGE_NB_BITS] = { 0 };
	//La matrice est initialis�e � 20 lignes car il faut d�clarer un tableau
	//avec des constantes (pas nb_colonnes). Aussi initialis� � 0.
	int somme[CODAGE_NB_BITS] = {0}; //Tableau des sommes
	int tab_bits[CODAGE_NB_BITS] = {0}; //Ligne � transformer en d�cimale
	int col, ligne = 0; //Colonne et ligne avec lesquelles travailler de la matrice
	int i; //Besoin d'un compteur

	//Construit la matrice binaire
	construire_mat_binaire(plateau, nb_colonnes, matrice);

	//Cr�e le tableau des sommes
	sommes_mat_binaire(matrice, 7, somme);

	//Trouver la premi�re somme impaire de gauche � droite
	col = position_premier_impaire(somme);

	//S'il n'y en a pas, indiquer qu'une erreur s'est produite
	if (col == -1)
	{
		*choix_colonne = -1;
		*choix_nb_pieces = -1;
	}
	
	//S'il y en a une...
	else
	{
		//Trouver la premi�re ligne ayant un bit 1 dans la colonne
		while (matrice[ligne][col] != 1) //�valuation du bit
			ligne++; //Passe � la prochaine ligne

		//Colonne par colonne, pour celles impaires, inverser le bit de signe :
		for (i = 0; i < CODAGE_NB_BITS; i++)
		{
			if (somme[i] % 2 == 1)
			{
				if (matrice[ligne][i] == 0)
					matrice[ligne][i] = 1; //Passer de 0 � 1
				else
					matrice[ligne][i] = 0; //Passer de 1 � 0
			}
		}

		//Transformation de la ligne en un tableau � 1 dimension
		for (i = (CODAGE_NB_BITS - 1); i >= 0; i--)
			tab_bits[i] = matrice[ligne][i];

		//Retourner le choix de la colonne et du nombre de pi�ces
		*choix_colonne = ligne;
		*choix_nb_pieces = (plateau[ligne] - codage_bin2dec(tab_bits)); //Conversion de la ligne en d�cimal
		//Nb de pi�ces actuelles - nb de pi�ces final = nb de pi�ces � enlever
	}
}

/* Permet de choisir une colonne du plateau de jeu
avec les touches fl�ch�es (gauche, droite) ou l�espace (� �).
Au d�part, on commence par la colonne [0].
La colonne actuellement s�lectionn�e sera toujours affich�e en ROUGE
avec la fonction "plateau_afficher". Le choix d�finitif est fait avec
la touche ENTER et on retourne le num�ro de la colonne choisie. */
static int choisir_colonne(int plateau[], int nb_colonnes)
{
	unsigned char ch;
	int choix_colonne = 0;

	//On affiche le plateau avant le choix, o� la col 0 est s�lectionn�e
	gotoxy(0, 5);
	plateau_afficher(plateau, nb_colonnes, choix_colonne);

	ch = _getch(); //on lit le premier caract�re

	while (ch != 13) //tant que pas enter
	{
		if (ch == 0 || ch == 224) //touche �tendue
			ch = _getch(); //lire le 2e code

		switch (ch)
		{
		case 75: //fleche gauche
			if (choix_colonne == 0)
				choix_colonne = nb_colonnes - 1;
			else
				choix_colonne--; //on va une colonne en dessous
			break;
		case 77: case 32: //fleche droite ou espace
			if (choix_colonne == nb_colonnes - 1)
				choix_colonne = 0;
			else
				choix_colonne++; //on va une colonne en haut
			break;
		}
		
		gotoxy(0, 5); //on ram�ne le curseur apr�s le texte d'en-t�te
		clreoscr();	//on efface ce qui vient apr�s, soit le plateau

		//on r�affiche le plateau avec la nouvelle colonne choisie en rouge
		plateau_afficher(plateau, nb_colonnes, choix_colonne);
		ch = _getch();
	}
	
	return choix_colonne;
}

/* D�clenche le tour de l'humain en demandant � l'usager
de choisir la colonne (avec un appel � "choisir_colonne")
et le nombre de pi�ces � retirer du plateau de jeu. 
Une fois le choix effectu�, la fonction doit faire appel � "nim_jouer_tour"
pour appliquer les changements au plateau. */
void tour_humain(int plateau[], int nb_colonnes)
{
	int choix_col;
	int nb_pieces;

	//Cette partie de la console sera conserv�e pendant le tour
	clrscr();
	printf("****** Tour Joueur ******\n\n");
	printf("==> Veuillez choisir la colonne souhaitee\n\n");
	printf("    <- , ->, <ESPACE> : Changer la selection   <ENTER> : Valider\n");
	
	//Pendant que le joueur navigue sur les colonnes, le plateau se r�affiche
	choix_col = choisir_colonne(plateau, nb_colonnes);

	//On r�affiche la page
	gotoxy(0, 1);
	clreoscr();
	gotoxy(0, 5);
	plateau_afficher(plateau, nb_colonnes, choix_col);

	//On demande au joueur combien de pi�ces choisir
	gotoxy(0, 2);
	printf("==> Nombre de pieces: Saisir une valeur entre %d et %d: ", 1, plateau[choix_col]);
	gotoxy(57, 2);
	nb_pieces = lire_entier(1, plateau[choix_col]);

	//si �a marche pas plus tard: cr�er variable inutile pour stocker 1
	nim_jouer_tour(plateau, nb_colonnes, choix_col, nb_pieces); 
}

/* D�clenche le tour de l'ordinateur. Pour connaitre le choix de l'ordinateur,
on fait appel � la fonction "nim_choix_ia".
Une fois le choix effectu�, la fonction doit faire appel � "nim_jouer_tour"
pour appliquer les changements au plateau. */
void tour_ia(int plateau[], int nb_colonnes, int difficulte)
{
	int choix_colonne = 0, choix_nb_pieces = 0;

	srand(time(NULL)); //R�initialise le random

	//Choix de l'ia selon la difficulte
	switch (difficulte)
	{
	case 1: //Choix al�atoire si difficult� 1
		nim_choix_ia_aleatoire(plateau, nb_colonnes, &choix_colonne, &choix_nb_pieces);
		break;
	case 2: //Pile ou face sur choix al�atoire ou intelligent si difficult� 2
		if ((rand() % 1) == 0)
			nim_choix_ia_aleatoire(plateau, nb_colonnes, &choix_colonne, &choix_nb_pieces);
		if ((rand() % 1) == 1)
			nim_choix_ia(plateau, nb_colonnes, difficulte, &choix_colonne, &choix_nb_pieces);
		break;
	case 3: //Choix intelligent si difficult� 3
		nim_choix_ia(plateau, nb_colonnes, difficulte, &choix_colonne, &choix_nb_pieces);
		if (choix_colonne == -1)
			nim_choix_ia_aleatoire(plateau, nb_colonnes, &choix_colonne, &choix_nb_pieces);
	}

	// On efface l'�cran pour mettre le message d'en-t�te
	clrscr();
	printf("****** Tour Ordinateur ******\n\n");
	printf("Ordinateur va prendre %d pieces de la colonne %d\n\n", choix_nb_pieces, choix_colonne);
	gotoxy(0, 5);
	plateau_afficher(plateau, nb_colonnes, choix_colonne); //affichage du plateau apr�s tour du joueur

	nim_jouer_tour(plateau, nb_colonnes, choix_colonne, choix_nb_pieces);
	
	
	// G�re le cas o� le choix de l'ordinateur est invalide
	/*while (nim_jouer_tour(plateau, nb_colonnes, choix_colonne, choix_nb_pieces) == 0)
	{
		printf("L'ordinateur a fait un choix invalide. G�n�ration d'un choix alternatif.\n");
		nim_choix_ia(plateau, nb_colonnes, difficulte, &choix_colonne, &choix_nb_pieces);
	} */
}

/* Fonction qui contr�le le jeu de nim:
elle donne la main, tour � tour, � chacun des deux joueurs
et d�clare le gagnant une fois la partie termin�e.
On quitte cette fonction quand la partie est termin�e.
Pour donner la main aux joueurs, on appelle les fonctions
"tour_humain" et "tour_ia" en alternance. 
Apr�s chaque tour, cette fonction se charge de d�fragmenter le plateau de jeu,
de modifier la taille du plateau � l'�cran et
d'afficher la nouvelle configuration du plateau de jeu. */
void demarrer_jeu(int niveau)
{
	int plateau[PLATEAU_MAX_COLONNES];
	int nb_colonnes; //nb colonnes
	int vainqueur = 0; //1 pour joueur, 0 pour ordi

	clrscr(); //clear console
	
	//demande le nb de colonnes
	printf("***** NOUVELLE PARTIE *****\nNombre de colonnes:\n");
	printf("Saisir une valeur entre 2 et %d : ", PLATEAU_MAX_COLONNES);
	nb_colonnes = lire_entier(2, PLATEAU_MAX_COLONNES);
	clrscr();

	//initialise le plateau avec nb al�atoires
	plateau_init(plateau, nb_colonnes);
	
	//d�fragmente le tableau et renvoie le nouveau nb de colonnes
	nb_colonnes = plateau_defragmenter(plateau, nb_colonnes);

	while (nb_colonnes > 0) //tant qu'il reste des pi�ces
	{
		//Tour de l'humain :
		tour_humain(plateau, nb_colonnes);
		vainqueur++; //le joueur gagne si le nb calcul� sera 1 apr�s son tour
		nb_colonnes = plateau_defragmenter(plateau, nb_colonnes); //d�fragmentation
		if (nb_colonnes == 0)
			break;

		//Tour de l'ia : 
		tour_ia(plateau, nb_colonnes, niveau);
		vainqueur--; //l'ia gagne si le nb calcul� sera 0 apr�s son tour
		nb_colonnes = plateau_defragmenter(plateau, nb_colonnes); //d�fragmentation
		
		//On met le syst�me en pause
		gotoxy(0, 3);
		system("pause");
	}

	//Affichage de l'�cran de victoire
	clrscr();
	printf("\n\n\n\n\n");
	plateau_afficher(plateau, nb_colonnes, 0);
	gotoxy(3, 4);
	if (vainqueur == 1)
		printf("L'humain a gagne!\n");
	if (vainqueur == 0)
		printf("L'ia a gagne!\n");
	else
		printf("Hum... gros probleme.\n");

	gotoxy(0, 5);
	system("pause");
}


/****************************************************
					 PARTIE 2
****************************************************/

/*
Fonction qui inverse les "nb_bits/2" premi�res valeurs d'un tableau de bits:
le premier bit devient la dernier (et inversement),
le deuxi�me devient l'avant dernier, etc.
Cette fonction est utilis�e par la fonction "codage_dec2bin".
Retour de 1 si l�op�ration a �t� effectu�e et 0 sinon ("nb_bits" trop gros).
*/
int inverser_tab_bits(int tab_bits[], int nb_bits)
{
	int tampon, i, j = nb_bits / 2;

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
Traduit un nombre d�cimal en binaire.
Le r�sultat est stock� dans le tableau "resultat" et le nombre de bits utilis�s est renvoy�.
Le codage du nombre d�cimal doit se faire en un maximum de CODAGE_NB_BITS (fix�e � 8).
La fonction renvoie le nombre de bits qui a �t� n�cessaire pour coder le nombre en binaire.
Si le nombre requiert plus que CODAGE_NB_BITS, la fonction renvoie 0.
*/
int codage_dec2bin(int nombre, int resultat[])
{
	int i;

	if (nombre >= (pow(2, CODAGE_NB_BITS))) // 2^8 et plus
		return 0;
	else
	{
		for (i = (CODAGE_NB_BITS-1); nombre || i >=0; i--) //Commence de la fin
		{
			resultat[i] = (nombre % 2);
			nombre /= 2;
		}

		return i;
	}
}

/*Traduit un tableau de bits, repr�sentant un nombre en binaire,
vers sa repr�sentation d�cimale. La valeur d�cimale est retourn�e
par la fonction. */
int codage_bin2dec(const int tab_bits[])
{
	int i, j =0;
	int nb = 0;

	for (i = (CODAGE_NB_BITS-1); i >=0; i--)
	{
		nb += (pow(2, j) * tab_bits[i]);
		j++; //Puisque la ligne est � l'envers
	}

	return nb;
}

/*Fonction d'une matrice binaire qui aide l'ordinateur � choisir comment jouer.
Chaque ligne de la matrice correspond � une colonne du plateau de jeu et
indique combien de pi�ces sont dans cette colonne en utilisant des 0 et des 1.
Le r�sultat est stock� dans le tableau "matrice".*/
void construire_mat_binaire(const int plateau[], int nb_colonnes, int matrice[][CODAGE_NB_BITS])
{
	// Pour chaque colonne sur le plateau
	int i, j;
	int ligne[CODAGE_NB_BITS]; //Ligne du tableau � copier dans la matrice
	int nb_bits; //Retour de codage_dec2bin

	for (i = 0; i < nb_colonnes; i++) {
		nb_bits = codage_dec2bin(plateau[i], ligne);

		if (nb_bits == 0)
		{
			clrscr();
			printf("Erreur! Le nb de bits est plus grand que 8.");
			system("pause");
		}

		for (j = 0; j < CODAGE_NB_BITS; j++)
			matrice[i][j] = ligne[j];
	}
}

void afficher_tab_bits(const int tab_bits[], int nb_bits) {

	int i = 0;
	while (i < nb_bits) {
		printf("%d ", tab_bits[i]);
		i++;
	}
}

/*
Calcule les sommes des colonnes d'une matrice binaire.
La matrice contient "nb_lignes" lignes et chaque ligne repr�sente
une colonne du plateau de jeu sous forme binaire (0 et 1).
Les r�sultats sont stock�s dans le tableau "sommes".
*/
void sommes_mat_binaire(const int matrice[][CODAGE_NB_BITS], int nb_lignes, int sommes[]) {
	// Pour chaque colonne (bit) de la matrice
	int colonne, ligne, somme;
	
	for (colonne = 0; colonne < CODAGE_NB_BITS; colonne++) {
		somme = 0; //Remettre somme � 0
		
		// Pour chaque ligne de la matrice
		for (ligne = 0; ligne < nb_lignes; ligne++) {
			// Ajoute la valeur de la case correspondante � la somme
			somme += matrice[ligne][colonne];
		}

		// Stocke la somme dans le tableau "somme"
		sommes[colonne] = somme;
	}
}

/*La fonction position_premier_impaire trouve la premi�re valeur impaire dans un tableau `tab` d'entiers.
Si aucune valeur impaire n'est trouv�e, elle renvoie -1.
Utilis�e dans le module NIM pour l'analyse de l'�tat du jeu.*/
int position_premier_impaire(const int tab[]) {
	int indice;
	
	for (indice = 0; indice < CODAGE_NB_BITS; indice++) {
		// V�rifie si la valeur � l'indice actuel est impaire
		if (tab[indice] % 2 != 0) {
			return indice; // Retourne l'indice de la premi�re valeur impaire
		}
	}

	return -1; // Si aucune valeur impaire n'est trouv�e, retourne -1
}