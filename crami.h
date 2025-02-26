#ifndef CRAMI_H
#define CRAMI_H

#include <QWidget>

#include "cresultat.h"
#include "constantes.h"

class QLabel;
class CMainWindow;

class CAfficheurImage;
class CBaseJoueurs;
class SDLPlayer;
class CResultat;

class CRami : public QWidget
{
    Q_OBJECT
public:
    explicit CRami(QWidget *parent = 0);

    void debuterJeu();

    void setNombreJoueur(const int nbj);
    void setArretPartie();
    void setSiAnimeCarte(const bool f_animecarte);
    void setNiveauJeu(const int niveau);
    void setSiSonOrdi(const bool f_son);
    void setVitesse(const int vitesse);
    void setVolume(const int volume);

    bool getEtatPartie();
    QString getMessageStatusBar();
    QList<CResultat> * getListeResultats();

protected:
    void mouseReleaseEvent(QMouseEvent * event);
    void mousePressEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent * event);
    void timerEvent(QTimerEvent * event);

private:
/// POUR TEST
    void Ordi_Test_A_Piocher(const quint8 *jeu);
    void Ordi_Test_A_Jeter1C(const quint8 *jeu);
    void Ordi_Test_A_Poser(const quint8 *jeu);
    void Ordi_Test_A_Poser1C(const quint8 *jeu);
    void Ordi_Test_A_PrisCJetee(const quint8 *jeu);
    void Ordi_Test_Fin();
    void insererCartesDansJeu(quint8 * aJeu, int nbcartes);
