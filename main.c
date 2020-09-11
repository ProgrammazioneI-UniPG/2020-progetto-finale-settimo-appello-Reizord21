/**
 * main.c
 *
 * Autore Marco Foggetti
 * esame Programmazione 1 - parte 1
 *
 *
*/

#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#include "gamelib.h"





int main(){
	char k;
	int scelta=0;
	time_t t;
	srand((unsigned) time(&t));

	pulisciSchermo;


	printf("\n\n Benvenuti nel gioco del Pianeta SABBIE !");


	do{

		printf("\n\nInserisci la tua scelta");
		printf("\n 1- Crea cunicoli" );
		printf("\n 2- Gioca (Devi aver già creato i cunicoli!)");
		printf("\n 3- Termina gioco \n");
		scanf("%d",&scelta);

		switch(scelta){
			case 1:{ //crea_cunicoli per giocatore gio
				      crea_cunicoli();


				    } break;
			case 2:{
				      gioca();

				    } break;
			case 3:{  //termina gioco
                      termina_gioco();

				    } break;


			default:{ printf("\n Scelta non prevista dal menu");
				      printf("\n Digitazione sbagliata?");

				      printf ("Premi un tasto per continuare");
				      scanf("%c",&k);
				      pulisciSchermo}
		}


	}while(scelta!=3);
	pulisciSchermo
	printf("\n\n La partita è terminata ");


}
