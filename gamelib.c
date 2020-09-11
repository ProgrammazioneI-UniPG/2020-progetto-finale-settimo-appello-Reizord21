/**
 * gamelib.c
 *
 * Autore Marco Foggetti
 * esame Programmazione 1 - parte 1
 *
 *
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "gamelib.h"



static Caverna *primo_cunicolo_arvais=NULL;
static Caverna *primo_cunicolo_hartornen=NULL;

static Scavatrice arvais;
static Scavatrice hartornen;

static int gio;

// numCaverne[i] indica quante caverne contiene il cunicolo del giocatore i
static int numCaverne[2];

static char *casata[2]={"Arvais","Hartornen"};

// turni[i] conta i turni del giocatore i
static int turni[2];

// vincitore vale 0 se ha vinto giocatore 0, 1 se ha vinto giocatore 1 , 333 se non ha vinto ancora nessuno
static int vincitore;  // vale 0 se ha vinto giocatore 0, 1 se ha vinto giocatore 1 ,

// map vale 2 se ambedue i cunicoli sono stati completati
static int map;

// mappe[i] vale 1 se il giocatore i ha completato il suo cunicolo
static int mappe[2];

// fuori[i] vale 1 se giocatore i uscito dal cunicolo
static int fuori[2];



static int ins_caverna(int g);
static int canc_caverna(int g);
static int stampa_cunicolo(int g);
static Caverna *init_cunicolo(int g); //inizializza cunicolo,  elimina cunicolo precedente
static Caverna *crea_caverna(int t);
static Caverna *crea_caverna_2();
static int disalloca_cunicolo(int g); // non ricorsivo
static int dealloca_cunicolo(Caverna *C); //ricorsivo
static int pop_caverna(int g);
static int deq_caverna(int g);
static int ins_testa(int g,Caverna *n, int dir);
static int ins_coda(int g, Caverna *n, int dir);
static void stampa_caverna(Caverna *corrente);
static int chiudi_cunicoli();
static int avanza(int g);
static int avanza_2(int g);
static int abbatti(int g);
static int aggira(Caverna *prec,Caverna *succ,int dir);
static int esci(int g);
static Scavatrice init_scavatrice(int g); //inizializza scavatrice
static int crollo_improvviso(); // numeri semi casuali di probabilità definita
static int scontro(int g); // numeri semi casuali di probabilità definita
static int definisci_vincitore();
static int rando(int n1,int n2, int n3); // numeri semi casuali di probabilità definita
static void stato_serbatoi(int g);







int gioca(){
    int scelta, g;
    Scavatrice *scava;
    vincitore=333;// valore che indica che non esiste ancora un vincitore

    turni[0]=turni[1]=0; // nessun turno di gioco per ora
    fuori[0]=fuori[1]=0;//il valore 0 vuol dire che nessun giocatore è uscito
    //char k;
    arvais=init_scavatrice(0);
    hartornen=init_scavatrice(1);

    /*  Test scontro()
    int z,w;

    for(z=0,w=0;z<50;z+=2,w=(w+1)%2){
            turni[0]=z;
            turni[1]=z+1;
            printf("\n Scontro   %d turni  %d",scontro(w), turni[w]);

    }
    printf("Premi enter per continuare");
    k=getchar();
    turni[0]=turni[1]=0;

    **fine test scontro()
    */




	if(map==2){  // cunicoli completati


        do{
            printf("\n\n        BENVENUTO SUL PIANETA SABBIE \n\n");
            printf("\n\nVincerà chi raccoglierà più UM (Unità di Melassa) \n\n");
            printf("\n\n        MENU GIOCO \n\n");
            printf("Scegli quale giocatore inizia la partita");
            printf("\n 1- Giocatore 1- Casata Arvais");
            printf("\n 2- Giocatore 2- Casata Hartornen \n");
            scanf("%d",&g);
            myflush;
            if(g==1){
                gio=0;
                }
            else
                if(g==2){
                    gio=1;
                 }
                 else{
                     printf("\n Scelta non prevista");
                     printf("\n Digita 1 oppure 2");
                     printf ("\n\nPremi enter per continuare\n");
                     myflush;


                     }
        }while((g!=1)&&(g!=2));

        if(g==0){
           scava=&arvais;
        }
        else{
            scava=&hartornen;
                }

        do{
            pulisciSchermo;
            if(fuori[0]*fuori[1]==1){
                     //vero se tutti e 2 i giocatori sono uscito dal cunicolo
                     //falso se almeno 1 giocatori è ancora nel cunicolo
                    definisci_vincitore();
            }
            if(vincitore>2){ //vincitore>2 vuol dire che non esiste ancora un vincitore

                if(fuori[gio]==1){ // il giocatore gio è uscito dal cunicolo

                    printf("Il giocatore %d, casata %s,  è uscito dal cunicolo",gio+1, casata[gio]);
                    gio=(gio+1)%2; //passiamo all'altro giocatore
                }
                if(fuori[gio]==1){

                    printf("Il giocatore %d, casata %s,  è uscito dal cunicolo",gio+1, casata[gio]);
                    gio=(gio+1)%2;  //passiamo all'altro giocatore

                }

                if(g==0){
                   scava=&arvais;
                }
                else{
                    scava=&hartornen;
                }
                stato_serbatoi(gio);
                stato_serbatoi((gio+1)%2);
                printf("\n\n  164  Gioca  il giocatore %d per la casata %s",gio+1, casata[gio]);
                if(scava->cav->stato==uscita)
                    printf("\nQuesta caverna ha una uscita!! Puoi uscire se vuoi!");
                 else  printf("\nQuesta caverna non ha uscite!!");
                printf("\n\nInserisci la tua scelta\n");
                printf("\n 1- Avanza");
                printf("\n 2- Abbatti" );
                printf("\n 3- Esci");
                printf("\n 4- Abbandona il gioco \n\n");

                scanf("%d",&scelta);
                myflush;

                switch(scelta){
                    case 1: {
                              avanza(gio);
                              turni[gio]++;
                              gio=(gio+1)%2;

                            } break;
                    case 2:{
                             abbatti(gio);
                             turni[gio]++;
                             gio=(gio+1)%2;

                            } break;

                    case 3:{  //esci dal cunicolo

                            if(scava->cav->stato==uscita){
                                esci(gio);
                                turni[gio]++;
                                gio=(gio+1)%2;
                            }
                            else {
                                printf("\n La caverna non ha uscite");
                                printf("\n Hai perso un turno!!");
                                printf ("\n\nPremi enter per continuare");
                                myflush;
                                pulisciSchermo;
                                }
                            turni[gio]++;
                            gio=(gio+1)%2;
                            } break;



                    case 4:{ printf("Abbandonate il gioco");
                             printf ("\n\nPremi enter per continuare");
                             myflush;
                             termina_gioco();
                           } break;

                    default:{ printf("\n Scelta non prevista dal menu");
                              printf("\n Digitazione sbagliata?");
                              printf ("\n\nPremi enter per continuare");
                              myflush;
                              pulisciSchermo;
                            }
                    }  // fine switch scelta
                  }
                  else {  /* si arriva qui solo se c'è già un vincitore. Quindi si esce
                           * dalla funzione gioca()
                           * e si torna al menu principale
                           *
                           */
                        scelta=4;
                        pulisciSchermo;
                        printf("\nGAME OVER!");
                        printf("\nGAME OVER!");
                        printf("\nGAME OVER!");
                        printf("\n\nVince il giocatore %d per la casata %s", vincitore+1, casata[vincitore]);
                        printf ("\n\nPremi enter per continuare");
                        myflush;
                        //termina_gioco();


                  }

        }while(scelta!=4);


    }//fine se map==2
    else {
            printf("\n\n La mappa non è completa, non potete ancora giocare");
            chiudi_cunicoli();
         }
    printf ("\n\nPremi enter per continuare");
    myflush;
    pulisciSchermo;

    return 0; // ritorno a menu principale
}

