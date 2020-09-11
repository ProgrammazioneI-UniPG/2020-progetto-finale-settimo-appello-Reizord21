#ifndef GAMELIB_H_INCLUDED
#define GAMELIB_H_INCLUDED

/**
 * gamelib.h
 *
 * Autore Marco Foggetti
 * esame Programmazione 1 - parte 1
 *
 *
*/

#include <stdio.h>
#include <stdlib.h>

#if defined(_WIN32)
  #define puliscischermo system("cls");
#else
  #define pulisciSchermo system("clear");
#endif

#define myflush while(getchar()!='\n');

typedef enum Quantita_melassa{nessuna=0,poca=2,molta=3} Quantita_melassa;
typedef enum Tipo_caverna{normale=0, speciale, accidentata,uscita} Tipo_caverna;
typedef enum Tipo_imprevisto{nessun_imprevisto=0,crollo=1,baco=2} Tipo_imprevisto;




/* Abbiamo due giocatori rappresentati da numeri:
 *
 * la variabile gio vale 0 per giocatore 1
 * la variabile gio vale 1 per giocatore 2
 *
 * Finita la creazione dei cunicoli, la partita
 * inizia con il turno del giocatore 1
 */



typedef struct Caverna{
	struct Caverna *avanti,*sinistra,*destra;
	Tipo_caverna stato;
	Quantita_melassa melassa;
	Tipo_imprevisto imprevisto;

	} Caverna;

typedef struct Scavatrice{
	struct Caverna *cav;
	int energia;
	int raccolta;
	} Scavatrice;


int crea_cunicoli();

int gioca();

void termina_gioco();

//  void prova_dealloca();











#endif // GAMELIB_H_INCLUDED
