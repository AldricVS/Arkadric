#include <stdio.h>
#include <SDL.h>
#include <SDL_ttf.h>

//STRUCTS
typedef struct Objet
{
    SDL_Surface *sprite;
    SDL_Rect pos;
    
}Objet;

enum Dessin{HERBE,FLEUR,ARBRE,ROCHER,CLEF,PIECE,SERRURE,PIEGE,MONSTRE,JOUEUR,RICHESSE};
enum Direction{HAUT,BAS,GAUCHE,DROITE};

//FONCTIONS,PROCEDURES
// dans map.c
int getNombreNiveaux();//permet de connaitre le nombre de niveaux
int chargeCarte(int nCarte,int carte[][20],int *positionInitialeX,int *positionInitialeY,int *piecesRestantes);

void afficheCarte(SDL_Surface *ecran,Objet decor, int carte[][20]);

// dans joueur.c
void deplaceJoueur(int direction,SDL_Rect *pos,SDL_Rect *partieJ,int carte[][20],int *clefs,char nbeclef[],
	int *pieces,char nbePieces[],int *tempsMort, int *score, int *piecesRestantes);

int synchroFPS(int deltaSeconds); // pour limiter à 60 images par seconde