static int crollo_improvviso(){
  int n,ris;
  // crollo se n vale 0 (1 su 4 valori) => ris=1 vuol dire CROLLO
  n=rand()%4;
  if(n==0)
    ris=1;
  else
     ris=0;
  return ris;
}

static int avanza(int g){
    //printf("281");
    //myflush;
    Scavatrice *scava;
    Caverna *precedente;
    int scelta,direzione=0;
    //char k;
    //printf("287");
    //myflush;

    if(g==0){
       scava=&arvais;
    }
    else{
        scava=&hartornen;
    }
    //printf("289");
    precedente=scava->cav; // caverna precedente, serve per eventuale aggiramento

    if(scava->cav->avanti!=NULL){
        scava->cav=scava->cav->avanti;  // possibile caverna successiva
        direzione=1;
        printf("\n Andiamo avanti");
        }
    else if(scava->cav->destra!=NULL){
         scava->cav=scava->cav->destra;   // possibile caverna successiva
         direzione=2;
         printf("\n Andiamo a destra");
         }
         else if(scava->cav->sinistra!=NULL){
              scava->cav=scava->cav->sinistra;   // possibile caverna successiva
              direzione=3;
              printf("\n Andiamo a sinistra");
              }
              else {
                printf("\n\nCaverna senza aperture!! Non posso avanzare");
                printf ("\n\nPremi enter per continuare\n");
				myflush;
                return 1;  //finisce il turno
              }

    if(crollo_improvviso()){
            printf("\n Crollo totale improvviso, non si può entrare nella caverna");
            printf("\n Dovrai aggirare l'ostacolo");
            printf ("\n\nPremi enter per continuare\n");
			myflush;
            aggira(precedente, scava->cav,direzione);
            // scava->cav è la caverna appena creata con aggira()
    }
    //else{
        if(scava->cav->imprevisto==crollo){
            scava->energia--;
            printf("\n Caverna con imprevisto crollo");
            printf("\n La tua energia diminuisce di 1");
            stato_serbatoi(g);
            printf ("\n\nPremi enter per continuare\n");
			myflush;
			}
        if(scava->energia<0){ vincitore=(g+1)%2;
            printf("Energia negativa");
            stato_serbatoi(g);
            printf("\n\nPremi enter per continuare\n");
            myflush;
            }
            else if((scava->cav->imprevisto==baco)&&(vincitore>2)){
                    scava->energia=0;
                    scava->raccolta=0;
                    printf("\nLa caverna in cui sei entrato ospita un temibile baco");
                    printf("\nChe divora tutta la tua melassa");
                    stato_serbatoi(g);
                    printf("\n\nPremi enter per continuare\n");
                    myflush;
                 }
        if ((scava->cav->melassa>0)&&(vincitore>2)){
            do{
            printf("\n Nella caverna si trovano %d UM", scava->cav->melassa);
            printf("\n In quale serbatoio deve andare?\n");
            printf("\n1 - Serbatoio energia scavatrice (limite max 4)");
            printf("\n2 - Serbatoio raccolta (limite max 10)\n");
            scanf("%d",&scelta);
            myflush;
            if (scelta==1){
                scava->energia=scava->energia+scava->cav->melassa;
                if(scava->energia>4){
                    scava->energia=4;
                    printf("\n Serbatoio energià già al massimo");
                    printf ("\n\nPremi enter per continuare");
                    myflush;
                    }
            }
            else
                if(scelta==2){
                    scava->raccolta=scava->raccolta+scava->cav->melassa;
                    if(scava->raccolta>10){
                        scava->raccolta=10;
                        printf("\n Serbatoio raccolta già al massimo");
                        printf ("\n\nPremi enter per continuare");
                        myflush;
                    }
                }
                else{
                        printf("\n Scelta non prevista dal menu");
                        printf ("\n\nPremi enter per continuare");
                        myflush;
                     }

            } while ((scelta!=1)&&(scelta!=2));// fine do
            stato_serbatoi(g);
            printf ("\n\nPremi enter per continuare");
            myflush;
        }
        else{
            printf("Nella caverna non c'è melassa");
            printf ("\n\nPremi enter per continuare\n");
			myflush;
        }
        if((scava->cav->stato==accidentata)&&(vincitore>2)){
            scava->energia--;
            printf("\nLa caverna in cui sei entrato é accidentata");
            printf("\nLa tua energia scende di 1");
            stato_serbatoi(g);
            printf ("\n\nPremi enter per continuare\n");
            myflush;
            if(scava->energia<0){
                vincitore=(g+1)%2;
                printf("\n La tua energia è negativa! Hai perso");
                printf ("\n\nPremi enter per continuare\n");
               myflush;
            }



        }
        else if((scava->cav->stato==speciale)&&(vincitore>2)){
                scava->energia++;

                printf("\nLa caverna in cui sei entrato é speciale");
                printf("\nLa tua energia sale di 1 (max 4 UM)");
                if(scava->energia>4){
                    scava->energia=4;
                    printf("\nEnergia già al massimo");
                    }
                stato_serbatoi(g);
                printf ("\n\nPremi enter per continuare\n");
                myflush
            }

   // }

    if((scava->energia<0)&&(vincitore>2)){
        vincitore=(g+1)%2;
        printf("\n La tua energia è negativa!");
        printf ("\n\nPremi enter per continuare\n");
        myflush;
	}

	if((scontro(g))&&(vincitore>2)){

        printf("\n\n Un Arvais ed un Hartornen nella stessa caverna!");
        printf("\n Hai uno scontro con l'altro giocatore!");
        stato_serbatoi(0);
        stato_serbatoi(1);
        printf ("\n\nPremi enter per continuare\n");
        myflush;
        definisci_vincitore();

	}

    return 0;

    }





