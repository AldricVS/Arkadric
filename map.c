#include "mainHeader.h"

int getNombreNiveaux()
{
    int nombreFichiers = 1;
    char nomCarte[128];
    while(1)//ne sort pas tant qu'il lit des fichiers
    {   
        nombreFichiers++;
        sprintf(nomCarte,"niveaux/carte%d.txt",nombreFichiers);
        
        if(fopen(nomCarte,"r")==NULL)
            break;
        
    }
    return nombreFichiers-1;
}

int chargeCarte(int nCarte,int carte[][20],int *positionInitialeX,int *positionInitialeY, int *piecesRestantes)
{
	*piecesRestantes = 0;

    FILE* fichier=NULL;
    char nomCarte[128];
    sprintf(nomCarte,"niveaux/carte%d.txt",nCarte);
    char ligneFichier[401] = {0} ;
    fichier= fopen(nomCarte,"r");
    if (fichier == NULL)
        return 0;
    fgets(ligneFichier,401,fichier);
    for(int i=0;i<20;i++)
    {
        for(int j = 0; j< 20; j++)
        {
            switch (ligneFichier[i * 20 + j])
            {
                case '0':
                    carte[j][i]=HERBE;
                    break;
                case '1':
                    carte[j][i]=FLEUR;
                    break;
                case '2':
                    carte[j][i]=ARBRE;
                    break;
                case '3':
                    carte[j][i]=ROCHER;
                    break;
                case '4':
                    carte[j][i]=CLEF;
                    break;
                case '5':
                    carte[j][i]=PIECE;
					*piecesRestantes += 1;
                    break;
                case '6':
                    carte[j][i]=SERRURE;
                    break;
                case '7':
                    carte[j][i]=PIEGE;
                    break;
                case '8':
                    carte[j][i]=MONSTRE;
                    break;
                case '9':
                    *positionInitialeX=j*32;
                    *positionInitialeY=i*32+32;
                    carte[j][i]=HERBE;
					break;
				case 'R':
					carte[j][i] = RICHESSE; // case RICHESSE
					break;
                default:
                    carte[j][i]=HERBE;
                    break;
            }
			
        }
    }
    fclose(fichier);
    return 1;
}


void afficheCarte(SDL_Surface *ecran,Objet decor, int carte[][20])
{
    SDL_Rect pos ={0,0};
    SDL_Rect partieImage={0,0,32,32};
    for (int i=0;i<20;i++)
    {
        for (int j=0;j<20;j++)
        {
            pos.x=i*32;
            pos.y=j*32+32;
            switch (carte[i][j])
            {
                case HERBE:
                    partieImage.x=0;
                    SDL_BlitSurface(decor.sprite,&partieImage,ecran,&pos);
                    break;
                case FLEUR:
                    partieImage.x=32;
                    SDL_BlitSurface(decor.sprite,&partieImage,ecran,&pos);
                    break;
                case ARBRE:
                    partieImage.x=64;
                    SDL_BlitSurface(decor.sprite,&partieImage,ecran,&pos);
                    break;
                case ROCHER:
                    partieImage.x=96;
                    SDL_BlitSurface(decor.sprite,&partieImage,ecran,&pos);
                    break;
                case CLEF:
                    partieImage.x=128;
                    SDL_BlitSurface(decor.sprite,&partieImage,ecran,&pos);
                    break;
                case PIECE:
                    partieImage.x=160;
                    SDL_BlitSurface(decor.sprite,&partieImage,ecran,&pos);
                    break;
                case SERRURE:
                    partieImage.x=192;
                    SDL_BlitSurface(decor.sprite,&partieImage,ecran,&pos);
                    break;
                case PIEGE:
                    partieImage.x=224;
                    SDL_BlitSurface(decor.sprite,&partieImage,ecran,&pos);
                    break;
                case MONSTRE:
                    partieImage.x=256;
                    SDL_BlitSurface(decor.sprite,&partieImage,ecran,&pos);
                    break;
                default:
                    break;
            }
        }
    }
}