///

    void loadImage(const QString &fileName, QImage *image, const int width, const int height);
    void afficher_g_p_Sud();
    void afficher_g_p_Nord();
    void afficher_g_p_Est();
    void afficher_g_p_Ouest();
    void effacerCartesJeuSud();
    void effacerCartePioche();
    void effacerCarteJetee();
    void effacerCartesPosees();
    void effacerCarreJoueursOrdi();
    void effacerNbCartesJoueurs();
    void afficherMsgQuiGagne(const QString &msg);
    void afficherMsgNom(const QString &msg, const int x, const int y);
    void afficherMsgPenalite(const QString &msg, const int x, const int y);
    void afficherMsgSB(const QString &msg);

    void Jeu_Debut_1_Partie();
    void Ordi_Joueurs_Joue();
    void Jeu_Fin_1_Partie();

    void jouerSonErreur();
    void jouerSonReste1Carte(const quint8 qui);
    void jouerSonPose1Carte();
    void jouerSonPoseCartes();
    void jouerSonGagne(const quint8 qui);
    void jouerSonJette1Carte();
    void jouerSonPiochePrendCarte(const quint8 qui);

    int  genererHasard_0_103();
    void melangerCartes();
    void distribuerCartes();
    void initialiserJeuJoueurs();
    void initialiserNbCartesJoueurs();
    void afficherCartesJeuSud();
    void afficherNbCartesEst();
    void afficherNbCartesNord();
    void afficherNbCartesOuest();
    void afficherNbCartesJoueurs();
    void afficherNbCartesSud();
    void afficherUneCarte(const int dx, const int dy, const int sx, const int sy);
    void dessinerUneCarte(const int dx, const int dy, const int sx, const int sy);
    void afficherUneCarte(const int dx, const int dy, const int carte);
    void effacerUneCarte(const int dx, const int dy);
    void afficherDessinJoueurs();
    void initialiserCartesJetees();
    void initialiserCartesPosees();
    void initialiserVariablesPoseJoueurs();
    void afficherDosCartePioche();

    bool Ordi_Est_Jouer();
    bool Ordi_Nord_Jouer();
    bool Ordi_Ouest_Jouer();
    void afficherRectOrangeEst();
    void afficherRectOrangeNord();
    void afficherRectOrangeOuest();
    void effacerRectOrangeEst();
    void effacerRectOrangeNord();
    void effacerRectOrangeOuest();
    void afficherRectangleOrange(const int x, const int y);
    void effacerRectangleOrange(const int x, const int y);
    int calculerPenaliteCartesJeuEst();
    int calculerPenaliteCartesJeuNord();
    int calculerPenaliteCartesJeuOuest();
    int calculerPenaliteCartesJeuSud();
    void afficherCartesJeuEst();
    void afficherCartesJeuNord();
    void afficherCartesJeuOuest();
    void afficherCartesJeu(const int x, const int y, const quint8 *aJeu, const int nbcarte);
    void rassemblerCartes();
    void afficherImageDebutJeu();
    void initialiserVarJoue_Sud();
    void restaurerEtatJeu();
    void sauvegarderEtatJeu();

    void gauchePressed_Jetees();
    void gauchePressed_Pioche();
    void gauchePressed_Cartes();
    void controlgauchePressed_Cartes();
    void gaucheReleased_Cartes();
    void gaucheReleased_Jetees();
    void gaucheReleased_Pioche();
    void gaucheReleased_Posees();
    void controlgaucheReleased_Cartes();
    void gaucheReleased_Erreur();

    void droitReleased_Jetees();
    void droitReleased_Cartes();
    void droitReleased_Erreur();

    QPoint calculerSourceXY1Carte(const int carte);
    void afficherAnimationPiocheCarte(const int carte, const quint8 joueur);
    void afficherAnimationPrendCarte(const int carte, const quint8 joueur);
    void afficherAnimationJetteCarte(const int carte, const quint8 joueur);
    void faireTemporisation(const long temporisation);
    void initialiserCartesSelect_Sud();
    void verifierPileCartesAPiocher();
    void afficherCartesSelect_Sud(const int posX, const int posY);
    void piocher1Carte(quint8 * ajeu, bool verifPile);
    void jeter1Carte(quint8 * ajeu, quint8 carte);

    void afficherCartesPoseesEst();
    void afficherCartesPoseesNord();
    void afficherCartesPoseesOuest();
    void afficherCartesPoseesSud();
    void afficherCartesPoseesJoueurs();
    void afficherCartesPosees(QList<quint8> & qlpose, const int posX, const int posY);
    quint8 prendreCarteJetee(quint8 * ajeu );
    bool simulerPrendreCarteJetee(quint8 * ajeu, const bool a_pose);
    quint8 chercherCartePlusPetiteValeur(quint8 * ajeu, const bool a_pose, int & nbpaspose);
    quint8 chercherCartePlusGrandeValeur(quint8 * ajeu, const bool a_pose);
    bool simulerPoserCarteJetee(quint8 * ajeu, const bool a_pose,const int nbcartes);
    quint8 calculerCarteAJeter(const bool a_poseJS, quint8 * ajeu, const bool a_pose, const int nbcartes, int & nbpaspose);

    void copierTableau(quint8 * aorg, quint8 * acopie, int nbitems);
    bool verifierPresence1Carte(quint8 * jeu, quint8 carte);
    int calculerPositionSurRectPosees();
    void afficherErreurPoseSud(const int numerreur);

    CAfficheurImage     *m_afficheurImage;
    CBaseJoueurs		*m_pBaseJoueurs;
    SDLPlayer           *m_mySDLPlayer;

    QLabel *m_gpSudLabel;
    QLabel *m_gpNordLabel;
    QLabel *m_gpEstLabel;
    QLabel *m_gpOuestLabel;

    QImage	m_imgMemPlateau;
    QImage	m_imgPlateau;
    QImage	m_imgCopie;

    QImage	m_imgFinJeu;
    QImage	m_imgDebutJeu;
    QImage	m_imgDessinCartes;
    QImage	m_imgFondCartesJoueur;
    QImage	m_imgFondCartesPosees;
    QImage	m_imgFondJetee;
    QImage	m_imgFondPioche;
    QImage	m_imgFondJoueursOrdi;
    QImage	m_imgDessinEst;
    QImage	m_imgDessinNord;
    QImage	m_imgDessinOuest;

    QString	m_messagestatusbar;

    QRect		m_rectpioche;
    QRect		m_rectjetees;
    QRect		m_rectcartes;
    QRect		m_rectposees;

    QList<CResultat>  m_qlresultat;

    int         m_posClicX;
    int			m_posClicY;
    int			m_keymodifier;
    Qt::MouseButton m_button;
    bool		m_feventmouse;

    bool		m_f_EtatPartie;
    int         m_timerId;
    bool        m_f_timerActif;
    bool		m_f_Fin1partie;
    PERIODEJEU	m_periodeJeu;
    int         m_nombredejoueur;
    int         m_numordre;
    quint8      m_ordrejeu[4];
    int         m_index_ds_ordrejeu;
    int         m_nombredetour;
    quint8      m_quigagne;
    quint8      m_quijoue;

    bool	m_f_animecarte;
    int     m_niveaujeu;
    bool    m_f_sonordi;
    int     m_vitesse;
    int     m_volume;

    long	m_tempo_anime;
    long	m_tempo_finpartie;
    long	m_tempo_attente;
    int     m_tempo_timer;

    quint8	m_jeucartes[104+2];
    int		m_index_carte_a_piocher;

    quint8	m_cartesjetees[110];
    int		m_index_carte_jetee;
    int		m_nbcartejetee;

    quint8  m_cartesposees[110];
    int     m_index_carte_posee;
    quint8  m_listepose[104];

    QLabel *m_nbCarteSudLabel;
    int		m_penalite_Sud;
    int		m_gagnee_Sud;
    int		m_nbcartes_Sud;
    quint8	m_jeu_Sud[16];
    quint8	m_cartesSelect_Sud[14];
    int		m_nbcartesSelect_Sud;
    bool	m_f_a_pose_Sud;
    bool	m_f_a_parle_Sud;
    QList<quint8> m_cartesposees_Sud;
    QList<quint8> m_flagCP_Sud;

    bool	m_fclicpioche;
    bool	m_fclicjetee;
    bool	m_fclic1carte;
    bool	m_fcliccartesselect;
    quint8	m_prise_ds_pioche_Sud;
    quint8	m_prise_ds_jetee_Sud;
    quint8	m_prise_ds_jeu_Sud;
    int	    m_indexcartecliquee;
    bool    m_fcarte_jetee_prise_posee;
    bool	m_fsauvegardeEtatJeu;

    quint8	m_movecartesSelect[13];
    bool	m_movecarteSud;
    int     m_moveposX;
    int     m_moveposY;
    int     m_movesrcX;
    int     m_movesrcY;
    QImage	m_movecopiePlateau;

    int     m_savenbcartesSud;
    quint8  m_savejeuSud[16];