static int avanza_2(int g){
    /* Questa è la funzione avanza() chiamata da abbatti()
     * Sceglie la direzione basandosi su quali sono le porte chiuse
     * (che quindi si possono abbattere)
     * invece che le porte aperte

    */

     Scavatrice *scava;
     int scelta;
     //char k;
     if(g==0){
       scava=&arvais;
     }
     else{
        scava=&hartornen;
     }


    if(scava->cav->imprevisto==crollo){
        scava->energia--;
        printf("\nLa caverna in cui sei entrato ha un imprevisto crollo");
        printf("\nLa tua energia scende di 1");
        stato_serbatoi(g);
        printf ("\n\nPremi enter per continuare\n");
        myflush;

        if(scava->energia<0){
        vincitore=(g+1)%2;
        printf("\n La tua energia è negativa!");
        printf ("\n\nPremi enter per continuare\n");
        myflush;
        }
        }
        else if((scava->cav->imprevisto==baco)&&(vincitore>2)){
            scava->energia=0;
            scava->raccolta=0;
            printf("\nLa caverna in cui sei entrato ospita un temibile baco");
            printf("\nChe divora tutta la tua melassa");
            stato_serbatoi(g);
            printf ("\n\nPremi enter per continuare\n");
            myflush;

            }



     if((scava->cav->melassa>0)&&(vincitore>2)){
        do{
            printf("\n Nella caverna si trovano  %d UM", scava->cav->melassa);
            printf("\n In quale serbatoio deve andare");
            printf("\n1 - Serbatoio energia scavatrice (max 4 UM)");
            printf("\n2 - Serbatoio raccolta  (max 10 UM)");
            scanf("%d",&scelta);
            myflush;
            if (scelta==1){
                scava->energia=scava->energia+scava->cav->melassa;

                if(scava->energia>4)
                    scava->energia=4;


            }
            else{
                if(scelta==2){
                    scava->raccolta=scava->raccolta+scava->cav->melassa;
                    if(scava->raccolta>10)
                        scava->raccolta=10;
                }
                else{
                        printf("\n Scelta non prevista dal menu: scrivi 1 oppure 2");
                        printf ("\n\nPremi enter per continuare");
                        myflush;
                     }
            }


        } while ((scelta!=1)&&(scelta!=2));// fine do
        stato_serbatoi(g);
        printf ("\n\nPremi enter per continuare\n");
        myflush;

    }
    else{
        printf("\n Nella caverna non c'è melassa da raccogliere");
        printf ("\n\nPremi enter per continuare");
        myflush;
    }
    if((scava->cav->stato==accidentata)&&(vincitore>2)){
        scava->energia--;
        printf("\nCaverna accidentata");
        printf("\nLa tua energia scende di 1");
        stato_serbatoi(g);
        printf ("\n\nPremi enter per continuare\n");
        myflush;
        if(scava->energia<0){
            vincitore=(g+1)%2;
            printf("\n Aaargh");
            printf ("\n\nPremi enter per continuare\n");
            myflush;

        }

     } else if((scava->cav->stato==speciale)&&(vincitore>2)){
                scava->energia++;
                if(scava->energia>4)
                    scava->energia=4;
                printf("\nCaverna speciale");
                printf("\nLa tua energia sale di 1 (max 4)");
                stato_serbatoi(g);
                printf ("\n\nPremi enter per continuare\n");
                myflush;
                }

    if((scava->energia<0)&&(vincitore>2)){
        vincitore=(g+1)%2;
        printf("\nEnergia negativa! Hai perso!");
        printf ("\n\nPremi enter per continuare");
        myflush;
    }
    else{
        if((scontro(g))&&(vincitore>2)){

        printf("\n\n Un Arvais ed un Hartornen nella stessa caverna!");
        printf("\n Hai uno scontro con l'altro giocatore!");
        printf ("\n\nPremi enter per continuare\n");
        myflush;
        definisci_vincitore();
        pulisciSchermo;
        }
    }


    return 0;
} // fine avanza_2()



