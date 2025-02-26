#ifndef CBASEJOUEURS_H
#define CBASEJOUEURS_H

#include <QWidget>

class CValeurCarte;
class CRetourPose;

class CBaseJoueurs
{
public:
    CBaseJoueurs();
    void trierJeu( quint8 * ajeu );
    void supprimerTrouJeu( quint8 * ajeu, const int nbcarte );

    quint8 chercherCartePlusPetiteValeur( quint8 * ajeu, const bool a_pose, int & nbpaspose, int niveaujeu = 0, quint8 *cjetees = 0, quint8 *cposees = 0 );
    quint8 chercherCartePlusGrandeValeur( quint8 * ajeu, const bool a_pose, int niveaujeu = 0, quint8 *cjetees = 0, quint8 *cposees = 0 );

    bool testerSiPeutPoserOrdi( quint8 * ajeu, const bool a_pose, const quint8 cjprise, quint8 * listepose );
    int testerSiPeutPoserSud( quint8 * ajeu, const bool a_pose, const quint8 cjprise, quint8 * listepose );

    CRetourPose faireListesPoses( QList<quint8> & qlpose, QList<quint8> & qlflag, quint8 * acpj, int icpj, quint8 * ajeu, int nbcartes, quint8 * listepose );

    CRetourPose inserer1CListesPoses( QList<quint8> & qlpose, QList<quint8> & qlflag, quint8 * acpj, int icpj, quint8 * ajeu, int nbcartes, quint8 carte );

    void enlever1Carte( quint8 * ajeu, quint8 carte );
    bool aSuite2( quint8 * ajeu, const bool a_pose );
    bool aMeme2( quint8 * ajeu, const bool a_pose );

    void calculerOrdreJeu( const int numordre, const int nombredejoueur, quint8 *ordrejeu );
    int calculerPenaliteCartesJeu( const quint8 *aJeu, const int nbcarte );

private:
    CValeurCarte calculerCouleurIndex1Carte( quint8 carte );
    int calculerTotal4Cartes( quint8 valeurcarte );
    int calculerIndex1Carte( quint8 * ajeu, quint8 carte );
    bool testerSiPeutPoserOrdiMaxCartes( quint8 * ajeu, const bool a_pose, const quint8 cjprise, quint8 * listepose );
    bool testerSiPeutPoserOrdiCartes( quint8 * ajeu, const bool a_pose, const quint8 cjprise, quint8 * listepose );
    int testerSiPeutPoserSudSuitesMemes( quint8 * ajeu, const bool a_pose, const quint8 cjprise, quint8 * listepose );
    int testerSiPeutPoserSudTierceMemes( quint8 * ajeu, const bool a_pose, const quint8 cjprise, quint8 * listepose );
};

#endif // CBASEJOUEURS_H