/*
    QImage	m_savePlateau;
    quint8	m_savejeucartes[104+2];
    int		m_saveindex_carte_a_tirer;

    quint8	m_savecartesjetees[110];
    int		m_saveindex_carte_jetee;
    int		m_savenbcartejetee;
*/


    QLabel *m_nbCarteNordLabel;
    int     m_penalite_Nord;
    int     m_gagnee_Nord;
    int     m_nbcartes_Nord;
    quint8	m_jeu_Nord[16];
    bool	m_f_a_pose_Nord;
    bool	m_f_a_parle_Nord;
    quint8	m_prise_ds_jetee_Nord;
    QList<quint8> m_cartesposees_Nord;
    QList<quint8> m_flagCP_Nord;
    int     m_nbpaspose_Nord;

    QLabel *m_nbCarteEstLabel;
    int     m_penalite_Est;
    int     m_gagnee_Est;
    int     m_nbcartes_Est;
    quint8	m_jeu_Est[16];
    bool	m_f_a_pose_Est;
    bool	m_f_a_parle_Est;
    quint8	m_prise_ds_jetee_Est;
    QList<quint8> m_cartesposees_Est;
    QList<quint8> m_flagCP_Est;
    int     m_nbpaspose_Est;

    QLabel *m_nbCarteOuestLabel;
    int     m_penalite_Ouest;
    int     m_gagnee_Ouest;
    int     m_nbcartes_Ouest;
    quint8	m_jeu_Ouest[16];
    bool	m_f_a_pose_Ouest;
    bool	m_f_a_parle_Ouest;
    quint8	m_prise_ds_jetee_Ouest;
    QList<quint8> m_cartesposees_Ouest;
    QList<quint8> m_flagCP_Ouest;
    int     m_nbpaspose_Ouest;

signals:
    void statusBarModified();

public slots:

};

#endif // CRAMI_H