static int abbatti(int g){
//     int i=0;
     Scavatrice *scava;
     int scelta;
     //char k;
     if(g==0){
       scava=&arvais;
     }
     else{
        scava=&hartornen;
     }

     scava->energia--;
     printf("\n\nAbbattere le caverne diminuisce la tue energia");
     stato_serbatoi(g);
     if(scava->energia<0){
        vincitore=(g+1)%2;

        printf("\n\nEnergia negativa! Hai perso!");
        printf ("\n\nPremi enter per continuare");
        myflush;
        }

      /*
        // Ci sono scontri quando si abbatte?
        if((scontro(g))&&(vincitore>2)){

        printf("\n\n Un Arvais ed un Hartornen nella stessa caverna!");
        printf("\n Hai uno scontro con l'altro giocatore!");
        printf ("\n\nPremi enter per continuare\n");
        k=getchar();
        definisci_vincitore();
        }
      */
     if(vincitore>2){
        do{
            pulisciSchermo;
            printf("\n\n   605  Giocatore %d per la casata %s",gio+1, casata[gio]);
            printf("\nLa caverna ha le seguenti porte ancora chiuse");

            if (scava->cav->avanti==NULL){

                printf("\n1 - avanti");

            }
             if (scava->cav->destra==NULL){

                printf("\n2 - destra");
            }
             if (scava->cav->sinistra==NULL){

                printf("\n3 - sinistra");
            }
            printf("\n\nScegli quale porta vuoi abbattere\n");

            scanf("%d",&scelta);
            myflush;

            switch(scelta){

                case 1:{ if (scava->cav->avanti==NULL){

                            scava->cav->avanti=crea_caverna_2();
                            scava->cav=scava->cav->avanti;// collega nuova caverna
                            printf("\n Hai abbattuto la porta davanti");
                         }
                         else {printf ("\nPorta già aperta, riprova");
                               scelta=5;
                               printf ("\n\nPremi enter per continuare");

                               //pulisciSchermo;
                          }

                        } break;
                case 2:{
                         if (scava->cav->destra==NULL){

                            scava->cav->destra=crea_caverna_2();
                            scava->cav=scava->cav->destra; // collega nuova caverna
                            printf("\n Hai abbattuto la porta a destra");
                          }
                          else {printf ("\nPorta già aperta, riprova");
                                scelta=5;
                                printf ("\n\nPremi enter per continuare");
                                myflush;
                                //k=getchar();

                                 //pulisciSchermo;
                          }


                        } break;
                case 3:{
                          if (scava->cav->sinistra==NULL){

                            scava->cav->sinistra=crea_caverna_2();
                            scava->cav=scava->cav->sinistra;// collega nuova caverna
                            printf("\n Hai abbattuto la porta sinistra");
                          }
                          else {printf ("\nPorta già aperta, riprova");
                                scelta=5;
                                printf ("\n\nPremi enter per continuare");

                                myflush;
                                //pulisciSchermo;
                          }

                        } break;

                default:{ printf("\n Scelta non prevista dal menu");
                          printf ("\n\nPremi enter per continuare");

                          myflush;
                          //pulisciSchermo;
                          }
                } //fine switch


        }while(((scelta!=1)&&(scelta!=2))&&(scelta!=3));

        //avanziamo nella nuova caverna
        avanza_2(g);


    }//fine if vincitore
    return 0;
}

static int scontro(int g){
    /* la funzione ritorna 1 (vero) in caso di scontro,
        ritorna 0 se non c'è scontro
    */
    int prob,n,ris=0;
    if(fuori[(g+1)%2])
        prob=0;
    else{
        prob=3*turni[g]+3;
        /* Se prob arriva o supera 100 le cose perdono l'effetto casuale,
         * la probabilità di scontro diventa una certezza.
         * E non è quel che si vuole.  Metto quindi un limite a quota 90.
         */
        if(prob>90) prob=90;
        n=rand()%100;
        if(n<prob)ris=1;
    }
    return ris;
}
static int definisci_vincitore(){
    /* Questa funzione definisce il vincitore in caso di scontro diretto
     * Ho pensato di dare un po' di valore anche all'energia
     * della scavatrice
     * ( se è 0 non vedo futuro in uno scontro diretto)
     * Se c'é pareggio, intervengo con un "lancio di moneta"
    */
    int ris0,ris1,rnd;
    ris0=arvais.energia/2+arvais.raccolta;
    ris1=hartornen.energia/2+hartornen.raccolta;
    if(ris0>ris1){
            vincitore=0;

    }
    else
        if(ris0<ris1){
            vincitore=1;

        }
        else{
             rnd=rando(50,50,0)-1;// valori 0 oppure 1 equiprobabili
             // il giocatore 0 oppure 1 sono perdenti
             printf("\nSta arrivando un baco!");
             printf("\nIl baco sta mangiando la scavatrice del giocatore %d!",rnd+1);
             printf("\n\n Premi enter per continuare");

             vincitore=(rnd+1)%2;

            }
    return vincitore;
}


