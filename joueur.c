#include "mainHeader.h"
#define SPEED 32

void deplaceJoueur(int direction, SDL_Rect *pos, SDL_Rect *partieJ, int carte[][20], int *clefs, char nbeclef[],
	int *pieces, char nbePieces[], int *tempsMort, int *score, int *piecesRestantes)
{
    int x,y,dirX,dirY,deplacementPossible;
    dirX = dirY = deplacementPossible = 0;
    switch(direction)
    {
        case HAUT:
            if(pos->y>32)
            {
                
                dirX = 0;
                dirY = -SPEED;
                deplacementPossible = 1;
            }
			partieJ->x = 32;
            y = (pos->y -64)/32;
            x = pos->x/32;
            break;
        case BAS:
            if(pos->y<640)
            {
                
                dirX = 0;
                dirY = SPEED;
                deplacementPossible = 1;
            }
			partieJ->x = 0;
            y = (pos->y)/32;
            x = pos->x/32;
            break;
        case GAUCHE:
            if(pos->x>0)
            {

                dirX = -SPEED;
                dirY = 0;
                deplacementPossible = 1;
            }
			partieJ->x = 64;
            y = (pos->y-32)/32;
            x = (pos->x -32)/32;
            break;
        case DROITE:
            if(pos->x<608)
            {
                
                dirX = SPEED;
                dirY = 0;
                deplacementPossible = 1;
            }
			partieJ->x = 96;
            y = (pos->y-32)/32;
            x = (pos->x +32)/32;
            break;
    }
    if(deplacementPossible){
    switch(carte[x][y])
    {
        case HERBE:
            pos->x += dirX;
            pos->y += dirY;
            break;
        case FLEUR:
            pos->x += dirX;
            pos->y += dirY;
            break;
        case ARBRE://pas de deplacement
            break;
        case ROCHER://non plus
            break;
        case CLEF:
            pos->x += dirX;
            pos->y += dirY;
            carte[x][y] = HERBE;
            *clefs = *clefs +1;
            sprintf(nbeclef,"clefs:%d",*clefs);
            break;
        case PIECE:
            pos->x += dirX;
            pos->y += dirY;
            carte[x][y] = HERBE;
            *pieces += 1;
            sprintf(nbePieces,"pieces:%d/10",*pieces);

			*piecesRestantes -= 1;
			*score += 1;


            break;
        case SERRURE:
            if(*clefs>0)
            {
                *clefs -= 1;
                sprintf(nbePieces,"pieces:%d/10",*pieces);
                pos->x += dirX;
                pos->y += dirY;
                carte[x][y] = HERBE;
            }
            break;
        case PIEGE:
            pos->x += dirX;
            pos->y += dirY;
            carte[x][y] = HERBE;
            *tempsMort = 180; 
            break;
        case MONSTRE:
            pos->x += dirX;
            pos->y += dirY;
            *tempsMort = 180;
            break;
		case RICHESSE:
			carte[x][y] = HERBE;
			printf("RICHESSE\n");
			*score = *piecesRestantes * *score;
			pos->x += dirX;
			pos->y += dirY;
			break;
    }
   }
}


int synchroFPS(int tempsTotal)
{
	int deltaSeconds = SDL_GetTicks();
	deltaSeconds -= tempsTotal;
	tempsTotal += deltaSeconds;
	if(deltaSeconds <16)
	{
		SDL_Delay(16-deltaSeconds);
	}
	return tempsTotal;
}
