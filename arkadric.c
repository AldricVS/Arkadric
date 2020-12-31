#include "mainHeader.h"
#undef main

int main(int argc, char* argv[])
{	
    int tempsTotal=0; // limitation images par seconde
    SDL_Event event;   //gestion des touches 
    //variables générales
    char nbeVies[128];
    int vies = 10;
    
    char nbeclef[128];
    int clefs = 0;
    
    char nbePieces[128];
    int pieces = 0;
    int tempsPause=0;
   
    int carte[20][20]={0};
    int nCarte = 1;
    char nbeCarte[128];
   
    int posInitialeX = 0;
    int posInitialeY = 32;
   
    int continuer = 1;
    int enTransition = 0; //transitions de niveau
    int fin = 0; // pour figer l'ecran a la fin du jeu
    int ecranFin_invisible = 1;  //animation image fin du jeu
    
    int nombreMaxNiveaux = getNombreNiveaux();
    
   //creation objets (définis plus loin) depuis une structure 
   Objet joueur,interface,decor,fond,txtVies,txtPieces,txtNiveau,txtClef,gameOver,victoire,tuto;
   joueur.sprite = interface.sprite = decor.sprite = fond.sprite = gameOver.sprite = victoire.sprite = tuto.sprite
   = txtVies.sprite = txtPieces.sprite = txtNiveau.sprite = txtClef.sprite = NULL; //Pour éviter erreurs mémoire


   int score = 0;
   int piecesRestantes = 0;



    //initialisation video
    if(SDL_Init(SDL_INIT_VIDEO)==1)
    {
        printf("erreur initialisation: %s",SDL_GetError());
        return 1;
    }
    
    SDL_EnableKeyRepeat(200,150); //Activation répétition touches

    TTF_Init();
    //création fenetre
    SDL_Surface *ecran=NULL;//initialise pointeur
    ecran = SDL_SetVideoMode(640,672,32, SDL_HWSURFACE | SDL_DOUBLEBUF);
	SDL_Surface *icone = SDL_LoadBMP("graphisme/m.bmp");
	SDL_SetColorKey(icone, SDL_SRCCOLORKEY, SDL_MapRGB(icone->format, 255, 0, 128));
	SDL_WM_SetCaption("Arkadric", NULL);
	SDL_WM_SetIcon(icone, NULL);
    //Creation arriere-plan
    fond.sprite = SDL_LoadBMP("graphisme/herbe.bmp");
    fond.pos.x = 0;
    fond.pos.y = 32;
    //INITIALISATION INTERFACE
    SDL_Color blanc={255,255,255};
    SDL_Color noir ={0,0,0};
    //initialisation texte vies
    TTF_Font *police = TTF_OpenFont("police/pixelFont.ttf", 32);
    txtVies.sprite= TTF_RenderText_Shaded(police,"vies: 10", blanc, noir);
    txtVies.pos.x = 5;
    txtVies.pos.y = 0;
    //initialisation texte pieces
    txtPieces.sprite= TTF_RenderText_Shaded(police,"pieces:0/10",blanc, noir);
    txtPieces.pos.x = 130;
    txtPieces.pos.y = 0;
    //initialisation texte niveau en cours
    txtNiveau.sprite= TTF_RenderText_Shaded(police,"niveau 1",blanc, noir);
    txtNiveau.pos.x = 531;
    txtNiveau.pos.y = 0;
    //initialisation texte nombre clef
    txtClef.sprite = TTF_RenderText_Shaded(police,"clefs:0",blanc, noir);
    txtClef.pos.x=300;
    txtClef.pos.y=0;
    //création sprite player
    joueur.sprite= SDL_LoadBMP("graphisme/setJoueur.bmp");
	SDL_SetColorKey(joueur.sprite,SDL_SRCCOLORKEY,SDL_MapRGB(joueur.sprite->format,255,0,128)); //transparence de l'image
    joueur.pos.x = 0;
    joueur.pos.y = 32;
    SDL_Rect partieJ = {0,0,32,32};
    //Creation sprite hud
    interface.sprite= SDL_CreateRGBSurface(SDL_HWSURFACE ,640,32,32,0,0,0,0);
    SDL_FillRect(interface.sprite,0,SDL_MapRGB(interface.sprite->format,0,0,0));
    //chargement image game over
    gameOver.sprite = SDL_LoadBMP("graphisme/gameover.bmp");
    SDL_SetColorKey(gameOver.sprite,SDL_SRCCOLORKEY,SDL_MapRGB(gameOver.sprite->format,255,0,128));
    gameOver.pos.x = 128;
    gameOver.pos.y = 256;
    int transparence = 0;
    SDL_SetAlpha(gameOver.sprite,SDL_SRCALPHA,transparence);
    //image vicoire
    victoire.sprite = SDL_LoadBMP("graphisme/victoryRoyale.bmp");
    SDL_SetColorKey(victoire.sprite,SDL_SRCCOLORKEY,SDL_MapRGB(victoire.sprite->format,255,0,128));
    victoire.pos.x = 128;
    victoire.pos.y = 256;
    int transparence2 = 0;
    SDL_SetAlpha(victoire.sprite,SDL_SRCALPHA,transparence2);
    //image "tutoriel"
    tuto.sprite = SDL_LoadBMP("graphisme/tuto.bmp");
    SDL_SetColorKey(tuto.sprite,SDL_SRCCOLORKEY,SDL_MapRGB(tuto.sprite->format,255,0,128));
    tuto.pos.x = 0;
    tuto.pos.y = 342;
    //chargement image "générale"
    decor.sprite = SDL_LoadBMP("graphisme/tileset.bmp");
    decor.pos.x = 0;
    decor.pos.y = 32;
    SDL_SetColorKey(decor.sprite,SDL_SRCCOLORKEY,SDL_MapRGB(decor.sprite->format,255,0,128));
    
    
    if(chargeCarte(nCarte,carte,&posInitialeX,&posInitialeY, &piecesRestantes)!=1)
    {
        fprintf(stdout,"erreur chargement carte: %s",SDL_GetError());
        return 1;
    }
    joueur.pos.x = posInitialeX;
    joueur.pos.y = posInitialeY;
    
    //BOUCLE PRINCIPALE
    while(continuer)
    {
        if(tempsPause == 0 && vies != 0)
        {
            while(SDL_PollEvent(&event))
            {
                switch(event.type)
                {
                    case SDL_QUIT:
                        continuer=0;
                        break;
                    case SDL_KEYDOWN:
                        switch(event.key.keysym.sym)
                        {
                        case SDLK_0:
                            continuer = 0;
                            break; 
                        case SDLK_r://reinitialise le niveau en cours (au cas ou on est bloqué)
                        	chargeCarte(nCarte,carte,&posInitialeX,&posInitialeY, &piecesRestantes);
                        	joueur.pos.x = posInitialeX;
                        	joueur.pos.y = posInitialeY;
                        	partieJ.x = 0;
                        	clefs = 0;
                        	pieces = 0;
                        	break;
                        case SDLK_ESCAPE:
                            continuer = 0;
                            break;
                        case SDLK_UP:
                            deplaceJoueur(HAUT,&joueur.pos,&partieJ,carte,&clefs,nbeclef,&pieces,nbePieces,&tempsPause,&score,&piecesRestantes);
                            break;
                        case SDLK_DOWN:
                            deplaceJoueur(BAS,&joueur.pos,&partieJ,carte,&clefs,nbeclef,&pieces,nbePieces,&tempsPause, &score, &piecesRestantes);
                            break;
                        case SDLK_LEFT:
                            deplaceJoueur(GAUCHE,&joueur.pos,&partieJ,carte,&clefs,nbeclef,&pieces,nbePieces,&tempsPause, &score, &piecesRestantes);
                            break;
                        case SDLK_RIGHT:
                            deplaceJoueur(DROITE,&joueur.pos,&partieJ,carte,&clefs,nbeclef,&pieces,nbePieces,&tempsPause, &score, &piecesRestantes);
                            break;
                        default:
                            break;
                        }
                        sprintf(nbeclef,"clefs:%d",clefs);
                        txtClef.sprite = TTF_RenderText_Shaded(police,nbeclef, blanc,noir);
                        sprintf(nbePieces,"pieces:%d/10",pieces);
                        txtPieces.sprite = TTF_RenderText_Shaded(police,nbePieces, blanc,noir);
						printf("score: %d\n", score);
                        break;
                    default:
                        break;
                    }
                }
                if(pieces == 10) //fin du niveau
                {
                    tempsPause = 120;
                    enTransition = 1;
                    if(nCarte==nombreMaxNiveaux)
                        fin = 1;
                }
            }
            else
            {
                while(SDL_PollEvent(&event)) // pour pouvoir quitter quand on est mort
                {
                    switch(event.type)
                    {
                        case SDL_QUIT:
                            continuer = 0;
                            break;
                        case SDL_KEYDOWN:
                            switch(event.key.keysym.sym)
                            {
                                case SDLK_ESCAPE:
                                    continuer = 0;
                                    break;
                            }
                    }
                }
                if(!enTransition)
                {
                    if(vies != 0)                
						tempsPause--;
                    partieJ.y = 32;
                    switch(tempsPause)
                    {
                	    case 179:
                	    	partieJ.x = 0;
                	    	break;
                	    case 160:
                	    	partieJ.x = 32;
                	    	break;
                	    case 140:
                	    	partieJ.x = 64;
                	    	break;
                	    case 120:
                		    partieJ.x = 96;
                    	case 100:
                	    	partieJ.x = 128;
                	    	break;
                	    case 80:
                	    	partieJ.x = 160;
                	    	break;
                	    case 0:
                	        if(vies != 0)
                	    	{
                	    	    vies--;
                                sprintf(nbeVies,"vies: %d",vies);
                        	    txtVies.sprite =TTF_RenderText_Shaded(police,nbeVies,blanc,noir);                		    
                	    	}    
                        	if (vies == 0)
                        	{
                        	    tempsPause = 0;
                        	    if(transparence<255)
                        	    {
                        	        transparence+= 5;
                                    SDL_SetAlpha(gameOver.sprite,SDL_SRCALPHA,transparence);    
                        	    }
                        	    
                    	    }
                            else
                            {                    	
                    	    joueur.pos.x=posInitialeX;
                    	    joueur.pos.y=posInitialeY;
						    partieJ.x = 0;
						    partieJ.y = 0;
						    }
                	    default:
                	    	break;
                    }
			    }
			    else //transition de niveau en cours ou fin
			    {
			        if(!fin)// alors en transition de niveau
			        {
                        tempsPause--;			        
			            if(tempsPause == 0)
			            {
			                enTransition = 0;
			                nCarte++;
                            chargeCarte(nCarte,carte,&posInitialeX,&posInitialeY, &piecesRestantes);
                            joueur.pos.x = posInitialeX;
                            joueur.pos.y = posInitialeY;
                            partieJ.x = 0;//remet animation initiale
                            clefs = 0;
                            pieces = 0;
                            sprintf(nbeclef,"clefs:%d",clefs);
                            txtClef.sprite = TTF_RenderText_Shaded(police,nbeclef, blanc,noir);
                            sprintf(nbePieces,"pieces:%d/10",pieces);
                            txtPieces.sprite = TTF_RenderText_Shaded(police,nbePieces, blanc,noir);
                            sprintf(nbeCarte,"niveau %d",nCarte);
                            txtNiveau.sprite = TTF_RenderText_Shaded(police,nbeCarte, blanc, noir);
			        }    }
			        else // fin ==> ne sort jamais de la pause
			        {
			            if(ecranFin_invisible)
			                transparence2 += 5;    
			            else
			                transparence2 -= 5;
			            if(transparence2 == 255)
			                ecranFin_invisible = 0;
			            else if(transparence2 == 0)
			                ecranFin_invisible = 1;
		
			            SDL_SetAlpha(victoire.sprite,SDL_SRCALPHA,transparence2); //affiche le texte en "respiration"
			        }
			    }
			 }
        
        //Dessin
        SDL_FillRect(ecran,NULL,SDL_MapRGB(ecran->format,0,0,0));
        SDL_BlitSurface(fond.sprite,NULL,ecran,&fond.pos);
        SDL_BlitSurface(interface.sprite,NULL,ecran,NULL);
        afficheCarte(ecran,decor,carte);
        SDL_BlitSurface(txtVies.sprite,NULL,ecran,&txtVies.pos);
        SDL_BlitSurface(txtPieces.sprite,NULL,ecran,&txtPieces.pos);
        SDL_BlitSurface(txtNiveau.sprite,NULL,ecran,&txtNiveau.pos);
        SDL_BlitSurface(txtClef.sprite,NULL,ecran,&txtClef.pos);
        SDL_BlitSurface(joueur.sprite,&partieJ,ecran,&joueur.pos);
        SDL_BlitSurface(gameOver.sprite,NULL,ecran,&gameOver.pos);
        SDL_BlitSurface(victoire.sprite,NULL,ecran,&victoire.pos);
        
        if(nCarte == 1)
            SDL_BlitSurface(tuto.sprite, NULL, ecran , &tuto.pos);
        
        SDL_Flip(ecran);
		tempsTotal = synchroFPS(tempsTotal);
        
    }
    SDL_FreeSurface(joueur.sprite);
    SDL_FreeSurface(victoire.sprite);
    SDL_FreeSurface(gameOver.sprite);
    SDL_FreeSurface(interface.sprite);
    SDL_FreeSurface(decor.sprite);
    SDL_FreeSurface(txtVies.sprite);
    SDL_FreeSurface(txtPieces.sprite);
    SDL_FreeSurface(txtNiveau.sprite);
    SDL_FreeSurface(fond.sprite);
	SDL_FreeSurface(icone);
    TTF_CloseFont(police);
    TTF_Quit();
    SDL_Quit();
    return 0;

}