static Caverna *crea_caverna_2(int g){
    Caverna *nuova;
    int i,a=20,u,n,s,ris;
    nuova=malloc(sizeof(Caverna));
    u=5*turni[g];
    n=(100-u-a)/2;
    s=100-u-a-n;
    ris=rand()%100;
    if(ris<u)i=4;
        else if(ris<(u+a)) i=3;
             else if(ris<(u+a+s)) i=2;
                   else i=1;
    switch(i){
        case 1: {nuova->stato=normale;}break;
        case 2: {nuova->stato=speciale;}break;
        case 3: {nuova->stato=accidentata;}break;
        case 4: {nuova->stato=uscita;}break;
        }
    i=rando(40,40,20);
    switch(i){
        case 1: {nuova->melassa=nessuna;}break;
        case 2: {nuova->melassa=poca;}break;
        case 3: {nuova->melassa=molta;}break;
        }

    i=rando(40,40,20);
     switch(i){
        case 1: {nuova->imprevisto=nessun_imprevisto;}break;
        case 2: {nuova->imprevisto=crollo;}break;
        case 3: {nuova->imprevisto=baco;}break;
        }
     nuova->avanti=NULL;
     nuova->destra=NULL;
     nuova->sinistra=NULL;

    return nuova;
}

static int aggira(Caverna * prec,Caverna *succ,int dir ){
   Caverna *nuova;
   int tipo;
   /*do{
      printf("\nChe tipo di caverna vuoi creare?\n");
      printf("\n1 - normale \n2 - speciale \n3 - accidentata\n\n" );
      scanf("%d",&tipo);
      if(((tipo!=1)&&(tipo!=2))&&(tipo!=3))
      printf("\nPuoi scegliere solo 1, 2 o 3");
   }while (((tipo!=1)&&(tipo!=2))&&(tipo!=3));
   */
    tipo=rand()%3+1; /*
    * Ho deciso che il giocatore avrà una caverna di tipo casuale
    * Il gioco diventa lentissimo se anche per le caverne da
    * aggiramento il giocatore deve scegliere il tipo della caverna
    * tipo è adesso generato casualmente
    * con i valori 1-2-3 teoricamente equiprobabili
    */
	nuova=crea_caverna(tipo);

	nuova->avanti=succ; //collego la caverna nuova alla caverna crollata

	/* Collego la caverna precedente a quella nuova,
	al posto della caverna crollata
	*/

    switch(dir){

           case 1:{ prec->avanti=nuova; }break;

           case 2:{ prec->destra=nuova;} break;

           case 3:{ prec->sinistra=nuova;} break;

        }

	return 0;
}
static int esci(int g){
    fuori[g]=1;
    return 0;
}

static Scavatrice init_scavatrice(int g){
    Scavatrice S;
    //
    if(g==0)
        S.cav=primo_cunicolo_arvais;
      else
        S.cav=primo_cunicolo_hartornen;
    S.energia=4;
    S.raccolta=0;

    return S;
}



int crea_cunicoli(){
	int scelta, g;
	//char k;
	do{
		printf("\n\n       ALLESTIMENTO CUNICOLI \n\n");
		printf("Scegli quale giocatore sta preparando la mappa");
		printf("\n 1- Giocatore 1- Casata Arvais");
		printf("\n 2- Giocatore 2- Casata Hartornen \n");
		scanf("%d",&g);
		myflush;
		if(g==1) gio=0;
			else if(g==2) gio=1;
				 else{
					 printf("\n Scelta non prevista");
					 printf("\n Digita 1 oppure 2");
				 }
	}while((g!=1)&&(g!=2));

	do{
		pulisciSchermo;
		printf("\n\nCostruisce il cunicolo il giocatore %d per la casata %s",gio+1, casata[gio]);
		printf("\n\nPer poter giocare ambedue i cunicoli devono avere almeno 10 caverne");
		printf("\n Solo allora i cunicoli potranno essere chiusi");
		printf("\n\n\nInserisci la tua scelta");
		printf("\n\n 0- Crea un nuovo cunicolo per la casata %s (elimina cunicolo precedente)", casata[gio]);
		printf("\n 1- Inserisci nuova caverna" );
		printf("\n 2- Cancella l'ultima caverna inserita");
		printf("\n 3- Stampa il cunicolo");
		printf("\n 4- Chiudi i cunicoli (e torna al menu principale)\n");
		scanf("%d",&scelta);
        myflush;
		switch(scelta){
		    case 0: {// inizializza cunicolo

                        init_cunicolo(gio);
                    } break;
			case 1:{ //crea_cunicoli per giocatore gio
					 ins_caverna(gio);
					 //gio=(gio+1)%2;

					} break;
			case 2:{  //gioca il giocatore gio, se mappe ok

					 canc_caverna(gio);


					} break;
			case 3:{
						stampa_cunicolo(gio);


					} break;
			case 4:{  chiudi_cunicoli(gio);

					} break;
			default:{ printf("\n Scelta non prevista dal menu");
					  printf("\n Digitazione sbagliata?");
					  printf ("\n\nPremi enter per continuare");
					  myflush;
					  pulisciSchermo;}
			}


	}while(scelta!=4);


	printf("\n\n Premi enter per continuare");
	myflush;
	pulisciSchermo;
	return 0;

	}  // fine funzione crea_cunicoli()

