#ifndef CVALEURJEU_H
#define CVALEURJEU_H

#include <QWidget>

class CSuite;
class CMeme;
class CValeurCarte;

class CValeurJeu
{
public:
    CValeurJeu();
    CValeurJeu( quint8 * ajeu, const bool a_pose, int niveaujeu = 0 );

    quint8 getCartePlusPetiteValeur( int & nbpaspose, quint8 *cjetees = 0, quint8 *cposees = 0 );
    quint8 getCartePlusGrandeValeur( const bool grande, quint8 *cjetees = 0, quint8 *cposees = 0 );
    bool peutPoserOrdiMaxCartes( const quint8 cjprise, quint8 * listepose );
    bool peutPoserOrdiCartes( const quint8 cjprise, quint8 * listepose );
    int peutPoserSudSuitesMemes( const quint8 cjprise, quint8 * listepose );
    int peutPoserSudTierceMemes( const quint8 cjprise, quint8 * listepose );
    bool aSuite2();
    bool aMeme2();

private:
    int m_niveaujeu;
    quint8 m_cartesjetees[110];
    quint8 m_cartesposees[110];

    int m_nbcartesjeu;
    int m_aNotes[6];
    quint8 m_aCartes[4][28];
    int m_aValeurs[4][28];
    bool m_apose;

    quint8 m_aOrgCartes[4][28];
    int m_aOrgValeurs[4][28];
    int m_aValeursSuites[4][28];
    int m_aValeursMemes[4][28];
    int m_aValeursSuitesInitiales[4][28];
    int m_aValeursMemesInitiales[4][28];

    int m_aValeursSuitesReference[4][28];
    int m_nbcartesSuiteA;
    int m_nbsuiteA;
    bool m_asuite4C;
    bool m_asuite5C;

    bool m_test_123_234;

    void initialiserCartes();
    void initialiserValeurs();

    void ajouterJeu( quint8 * ajeu );
    void ajouter1Carte( quint8 carte );

    QPoint chercherCartePlusPetiteValeur();
    quint8 chercherCartePlusPetiteValeur( bool a_pose, int & nbpaspose );
    quint8 chercherCartePlusGrandeValeur();
    void calculerValeurTierce();
    void calculerValeurMeme();

    void mettreValeurSuite_1_3( int icouleur, int icarte0, int icarte2, quint8 carte0, quint8 carte2, int note );
    void mettreValeurSuite2( int icouleur, int icarte, quint8 carte0, quint8 carte1, int note );
    void mettreValeurSuite_R_AS( int icouleur, int icarte, quint8 carte0, int note );
    void mettreValeurTierce( int icouleur, int icarte, quint8 carte0, quint8 carte1, quint8 carte2, int note );
    void mettreValeurTierce_D_R_AS( int icouleur, int icarte, quint8 carte0, quint8 carte1, int note );

//    void mettreValeurIsolee( int icouleur, int icarte, quint8 carte );

    void mettreValeur4Meme( int index0, int index1, int index2, int index3, quint8 carte, int note );
    void mettreValeur3Meme( int icouleur0, int icouleur1, int icouleur2, int index0, int index1, int index2, quint8 carte, int note );
    void mettreValeur2Meme( int icouleur0, int icouleur1, int index0, int index1, quint8 carte, int note );

    void sauverCartes();
    void restaurerCartes();
    void sauverValeurs();
    void restaurerValeurs();

    void noterSuites();
    void sauverValeursSuites();
    void sauverValeursSuitesInitiales();
    void restaurerValeursSuitesInitiales();

    bool chercherSuitesCarreau( QList<CSuite> & liste );
    bool chercherSuitesCoeur( QList<CSuite> & liste );
    bool chercherSuitesPique( QList<CSuite> & liste );
    bool chercherSuitesTrefle( QList<CSuite> & liste );
    bool chercherSuites( QList<CSuite> & liste, const int couleur );
    bool chercherTierce( QList<CSuite> & liste, const int icouleur, const int icarte );

    void noterMemes();
    void noterMemesOrdi();
    void sauverValeursMemes();
    void sauverValeursMemesInitiales();
    void restaurerValeursMemesInitiales();

    bool chercherMemes( QList<CMeme> & liste );
    bool existeMemeD();
    bool existeMemeR();

    int calculerPointsCarte( int icouleur, int icarte );
    bool estPair( const int entier );
    CValeurCarte calculerCouleurIndex1Carte( quint8 carte );
    void faireListeCartesSuites( quint8 * listecs, QList<CSuite> & liste );
    void faireListeCartesMemes( quint8 * listecm, QList<CMeme> & liste );
    void eliminerDoublons( quint8 * listecs, quint8 * listecm );
    int compterNbCartesSuites( QList<CSuite> & liste );
    int compterNbPointsSuites( QList<CSuite> & liste );
    int compterNbCartesMemes( QList<CMeme> & liste );
    int compterNbPointsMemes( QList<CMeme> & liste );
    void faireListeCartesSM( QList<quint8> & liste, QList<CSuite> & lsuite, QList<CMeme> & lmeme );
    void faireListePose( quint8 * listepose, QList<CSuite> & lsuite, QList<CMeme> & lmeme );
    void calculerNotesTestPose();
    void calculerNotesPasPose();
    void calculerNotesAPose();
    bool a1Suite();
    void copierTableau( quint8 * aorg, quint8 * acopie, int nbitems );
    int compterPresenceCarte( quint8 *acartes, quint8 carte );
    int calculerReductionNote( quint8 carte0, quint8 carte1 );
    void enleverNotes23();
    void enleverNotesDoublons( const int note );

    void sauverValeursSuitesReference();
    int compterNbSuites( QList<CSuite> & liste );
    void eliminerDoublons_123_234();
    void eliminerDoublons_123et234();

    void writeFileLC( const QString &filename, quint8 * LC );
    void writeFileValeurs( const QString &filename );
    void writeFileValeursSuites( const QString &filename );
    void writeFileValeursMemes( const QString &filename );
    void writeFileCartes( const QString &filename );
    void writeFile( const QString &filename, int (*valeurs)[28], int nblignes );
    void writeFile( const QString &filename, quint8 (*cartes)[28], int nblignes );
};

#endif // CVALEURJEU_H
