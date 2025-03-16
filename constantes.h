#ifndef CONSTANTES_H
#define CONSTANTES_H

const QString VERSION_PROG = "0.6.6";
const QString NOM_PROG = "Rami";
const QString ORGANISATION = "JND-Software";
const QString COPYRIGHT = "@ 2007-2025 JND-Software Inc";

enum	PERIODEJEU { ATTENTE, DEBUT1PARTIE, JOUEURSJOUE, FIN1PARTIE,
                        ESTJOUE, NORDJOUE, OUESTJOUE, SUDJOUE };

enum	POSCLIC { POSERREUR, POSPIOCHE, POSJETEES, POSCARTES, POSPOSEES };

enum	CODEFIN { CONTINUE, SUDGAGNE, ESTGAGNE, NORDGAGNE, OUESTGAGNE, PARTIENULLE, ARRETPARTIE, ERREUR };

enum CODERETOUR { RETOUR_OK = 1, PAS_TIERCE = 10, PAS_51 = 20, PAS_CONFORME = 30, PAS_CJP = 40, PAS_SUR_POSE = 50, PAS_PIOCHE_PRISCJ = 60, PAS_ENCORE_POSE = 70, PAS_DERNIERE_CARTE = 80, ERREUR_INCONNUE = 100 };

enum NIVEAUJEU { NIVEAU_FAIBLE = 10, NIVEAU_MOYEN = 20, NIVEAU_FORT = 30 };

enum POSSURPOSE { SURPOSE_EST, SURPOSE_NORD, SURPOSE_OUEST, SURPOSE_SUD, SURPOSE_ERREUR };

enum VITESSE { VITESSE_LENTE, VITESSE_NORMALE, VITESSE_RAPIDE };

const int RAMI_FERMER = 10;
const int RAMI_QUITTER = 20;

const int MAX_PASPOSE_FAIBLE = 13;
const int MAX_PASPOSE_MOYEN = 9;
const int MAX_PASPOSE_FORT = 4;

const int OFFSET = 90;
const int W_AFF_ECRAN = 1460;

const   quint8 PERSONNE = 0;
const   quint8 SUD = 10;
const   quint8 NORD = 20;
const   quint8 EST = 30;
const   quint8 OUEST = 40;
const	quint8 NBTOTCARTES = 104;
const   quint8 PASCARTE = 0;
const   quint8 DOSCARTE = 81;
const   quint8 RIENCARTE = 90;
const   quint8 ERREUR_CARTE = 250;
const   quint8 MARKSUITE = 123;
const   quint8 MARKMEME = 99;
const   quint8 MARKFIN = 255;

const long TEMPO_ANIME = 75;		/// en milliseconde (1000 = 1 seconde)
const long TEMPO_FINPARTIE = 2000;
const long TEMPO_ATTENTE = 500;
const int TEMPO_TIMER = 125;

const int TEMPO_VALEUR_BOUCLE = 22;

/// CONSTANTES CVALEURJEU
enum  NOTESPASPOSE { PASPOSE_MEME2 = 300, PASPOSE_MEME3 = 600, PASPOSE_MEME4 = 800, PASPOSE_SUITECXC = 300, PASPOSE_SUITE2 = 300, PASPOSE_TIERCE = 1000 };

enum  NOTESAPOSE { APOSE_MEME2 = 100, APOSE_MEME3 = 200, APOSE_MEME4 = 300, APOSE_SUITECXC = 100, APOSE_SUITE2 = 100, APOSE_TIERCE = 400 };

enum  NOTESTESTPOSE { TESTPOSE_MEME3 = 300, TESTPOSE_MEME4 = 500, TESTPOSE_SUITE = 1000 };

const int COEUR = 0;
const int PIQUE = 1;
const int CARREAU = 2;
const int TREFLE = 3;

const	quint8 ordre2joueurs[8]	 = {SUD,NORD,0,0,
                                            NORD,SUD,0,0};

const	quint8 ordre3joueurs[12] = {SUD,EST,OUEST,0,
                                            EST,OUEST,SUD,0,
                                            OUEST,SUD,EST,0};

const	quint8 ordre4joueurs[16] = {SUD,EST,NORD,OUEST,
                                            EST,NORD,OUEST,SUD,
                                            NORD,OUEST,SUD,EST,
                                            OUEST,SUD,EST,NORD};

const quint8 jeucartesorg[104] = {1,2,3,4,5,6,7,8,9,10,11,12,13,
                                            21,22,23,24,25,26,27,28,29,30,31,32,33,
                                            41,42,43,44,45,46,47,48,49,50,51,52,53,
                                            61,62,63,64,65,66,67,68,69,70,71,72,73,
                                            1,2,3,4,5,6,7,8,9,10,11,12,13,
                                            21,22,23,24,25,26,27,28,29,30,31,32,33,
                                            41,42,43,44,45,46,47,48,49,50,51,52,53,
                                            61,62,63,64,65,66,67,68,69,70,71,72,73};

const	int tablepenalitecarte[13] = {10,2,3,4,5,6,7,8,9,10,10,10,10};

#endif // CONSTANTES_H