static int chiudi_cunicoli(){
    //char k;
    mappe[0]=mappe[1]=0;
    if(numCaverne[0]>=3){
						  mappe[0]=1;

						  }
	if(numCaverne[1]>=3)
						  mappe[1]=1;
	map=mappe[1]+mappe[0];
	if(map==2)
	  printf("\n\n Avete correttamente completato ambedue i cunicoli");
	  else{
		 if(mappe[0]==1)
			 printf("\n Non completa la mappa del giocatore 2 - Hartornen");
		 else
            if(mappe[0]==1)
                printf("\n Non completa la mappa del giocatore 1 - Arvais");
      }
    printf("\n\n Premi enter per continuare");
	myflush;
	pulisciSchermo;


return 0;
}// fine funzione chiudi_cunicoli()


static int ins_caverna(int g){
	//int inserita=0;
	Caverna *nuova;
	int tipo, direzione=0;

	do{
        printf("\n\nChe tipo di caverna vuoi creare?\n");
        printf("\n1 - normale \n2 - speciale \n3 - accidentata\n\n" );
        scanf("%d",&tipo);
        myflush;
        if(((tipo!=1)&&(tipo!=2))&&(tipo!=3)){
            printf("\nPuoi scegliere solo 1, 2 o 3");
            printf("\n\n Premi enter per continuare");
	        myflush;
	        }
	}while (((tipo!=1)&&(tipo!=2))&&(tipo!=3));

	nuova=crea_caverna(tipo);


    do{
        printf("\n\nVuoi inserire in testa o in coda ?");
        printf("\n1 - testa \n2 - coda \n\n" );
        scanf("%d",&tipo);
        myflush;
        if((tipo!=1)&&(tipo!=2)){
            printf("\nPuoi scegliere solo 1 o  2 ");
            printf("\n\n Premi enter per continuare");
	        myflush;
	        }

	}while ((tipo!=1)&&(tipo!=2));


    do{
        printf("\n\nIn che direzione vuoi andare ?");
        printf("\n1 - avanti \n2 - destra \n 3 - sinistra \n" );
        scanf("%d",&direzione);
        myflush;
        if(((direzione!=1)&&(direzione!=2))&&(direzione!=3)){
            printf("\nPuoi scegliere solo 1, 2 o 3");
            printf("\n\n Premi enter per continuare");
	        myflush;
	        }
    }while (((direzione!=1)&&(direzione!=2))&&(direzione!=3));

    if(tipo==2){
          ins_coda(g,nuova,direzione);
    }
    else {
          ins_testa(g,nuova,direzione);
            }


	return numCaverne[g];
}

static Caverna *crea_caverna(int t){
    int i;
    Caverna *nuova;

    //printf("chiamata crea_caverna()");

    nuova=malloc(sizeof(Caverna));
    switch(t){
        case 1: {nuova->stato=normale;}break;
        case 2: {nuova->stato=speciale;}break;
        case 3: {nuova->stato=accidentata;}break;
        }
    i=rando(50,30,20);
    switch(i){
        case 1: {nuova->melassa=nessuna;}break;
        case 2: {nuova->melassa=poca;}break;
        case 3: {nuova->melassa=molta;}break;
        }

    i=rando(50,35,15);
     switch(i){
        case 1: {nuova->imprevisto=nessun_imprevisto;}break;
        case 2: {nuova->imprevisto=crollo;}break;
        case 3: {nuova->imprevisto=baco;}break;
        }
     nuova->avanti=NULL;
     nuova->destra=NULL;
     nuova->sinistra=NULL;

    return nuova;
}

int rando(int n1, int n2, int n3){
    int rndo;
    rndo=rand()%100; // genero valori da 0 a 99
    if(rndo<n1)
        rndo=1;
        else
            if(rndo>=(n1+n2))
                rndo=3;
                else rndo=2;

    return rndo;
}

int ins_testa(int g,Caverna *n, int dir){

    Caverna *temp;


    if(g==0){
        if(primo_cunicolo_arvais==NULL)
            init_cunicolo(g);
        temp=primo_cunicolo_arvais;
    }
    else{
        if(primo_cunicolo_hartornen==NULL)
            init_cunicolo(g);
        temp=primo_cunicolo_hartornen;
    }

    //if(temp->avanti==NULL) // Il cunicolo è creato ma vuoto, si va solo in avanti
      //  {dir=1;}

    switch(dir){

       case 1:{ n->avanti=temp->avanti; }break;

       case 2:{ n->destra=temp->avanti;} break;

       case 3:{ n->sinistra=temp->avanti;} break;

    }

    temp->avanti=n;

    numCaverne[g]++;
    return numCaverne[g];
}
int ins_coda(int g, Caverna *n, int dir){

    Caverna *corrente, *next;


    if(g==0){
        if(primo_cunicolo_arvais==NULL)
            init_cunicolo(g);
        corrente=primo_cunicolo_arvais;
    }
    else{
        if(primo_cunicolo_hartornen==NULL)
            init_cunicolo(g);
       corrente=primo_cunicolo_hartornen;
    }

    if(corrente->avanti==NULL) // Il cunicolo è creato ma vuoto, faccio ins in testa
        {corrente->avanti=n;}
    else{
        next=corrente;

        while(next!=NULL){
                //prima provo a vedere se c'è una caverna successiva
            if(next->avanti!=NULL)
                next=next->avanti;
               else{
                    if(next->destra!=NULL)
                        next=next->destra;
                        else{
                            if(next->sinistra!=NULL)
                                 next=next->sinistra;
                                else next=NULL; // non ci sono caverne successive
                        }
               }
            if(next!=NULL)
                corrente=next;

            //printf("next %p",next);

        }



         switch(dir){

           case 1:{ corrente->avanti=n; }break;

           case 2:{ corrente->destra=n;} break;

           case 3:{ corrente->sinistra=n;} break;

        }
    }

    numCaverne[g]++;
    return numCaverne[g];
}
int disalloca_cunicolo(int g){
    //char k;
    /*Non ricorsivo, non cancella le caverne aggiunte
    * durante la fase di gioco
    * con le funzioni abbatti() e aggira()
    * Si può usare solo in fase di creazone cunicoli.
    */
    Caverna *temp, *corrente, *next;
    if(g==1)
        temp=primo_cunicolo_arvais;
        else
            temp=primo_cunicolo_hartornen;

    corrente=temp;

    while(corrente!=NULL){
            //prima provo a vedere se c'è una caverna successiva
        if(corrente->avanti!= NULL)
            next=corrente->avanti;
           else
                if(corrente->destra!=NULL)
                    next=corrente->destra;
                    else
                        if(corrente->sinistra!=NULL)
                            next=corrente->sinistra;
                            else next=NULL; // non ci sono caverne successive
        free(corrente);//disalloco la precedente
        corrente=next;

    }

    //elimino il cunicolo
    if(g==0)
        primo_cunicolo_arvais=NULL;
        else
            primo_cunicolo_hartornen=NULL;
    numCaverne[g]=0;
    //printf("Cunicolo casata %s eliminato",casata[g]);
    printf ("\n\nPremi enter per continuare");
	myflush;
	pulisciSchermo;

    return 0;

}
/*  // prova dealloca

void prova_dealloca(Caverna *C){

    if(C!=NULL){
        if(C->avanti!=NULL)
            dealloca_cunicolo(C->avanti);
        if(C->destra!=NULL)
            dealloca_cunicolo(C->destra);
        if(C->sinistra!=NULL)
            dealloca_cunicolo(C->sinistra);

        free(C);
        C=NULL;
        map=mappe[0]=mappe[1]=0;

    }


}

*/


int dealloca_cunicolo(Caverna *C){
     // ricorsivo, utilizzato a fine gioco

    if(C!=NULL){
        if(C->avanti!=NULL){
            dealloca_cunicolo(C->avanti);
            free(C->avanti);
            C->avanti=NULL;
        }

        if(C->destra!=NULL){
            dealloca_cunicolo(C->destra);
            free(C->destra);
            C->destra=NULL;
        }
        if(C->sinistra!=NULL){
            dealloca_cunicolo(C->sinistra);
            free(C->sinistra);
            C->sinistra=NULL;
        }



    }

    return 0;

}


static int canc_caverna(int g) {
        /* cancello l'ultima caverna creata, in testa o in coda
       a scelta dell'utente
    */
    int tipo;
    //char k;

	printf("\n chiamata canc_caverna");

	do{
        printf("\nVuoi cancellare in testa o in coda?");
        printf("\n1 - testa \n2- coda \n" );
        scanf("%d",&tipo);
        myflush;
        if((tipo!=1)&&(tipo!=2))
            printf("\nPuoi scegliere solo 1 oppure 2");
	}while ((tipo!=1)&&(tipo!=2));

	if(tipo==1)
        pop_caverna(g);
        else
            deq_caverna(g);
    printf("Caverna eliminata");
    printf ("\n\nPremi enter per continuare");
	myflush;
	pulisciSchermo;

	return numCaverne[g];
}

static Caverna *init_cunicolo(int g){
    Caverna *temp;
    temp=malloc(sizeof(Caverna));
    temp->avanti=NULL;
    temp->sinistra=NULL;
    temp->destra=NULL;
    //disalloca_cunicolo(g);
    numCaverne[g]=0;
    if(g==0)
        primo_cunicolo_arvais=temp;
       else
            primo_cunicolo_hartornen=temp;

    return temp;
}

int pop_caverna(int g){
    Caverna *corrente,*oldprimo;
    //char k;

    if(g==0){
        if(primo_cunicolo_arvais==NULL)
            init_cunicolo(g);
        corrente=primo_cunicolo_arvais;
    }
    else{
        if(primo_cunicolo_hartornen==NULL)
            init_cunicolo(g);
       corrente=primo_cunicolo_hartornen;
    }

    if(corrente->avanti==NULL) // Il cunicolo esiste ma è vuoto
        {printf("\nCunicolo della casata  %s vuoto", casata[g]);}
    else{

         numCaverne[g]--;
         oldprimo=corrente->avanti;

         if(corrente->avanti->avanti!= NULL){
            corrente->avanti=corrente->avanti->avanti;
            }
         else
            if(corrente->avanti->destra!= NULL){
                corrente->avanti=corrente->avanti->destra;
               }
            else
                if(corrente->avanti->sinistra!= NULL){
                    corrente->avanti=corrente->avanti->sinistra;
                   }
                else corrente->avanti=NULL; // non ci sono caverne successive
       }// fine else cunicolo vuoto



    free(oldprimo);//disalloco la prima caverna del cunicolo
    printf("Caverna eliminata");
    printf ("\n\nPremi enter per continuare");
	myflush;
	pulisciSchermo;

    return 0;
    }

int deq_caverna(int g){
    /*scorriamo fino in fondo, aggiorniamo puntatore della precedente
    * e cancelliamo ultima caverna
    */
    Caverna *corrente, *next, *prec;
    //char k;

   if(g==0){
        if(primo_cunicolo_arvais==NULL)
            init_cunicolo(g);
        corrente=primo_cunicolo_arvais;
    }
    else{
        if(primo_cunicolo_hartornen==NULL)
            init_cunicolo(g);
       corrente=primo_cunicolo_hartornen;
    }

    if(corrente->avanti==NULL) // Il cunicolo esiste ma è vuoto
        {printf("\nCunicolo della casata  %s vuoto", casata[g]);
         printf("Caverna eliminata");
         printf ("\n\nPremi enter per continuare");
         myflush;
         pulisciSchermo;
        }
    else{
        next=corrente->avanti;
        prec=corrente;
        numCaverne[g]--;
        while(next!=NULL){
            //prima provo a vedere se c'è una caverna successiva
            if(next->avanti!= NULL){
                prec=next;
                next=next->avanti;

                }
               else
                    if(next->destra!=NULL){
                        prec=next;
                        next=next->destra;
                        }
                        else
                            if(next->sinistra!=NULL){
                                prec=next;
                                next=next->sinistra;
                                }
                                else next=NULL; // non ci sono caverne successive

        } //fine while
       prec->avanti=NULL;
       prec->destra=NULL;
       prec->sinistra=NULL;
       printf("Caverna eliminata");
       printf ("\n\nPremi enter per continuare");
       myflush;
       pulisciSchermo;

        }//fine if cunicolo vuoto


    return 0;
};

static int stampa_cunicolo(int g){
    //char k;
	Caverna *corrente;


    if(g==0)
       corrente=primo_cunicolo_arvais;
       else
       corrente=primo_cunicolo_hartornen;
    if(corrente==NULL)
        printf("\n Cunicolo della casata  %s  non ancora creato\n",casata[g]);
    else{
        if(corrente->avanti==NULL)
                printf("\n Il cunicolo della casata %s è vuoto",casata[g]);
            else
                {   printf("\n Il cunicolo della casata %s contiene %d caverne",casata[g],numCaverne[g]);
                    printf("\n\n\n Inizia stampa sequenza caverne\n");
                    //stampare la radice
                    printf("\nNodo iniziale \n");
                    printf("\nLink avanti del nodo iniziale  --  %p",corrente->avanti);

                    corrente=corrente->avanti;
                    //stampa_caverna(corrente);
                    while(corrente!=NULL){
                        stampa_caverna(corrente);
                        // provo a vedere se c'è una caverna successiva
                        if(corrente->avanti!= NULL){
                            corrente=corrente->avanti;

                            }
                           else
                                if(corrente->destra!=NULL){
                                    corrente=corrente->destra;

                                    }
                                    else{
                                        if(corrente->sinistra!=NULL){
                                            corrente=corrente->sinistra;
                                            }
                                            else corrente=NULL; // non ci sono caverne successive
                                     }

                    } //fine while

             } //fine else
        }//fine else
    printf("\n\nFine del cunicolo della casata %s", casata[g]);
    printf("\n\n Premi enter per proseguire");
	myflush;

	pulisciSchermo;

	return 0;
}

void stampa_caverna(Caverna *corrente){

    printf("\n\n\nStato della caverna  --");
    switch(corrente->stato){
       case 0:printf(" normale");break;
       case 1:printf(" speciale");break;
       case 2:printf(" accidentata");break;
       case 3:printf(" uscita");break;
    }
    printf("\nMelassa della caverna  --");
    switch(corrente->melassa){
       case 0:printf(" nessuna");break;
       case 2:printf(" poca");break;
       case 3:printf(" molta");break;
    }
    printf("\nImprevisti della caverna  --");
    switch(corrente->imprevisto){
       case 0:printf(" nessun imprevisto");break;
       case 1:printf(" crollo");break;
       case 2:printf(" baco");break;
    }
    printf("\nLink avanti della caverna    -- %p",corrente->avanti);
    printf("\nLink destra della caverna    -- %p",corrente->destra);
    printf("\nLink sinistra della caverna  -- %p",corrente->sinistra);


}

static void stato_serbatoi(int g){
    Scavatrice *scava;

    if(g==0){
       scava=&arvais;
    }
    else{
        scava=&hartornen;
    }


    if (g==vincitore){
        printf("\n\n\n             Vincitore il giocatore %d  della casata  %s",g+1, casata[g]);
    }
    else{
        printf("\n\n         Giocatore %d  della casata  %s  --  Stato serbatoi:",g+1, casata[g]);
        }
    //printf("\n\n Stato serbatoi:");
    printf("\n\nEnergia  %d  UM",scava->energia);
    printf("\nRaccolta %d  UM",scava->raccolta);
    myflush;

}




void termina_gioco(){
    //printf("termina gioco 1478");
    //printf("\n Ha vinto il giocatore %d  per la casata %s", vincitore+1, casata[vincitore]);
    dealloca_cunicolo(primo_cunicolo_arvais);
    printf("dopo dealloca arvais 1510");
    dealloca_cunicolo(primo_cunicolo_hartornen);
    //printf("dopo dealloca hartornen 1483");
    numCaverne[0]=numCaverne[1]=0;

}

