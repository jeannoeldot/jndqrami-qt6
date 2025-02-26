#include <QFile>
#include <QTextStream>

#include "cvaleurjeu.h"

#include "miseaupoint.h"

#include "constantes.h"

#include "csuite.h"
#include "cmeme.h"
#include "cvaleurcarte.h"


CValeurJeu::CValeurJeu()
{
}

CValeurJeu::CValeurJeu( quint8 * ajeu, const bool a_pose, int niveaujeu )
{
  m_niveaujeu = niveaujeu;
  m_apose = a_pose;

  m_test_123_234 = true;

  initialiserCartes();
  initialiserValeurs();

  ajouterJeu( ajeu );

    /// @todo MIS POUR TEST
#ifdef MISE_AU_POINT
  writeFileValeurs( "/tmp/jndqrami/debug/val_jeu.txt" );
#endif
}

quint8 CValeurJeu::getCartePlusPetiteValeur( int & nbpaspose, quint8 *cjetees, quint8 *cposees )
{
  if( cjetees != 0 && cposees != 0 )
  {
    copierTableau( cjetees, m_cartesjetees, 110 );
    copierTableau( cposees, m_cartesposees, 110 );
  }

  if( false == m_apose )
  {
    calculerNotesPasPose();
  }
  else
  {
    calculerNotesAPose();
  }
  quint8 carte = chercherCartePlusPetiteValeur( m_apose, nbpaspose );
  return carte;
}

quint8 CValeurJeu::getCartePlusGrandeValeur( const bool grande, quint8 *cjetees, quint8 *cposees )
{
  if( cjetees != 0 && cposees != 0 )
  {
    copierTableau( cjetees, m_cartesjetees, 110 );
    copierTableau( cposees, m_cartesposees, 110 );
  }

  if( false == m_apose )
  {
    calculerNotesPasPose();
  }
  else
  {
    calculerNotesAPose();
  }

  quint8 carte = ERREUR_CARTE;

  if( grande == true )
  {
    carte = chercherCartePlusGrandeValeur();
  }
  else
  {
    int rien = 0;
    carte = chercherCartePlusPetiteValeur( m_apose, rien );
  }

  return carte;
}

int CValeurJeu::peutPoserSudTierceMemes( const quint8 cjprise, quint8 * listepose )
{
  int coderetour;
  int nbpointAD;
  int nbcartesAD;
  int nbpointCB;
  int nbcartesCB;
  bool okCjpAD;
  bool okCjpCB;
  bool okAD;
  bool okCB;
  QList<CSuite> suiteOK;
  QList<CMeme> memeOK;
  quint8 listecartesSuites[104];
  quint8 listecartesMemes[104];

  calculerNotesTestPose();

  sauverCartes();

  initialiserValeurs();
  noterSuites();
  sauverValeursSuites();
  sauverValeursSuitesInitiales();

  initialiserValeurs();
  noterMemes();
  sauverValeursMemes();
  sauverValeursMemesInitiales();

  for( int icouleur = 0; icouleur < 4; icouleur++ )
  {
    for( int icarte = 0; icarte < 24; icarte += 2 )
    {
      QList<CSuite> suiteA;
      QList<CMeme> memeB;
      QList<CSuite> suiteC;
      QList<CMeme> memeD;

      restaurerCartes();
      restaurerValeursSuitesInitiales();
      bool a_tierceA = chercherTierce( suiteA, icouleur, icarte );
      if( true == a_tierceA )
      {
        restaurerValeursMemesInitiales();
        chercherMemes( memeB );
        faireListeCartesSuites( listecartesSuites, suiteA );
        faireListeCartesMemes( listecartesMemes, memeB );
        eliminerDoublons( listecartesSuites, listecartesMemes );

        initialiserValeurs();
        noterSuites();
        sauverValeursSuites();
        chercherTierce( suiteC, icouleur, icarte );

        initialiserValeurs();
        noterMemes();
        sauverValeursMemes();
        chercherMemes( memeD );

/// Compte nb Cartes et nb Points de A-D et C-B
        nbpointAD = 0;
        nbcartesAD = 0;
        nbcartesAD = nbcartesAD + compterNbCartesSuites( suiteA );
        nbpointAD = nbpointAD + compterNbPointsSuites( suiteA );
        nbcartesAD = nbcartesAD + compterNbCartesMemes( memeD );
        nbpointAD = nbpointAD + compterNbPointsMemes( memeD );

        nbpointCB = 0;
        nbcartesCB = 0;
        nbcartesCB = nbcartesCB + compterNbCartesSuites( suiteC );
        nbpointCB = nbpointCB + compterNbPointsSuites( suiteC );
        nbcartesCB = nbcartesCB + compterNbCartesMemes( memeB );
        nbpointCB = nbpointCB + compterNbPointsMemes( memeB );

/// Verifie si Carte Jetee Prise dans liste si cprise != PASCARTE
        restaurerCartes();
        okCjpAD = false;
        okCjpCB = false;
        if( PASCARTE != cjprise )
        {
          QList<quint8> listecartesAD;
          QList<quint8> listecartesCB;
          faireListeCartesSM( listecartesAD, suiteA, memeD );
          faireListeCartesSM( listecartesCB, suiteC, memeB );
          okCjpAD = listecartesAD.contains( cjprise );
          okCjpCB = listecartesAD.contains( cjprise );
        }

/// Positionne Bools
        okAD = false;
        okCB = false;
        bool okSuite = false;
        bool ok51 = false;
        bool okConforme = false;
        bool okCjp = false;

        if( false == m_apose )    /// si n'a pas encore posé
        {
          if( false == suiteA.isEmpty() )   /// 1 Tierce OK
          {
            okSuite = true;
            if( nbpointAD >= 51 )           /// points OK
            {
              ok51 = true;
              if( cjprise != PASCARTE )
              {
                if ( true == okCjpAD )
                {
                  okCjp = true;           /// Jetee Prise OK
                  if( nbcartesAD == m_nbcartesjeu )
                  {
                    okConforme = true;    /// Conforme OK
                    okAD = true;
                  }
                }
              }
              else
              {
                if( nbcartesAD == m_nbcartesjeu )
                {
                  okConforme = true;
                  okAD = true;
                }
              }
            }
          }
          if( false == suiteC.isEmpty() )   /// 1 Tierce OK
          {
            okSuite = true;
            if( nbpointCB >= 51 )           /// points OK
            {
              ok51 = true;
              if( cjprise != PASCARTE )
              {
                if( true == okCjpCB )
                {
                  okCjp = true;           /// Jetee Prise OK
                  if( nbcartesCB == m_nbcartesjeu )
                  {
                    okConforme = true;    /// Conforme OK
                    okCB = true;
                  }
                }
              }
              else
              {
                if( nbcartesCB == m_nbcartesjeu )
                {
                  okConforme = true;
                  okCB = true;
                }
              }
            }
          }
        }
        else    /// à déjà posé
        {
/// Test CONFORME
          if( false == suiteA.isEmpty() || false == memeD.isEmpty() )
          {
            if( cjprise != PASCARTE )
            {
              if ( true == okCjpAD )
              {
                okCjp = true;
                if( nbcartesAD == m_nbcartesjeu )
                {
                  okConforme = true;
                  okAD = true;
                }
              }
            }
            else
            {
              if( nbcartesAD == m_nbcartesjeu )
              {
                okConforme = true;
                okAD = true;
              }
            }
          }
          if( false == suiteC.isEmpty() || false == memeB.isEmpty()  )
          {
            if( cjprise != PASCARTE )
            {
              if( true == okCjpCB )
              {
                okCjp = true;
                if( nbcartesCB == m_nbcartesjeu )
                {
                  okConforme = true;
                  okCB = true;
                }
              }
            }
            else
            {
              if( nbcartesCB == m_nbcartesjeu )
              {
                okConforme = true;
                okCB = true;
              }
            }
          }
        }
/// Fait comparaisons
        if( okAD == true || okCB == true )
        {
          if( okAD == true && okCB == false )
          {
            suiteOK = suiteA;
            memeOK = memeD;
          }
          if( okAD == false && okCB == true )
          {
            suiteOK = suiteC;
            memeOK = memeB;
          }
          if( okAD == true && okCB == true )
          {
            bool isDone = false;
            if( isDone == false && nbcartesAD >= nbcartesCB ) /// a posé ou pas
            {
              suiteOK = suiteA;
              memeOK = memeD;
              isDone = true;
            }
            if( isDone == false && nbcartesCB > nbcartesAD )
            {
              suiteOK = suiteC;
              memeOK = memeB;
              isDone = true;
            }
          }
          faireListePose( listepose, suiteOK, memeOK );
          return RETOUR_OK;
        }
        else
        {
          if( false == m_apose )    /// si n'a pas encore posé
          {
            coderetour = ERREUR_INCONNUE;
            if( okSuite == false )
              coderetour = PAS_TIERCE;
            if( ok51 == false )
              coderetour = PAS_51;
            if( cjprise != PASCARTE && false == okCjp )
              coderetour = PAS_CJP;
            if( okConforme == false )
              coderetour = PAS_CONFORME;
          }
          else    /// à déjà posé
          {
            coderetour = ERREUR_INCONNUE;
            if( cjprise != PASCARTE && false == okCjp )
              coderetour = PAS_CJP;
            if( okConforme == false )
              coderetour = PAS_CONFORME;
          }
        }
      }
      else
      {
        coderetour = PAS_TIERCE;
      }
    }
  }
  return coderetour;
}

int CValeurJeu::peutPoserSudSuitesMemes( const quint8 cjprise, quint8 * listepose )
{
  int nbpointAD;
  int nbcartesAD;
  int nbpointCB;
  int nbcartesCB;
  bool okCjpAD;
  bool okCjpCB;
  bool okAD;
  bool okCB;
  QList<CSuite> suiteOK;
  QList<CMeme> memeOK;
  quint8 listecartesSuites[104];
  quint8 listecartesMemes[104];

  calculerNotesTestPose();

  sauverCartes();

  initialiserValeurs();
  noterSuites();
  sauverValeursSuites();

  QList<CSuite> suiteA;
  QList<CMeme> memeB;
  QList<CSuite> suiteC;
  QList<CMeme> memeD;

  chercherSuitesCoeur( suiteA );
  chercherSuitesPique( suiteA );
  chercherSuitesCarreau( suiteA );
  chercherSuitesTrefle( suiteA );

  initialiserValeurs();
  noterMemes();
  sauverValeursMemes();

  chercherMemes( memeB );

/// Fabrique Liste Cartes Suites
  faireListeCartesSuites( listecartesSuites, suiteA );

/// Fabrique Liste Cartes Memes
  faireListeCartesMemes( listecartesMemes, memeB );

/// Cherche et élimine Doublons dans Suites-Memes
  eliminerDoublons( listecartesSuites, listecartesMemes );

  initialiserValeurs();
  noterSuites();
  sauverValeursSuites();
  chercherSuitesCoeur( suiteC );
  chercherSuitesPique( suiteC );
  chercherSuitesCarreau( suiteC );
  chercherSuitesTrefle( suiteC );

  initialiserValeurs();
  noterMemes();
  sauverValeursMemes();
  chercherMemes( memeD );

/// Compte nb Cartes et nb Points de A-D et C-B
  nbpointAD = 0;
  nbcartesAD = 0;
  nbcartesAD = nbcartesAD + compterNbCartesSuites( suiteA );
  nbpointAD = nbpointAD + compterNbPointsSuites( suiteA );
  nbcartesAD = nbcartesAD + compterNbCartesMemes( memeD );
  nbpointAD = nbpointAD + compterNbPointsMemes( memeD );

  nbpointCB = 0;
  nbcartesCB = 0;
  nbcartesCB = nbcartesCB + compterNbCartesSuites( suiteC );
  nbpointCB = nbpointCB + compterNbPointsSuites( suiteC );
  nbcartesCB = nbcartesCB + compterNbCartesMemes( memeB );
  nbpointCB = nbpointCB + compterNbPointsMemes( memeB );

/// Verifie si Carte Jetee Prise dans liste si cprise != PASCARTE
  restaurerCartes();
  okCjpAD = false;
  okCjpCB = false;
  if( PASCARTE != cjprise )
  {
    QList<quint8> listecartesAD;
    QList<quint8> listecartesCB;
    faireListeCartesSM( listecartesAD, suiteA, memeD );
    faireListeCartesSM( listecartesCB, suiteC, memeB );
    okCjpAD = listecartesAD.contains( cjprise );
    okCjpCB = listecartesAD.contains( cjprise );
  }

/// Positionne Bools
  okAD = false;
  okCB = false;
  bool okSuite = false;
  bool ok51 = false;
  bool okConforme = false;
  bool okCjp = false;

  if( false == m_apose )    /// si n'a pas encore posé
  {
    if( false == suiteA.isEmpty() )   /// 1 Tierce OK
    {
      okSuite = true;
      if( nbpointAD >= 51 )           /// points OK
      {
        ok51 = true;
        if( cjprise != PASCARTE )
        {
          if ( true == okCjpAD )
          {
            okCjp = true;           /// Jetee Prise OK
            if( nbcartesAD == m_nbcartesjeu )
            {
              okConforme = true;    /// Conforme OK
              okAD = true;
            }
          }
        }
        else
        {
          if( nbcartesAD == m_nbcartesjeu )
          {
            okConforme = true;
            okAD = true;
          }
        }
      }
    }
    if( false == suiteC.isEmpty() )   /// 1 Tierce OK
    {
      okSuite = true;
      if( nbpointCB >= 51 )           /// points OK
      {
        ok51 = true;
        if( cjprise != PASCARTE )
        {
          if( true == okCjpCB )
          {
            okCjp = true;           /// Jetee Prise OK
            if( nbcartesCB == m_nbcartesjeu )
            {
              okConforme = true;    /// Conforme OK
              okCB = true;
            }
          }
        }
        else
        {
          if( nbcartesCB == m_nbcartesjeu )
          {
            okConforme = true;
            okCB = true;
          }
        }
      }
    }
  }
  else    /// à déjà posé
  {
/// Test CONFORME
    if( false == suiteA.isEmpty() || false == memeD.isEmpty() )
    {
//      okConforme = true;
      if( cjprise != PASCARTE )
      {
        if ( true == okCjpAD )
        {
          okCjp = true;
          if( nbcartesAD == m_nbcartesjeu )
          {
            okConforme = true;
            okAD = true;
          }
        }
      }
      else
      {
        if( nbcartesAD == m_nbcartesjeu )
        {
          okConforme = true;
          okAD = true;
        }
      }
    }
    if( false == suiteC.isEmpty() || false == memeB.isEmpty()  )
    {
//      okConforme = true;
      if( cjprise != PASCARTE )
      {
        if( true == okCjpCB )
        {
          okCjp = true;
          if( nbcartesCB == m_nbcartesjeu )
          {
            okConforme = true;
            okCB = true;
          }
        }
      }
      else
      {
        if( nbcartesCB == m_nbcartesjeu )
        {
          okConforme = true;
          okCB = true;
        }
      }
    }
  }

/// Fait comparaisons
  if( okAD == true || okCB == true )
  {
    if( okAD == true && okCB == false )
    {
      suiteOK = suiteA;
      memeOK = memeD;
    }
    if( okAD == false && okCB == true )
    {
      suiteOK = suiteC;
      memeOK = memeB;
    }
    if( okAD == true && okCB == true )
    {
      bool isDone = false;
      if( isDone == false && nbcartesAD >= nbcartesCB ) /// a posé ou pas
      {
        suiteOK = suiteA;
        memeOK = memeD;
        isDone = true;
      }
      if( isDone == false && nbcartesCB > nbcartesAD )
      {
        suiteOK = suiteC;
        memeOK = memeB;
        isDone = true;
      }
    }

/// Fait liste pose
    faireListePose( listepose, suiteOK, memeOK );

    return RETOUR_OK;
  }
  else
  {
    if( false == m_apose )    /// si n'a pas encore posé
    {
      if( okSuite == false )
        return PAS_TIERCE;
      if( ok51 == false )
        return PAS_51;
      if( cjprise != PASCARTE && false == okCjp )
        return PAS_CJP;
      if( okConforme == false )
        return PAS_CONFORME;
      return ERREUR_INCONNUE;
    }
    else    /// à déjà posé
    {
      if( cjprise != PASCARTE && false == okCjp )
        return PAS_CJP;
      if( okConforme == false )
        return PAS_CONFORME;
      return ERREUR_INCONNUE;
    }
  }
//  return coderetour;
}

bool CValeurJeu::peutPoserOrdiCartes( const quint8 cjprise, quint8 * listepose )
{
  int nbpointAD;
  int nbcartesAD;
  int nbpointCB;
  int nbcartesCB;
  bool okCjpAD;
  bool okCjpCB;
  bool okAD;
  bool okCB;
  QList<CSuite> suiteOK;
  QList<CMeme> memeOK;
  quint8 listecartesSuites[104];
  quint8 listecartesMemes[104];

  calculerNotesTestPose();

/// Enleve + petite carte ds Jeu : ne peut poser que nbcartes - 1
  m_test_123_234 = false;
  QPoint p = chercherCartePlusPetiteValeur();
  m_aCartes[ p.x() ][ p.y() ] = 0;

  sauverCartes();

  initialiserValeurs();
  noterSuites();
  sauverValeursSuites();
  sauverValeursSuitesInitiales();

  initialiserValeurs();
  noterMemes();
  sauverValeursMemes();
  sauverValeursMemesInitiales();

  for( int icouleur = 0; icouleur < 4; icouleur++ )
  {
    for( int icarte = 0; icarte < 24; icarte += 2 )
    {
      QList<CSuite> suiteA;
      QList<CMeme> memeB;
      QList<CSuite> suiteC;
      QList<CMeme> memeD;

      restaurerCartes();
      restaurerValeursSuitesInitiales();
      bool a_tierceA = chercherTierce( suiteA, icouleur, icarte );
      if( true == a_tierceA )
      {
        restaurerValeursMemesInitiales();
        chercherMemes( memeB );
        faireListeCartesSuites( listecartesSuites, suiteA );
        faireListeCartesMemes( listecartesMemes, memeB );
        eliminerDoublons( listecartesSuites, listecartesMemes );

        initialiserValeurs();
        noterSuites();
        sauverValeursSuites();
        chercherTierce( suiteC, icouleur, icarte );

        initialiserValeurs();
        noterMemes();
        sauverValeursMemes();
        chercherMemes( memeD );

/// Compte nb Cartes et nb Points de A-D et C-B
        nbpointAD = 0;
        nbcartesAD = 0;
        nbcartesAD = nbcartesAD + compterNbCartesSuites( suiteA );
        nbpointAD = nbpointAD + compterNbPointsSuites( suiteA );
        nbcartesAD = nbcartesAD + compterNbCartesMemes( memeD );
        nbpointAD = nbpointAD + compterNbPointsMemes( memeD );

        nbpointCB = 0;
        nbcartesCB = 0;
        nbcartesCB = nbcartesCB + compterNbCartesSuites( suiteC );
        nbpointCB = nbpointCB + compterNbPointsSuites( suiteC );
        nbcartesCB = nbcartesCB + compterNbCartesMemes( memeB );
        nbpointCB = nbpointCB + compterNbPointsMemes( memeB );

/// Verifie si Carte Jetee Prise dans liste si cprise != PASCARTE
        restaurerCartes();
        okCjpAD = false;
        okCjpCB = false;
        if( PASCARTE != cjprise )
        {
          QList<quint8> listecartesAD;
          QList<quint8> listecartesCB;
          faireListeCartesSM( listecartesAD, suiteA, memeD );
          faireListeCartesSM( listecartesCB, suiteC, memeB );
          okCjpAD = listecartesAD.contains( cjprise );
          okCjpCB = listecartesAD.contains( cjprise );
        }

/// Positionne Bools
        okAD = false;
        okCB = false;
        if( false == m_apose )    /// si n'a pas encore posé
        {
          if( false == suiteA.isEmpty() )   /// 1 Tierce OK
          {
            if( nbpointAD >= 51 )           /// points OK
            {
              if( cjprise != PASCARTE )
              {
                if ( true == okCjpAD )  okAD = true;  /// Jetee Prise OK
              }
              else
              {
                okAD = true;
              }
            }
          }
          if( false == suiteC.isEmpty() )   /// 1 Tierce OK
          {
            if( nbpointCB >= 51 )           /// points OK
            {
              if( cjprise != PASCARTE )
              {
                if( true == okCjpCB )  okCB = true;  /// Jetee Prise OK
              }
              else
              {
                okCB = true;
              }
            }
          }
        }
        else    /// à déjà posé
        {
      /// Test CONFORME
          if( false == suiteA.isEmpty() || false == memeD.isEmpty() )
          {
            if( cjprise != PASCARTE )
            {
              if ( true == okCjpAD )  okAD = true;  /// Jetee Prise OK
            }
            else
            {
              okAD = true;
            }
          }
          if( false == suiteC.isEmpty() || false == memeB.isEmpty()  )
          {
            if( cjprise != PASCARTE )
            {
              if( true == okCjpCB )  okCB = true;  /// Jetee Prise OK
            }
            else
            {
              okCB = true;
            }
          }
        }
/// Fait comparaisons
        if( okAD == true || okCB == true )
        {
          if( okAD == true && okCB == false )
          {
            suiteOK = suiteA;
            memeOK = memeD;
          }
          if( okAD == false && okCB == true )
          {
            suiteOK = suiteC;
            memeOK = memeB;
          }
          if( okAD == true && okCB == true )
          {
            bool isDone = false;
            if( false == m_apose && isDone == false && nbcartesAD == 13 )  /// si n'a pas encore posé
            {
              suiteOK = suiteA;
              memeOK = memeD;
              isDone = true;
            }
            if( false == m_apose && isDone == false && nbcartesCB == 13 )
            {
              suiteOK = suiteC;
              memeOK = memeB;
              isDone = true;
            }
            if( false == m_apose && isDone == false && nbcartesAD == 10 )
            {
              suiteOK = suiteA;
              memeOK = memeD;
              isDone = true;
            }
            if( false == m_apose && isDone == false && nbcartesCB == 10 )
            {
              suiteOK = suiteC;
              memeOK = memeB;
              isDone = true;
            }
            if( isDone == false && nbcartesAD >= nbcartesCB ) /// a posé ou pas
            {
              suiteOK = suiteA;
              memeOK = memeD;
              isDone = true;
            }
            if( isDone == false && nbcartesCB > nbcartesAD )
            {
              suiteOK = suiteC;
              memeOK = memeB;
              isDone = true;
            }
          }
          faireListePose( listepose, suiteOK, memeOK );
          return true;
        }
      }
    }
  }

  return false;
}

bool CValeurJeu::peutPoserOrdiMaxCartes( const quint8 cjprise, quint8 * listepose )
{
  int nbpointAD;
  int nbcartesAD;
  int nbpointCB;
  int nbcartesCB;
  bool okCjpAD;
  bool okCjpCB;
  bool okAD;
  bool okCB;
  QList<CSuite> suiteOK;
  QList<CMeme> memeOK;
  quint8 listecartesSuites[104];
  quint8 listecartesMemes[104];

  calculerNotesTestPose();

/// Enleve + petite carte ds Jeu : ne peut poser que nbcartes - 1
  m_test_123_234 = false;
  QPoint p = chercherCartePlusPetiteValeur();
  m_aCartes[ p.x() ][ p.y() ] = 0;

  sauverCartes();
  initialiserValeurs();
  noterSuites();
  sauverValeursSuites();

  sauverValeursSuitesReference();

  QList<CSuite> suiteA;
  QList<CMeme> memeB;
  QList<CSuite> suiteC;
  QList<CMeme> memeD;

  chercherSuitesCoeur( suiteA );
  chercherSuitesPique( suiteA );
  chercherSuitesCarreau( suiteA );
  chercherSuitesTrefle( suiteA );

  m_nbcartesSuiteA = compterNbCartesSuites( suiteA );
//  m_nbsuiteA = compterNbSuites( suiteA );
  m_asuite4C = false;
  m_asuite5C = false;
//  if( m_nbsuiteA == 1 && m_nbcartesSuiteA == 4 )  m_asuite4C = true;
//  if( m_nbsuiteA == 1 && m_nbcartesSuiteA == 5 )  m_asuite5C = true;
  if( m_nbcartesSuiteA == 4 )  m_asuite4C = true;
  if( m_nbcartesSuiteA == 5 )  m_asuite5C = true;

  initialiserValeurs();
  noterMemesOrdi();
  sauverValeursMemes();
  chercherMemes( memeB );

/// Fabrique Liste Cartes Suites
  faireListeCartesSuites( listecartesSuites, suiteA );

/// Fabrique Liste Cartes Memes
  faireListeCartesMemes( listecartesMemes, memeB );

/// Cherche et élimine Doublons dans Suites-Memes
  eliminerDoublons( listecartesSuites, listecartesMemes );

  initialiserValeurs();
  noterSuites();
  sauverValeursSuites();
  chercherSuitesCoeur( suiteC );
  chercherSuitesPique( suiteC );
  chercherSuitesCarreau( suiteC );
  chercherSuitesTrefle( suiteC );

  initialiserValeurs();
  noterMemesOrdi();
  sauverValeursMemes();
  chercherMemes( memeD );

/// Compte nb Cartes et nb Points de A-D et C-B
  nbpointAD = 0;
  nbcartesAD = 0;
  nbcartesAD = nbcartesAD + compterNbCartesSuites( suiteA );
  nbpointAD = nbpointAD + compterNbPointsSuites( suiteA );
  nbcartesAD = nbcartesAD + compterNbCartesMemes( memeD );
  nbpointAD = nbpointAD + compterNbPointsMemes( memeD );

  nbpointCB = 0;
  nbcartesCB = 0;
  nbcartesCB = nbcartesCB + compterNbCartesSuites( suiteC );
  nbpointCB = nbpointCB + compterNbPointsSuites( suiteC );
  nbcartesCB = nbcartesCB + compterNbCartesMemes( memeB );
  nbpointCB = nbpointCB + compterNbPointsMemes( memeB );

/// Verifie si Carte Jetee Prise dans liste si cprise != PASCARTE
  restaurerCartes();
  okCjpAD = false;
  okCjpCB = false;
  if( PASCARTE != cjprise )
  {
    QList<quint8> listecartesAD;
    QList<quint8> listecartesCB;
    faireListeCartesSM( listecartesAD, suiteA, memeD );
    faireListeCartesSM( listecartesCB, suiteC, memeB );
    okCjpAD = listecartesAD.contains( cjprise );
    okCjpCB = listecartesAD.contains( cjprise );
  }

/// Positionne Bools
  okAD = false;
  okCB = false;
  if( false == m_apose )    /// si n'a pas encore posé
  {
    if( false == suiteA.isEmpty() )   /// 1 Tierce OK
    {
      if( nbpointAD >= 51 )           /// points OK
      {
        if( cjprise != PASCARTE )
        {
          if ( true == okCjpAD )  okAD = true;  /// Jetee Prise OK
        }
        else
        {
          okAD = true;
        }
      }
    }
    if( false == suiteC.isEmpty() )   /// 1 Tierce OK
    {
      if( nbpointCB >= 51 )           /// points OK
      {
        if( cjprise != PASCARTE )
        {
          if( true == okCjpCB )  okCB = true;  /// Jetee Prise OK
        }
        else
        {
          okCB = true;
        }
      }
    }
  }
  else    /// à déjà posé
  {
/// Test CONFORME
    if( false == suiteA.isEmpty() || false == memeD.isEmpty() )
    {
      if( cjprise != PASCARTE )
      {
        if ( true == okCjpAD )  okAD = true;  /// Jetee Prise OK
      }
      else
      {
        okAD = true;
      }
    }
    if( false == suiteC.isEmpty() || false == memeB.isEmpty()  )
    {
      if( cjprise != PASCARTE )
      {
        if( true == okCjpCB )  okCB = true;  /// Jetee Prise OK
      }
      else
      {
        okCB = true;
      }
    }
  }

/// Fait comparaisons
  if( okAD == true || okCB == true )
  {
    if( okAD == true && okCB == false )
    {
      suiteOK = suiteA;
      memeOK = memeD;
    }
    if( okAD == false && okCB == true )
    {
      suiteOK = suiteC;
      memeOK = memeB;
    }
    if( okAD == true && okCB == true )
    {
      bool isDone = false;
      if( false == m_apose && isDone == false && nbcartesAD == 13 )  /// si n'a pas encore posé
      {
        suiteOK = suiteA;
        memeOK = memeD;
        isDone = true;
      }
      if( false == m_apose && isDone == false && nbcartesCB == 13 )
      {
        suiteOK = suiteC;
        memeOK = memeB;
        isDone = true;
      }
      if( false == m_apose && isDone == false && nbcartesAD == 10 )
      {
        suiteOK = suiteA;
        memeOK = memeD;
        isDone = true;
      }
      if( false == m_apose && isDone == false && nbcartesCB == 10 )
      {
        suiteOK = suiteC;
        memeOK = memeB;
        isDone = true;
      }
      if( isDone == false && nbcartesAD >= nbcartesCB ) /// a posé ou pas
      {
        suiteOK = suiteA;
        memeOK = memeD;
        isDone = true;
      }
      if( isDone == false && nbcartesCB > nbcartesAD )
      {
        suiteOK = suiteC;
        memeOK = memeB;
        isDone = true;
      }
    }

/// Fait liste pose
    faireListePose( listepose, suiteOK, memeOK );

    return true;
  }

  return false;
}

QPoint CValeurJeu::chercherCartePlusPetiteValeur()
{
  QPoint p;
  calculerValeurTierce();
  calculerValeurMeme();
  int icouleur;
  int icarte;
  int petitevaleur = 10000;
  for( int i = 0; i < 4; i++ )
  {
    for( int j = 0; j < 26; j++ )
    {
      int val = m_aValeurs[ i ][ j ];
      if( val != 0 && val < petitevaleur )
      {
        petitevaleur = val;
        icouleur = i;
        icarte = j;
      }
    }
  }
  p.setX( icouleur );
  p.setY( icarte );

  return p;
}

quint8 CValeurJeu::chercherCartePlusPetiteValeur( bool a_pose, int & nbpaspose )
{
  calculerValeurTierce();

    /// @todo MIS POUR TEST
#ifdef MISE_AU_POINT
  writeFileValeurs( "/tmp/jndqrami/debug/val_tierce.txt" );
#endif

  calculerValeurMeme();

    /// @todo MIS POUR TEST
#ifdef MISE_AU_POINT
  writeFileValeurs( "/tmp/jndqrami/debug/val_T-M.txt" );
#endif

  quint8 carte = ERREUR_CARTE;
  int icouleur;
  int icarte;
  int petitevaleur;
  if( false == a_pose )
  {

/// Préserve AS 1 => 14
    for( int i = 0; i < 4; i++ )
    {
      if( m_aValeurs[ i ][ 0 ] == 1 )
      {
        m_aValeurs[ i ][ 0 ] = 14;
      }
    }

    nbpaspose++;
    if( m_niveaujeu == NIVEAU_FAIBLE && nbpaspose >= MAX_PASPOSE_FAIBLE )
      enleverNotes23();
    if( m_niveaujeu == NIVEAU_FAIBLE && nbpaspose >= MAX_PASPOSE_FAIBLE + 2 )
      enleverNotesDoublons( PASPOSE_MEME3 );
    if( m_niveaujeu == NIVEAU_MOYEN && nbpaspose >= MAX_PASPOSE_MOYEN )
      enleverNotes23();
    if( m_niveaujeu == NIVEAU_MOYEN && nbpaspose >= MAX_PASPOSE_MOYEN + 2 )
      enleverNotesDoublons( PASPOSE_MEME3 );
    if( m_niveaujeu == NIVEAU_FORT && nbpaspose >= MAX_PASPOSE_FORT )
      enleverNotes23();
    if( m_niveaujeu == NIVEAU_FORT && nbpaspose >= MAX_PASPOSE_FORT + 2 )
      enleverNotesDoublons( PASPOSE_MEME3 );

    petitevaleur = 10000;
    for( int i = 0; i < 4; i++ )
    {
      for( int j = 0; j < 26; j++ )
      {
        int val = m_aValeurs[ i ][ j ];
        if( val != 0 && val < petitevaleur )
        {
          petitevaleur = val;
          icouleur = i;
          icarte = j;
        }
      }
    }
  }
  else	/// A Posé
  {
    for( int i = 0; i < 4; i++ )
    {
      if( m_aValeurs[ i ][ 0 ] == 1 )
      {
        m_aValeurs[ i ][ 0 ] = 14;   /// AS 1 => 14
      }
    }

    enleverNotesDoublons( APOSE_MEME3 );

    petitevaleur = 0;
    for( int i = 0; i < 4; i++ )
    {
      for( int j = 0; j < 26; j++ )
      {
        int val = m_aValeurs[ i ][ j ];
        if( val != 0 && val > petitevaleur && val <= 14 )  /// Carte Isolée
        {
          petitevaleur = val;
          icouleur = i;
          icarte = j;
        }
      }
    }
    if( petitevaleur == 0 )   /// si non trouvé => + petite valeur
    {
      petitevaleur = 10000;
      for( int i = 0; i < 4; i++ )
      {
        for( int j = 0; j < 26; j++ )
        {
          int val = m_aValeurs[ i ][ j ];
          if( val != 0 && val < petitevaleur )
          {
            petitevaleur = val;
            icouleur = i;
            icarte = j;
          }
        }
      }
    }
  }

  carte = m_aCartes[ icouleur ][ icarte ];
  carte = carte + ( icouleur * 20 );

  return carte;
}

void CValeurJeu::enleverNotesDoublons( const int note )
{
  int val;
  int valcarte;
  for( int icouleur = 0; icouleur < 4; icouleur++ )
  {
    valcarte = 1;
    for( int icarte = 0; icarte < 26; icarte += 2 )
    {
      val = m_aValeurs[ icouleur ][ icarte + 1 ];
      if( val != 0 && val < note )
      {
        m_aValeurs[ icouleur ][ icarte + 1 ] = valcarte;
      }
      valcarte += 1;
    }
  }
}

void CValeurJeu::enleverNotes23()
{
  int val;
  for( int icouleur = 0; icouleur < 4; icouleur++ )
  {
/// 2
    val = m_aValeurs[ icouleur ][ 2 ];
    if( val != 0 )  m_aValeurs[ icouleur ][ 2 ] = 2;

    val = m_aValeurs[ icouleur ][ 3 ];
    if( val != 0 )  m_aValeurs[ icouleur ][ 3 ] = 2;

/// 3
    val = m_aValeurs[ icouleur ][ 4 ];
    if( val != 0 )  m_aValeurs[ icouleur ][ 4 ] = 3;

    val = m_aValeurs[ icouleur ][ 5 ];
    if( val != 0 )  m_aValeurs[ icouleur ][ 5 ] = 3;
  }
}

quint8 CValeurJeu::chercherCartePlusGrandeValeur()
{
  m_aNotes[ 0 ] = 0;
  m_aNotes[ 1 ] = 0;
  m_aNotes[ 2 ] = 0;
  m_aNotes[ 3 ] = 0;
  m_aNotes[ 4 ] = 0;
  m_aNotes[ 5 ] = 0;

  calculerValeurTierce();
  calculerValeurMeme();

  quint8 carte = ERREUR_CARTE;
  int icouleur;
  int icarte;
  int petitevaleur;

  for( int i = 0; i < 4; i++ )
  {
    if( m_aValeurs[ i ][ 0 ] == 1 )
    {
      m_aValeurs[ i ][ 0 ] = 14;   /// AS 1 => 14
    }
  }
  petitevaleur = 0;
  for( int i = 0; i < 4; i++ )
  {
    for( int j = 0; j < 26; j++ )
    {
      int val = m_aValeurs[ i ][ j ];
      if( val != 0 && val > petitevaleur )
      {
        petitevaleur = val;
        icouleur = i;
        icarte = j;
      }
    }
  }

  carte = m_aCartes[ icouleur ][ icarte ];
  carte = carte + ( icouleur * 20 );

  return carte;
}

void CValeurJeu::ajouter1Carte( quint8 carte )
{
  if( carte != PASCARTE )
  {
    quint8 valcarte;
    int icouleur;
    int icarte;

    CValeurCarte cvcarte = calculerCouleurIndex1Carte( carte );
    icouleur = cvcarte.couleur();
    icarte = cvcarte.index();
    valcarte = cvcarte.valeurCarte();

    if( m_aCartes[ icouleur ][ icarte ] != PASCARTE )
    {
      icarte++;
    }
    m_aCartes[ icouleur ][ icarte ] = valcarte;
    m_aValeurs[ icouleur ][ icarte ] = valcarte;
  }
}

void CValeurJeu::ajouterJeu( quint8 * ajeu )
{
  m_nbcartesjeu = 0;
  for( int i = 0; i < 14; i++ )
  {
    quint8 carte = ajeu[ i ];
    if( carte != PASCARTE )
    {
      ajouter1Carte( carte );
      m_nbcartesjeu++;
    }
  }
}

void CValeurJeu::calculerValeurMeme()
{
  int index0, index1, index2, index3;
  int icouleur = 0;
  for( int icarte = 0; icarte < 26; icarte += 2 )
  {
    quint8 carte0 = PASCARTE;
    quint8 carte1 = PASCARTE;
    quint8 carte2 = PASCARTE;
    quint8 carte3 = PASCARTE;

    index0 = icarte;
    carte0 = m_aCartes[ icouleur + 0 ][ icarte ];
    if( carte0 == m_aCartes[ icouleur + 0 ][ icarte + 1 ] )
    {
      index0 = icarte + 1;
    }
    index1 = icarte;
    carte1 = m_aCartes[ icouleur + 1 ][ icarte ];
    if( carte1 == m_aCartes[ icouleur + 1 ][ icarte + 1 ] )
    {
      index1 = icarte + 1;
    }
    index2 = icarte;
    carte2 = m_aCartes[ icouleur + 2 ][ icarte ];
    if( carte2 == m_aCartes[ icouleur + 2 ][ icarte + 1 ] )
    {
      index2 = icarte + 1;
    }
    index3 = icarte;
    carte3 = m_aCartes[ icouleur + 3 ][ icarte ];
    if( carte3 == m_aCartes[ icouleur + 3 ][ icarte + 1 ] )
    {
      index3 = icarte + 1;
    }

    if( carte0 != PASCARTE && carte0 == carte1 && carte0 == carte2 && carte0 == carte3 )
    {
      mettreValeur4Meme( index0, index1, index2, index3, carte0, m_aNotes[2] );
      continue;
    }

    if( carte0 != PASCARTE && carte0 == carte2 && carte0 == carte3 )
    {
      mettreValeur3Meme( icouleur + 0, icouleur + 2, icouleur + 3, index0, index2, index3, carte0, m_aNotes[1] );
      continue;
    }

    if( carte0 != PASCARTE && carte0 == carte1 && carte0 == carte3 )
    {
      mettreValeur3Meme( icouleur + 0, icouleur + 1, icouleur + 3, index0, index1, index3, carte0, m_aNotes[1] );
      continue;
    }

    if( carte0 != PASCARTE && carte0 == carte1 && carte0 == carte2 )
    {
      mettreValeur3Meme( icouleur + 0, icouleur + 1, icouleur + 2, index0, index1, index2, carte0, m_aNotes[1] );
      continue;
    }

    if( carte1 != PASCARTE && carte1 == carte2 && carte1 == carte3 )
    {
      mettreValeur3Meme( icouleur + 1, icouleur + 2, icouleur + 3, index1, index2, index3, carte1, m_aNotes[1] );
      continue;
    }

    int reduction;
    quint8 carte_0;
    quint8 carte_1;
    quint8 ctest;

    if( carte0 != PASCARTE && carte0 == carte1 )
    {

      reduction = 0;
      if( m_niveaujeu != 0 )
      {
        ctest = m_aCartes[ icouleur + 0 ][ index0 ];
        ctest = ctest + ( icouleur * 20 );
        carte_0 = ctest + 40 + 1;
        carte_1 = ctest + 60 - 1;
        reduction = calculerReductionNote( carte_0, carte_1 );
      }
      m_aNotes[0] = m_aNotes[0] - reduction;
      if( m_aNotes[0] < 0 )  m_aNotes[0] = 0;

      mettreValeur2Meme( icouleur + 0, icouleur + 1, index0, index1, carte0, m_aNotes[0] );
      continue;
    }

    if( carte0 != PASCARTE && carte0 == carte2 )
    {

      reduction = 0;
      if( m_niveaujeu != 0 )
      {
        ctest = m_aCartes[ icouleur + 0 ][ index0 ];
        ctest = ctest + ( icouleur * 20 );
        carte_0 = ctest + 20 + 1;
        carte_1 = ctest + 60 - 1;
        reduction = calculerReductionNote( carte_0, carte_1 );
      }
      m_aNotes[0] = m_aNotes[0] - reduction;
      if( m_aNotes[0] < 0 )  m_aNotes[0] = 0;

      mettreValeur2Meme( icouleur + 0, icouleur + 2, index0, index2, carte0, m_aNotes[0] );
      continue;
    }

    if( carte0 != PASCARTE && carte0 == carte3 )
    {

      reduction = 0;
      if( m_niveaujeu != 0 )
      {
        ctest = m_aCartes[ icouleur + 0 ][ index0 ];
        ctest = ctest + ( icouleur * 20 );
        carte_0 = ctest + 20 + 1;
        carte_1 = ctest + 40 - 1;
        reduction = calculerReductionNote( carte_0, carte_1 );
      }
      m_aNotes[0] = m_aNotes[0] - reduction;
      if( m_aNotes[0] < 0 )  m_aNotes[0] = 0;

      mettreValeur2Meme( icouleur + 0, icouleur + 3, index0, index3, carte0, m_aNotes[0] );
      continue;
    }

    if( carte1 != PASCARTE && carte1 == carte2 )
    {

      reduction = 0;
      if( m_niveaujeu != 0 )
      {
        ctest = m_aCartes[ icouleur + 1 ][ index1 ];
        ctest = ctest + ( icouleur * 20 );
        carte_0 = ctest - 20 + 1;
        carte_1 = ctest + 40 - 1;
        reduction = calculerReductionNote( carte_0, carte_1 );
      }
      m_aNotes[0] = m_aNotes[0] - reduction;
      if( m_aNotes[0] < 0 )  m_aNotes[0] = 0;

      mettreValeur2Meme( icouleur + 1, icouleur + 2, index1, index2, carte1, m_aNotes[0] );
      continue;
    }

    if( carte1 != PASCARTE && carte1 == carte3 )
    {

      reduction = 0;
      if( m_niveaujeu != 0 )
      {
        ctest = m_aCartes[ icouleur + 1 ][ index1 ];
        ctest = ctest + ( icouleur * 20 );
        carte_0 = ctest - 20 + 1;
        carte_1 = ctest + 20 - 1;
        reduction = calculerReductionNote( carte_0, carte_1 );
      }
      m_aNotes[0] = m_aNotes[0] - reduction;
      if( m_aNotes[0] < 0 )  m_aNotes[0] = 0;

      mettreValeur2Meme( icouleur + 1, icouleur + 3, index1, index3, carte1, m_aNotes[0] );
      continue;
    }

    if( carte2 != PASCARTE && carte2 == carte3 )
    {

      reduction = 0;
      if( m_niveaujeu != 0 )
      {
        ctest = m_aCartes[ icouleur + 2 ][ index2 ];
        ctest = ctest + ( icouleur * 20 );
        carte_0 = ctest - 40 + 1;
        carte_1 = ctest - 20 - 1;
        reduction = calculerReductionNote( carte_0, carte_1 );
      }
      m_aNotes[0] = m_aNotes[0] - reduction;
      if( m_aNotes[0] < 0 )  m_aNotes[0] = 0;

      mettreValeur2Meme( icouleur + 2, icouleur + 3, index2, index3, carte2, m_aNotes[0] );
      continue;
    }
  }
}

void CValeurJeu::calculerValeurTierce()
{
  for( int icouleur = 0; icouleur < 4; icouleur++ )
  {
    int icarte = 0;
    while( icarte <= 24 )
    {
      quint8 carte0 = PASCARTE;
      quint8 carte1 = PASCARTE;
      quint8 carte2 = PASCARTE;

      if( icarte == 24 )        /// Test AS : CAS Suite = R AS
      {
        carte0 = m_aCartes[ icouleur ][ icarte ];
        carte1 = m_aCartes[ icouleur ][ 0 ];
        if( carte0 != PASCARTE )
        {
          if( carte1 == 1 )
          {
            mettreValeurSuite_R_AS( icouleur, icarte, carte0, m_aNotes[3] );
          }
        }
      }
      else
      {
        if( icarte == 22 )		/// Test AS : CAS Tierce = D R AS
        {
          carte0 = m_aCartes[ icouleur ][ icarte ];
          carte1 = m_aCartes[ icouleur ][ icarte + 2 ];
          carte2 = m_aCartes[ icouleur ][ 0 ];
          if( carte0 != PASCARTE )
          {
            if( carte1 == carte0 + 1 && carte2 == 1 )
            {
              mettreValeurTierce_D_R_AS( icouleur, icarte, carte0, carte1, m_aNotes[5] );
            }
            else
            {
              if( carte1 == carte0 + 1 )    /// si D R
              {
                mettreValeurSuite2( icouleur, icarte, carte0, carte1, m_aNotes[3] );
              }
              else
              {
                if( carte2 == 1 )   /// si D - AS
                {
                  mettreValeurSuite_1_3( icouleur, icarte, 0, carte0, 14, m_aNotes[4] );
                }
              }
            }
          }
        }
        else
        {
          carte0 = m_aCartes[ icouleur ][ icarte ];
          carte1 = m_aCartes[ icouleur ][ icarte + 2 ];
          carte2 = m_aCartes[ icouleur ][ icarte + 4 ];
          if( carte0 != PASCARTE )
          {
            if( carte1 == carte0 + 1 && carte2 == carte0 + 2 )
            {
                mettreValeurTierce( icouleur, icarte, carte0, carte1, carte2, m_aNotes[5] );
            }
            else
            {
              if( carte1 == carte0 + 1 )
              {
                mettreValeurSuite2( icouleur, icarte, carte0, carte1, m_aNotes[3] );
              }
              else
              {
                if( carte2 == carte0 + 2 )   /// si C - C
                {
                  mettreValeurSuite_1_3( icouleur, icarte, icarte + 4, carte0, carte2, m_aNotes[4] );
                }
              }
            }
          }
        }
      }

      icarte = icarte + 2;
    }
  }

/// élimine doublons 123 et 234
  eliminerDoublons_123_234();

  int valseuil = 2000;
  for( int icouleur = 0; icouleur < 4; icouleur++ )
  {
    for( int icarte = 0; icarte < 26; icarte++ )
    {
      if( icarte >= 2 )
      {
        valseuil = 3000;
      }
      int valeur;
      valeur = m_aValeurs[ icouleur ][ icarte ];
      if( valeur >= valseuil && m_aCartes[ icouleur ][ icarte + 1 ] != PASCARTE )
      {
        valeur = m_aValeurs[ icouleur ][ icarte + 1 ];
        valeur = valeur + 1000;
        m_aValeurs[ icouleur ][ icarte + 1 ] = valeur;
      }
    }
  }
}

void CValeurJeu::mettreValeurSuite2( int icouleur, int icarte, quint8 carte0, quint8 carte1, int note )
{
  int valeur;
  int reduction = 0;

  if( m_niveaujeu != 0 )
  {
    quint8 carte_0 = m_aCartes[ icouleur ][ icarte ];
    carte_0 = carte_0 + ( icouleur * 20 );
    quint8 carte_1 = m_aCartes[ icouleur ][ icarte + 2 ];
    carte_1 = carte_1 + ( icouleur * 20 );
    reduction = calculerReductionNote( carte_0, carte_1 );
  }
  note = note - reduction;
  if( note < 0 )  note = 0;

  valeur = m_aValeurs[ icouleur ][ icarte ];
  valeur = valeur + carte0;
  valeur = valeur + note;
  m_aValeurs[ icouleur ][ icarte ] = valeur;

  valeur = m_aValeurs[ icouleur ][ icarte + 2 ];
  valeur = valeur + carte1;
  valeur = valeur + note;
  m_aValeurs[ icouleur ][ icarte + 2 ] = valeur;
}

void CValeurJeu::mettreValeurSuite_1_3( int icouleur, int icarte0, int icarte2, quint8 carte0, quint8 carte2, int note )
{
  int valeur;
  int reduction = 0;

  if( m_niveaujeu != 0 )
  {
    quint8 carte_0 = m_aCartes[ icouleur ][ icarte0 ];
    carte_0 = carte_0 + ( icouleur * 20 );
    reduction = calculerReductionNote( 1, carte_0 );
  }
  note = note - reduction;
  if( note < 0 )  note = 0;

  valeur = m_aValeurs[ icouleur ][ icarte0 ];
  valeur = valeur + carte0;
  valeur = valeur + note;
  m_aValeurs[ icouleur ][ icarte0 ] = valeur;

  valeur = m_aValeurs[ icouleur ][ icarte2 ];
  valeur = valeur + carte2;
  valeur = valeur + note;
  m_aValeurs[ icouleur ][ icarte2 ] = valeur;
}

void CValeurJeu::mettreValeurTierce( int icouleur, int icarte, quint8 carte0, quint8 carte1, quint8 carte2, int note )
{
  int valeur;

  valeur = m_aValeurs[ icouleur ][ icarte ];
  valeur = valeur + carte0;
  valeur = valeur + note;
  m_aValeurs[ icouleur ][ icarte ] = valeur;

  valeur = m_aValeurs[ icouleur ][ icarte + 2 ];
  valeur = valeur + carte1;
  valeur = valeur + note;
  m_aValeurs[ icouleur ][ icarte + 2 ] = valeur;

  valeur = m_aValeurs[ icouleur ][ icarte + 4 ];
  valeur = valeur + carte2;
  valeur = valeur + note;
  m_aValeurs[ icouleur ][ icarte + 4 ] = valeur;
}

void CValeurJeu::mettreValeurTierce_D_R_AS( int icouleur, int icarte, quint8 carte0, quint8 carte1, int note )
{
  int valeur;

  valeur = m_aValeurs[ icouleur ][ icarte ];
  valeur = valeur + carte0;
  valeur = valeur + note;
  m_aValeurs[ icouleur ][ icarte ] = valeur;

  valeur = m_aValeurs[ icouleur ][ icarte + 2 ];
  valeur = valeur + carte1;
  valeur = valeur + note;
  m_aValeurs[ icouleur ][ icarte + 2 ] = valeur;

  valeur = m_aValeurs[ icouleur ][ 0 ];
  valeur = valeur + 14;
  valeur = valeur + note;
  m_aValeurs[ icouleur ][ 0 ] = valeur;
}

void CValeurJeu::mettreValeurSuite_R_AS( int icouleur, int icarte, quint8 carte0, int note )
{
  int valeur;
  int reduction = 0;

  if( m_niveaujeu != 0 )
  {
    quint8 carte_0 = m_aCartes[ icouleur ][ icarte ];
    carte_0 = carte_0 + ( icouleur * 20 );
    reduction = calculerReductionNote( 1, carte_0 - 2 );
  }
  note = note - reduction;
  if( note < 0 )  note = 0;

  valeur = m_aValeurs[ icouleur ][ icarte ];
  valeur = valeur + carte0;
  valeur = valeur + note;
  m_aValeurs[ icouleur ][ icarte ] = valeur;

  valeur = m_aValeurs[ icouleur ][ 0 ];
  valeur = valeur + 14;
  valeur = valeur + note;
  m_aValeurs[ icouleur ][ 0 ] = valeur;
}

void CValeurJeu::mettreValeur4Meme( int index0, int index1, int index2, int index3, quint8 carte, int note )
{
  int valeur;

  if( index0 == 0 || index0 == 1 ) /// si AS
  {
    carte = 14;
  }

  valeur = m_aValeurs[ 0 ][ index0 ];
  valeur = valeur + carte;
  valeur = valeur + note;
  m_aValeurs[ 0 ][ index0 ] = valeur;

  valeur = m_aValeurs[ 1 ][ index1 ];
  valeur = valeur + carte;
  valeur = valeur + note;
  m_aValeurs[ 1 ][ index1 ] = valeur;

  valeur = m_aValeurs[ 2 ][ index2 ];
  valeur = valeur + carte;
  valeur = valeur + note;
  m_aValeurs[ 2 ][ index2 ] = valeur;

  valeur = m_aValeurs[ 3 ][ index3 ];
  valeur = valeur + carte;
  valeur = valeur + note;
  m_aValeurs[ 3 ][ index3 ] = valeur;
}

void CValeurJeu::mettreValeur3Meme( int icouleur0, int icouleur1, int icouleur2, int index0, int index1, int index2, quint8 carte, int note )
{
  int valeur;

  if( index0 == 0 || index0 == 1 ) /// si AS
  {
    carte = 14;
  }

  valeur = m_aValeurs[ icouleur0 ][ index0 ];
  valeur = valeur + carte;
  valeur = valeur + note;
  m_aValeurs[ icouleur0 ][ index0 ] = valeur;

  valeur = m_aValeurs[ icouleur1 ][ index1 ];
  valeur = valeur + carte;
  valeur = valeur + note;
  m_aValeurs[ icouleur1 ][ index1 ] = valeur;

  valeur = m_aValeurs[ icouleur2 ][ index2 ];
  valeur = valeur + carte;
  valeur = valeur + note;
  m_aValeurs[ icouleur2 ][ index2 ] = valeur;
}

void CValeurJeu::mettreValeur2Meme( int icouleur0, int icouleur1, int index0, int index1, quint8 carte, int note )
{
  int valeur;

  if( index0 == 0 || index0 == 1 ) /// si AS
  {
    carte = 14;
  }

  valeur = m_aValeurs[ icouleur0 ][ index0 ];
  valeur = valeur + carte;
  valeur = valeur + note;
  m_aValeurs[ icouleur0 ][ index0 ] = valeur;

  valeur = m_aValeurs[ icouleur1 ][ index1 ];
  valeur = valeur + carte;
  valeur = valeur + note;
  m_aValeurs[ icouleur1 ][ index1 ] = valeur;
}

void CValeurJeu::initialiserCartes()
{
  for( int i = 0; i < 4; i++ )
  {
    for(int j = 0; j < 26; j++ )
    {
      m_aCartes[ i ][ j ] = PASCARTE;
    }
    m_aCartes[ i ][ 26 ] = MARKFIN;
    m_aCartes[ i ][ 27 ] = MARKFIN;
  }
}

void CValeurJeu::initialiserValeurs()
{
  for( int i = 0; i < 4; i++ )
  {
    for(int j = 0; j < 26; j++ )
    {
      m_aValeurs[ i ][ j ] = 0;
    }
    m_aValeurs[ i ][ 26 ] = 0;
    m_aValeurs[ i ][ 27 ] = 0;
  }
}

void CValeurJeu::sauverCartes()
{
  for( int i = 0; i < 4; i++ )
  {
    for( int j = 0; j < 28; j++ )
    {
      m_aOrgCartes[ i ][ j ] = m_aCartes[ i ][ j ];
    }
  }
}

void CValeurJeu::restaurerCartes()
{
  for( int i = 0; i < 4; i++ )
  {
    for( int j = 0; j < 28; j++ )
    {
      m_aCartes[ i ][ j ] = m_aOrgCartes[ i ][ j ];
    }
  }
}

void CValeurJeu::sauverValeurs()
{
  for( int i = 0; i < 4; i++ )
  {
    for( int j = 0; j < 28; j++ )
    {
      m_aOrgValeurs[ i ][ j ] = m_aValeurs[ i ][ j ];
    }
  }
}

void CValeurJeu::restaurerValeurs()
{
  for( int i = 0; i < 4; i++ )
  {
    for( int j = 0; j < 28; j++ )
    {
      m_aValeurs[ i ][ j ] = m_aOrgValeurs[ i ][ j ];
    }
  }
}

void CValeurJeu::noterSuites()
{
  for( int icouleur = 0; icouleur < 4; icouleur++ )
  {
    int icarte = 0;
    while( icarte <= 24 )
    {
      quint8 carte0 = PASCARTE;
      quint8 carte1 = PASCARTE;
      quint8 carte2 = PASCARTE;

      if( icarte == 22 )		/// Test AS : CAS Tierce = D R AS
      {
        carte0 = m_aCartes[ icouleur ][ icarte ];
        carte1 = m_aCartes[ icouleur ][ icarte + 2 ];
        carte2 = m_aCartes[ icouleur ][ 0 ];
        if( carte0 != PASCARTE )
        {
          if( carte1 == carte0 + 1 && carte2 == 1 )
          {
            mettreValeurTierce_D_R_AS( icouleur, icarte, carte0, carte1, m_aNotes[5] );
          }
        }
      }
      else
      {
        carte0 = m_aCartes[ icouleur ][ icarte ];
        carte1 = m_aCartes[ icouleur ][ icarte + 2 ];
        carte2 = m_aCartes[ icouleur ][ icarte + 4 ];
        if( carte0 != PASCARTE )
        {
          if( carte1 == carte0 + 1 && carte2 == carte0 + 2 )
          {
            mettreValeurTierce( icouleur, icarte, carte0, carte1, carte2, m_aNotes[5] );
          }
        }
      }
      icarte = icarte + 2;
    }
  }

/// @todo MIS POUR TEST
#ifdef MISE_AU_POINT
  writeFileValeurs( "/tmp/jndqrami/debug/valeurs.txt" );
#endif

}

void CValeurJeu::sauverValeursSuites()
{
  for( int i = 0; i < 4; i++ )
  {
    for( int j = 0; j < 28; j++ )
    {
      m_aValeursSuites[ i ][ j ] = m_aValeurs[ i ][ j ];
    }
  }

/// @todo MIS POUR TEST
#ifdef MISE_AU_POINT
  writeFileValeursSuites( "/tmp/jndqrami/debug/valeurs-suites.txt" );
#endif

}

void CValeurJeu::sauverValeursSuitesInitiales()
{
  for( int i = 0; i < 4; i++ )
  {
    for( int j = 0; j < 28; j++ )
    {
      m_aValeursSuitesInitiales[ i ][ j ] = m_aValeursSuites[ i ][ j ];
    }
  }
}

void CValeurJeu::restaurerValeursSuitesInitiales()
{
  for( int i = 0; i < 4; i++ )
  {
    for( int j = 0; j < 28; j++ )
    {
      m_aValeursSuites[ i ][ j ] = m_aValeursSuitesInitiales[ i ][ j ];
    }
  }
}

void CValeurJeu::sauverValeursSuitesReference()
{
  for( int i = 0; i < 4; i++ )
  {
    for( int j = 0; j < 28; j++ )
    {
      m_aValeursSuitesReference[ i ][ j ] = m_aValeurs[ i ][ j ];
    }
  }
}

bool CValeurJeu::chercherSuitesCarreau( QList<CSuite> & liste )
{
  bool a_suite = false;

  a_suite = chercherSuites( liste, CARREAU );

  return a_suite;
}

bool CValeurJeu::chercherSuitesCoeur( QList<CSuite> & liste )
{
  bool a_suite = false;

  a_suite = chercherSuites( liste, COEUR );

  return a_suite;
}

bool CValeurJeu::chercherSuitesPique( QList<CSuite> & liste )
{
  bool a_suite = false;

  a_suite = chercherSuites( liste, PIQUE );

  return a_suite;
}

bool CValeurJeu::chercherSuitesTrefle( QList<CSuite> & liste )
{
  bool a_suite = false;

  a_suite = chercherSuites( liste, TREFLE );

  return a_suite;
}

bool CValeurJeu::chercherSuites( QList<CSuite> & liste, const int couleur )
{
  int nbcartes;
  int nbpoints;
  int index1c;
  bool a_suite;
  int icarte;
  CSuite suite;

/// cas valeur >= 3000
  nbcartes = 0;
  nbpoints = 0;
  index1c = -1;
  a_suite = false;
  icarte = 0;
  bool AS_mis = false;

  while( icarte <= 22 )
  {
    int val = m_aValeursSuites[ couleur ][ icarte ];
    if( val >= 3000 )
    {
      if( icarte == 22 && AS_mis == true )
      {
        while( icarte >= 0 && m_aValeursSuites[ couleur ][ icarte ] >= 1000 )
        {
          icarte -= 2;
        }
        icarte += 2;
        index1c = icarte;
        nbcartes = 0;
        nbpoints = 0;
        while( val >= 1000 && icarte <= 24 )
        {
          nbcartes++;
          nbpoints = nbpoints + calculerPointsCarte( couleur, icarte );
          icarte += 2;
          val = m_aValeursSuites[ couleur ][ icarte ];
        }
        suite.setNombreCartes( nbcartes );
        suite.setNombrePoints( nbpoints );
        suite.setIndex1ereCarte( index1c );
        suite.setCouleur( couleur );
        liste.append( suite );
        a_suite = true;
        continue;
      }

      if( m_aCartes[ couleur ][ icarte + 1 ] != PASCARTE )
      {
        int index = icarte;
        while( index >= 0 && m_aValeursSuites[ couleur ][ index ] >= 1000 )
        {
          index -= 2;
        }
        index += 2;
        if( index == 0 )  AS_mis = true;
        index1c = index;
        nbcartes = 0;
        nbpoints = 0;
        while( val >= 1000 && index <= icarte )
        {
          nbcartes++;
          nbpoints = nbpoints + calculerPointsCarte( couleur, index );
          index += 2;
          val = m_aValeursSuites[ couleur ][ index ];
        }
        suite.setNombreCartes( nbcartes );
        suite.setNombrePoints( nbpoints );
        suite.setIndex1ereCarte( index1c );
        suite.setCouleur( couleur );
        liste.append( suite );
        a_suite = true;

        nbcartes = 1;
        nbpoints = calculerPointsCarte( couleur, icarte );
        index1c = icarte + 1;
        icarte += 2;
        while( val >= 1000 && icarte <= 24 )
        {
          nbcartes++;
          nbpoints = nbpoints + calculerPointsCarte( couleur, icarte );
          icarte += 2;
          if( icarte == 26 && m_aCartes[ couleur ][ 0 ] == 1 && AS_mis == false )    /// si AS
          {
            nbcartes++;
            nbpoints = nbpoints + 10;
          }
          val = m_aValeursSuites[ couleur ][ icarte ];
        }
        suite.setNombreCartes( nbcartes );
        suite.setNombrePoints( nbpoints );
        suite.setIndex1ereCarte( index1c );
        suite.setCouleur( couleur );
        liste.append( suite );
        a_suite = true;
        continue;
      }
    }
    icarte = icarte + 2;
  }
  if( a_suite == true ) return a_suite;

/// cas valeur >= 1000 et <= 2000
  nbcartes = 0;
  nbpoints = 0;
  index1c = -1;
  a_suite = false;
  icarte = 0;

  while( icarte <= 22 )
  {
    int val = m_aValeursSuites[ couleur ][ icarte ];
    if( val >= 1000 )
    {
      if( icarte == 0 )   /// Cas AS
      {
        if( val >= 2000 )   /// cas 1 1 2 3 ...D R ou 1 2 3 ... D R
        {
          if( m_aCartes[ couleur ][ 1 ] == 1 )  /// cas 1 1 2 3 ... D R
          {
            nbcartes = 1;   /// 1
            nbpoints = 1;
            index1c = 1;
            icarte = 2;
            while( val >= 1000 && icarte <= 24 )
            {
              nbcartes++;
              nbpoints = nbpoints + calculerPointsCarte( couleur, icarte );
              icarte += 2;
              val = m_aValeursSuites[ couleur ][ icarte ];
            }
            suite.setNombreCartes( nbcartes );   /// Met suite 1 2 3 ...
            suite.setNombrePoints( nbpoints );
            suite.setIndex1ereCarte( index1c );
            suite.setCouleur( couleur );
            liste.append( suite );
            a_suite = true;
            continue;
          }
          else    /// cas 1 2 3 .... D R
          {
            if( m_aValeursSuites[ couleur ][ 2 ] >= 2000 && m_aValeursSuites[ couleur ][ 4 ] >= 2000 )  /// cas 1 2 3 4 ... D R
            {
              icarte += 2;
              continue;
            }
            else   /// cas 1 2 3 .... D R
            {
              if( true == existeMemeD() || true == existeMemeR() )
              {
                nbcartes = 0;
                nbpoints = 0;
                index1c = icarte;
                while( val >= 1000 && icarte <= 24 )
                {
                  nbcartes++;
                  nbpoints = nbpoints + calculerPointsCarte( couleur, icarte );
                  icarte += 2;
                  val = m_aValeursSuites[ couleur ][ icarte ];
                }
                suite.setNombreCartes( nbcartes );
                suite.setNombrePoints( nbpoints );
                suite.setIndex1ereCarte( index1c );
                suite.setCouleur( couleur );
                liste.append( suite );
                a_suite = true;
                continue;
              }
              else
              {
                m_aValeursSuites[ couleur ][ 2 ] = 0;
                m_aValeursSuites[ couleur ][ 4 ] = 0;
              }
            }
          }
        }
        else    /// >= 1000 et <= 2000 -> cas 1 2 3 ... ou cas 1 ... D R
        {
          if( m_aValeursSuites[ couleur ][ 2 ] < 1000 )   /// cas 1 ... D R
          {
            icarte += 2;
            continue;
          }
          else  /// cas 1 2 3 ...
          {
            nbcartes = 0;
            nbpoints = 0;
            index1c = icarte;
            while( val >= 1000 && icarte <= 24 )
            {
              nbcartes++;
              nbpoints = nbpoints + calculerPointsCarte( couleur, icarte );
              icarte += 2;
              val = m_aValeursSuites[ couleur ][ icarte ];
            }
            suite.setNombreCartes( nbcartes );
            suite.setNombrePoints( nbpoints );
            suite.setIndex1ereCarte( index1c );
            suite.setCouleur( couleur );
            liste.append( suite );
            a_suite = true;
            continue;
          }
        }
      }
      else
      {
        nbcartes = 0;
        nbpoints = 0;
        index1c = icarte;
        while( val >= 1000 && icarte <= 24 )
        {
          nbcartes++;
          nbpoints = nbpoints + calculerPointsCarte( couleur, icarte );
          icarte += 2;
          if( icarte == 26 && m_aCartes[ couleur ][ 0 ] == 1 )    /// si AS
          {
            nbcartes++;
            nbpoints = nbpoints + 10;
          }
          val = m_aValeursSuites[ couleur ][ icarte ];
        }
        suite.setNombreCartes( nbcartes );
        suite.setNombrePoints( nbpoints );
        suite.setIndex1ereCarte( index1c );
        suite.setCouleur( couleur );
        liste.append( suite );
        a_suite = true;
        continue;
      }
    }
    icarte = icarte + 2;
  }
  return a_suite;
}

bool CValeurJeu::chercherTierce( QList<CSuite> & liste, const int icouleur, const int icarte )
{
  int nbpoints;
  CSuite tierce;

  int val1c = m_aValeursSuites[ icouleur ][ icarte + 0 ];
  int val2c = m_aValeursSuites[ icouleur ][ icarte + 2 ];
  int index3c = icarte + 4;
  if( index3c == 26 )
    index3c = 0;
  int val3c = m_aValeursSuites[ icouleur ][ index3c ];

  if( val1c >= 1000 && val2c >= 1000 && val3c >= 1000 )
  {
    nbpoints = 0;
    nbpoints = nbpoints + calculerPointsCarte( icouleur, icarte + 0 );
    nbpoints = nbpoints + calculerPointsCarte( icouleur, icarte + 2 );
    nbpoints = nbpoints + calculerPointsCarte( icouleur, index3c );

    tierce.setNombreCartes( 3 );
    tierce.setNombrePoints( nbpoints );
    tierce.setIndex1ereCarte( icarte );
    tierce.setCouleur( icouleur );
    liste.append( tierce );
    return true;
  }

  return false;
}

void CValeurJeu::noterMemes()
{
  int index0, index1, index2, index3;
  int icouleur = 0;
  for( int icarte = 0; icarte < 26; icarte += 2 )
  {
    quint8 carte0 = PASCARTE;
    quint8 carte1 = PASCARTE;
    quint8 carte2 = PASCARTE;
    quint8 carte3 = PASCARTE;

    index0 = icarte;
    carte0 = m_aCartes[ icouleur + 0 ][ icarte ];
    if( m_aCartes[ icouleur + 0 ][ icarte + 1 ] != PASCARTE )
    {
      index0 = icarte + 1;
      carte0 = m_aCartes[ icouleur + 0 ][ icarte + 1 ];
    }

    index1 = icarte;
    carte1 = m_aCartes[ icouleur + 1 ][ icarte ];
    if( m_aCartes[ icouleur + 1 ][ icarte + 1 ] != PASCARTE )
    {
      index1 = icarte + 1;
      carte1 = m_aCartes[ icouleur + 1 ][ icarte + 1 ];
    }

    index2 = icarte;
    carte2 = m_aCartes[ icouleur + 2 ][ icarte ];
    if( m_aCartes[ icouleur + 2 ][ icarte + 1 ] != PASCARTE )
    {
      index2 = icarte + 1;
      carte2 = m_aCartes[ icouleur + 2 ][ icarte + 1 ];
    }

    index3 = icarte;
    carte3 = m_aCartes[ icouleur + 3 ][ icarte ];
    if( m_aCartes[ icouleur + 3 ][ icarte + 1 ] != PASCARTE )
    {
      index3 = icarte + 1;
      carte3 = m_aCartes[ icouleur + 3 ][ icarte + 1 ];
    }

    if( carte0 != PASCARTE && carte0 == carte1 && carte0 == carte2 && carte0 == carte3 )
    {
      mettreValeur4Meme( index0, index1, index2, index3, carte0, m_aNotes[2] );
      continue;
    }

    if( carte0 != PASCARTE && carte0 == carte2 && carte0 == carte3 )
    {
      mettreValeur3Meme( icouleur + 0, icouleur + 2, icouleur + 3, index0, index2, index3, carte0, m_aNotes[1] );
      continue;
    }

    if( carte0 != PASCARTE && carte0 == carte1 && carte0 == carte3 )
    {
      mettreValeur3Meme( icouleur + 0, icouleur + 1, icouleur + 3, index0, index1, index3, carte0, m_aNotes[1] );
      continue;
    }

    if( carte0 != PASCARTE && carte0 == carte1 && carte0 == carte2 )
    {
      mettreValeur3Meme( icouleur + 0, icouleur + 1, icouleur + 2, index0, index1, index2, carte0, m_aNotes[1] );
      continue;
    }

    if( carte1 != PASCARTE && carte1 == carte2 && carte1 == carte3 )
    {
      mettreValeur3Meme( icouleur + 1, icouleur + 2, icouleur + 3, index1, index2, index3, carte1, m_aNotes[1] );
      continue;
    }
  }
}

void CValeurJeu::noterMemesOrdi()
{
  quint8 carte0, carte1, carte2, carte3;
  int valref0, valref1, valref2, valref3;
//  bool a_mis_meme = false;
  int index0, index1, index2, index3;
  int icouleur = 0;
  for( int icarte = 0; icarte < 26; icarte += 2 )
  {
    carte0 = PASCARTE;
    carte1 = PASCARTE;
    carte2 = PASCARTE;
    carte3 = PASCARTE;
    valref0 =0;
    valref1 =0;
    valref2 =0;
    valref3 =0;

    index0 = icarte;
    carte0 = m_aCartes[ icouleur + 0 ][ icarte ];
    valref0 = m_aValeursSuitesReference[ icouleur + 0 ][ icarte ];
    if( m_aCartes[ icouleur + 0 ][ icarte + 1 ] != PASCARTE )
    {
      index0 = icarte + 1;
      carte0 = m_aCartes[ icouleur + 0 ][ icarte + 1 ];
      valref0 = m_aValeursSuitesReference[ icouleur + 0 ][ icarte + 1 ];
    }

    index1 = icarte;
    carte1 = m_aCartes[ icouleur + 1 ][ icarte ];
    valref1 = m_aValeursSuitesReference[ icouleur + 1 ][ icarte ];
    if( m_aCartes[ icouleur + 1 ][ icarte + 1 ] != PASCARTE )
    {
      index1 = icarte + 1;
      carte1 = m_aCartes[ icouleur + 1 ][ icarte + 1 ];
      valref1 = m_aValeursSuitesReference[ icouleur + 1 ][ icarte + 1 ];
    }

    index2 = icarte;
    carte2 = m_aCartes[ icouleur + 2 ][ icarte ];
    valref2 = m_aValeursSuitesReference[ icouleur + 2 ][ icarte ];
    if( m_aCartes[ icouleur + 2 ][ icarte + 1 ] != PASCARTE )
    {
      index2 = icarte + 1;
      carte2 = m_aCartes[ icouleur + 2 ][ icarte + 1 ];
      valref2 = m_aValeursSuitesReference[ icouleur + 2 ][ icarte + 1 ];
    }

    index3 = icarte;
    carte3 = m_aCartes[ icouleur + 3 ][ icarte ];
    valref3 = m_aValeursSuitesReference[ icouleur + 3 ][ icarte ];
    if( m_aCartes[ icouleur + 3 ][ icarte + 1 ] != PASCARTE )
    {
      index3 = icarte + 1;
      carte3 = m_aCartes[ icouleur + 3 ][ icarte + 1 ];
      valref3 = m_aValeursSuitesReference[ icouleur + 3 ][ icarte + 1 ];
    }

    if( carte0 != PASCARTE && carte0 == carte1 && carte0 == carte2 && carte0 == carte3 )
    {
      mettreValeur4Meme( index0, index1, index2, index3, carte0, m_aNotes[2] );
      continue;
    }

    if( carte0 != PASCARTE && carte0 == carte2 && carte0 == carte3 )
    {
      int note = m_aNotes[1];
      if( valref0 >= 3000 || valref2 >= 3000 || valref3 >= 3000 )
      {
        if( m_asuite5C == true )
        {
          note = 0;
        }
      }
      else
      {
        if( valref0 >= 2000 || valref2 >= 2000 || valref3 >= 2000 )
        {
          if( m_asuite5C == true )
          {
            int indexref0 = index0;
            int indexref2 = index2;
            int indexref3 = index3;
            if( true == estPair( indexref0 ) )
            {
              indexref0 -= 2;
            }
            else
            {
              indexref0 -= 3;
            }
            if( true == estPair( indexref2 ) )
            {
              indexref2 -= 2;
            }
            else
            {
              indexref2 -= 3;
            }
            if( true == estPair( indexref3 ) )
            {
              indexref3 -= 2;
            }
            else
            {
              indexref3 -= 3;
            }
            int ref0 = m_aValeursSuitesReference[ icouleur + 0 ][ indexref0 ];
            int ref2 = m_aValeursSuitesReference[ icouleur + 2 ][ indexref2 ];
            int ref3 = m_aValeursSuitesReference[ icouleur + 3 ][ indexref3 ];
            if( ref0 >= 3000 || ref2 >= 3000 || ref3 >= 3000 )
            {
              note = 0;
            }

          }
          if( m_asuite4C == true )
          {
            note = 0;
          }
        }
        else
        {
          if( valref0 >= 1000 || valref2 >= 1000 || valref3 >= 1000 )
          {
          }
        }
      }

      mettreValeur3Meme( icouleur + 0, icouleur + 2, icouleur + 3, index0, index2, index3, carte0, note );
      continue;
    }

    if( carte0 != PASCARTE && carte0 == carte1 && carte0 == carte3 )
    {
      int note = m_aNotes[1];
      if( valref0 >= 3000 || valref1 >= 3000 || valref3 >= 3000 )
      {
        if( m_asuite5C == true )
        {
          note = 0;
        }
      }
      else
      {
        if( valref0 >= 2000 || valref1 >= 2000 || valref3 >= 2000 )
        {
          if( m_asuite5C == true )
          {
            int indexref0 = index0;
            int indexref1 = index1;
            int indexref3 = index3;
            if( true == estPair( indexref0 ) )
            {
              indexref0 -= 2;
            }
            else
            {
              indexref0 -= 3;
            }
            if( true == estPair( indexref1 ) )
            {
              indexref1 -= 2;
            }
            else
            {
              indexref1 -= 3;
            }
            if( true == estPair( indexref3 ) )
            {
              indexref3 -= 2;
            }
            else
            {
              indexref3 -= 3;
            }
            int ref0 = m_aValeursSuitesReference[ icouleur + 0 ][ indexref0 ];
            int ref1 = m_aValeursSuitesReference[ icouleur + 1 ][ indexref1 ];
            int ref3 = m_aValeursSuitesReference[ icouleur + 3 ][ indexref3 ];
            if( ref0 >= 3000 || ref1 >= 3000 || ref3 >= 3000 )
            {
              note = 0;
            }

          }
          if( m_asuite4C == true )
          {
            note = 0;
          }
        }
        else
        {
          if( valref0 >= 1000 || valref1 >= 1000 || valref3 >= 1000 )
          {
          }
        }
      }

      mettreValeur3Meme( icouleur + 0, icouleur + 1, icouleur + 3, index0, index1, index3, carte0, note );
      continue;
    }

    if( carte0 != PASCARTE && carte0 == carte1 && carte0 == carte2 )
    {
      int note = m_aNotes[1];
      if( valref0 >= 3000 || valref1 >= 3000 || valref2 >= 3000 )
      {
        if( m_asuite5C == true )
        {
          note = 0;
        }
      }
      else
      {
        if( valref0 >= 2000 || valref1 >= 2000 || valref2 >= 2000 )
        {
          if( m_asuite5C == true )
          {
            int indexref0 = index0;
            int indexref1 = index1;
            int indexref2 = index2;
            if( true == estPair( indexref0 ) )
            {
              indexref0 -= 2;
            }
            else
            {
              indexref0 -= 3;
            }
            if( true == estPair( indexref1 ) )
            {
              indexref1 -= 2;
            }
            else
            {
              indexref1 -= 3;
            }
            if( true == estPair( indexref2 ) )
            {
              indexref2 -= 2;
            }
            else
            {
              indexref2 -= 3;
            }
            int ref0 = m_aValeursSuitesReference[ icouleur + 0 ][ indexref0 ];
            int ref1 = m_aValeursSuitesReference[ icouleur + 1 ][ indexref1 ];
            int ref2 = m_aValeursSuitesReference[ icouleur + 2 ][ indexref2 ];
            if( ref0 >= 3000 || ref1 >= 3000 || ref2 >= 3000 )
            {
              note = 0;
            }

          }
          if( m_asuite4C == true )
          {
            note = 0;
          }
        }
        else
        {
          if( valref0 >= 1000 || valref1 >= 1000 || valref2 >= 1000 )
          {
          }
        }
      }

      mettreValeur3Meme( icouleur + 0, icouleur + 1, icouleur + 2, index0, index1, index2, carte0, note );
      continue;
    }

    if( carte1 != PASCARTE && carte1 == carte2 && carte1 == carte3 )
    {
      int note = m_aNotes[1];
      if( valref1 >= 3000 || valref2 >= 3000 || valref3 >= 3000 )
      {
        if( m_asuite5C == true )
        {
          note = 0;
        }
      }
      else
      {
        if( valref1 >= 2000 || valref2 >= 2000 || valref3 >= 2000 )
        {
          if( m_asuite5C == true )
          {
            int indexref1 = index1;
            int indexref2 = index2;
            int indexref3 = index3;
            if( true == estPair( indexref1 ) )
            {
              indexref1 -= 2;
            }
            else
            {
              indexref1 -= 3;
            }
            if( true == estPair( indexref2 ) )
            {
              indexref2 -= 2;
            }
            else
            {
              indexref2 -= 3;
            }
            if( true == estPair( indexref3 ) )
            {
              indexref3 -= 2;
            }
            else
            {
              indexref3 -= 3;
            }
            int ref1 = m_aValeursSuitesReference[ icouleur + 1 ][ indexref1 ];
            int ref2 = m_aValeursSuitesReference[ icouleur + 2 ][ indexref2 ];
            int ref3 = m_aValeursSuitesReference[ icouleur + 3 ][ indexref3 ];
            if( ref1 >= 3000 || ref2 >= 3000 || ref3 >= 3000 )
            {
              note = 0;
            }

          }
          if( m_asuite4C == true )
          {
            note = 0;
          }
        }
        else
        {
          if( valref1 >= 1000 || valref2 >= 1000 || valref3 >= 1000 )
          {
          }
        }
      }

      mettreValeur3Meme( icouleur + 1, icouleur + 2, icouleur + 3, index1, index2, index3, carte1, note );
      continue;
    }
  }
}

void CValeurJeu::sauverValeursMemes()
{
  for( int i = 0; i < 4; i++ )
  {
    for( int j = 0; j < 28; j++ )
    {
      m_aValeursMemes[ i ][ j ] = m_aValeurs[ i ][ j ];
    }
  }

/// @todo MIS POUR TEST
#ifdef MISE_AU_POINT
  writeFileValeursMemes( "/tmp/jndqrami/debug/valeurs-memes.txt" );
#endif
  ///

}

void CValeurJeu::sauverValeursMemesInitiales()
{
  for( int i = 0; i < 4; i++ )
  {
    for( int j = 0; j < 28; j++ )
    {
      m_aValeursMemesInitiales[ i ][ j ] = m_aValeursMemes[ i ][ j ];
    }
  }
}

void CValeurJeu::restaurerValeursMemesInitiales()
{
  for( int i = 0; i < 4; i++ )
  {
    for( int j = 0; j < 28; j++ )
    {
      m_aValeursMemes[ i ][ j ] = m_aValeursMemesInitiales[ i ][ j ];
    }
  }
}

int CValeurJeu::calculerPointsCarte( int icouleur, int icarte )
{
  int nbpoints = 0;
  quint8 carte = m_aCartes[ icouleur ][ icarte ];
  if( carte <= 10 )
  {
    nbpoints = carte;
  }
  else
  {
    nbpoints = 10;
  }

  return nbpoints;
}

bool CValeurJeu::chercherMemes( QList<CMeme> & liste )
{
  bool a_meme = false;
  int nbcartes;
  int nbpoints;
  CMeme meme;

  quint8 carte1, carte2, carte3, carte4;
  int valm1, valm2, valm3, valm4;
  int index1, index2, index3, index4;
  int icouleur = 0;
  for( int icarte = 0; icarte < 26; icarte += 2 )
  {
    carte1 = PASCARTE;
    carte2 = PASCARTE;
    carte3 = PASCARTE;
    carte4 = PASCARTE;
    valm1 = 0;
    valm2 = 0;
    valm3 = 0;
    valm4 = 0;

    index1 = icarte;
    carte1 = m_aCartes[ icouleur + 0 ][ icarte ];
    valm1 = m_aValeursMemes[ icouleur + 0 ][ icarte ];
    if( m_aValeursMemes[ icouleur + 0 ][ icarte + 1 ] != 0 )
    {
      index1 = icarte + 1;
      carte1 = m_aCartes[ icouleur + 0 ][ icarte + 1 ];
      valm1 = m_aValeursMemes[ icouleur + 0 ][ icarte + 1 ];
    }

    index2 = icarte;
    carte2 = m_aCartes[ icouleur + 1 ][ icarte ];
    valm2 = m_aValeursMemes[ icouleur + 1 ][ icarte ];
    if( m_aValeursMemes[ icouleur + 1 ][ icarte + 1 ] != 0 )
    {
      index2 = icarte + 1;
      carte2 = m_aCartes[ icouleur + 1 ][ icarte + 1 ];
      valm2 = m_aValeursMemes[ icouleur + 1 ][ icarte + 1 ];
    }

    index3 = icarte;
    carte3 = m_aCartes[ icouleur + 2 ][ icarte ];
    valm3 = m_aValeursMemes[ icouleur + 2 ][ icarte ];
    if( m_aValeursMemes[ icouleur + 2 ][ icarte + 1 ] != 0 )
    {
      index3 = icarte + 1;
      carte3 = m_aCartes[ icouleur + 2 ][ icarte + 1 ];
      valm3 = m_aValeursMemes[ icouleur + 2 ][ icarte + 1 ];
    }

    index4 = icarte;
    carte4 = m_aCartes[ icouleur + 3 ][ icarte ];
    valm4 = m_aValeursMemes[ icouleur + 3 ][ icarte ];
    if( m_aValeursMemes[ icouleur + 3 ][ icarte + 1 ] != 0 )
    {
      index4 = icarte + 1;
      carte4 = m_aCartes[ icouleur + 3 ][ icarte + 1 ];
      valm4 = m_aValeursMemes[ icouleur + 3 ][ icarte + 1 ];
    }

    if( valm1 >= TESTPOSE_MEME4 && valm2 >= TESTPOSE_MEME4 && valm3 >= TESTPOSE_MEME4 && valm4 >= TESTPOSE_MEME4 )
//    if( carte1 != PASCARTE && carte1 == carte2 && carte1 == carte3 && carte1 == carte4 )
    {
      nbcartes = 4;
      nbpoints = 0;
      int ptsc = calculerPointsCarte( icouleur + 0, index1 );
      if( ptsc == 1 ) ptsc = 10;
      nbpoints = nbpoints + ( ptsc * nbcartes );

      meme.setNombreCartes( nbcartes );
      meme.setNombrePoints( nbpoints );
      meme.setIndex1ereCarte( index1 );
      meme.setIndex2emeCarte( index2 );
      meme.setIndex3emeCarte( index3 );
      meme.setIndex4emeCarte( index4 );
      liste.append( meme );
      a_meme = true;
      continue;
    }

    if( valm1 >= TESTPOSE_MEME3 && valm3 >= TESTPOSE_MEME3 && valm4 >= TESTPOSE_MEME3 )
//    if( carte1 != PASCARTE && carte1 == carte3 && carte1 == carte4 )
    {
      nbcartes = 3;
      nbpoints = 0;
      int ptsc = calculerPointsCarte( icouleur + 0, index1 );
      if( ptsc == 1 ) ptsc = 10;
      nbpoints = nbpoints + ( ptsc * nbcartes );

      meme.setNombreCartes( nbcartes );
      meme.setNombrePoints( nbpoints );
      meme.setIndex1ereCarte( index1 );
      meme.setIndex2emeCarte( -1 );
      meme.setIndex3emeCarte( index3 );
      meme.setIndex4emeCarte( index4 );
      liste.append( meme );
      a_meme = true;
      continue;
    }

    if( valm1 >= TESTPOSE_MEME3 && valm2 >= TESTPOSE_MEME3 && valm4 >= TESTPOSE_MEME3 )
//    if( carte1 != PASCARTE && carte1 == carte2 && carte1 == carte4 )
    {
      nbcartes = 3;
      nbpoints = 0;
      int ptsc = calculerPointsCarte( icouleur + 0, index1 );
      if( ptsc == 1 ) ptsc = 10;
      nbpoints = nbpoints + ( ptsc * nbcartes );

      meme.setNombreCartes( nbcartes );
      meme.setNombrePoints( nbpoints );
      meme.setIndex1ereCarte( index1 );
      meme.setIndex2emeCarte( index2 );
      meme.setIndex3emeCarte( -1 );
      meme.setIndex4emeCarte( index4 );
      liste.append( meme );
      a_meme = true;
      continue;
    }

    if( valm1 >= TESTPOSE_MEME3 && valm2 >= TESTPOSE_MEME3 && valm3 >= TESTPOSE_MEME3 )
//    if( carte1 != PASCARTE && carte1 == carte2 && carte1 == carte3 )
    {
      nbcartes = 3;
      nbpoints = 0;
      int ptsc = calculerPointsCarte( icouleur + 0, index1 );
      if( ptsc == 1 ) ptsc = 10;
      nbpoints = nbpoints + ( ptsc * nbcartes );

      meme.setNombreCartes( nbcartes );
      meme.setNombrePoints( nbpoints );
      meme.setIndex1ereCarte( index1 );
      meme.setIndex2emeCarte( index2 );
      meme.setIndex3emeCarte( index3 );
      meme.setIndex4emeCarte( -1 );
      liste.append( meme );
      a_meme = true;
      continue;
    }

    if( valm2 >= TESTPOSE_MEME3 && valm3 >= TESTPOSE_MEME3 && valm4 >= TESTPOSE_MEME3 )
//    if( carte2 != PASCARTE && carte2 == carte3 && carte2 == carte4 )
    {
      nbcartes = 3;
      nbpoints = 0;
      int ptsc = calculerPointsCarte( icouleur + 1, index2 );
      if( ptsc == 1 ) ptsc = 10;
      nbpoints = nbpoints + ( ptsc * nbcartes );

      meme.setNombreCartes( nbcartes );
      meme.setNombrePoints( nbpoints );
      meme.setIndex1ereCarte( -1 );
      meme.setIndex2emeCarte( index2 );
      meme.setIndex3emeCarte( index3 );
      meme.setIndex4emeCarte( index4 );
      liste.append( meme );
      a_meme = true;
      continue;
    }
  }
  return a_meme;
}

bool CValeurJeu::existeMemeD()
{
  bool existe = false;

  int nbCarte = 0;
  quint8 carte1_0 = PASCARTE;
  quint8 carte1_1 = PASCARTE;
  quint8 carte2_0 = PASCARTE;
  quint8 carte2_1 = PASCARTE;
  quint8 carte3_0 = PASCARTE;
  quint8 carte3_1 = PASCARTE;
  quint8 carte4_0 = PASCARTE;
  quint8 carte4_1 = PASCARTE;

  carte1_0 = m_aCartes[ 0 ][ 22 ];
  carte1_1 = m_aCartes[ 0 ][ 23 ];
  if( carte1_0 != PASCARTE || carte1_1 != PASCARTE )  nbCarte += 1;

  carte2_0 = m_aCartes[ 1 ][ 22 ];
  carte2_1 = m_aCartes[ 1 ][ 23 ];
  if( carte2_0 != PASCARTE || carte2_1 != PASCARTE )  nbCarte += 1;

  carte3_0 = m_aCartes[ 2 ][ 22 ];
  carte3_1 = m_aCartes[ 2 ][ 23 ];
  if( carte3_0 != PASCARTE || carte3_1 != PASCARTE )  nbCarte += 1;

  carte4_0 = m_aCartes[ 3 ][ 22 ];
  carte4_1 = m_aCartes[ 3 ][ 23 ];
  if( carte4_0 != PASCARTE || carte4_1 != PASCARTE )  nbCarte += 1;

  if( nbCarte >= 3 )  existe = true;

  return existe;
}

bool CValeurJeu::existeMemeR()
{
  bool existe = false;

  int nbCarte = 0;
  quint8 carte1_0 = PASCARTE;
  quint8 carte1_1 = PASCARTE;
  quint8 carte2_0 = PASCARTE;
  quint8 carte2_1 = PASCARTE;
  quint8 carte3_0 = PASCARTE;
  quint8 carte3_1 = PASCARTE;
  quint8 carte4_0 = PASCARTE;
  quint8 carte4_1 = PASCARTE;

  carte1_0 = m_aCartes[ 0 ][ 24 ];
  carte1_1 = m_aCartes[ 0 ][ 25 ];
  if( carte1_0 != PASCARTE || carte1_1 != PASCARTE )  nbCarte += 1;

  carte2_0 = m_aCartes[ 1 ][ 24 ];
  carte2_1 = m_aCartes[ 1 ][ 25 ];
  if( carte2_0 != PASCARTE || carte2_1 != PASCARTE )  nbCarte += 1;

  carte3_0 = m_aCartes[ 2 ][ 24 ];
  carte3_1 = m_aCartes[ 2 ][ 25 ];
  if( carte3_0 != PASCARTE || carte3_1 != PASCARTE )  nbCarte += 1;

  carte4_0 = m_aCartes[ 3 ][ 24 ];
  carte4_1 = m_aCartes[ 3 ][ 25 ];
  if( carte4_0 != PASCARTE || carte4_1 != PASCARTE )  nbCarte += 1;

  if( nbCarte >= 3 )  existe = true;

  return existe;
}

void CValeurJeu::writeFileValeurs( const QString &filename )
{
  writeFile( filename, m_aValeurs, 4 );
}

void CValeurJeu::writeFileValeursSuites( const QString &filename )
{
  writeFile( filename, m_aValeursSuites, 4 );
}

void CValeurJeu::writeFileValeursMemes( const QString &filename )
{
  writeFile( filename, m_aValeursMemes, 4 );
}

void CValeurJeu::writeFileCartes( const QString &filename )
{
  writeFile( filename, m_aCartes, 4 );
}

void CValeurJeu::writeFile( const QString &filename, int (*valeurs)[28], int nblignes )
{
  QFile file( filename );
  if( true == file.exists() )
  {
    file.remove ();
  }
  if( !file.open( QIODevice::WriteOnly ) )
  {
    return;
  }
  int val;
  QTextStream out( &file );
  for( int i = 0; i < nblignes; i++ )
  {
    for( int j = 0; j < 26; j++ )
    {
      val = valeurs[ i ][ j ];
      if( val == 0 )
      {
        out << " ";
      }
      else
      {
        out << val;
      }
    }
    out << Qt::endl;
  }
}

void CValeurJeu::writeFile( const QString &filename, quint8 (*cartes)[28], int nblignes )
{
  QFile file( filename );
  if( true == file.exists() )
  {
    file.remove ();
  }
  if( !file.open( QIODevice::WriteOnly ) )
  {
    return;
  }
  int val;
  QTextStream out( &file );
  for( int i = 0; i < nblignes; i++ )
  {
    for( int j = 0; j < 26; j++ )
    {
      val = cartes[ i ][ j ];
      if( val == 0 )
      {
        out << " ";
      }
      else
      {
        out << val;
      }
    }
    out << Qt::endl;
  }
}

void CValeurJeu::writeFileLC( const QString &filename, quint8 * LC )
{
  QFile file( filename );
  if( true == file.exists() )
  {
    file.remove ();
  }
  if( !file.open( QIODevice::WriteOnly ) )
  {
    return;
  }
  int val;
  QTextStream out( &file );
  for( int i = 0; i < 104; i++ )
  {
    val = LC[ i ];
    if( val != 0 )
    {
      out << val << " ";
    }
  }
}

bool CValeurJeu::estPair( const int entier )
{
  bool estPair = false;
  int n = entier % 2;
  if( n == 0 )  estPair = true;
  return estPair;
}

CValeurCarte CValeurJeu::calculerCouleurIndex1Carte( quint8 carte )
{
  CValeurCarte cvcarte;
  quint8 valcarte;
  int icouleur;
  int icarte;
  if( carte >= 1 && carte <= 13 )
  {
    valcarte = carte;
    icouleur = 0;
    icarte = ( carte - 1 ) * 2;
    cvcarte.setValeurCarte( valcarte );
    cvcarte.setCouleur( icouleur );
    cvcarte.setIndex( icarte );
  }
  if( carte >= 21 && carte <= 33 )
  {
    valcarte = carte - 20;
    icouleur = 1;
    icarte = ( carte - 21 ) * 2;
    cvcarte.setValeurCarte( valcarte );
    cvcarte.setCouleur( icouleur );
    cvcarte.setIndex( icarte );
  }
  if( carte >= 41 && carte <= 53 )
  {
    valcarte = carte - 40;
    icouleur = 2;
    icarte = ( carte - 41 ) * 2;
    cvcarte.setValeurCarte( valcarte );
    cvcarte.setCouleur( icouleur );
    cvcarte.setIndex( icarte );
  }
  if( carte >= 61 && carte <= 73 )
  {
    valcarte = carte - 60;
    icouleur = 3;
    icarte = ( carte - 61 ) * 2;
    cvcarte.setValeurCarte( valcarte );
    cvcarte.setCouleur( icouleur );
    cvcarte.setIndex( icarte );
  }

  return cvcarte;
}

void CValeurJeu::faireListeCartesSuites( quint8 * listecs, QList<CSuite> & liste )
{
  for( int i = 0; i < 104; i++ )
  {
    listecs[ i ] = 0;
  }
  int indexLCS = 0;
  quint8 valcarte;
  quint8 carte;
  CSuite suite;
  QListIterator<CSuite> itS( liste );
  while( itS.hasNext() )
  {
    suite = itS.next();
    int nbcS = suite.nombreCartes();
    int index1cS = suite.index1ereCarte();
    int coulS = suite.couleur();
    while( nbcS > 0 )
    {
      valcarte = m_aCartes[ coulS ][ index1cS ];
      carte = valcarte + ( coulS * 20 );
      listecs[ indexLCS ] = carte;
      indexLCS ++;
      if( true == estPair( index1cS ) )
      {
        index1cS += 2;
      }
      else
      {
        index1cS += 1;
      }
      if( index1cS >= 26 )  index1cS = 0; /// si AS
      nbcS --;
    }
  }

/// @todo MIS POUR TEST
#ifdef MISE_AU_POINT
  writeFileLC( "/tmp/jndqrami/debug/liste-C-S.txt", listecs );
#endif
  ///
}

void CValeurJeu::faireListeCartesMemes( quint8 * listecm, QList<CMeme> & liste )
{
  for( int i = 0; i < 104; i++ )
  {
    listecm[ i ] = 0;
  }
  int indexLCM = 0;
  quint8 valcarte;
  quint8 carte;
  CMeme meme;
  QListIterator<CMeme> itM( liste );
  while( itM.hasNext() )
  {
    meme = itM.next();
    int index1cM = meme.index1ereCarte();
    int index2cM = meme.index2emeCarte();
    int index3cM = meme.index3emeCarte();
    int index4cM = meme.index4emeCarte();
    if( index1cM != -1 )
    {
      valcarte = m_aCartes[ 0 ][ index1cM ];
      carte = valcarte + 0;
      listecm[ indexLCM ] = carte;
      indexLCM ++;
    }
    if( index2cM != -1 )
    {
      valcarte = m_aCartes[ 1 ][ index2cM ];
      carte = valcarte + 20;
      listecm[ indexLCM ] = carte;
      indexLCM ++;
    }
    if( index3cM != -1 )
    {
      valcarte = m_aCartes[ 2 ][ index3cM ];
      carte = valcarte + 40;
      listecm[ indexLCM ] = carte;
      indexLCM ++;
    }
    if( index4cM != -1 )
    {
      valcarte = m_aCartes[ 3 ][ index4cM ];
      carte = valcarte + 60;
      listecm[ indexLCM ] = carte;
      indexLCM ++;
    }
  }

/// @todo MIS POUR TEST
#ifdef MISE_AU_POINT
  writeFileLC( "/tmp/jndqrami/debug/liste-C-M.txt", listecm );
#endif
  ///
}

void CValeurJeu::eliminerDoublons( quint8 * listecs, quint8 * listecm )
{
//  int nbdoublons = 0;
  int indexLCS = 0;
  while( listecs[ indexLCS ] != 0 )
  {
    quint8 carteS = listecs[ indexLCS ];
    int indexLCM = 0;
    while( listecm[ indexLCM ] != 0 )
    {
      quint8 carteM = listecm[ indexLCM ];
      if( carteS == carteM )
      {
        CValeurCarte cvcarte = calculerCouleurIndex1Carte( carteS );
        int icouleur = cvcarte.couleur();
        int icarte = cvcarte.index();
        if( m_aCartes[ icouleur ][ icarte + 1 ] != PASCARTE )
        {
          m_aCartes[ icouleur ][ icarte + 1 ] = 0;
        }
        else
        {
          m_aCartes[ icouleur ][ icarte ] = 0;
        }

/*        nbdoublons++;
        if( nbdoublons >= 2 )
        {
          int var_sert_a_rien = 10;
        }*/
      }
      indexLCM ++;
    }
    indexLCS ++;
  }

/// @todo MIS POUR TEST
#ifdef MISE_AU_POINT
  writeFileCartes( "/tmp/jndqrami/debug/Cartes-SUPPR-Doublons.txt" );
#endif
}

void CValeurJeu::faireListeCartesSM( QList<quint8> & liste, QList<CSuite> & lsuite, QList<CMeme> & lmeme )
{
  quint8 valcarte;
  quint8 carte;
  CSuite suite;
  QListIterator<CSuite> itS( lsuite );
  while( itS.hasNext() )
  {
    suite = itS.next();
    int nbcS = suite.nombreCartes();
    int index1cS = suite.index1ereCarte();
    int coulS = suite.couleur();
    while( nbcS > 0 )
    {
      valcarte = m_aCartes[ coulS ][ index1cS ];
      carte = valcarte + ( coulS * 20 );
      liste.append( carte );
      if( true == estPair( index1cS ) )
      {
        index1cS += 2;
      }
      else
      {
        index1cS += 1;
      }
      if( index1cS >= 26 )  index1cS = 0; /// si AS
      nbcS --;
    }
  }

  CMeme meme;
  QListIterator<CMeme> itM( lmeme );
  while( itM.hasNext() )
  {
    meme = itM.next();
    int index1cM = meme.index1ereCarte();
    int index2cM = meme.index2emeCarte();
    int index3cM = meme.index3emeCarte();
    int index4cM = meme.index4emeCarte();
    if( index1cM != -1 )
    {
      valcarte = m_aCartes[ 0 ][ index1cM ];
      carte = valcarte + 0;
      liste.append( carte );
    }
    if( index2cM != -1 )
    {
      valcarte = m_aCartes[ 1 ][ index2cM ];
      carte = valcarte + 20;
      liste.append( carte );
    }
    if( index3cM != -1 )
    {
      valcarte = m_aCartes[ 2 ][ index3cM ];
      carte = valcarte + 40;
      liste.append( carte );
    }
    if( index4cM != -1 )
    {
      valcarte = m_aCartes[ 3 ][ index4cM ];
      carte = valcarte + 60;
      liste.append( carte );
    }
  }
}

int CValeurJeu::compterNbSuites( QList<CSuite> & liste )
{
  int nbsuite = 0;
  CSuite suite;

  QListIterator<CSuite> it( liste );
  while( it.hasNext() )
  {
    nbsuite++;
    suite = it.next();
  }

  return nbsuite;
}

int CValeurJeu::compterNbCartesSuites( QList<CSuite> & liste )
{
  int nbcartes = 0;
  CSuite suite;

  QListIterator<CSuite> it( liste );
  while( it.hasNext() )
  {
    suite = suite + it.next();
  }
  nbcartes = suite.nombreCartes();

  return nbcartes;
}

int CValeurJeu::compterNbPointsSuites( QList<CSuite> & liste )
{
  int nbpoints = 0;
  CSuite suite;

  QListIterator<CSuite> it( liste );
  while( it.hasNext() )
  {
    suite = suite + it.next();
  }
  nbpoints = suite.nombrePoints();

  return nbpoints;
}

int CValeurJeu::compterNbCartesMemes( QList<CMeme> & liste )
{
  int nbcartes = 0;
  CMeme meme;

  QListIterator<CMeme> it( liste );
  while( it.hasNext() )
  {
    meme = meme + it.next();
  }
  nbcartes = meme.nombreCartes();

  return nbcartes;
}

int CValeurJeu::compterNbPointsMemes( QList<CMeme> & liste )
{
  int nbpoints = 0;
  CMeme meme;

  QListIterator<CMeme> it( liste );
  while( it.hasNext() )
  {
    meme = meme + it.next();
  }
  nbpoints = meme.nombrePoints();

  return nbpoints;
}

void CValeurJeu::faireListePose( quint8 * listepose, QList<CSuite> & lsuite, QList<CMeme> & lmeme )
{
  for( int i = 0; i < 104; i++ )
  {
    listepose[ i ] = MARKFIN;
  }
  int indexLP = 0;

quint8 valcarte;
quint8 carte;
  CSuite suite;
  QListIterator<CSuite> itS( lsuite );
  while( itS.hasNext() )
  {
    suite = itS.next();
    int nbcS = suite.nombreCartes();
    int index1cS = suite.index1ereCarte();
    int coulS = suite.couleur();
    listepose[ indexLP ] = MARKSUITE;
    indexLP ++;
    listepose[ indexLP ] = nbcS;
    indexLP ++;

    while( nbcS > 0 )
    {
      valcarte = m_aCartes[ coulS ][ index1cS ];
      carte = valcarte + ( coulS * 20 );
      listepose[ indexLP ] = carte;
      indexLP ++;
      if( true == estPair( index1cS ) )
      {
        index1cS += 2;
      }
      else
      {
        index1cS += 1;
      }
      if( index1cS >= 26 )  index1cS = 0; /// si AS
      nbcS --;
    }
  }

  CMeme meme;
  QListIterator<CMeme> itM( lmeme );
  while( itM.hasNext() )
  {
    meme = itM.next();
    int nbcm = meme.nombreCartes();
    int index1cM = meme.index1ereCarte();
    int index2cM = meme.index2emeCarte();
    int index3cM = meme.index3emeCarte();
    int index4cM = meme.index4emeCarte();
    listepose[ indexLP ] = MARKMEME;
    indexLP ++;
    listepose[ indexLP ] = nbcm;
    indexLP ++;

    if( index1cM != -1 )
    {
      valcarte = m_aCartes[ 0 ][ index1cM ];
      carte = valcarte + 0;
      listepose[ indexLP ] = carte;
      indexLP ++;
    }
    if( index2cM != -1 )
    {
      valcarte = m_aCartes[ 1 ][ index2cM ];
      carte = valcarte + 20;
      listepose[ indexLP ] = carte;
      indexLP ++;
    }
    if( index3cM != -1 )
    {
      valcarte = m_aCartes[ 2 ][ index3cM ];
      carte = valcarte + 40;
      listepose[ indexLP ] = carte;
      indexLP ++;
    }
    if( index4cM != -1 )
    {
      valcarte = m_aCartes[ 3 ][ index4cM ];
      carte = valcarte + 60;
      listepose[ indexLP ] = carte;
      indexLP ++;
    }
  }

/// @todo MIS POUR TEST
#ifdef MISE_AU_POINT
  writeFileLC( "/tmp/jndqrami/debug/liste-POSE.txt", listepose );
#endif
  ///

}

void CValeurJeu::calculerNotesTestPose()
{
//enum	NOTESTESTPOSE { TESTPOSE_MEME3 = 300, TESTPOSE_MEME4 = 500, TESTPOSE_SUITE = 1000 };

  m_aNotes[ 0 ] = 0;
  m_aNotes[ 1 ] = TESTPOSE_MEME3;
  m_aNotes[ 2 ] = TESTPOSE_MEME4;
  m_aNotes[ 3 ] = 0;
  m_aNotes[ 4 ] = 0;
  m_aNotes[ 5 ] = TESTPOSE_SUITE;

}

void CValeurJeu::calculerNotesPasPose()
{
// enum	NOTESPASPOSE { PASPOSE_MEME2 = 300, PASPOSE_MEME3 = 600, PASPOSE_MEME4 = 800, PASPOSE_SUITECXC = 300, PASPOSE_SUITE2 = 300, PASPOSE_TIERCE = 1000 };

  if( true == a1Suite() )
  {
    m_aNotes[ 0 ] = PASPOSE_MEME2;
    m_aNotes[ 1 ] = PASPOSE_MEME3;
    m_aNotes[ 2 ] = PASPOSE_MEME4;
    m_aNotes[ 3 ] = PASPOSE_SUITECXC;
    m_aNotes[ 4 ] = PASPOSE_SUITE2;
    m_aNotes[ 5 ] = PASPOSE_TIERCE;
  }
  else
  {
    m_aNotes[ 0 ] = PASPOSE_MEME2 - 200;  /// = 100
    m_aNotes[ 1 ] = PASPOSE_MEME3 - 400;  /// = 200
    m_aNotes[ 2 ] = PASPOSE_MEME4 - 500;  /// = 300
    m_aNotes[ 3 ] = PASPOSE_SUITECXC + 200; /// = 500
    m_aNotes[ 4 ] = PASPOSE_SUITE2 + 200;   /// = 500
    m_aNotes[ 5 ] = PASPOSE_TIERCE;
  }

}

void CValeurJeu::calculerNotesAPose()
{
//enum	NOTESAPOSE { APOSE_MEME2 = 100, APOSE_MEME3 = 200, APOSE_MEME4 = 300, APOSE_SUITECXC = 100, APOSE_SUITE2 = 100, APOSE_TIERCE = 400 };

  if( m_nbcartesjeu > 3 )
  {
    m_aNotes[ 0 ] = APOSE_MEME2;
    m_aNotes[ 1 ] = APOSE_MEME3;
    m_aNotes[ 2 ] = APOSE_MEME4;
    m_aNotes[ 3 ] = APOSE_SUITECXC;
    m_aNotes[ 4 ] = APOSE_SUITE2;
    m_aNotes[ 5 ] = APOSE_TIERCE;
  }
  else
  {
    m_aNotes[ 0 ] = 0;
    m_aNotes[ 1 ] = 0;
    m_aNotes[ 2 ] = 0;
    m_aNotes[ 3 ] = 0;
    m_aNotes[ 4 ] = 0;
    m_aNotes[ 5 ] = 0;
  }
}

bool CValeurJeu::a1Suite()
{
  bool aSuite = false;

  m_aNotes[ 0 ] = 0;
  m_aNotes[ 1 ] = 0;
  m_aNotes[ 2 ] = 0;
  m_aNotes[ 3 ] = 0;
  m_aNotes[ 4 ] = 0;
  m_aNotes[ 5 ] = 1000;

  sauverCartes();
  sauverValeurs();

  initialiserValeurs();
  noterSuites();

  int icarte = 0;
  int icouleur = 0;

  for( icouleur = 0; icouleur < 4; icouleur++ )
  {
    for( icarte = 0; icarte < 24; icarte += 2 )
    {
      int val = m_aValeurs[ icouleur ][ icarte ];
      if( val >= 1000 )
      {
        aSuite = true;
      }
    }
  }
  restaurerCartes();
  restaurerValeurs();

  return aSuite;
}

bool CValeurJeu::aSuite2()
{
  bool aSuite = false;

  m_aNotes[ 0 ] = 0;
  m_aNotes[ 1 ] = 0;
  m_aNotes[ 2 ] = 0;
  m_aNotes[ 3 ] = 1000;
  m_aNotes[ 4 ] = 1000;
  m_aNotes[ 5 ] = 0;

  sauverCartes();
  sauverValeurs();

  initialiserValeurs();
  calculerValeurTierce();

  int icarte = 0;
  int icouleur = 0;

  for( icouleur = 0; icouleur < 4; icouleur++ )
  {
    for( icarte = 0; icarte < 24; icarte += 2 )
    {
      int val = m_aValeurs[ icouleur ][ icarte ];
      if( val >= 1000 )
      {
        aSuite = true;
      }
    }
  }
  restaurerCartes();
  restaurerValeurs();

  return aSuite;
}

bool CValeurJeu::aMeme2()
{
  bool aMeme = false;

  m_aNotes[ 0 ] = 1000;
  m_aNotes[ 1 ] = 0;
  m_aNotes[ 2 ] = 0;
  m_aNotes[ 3 ] = 0;
  m_aNotes[ 4 ] = 0;
  m_aNotes[ 5 ] = 0;

  sauverCartes();
  sauverValeurs();

  initialiserValeurs();
  calculerValeurMeme();

  int icarte = 0;
  int icouleur = 0;

  for( icouleur = 0; icouleur < 4; icouleur++ )
  {
    for( icarte = 0; icarte < 24; icarte += 2 )
    {
      int val = m_aValeurs[ icouleur ][ icarte ];
      if( val >= 1000 )
      {
        aMeme = true;
      }
    }
  }
  restaurerCartes();
  restaurerValeurs();

  return aMeme;
}

void CValeurJeu::copierTableau( quint8 * aorg, quint8 * acopie, int nbitems )
{
  for( int i = 0; i < nbitems; i++ )
  {
    acopie[ i ] = aorg[ i ];
  }
}

int CValeurJeu::compterPresenceCarte( quint8 * acartes, quint8 carte )
{
  int maxcartes;
  switch( m_niveaujeu )
  {
    case NIVEAU_FAIBLE :
      maxcartes = 4;
      break;
    case NIVEAU_MOYEN :
      maxcartes = 10;
      break;
    case NIVEAU_FORT :
      maxcartes = 110;
      break;
  }

  int nbcartes = 0;
  int nbfois = 0;
  for( int i = 0; i < 110; i++ )
  {
    quint8 ctest = acartes[ i ];
    if( ctest != PASCARTE && ctest != MARKFIN )
    {
      nbcartes++;
      if( nbcartes <= maxcartes && carte == ctest )   nbfois++;
    }
  }
  return nbfois;
}

int CValeurJeu::calculerReductionNote( quint8 carte0, quint8 carte1 )
{
  int nbfois = 0;
  quint8 carte;
  carte = carte0 - 1;
  if( carte != 0 && carte != 20 && carte != 40 && carte != 60 )
  {
    nbfois += compterPresenceCarte( m_cartesjetees, carte );
    nbfois += compterPresenceCarte( m_cartesposees, carte );
  }
  carte = carte1 + 1;
  nbfois += compterPresenceCarte( m_cartesjetees, carte );
  nbfois += compterPresenceCarte( m_cartesposees, carte );

  if( nbfois == 0 )    return 0;
  if( nbfois == 1 )
  {
    if( m_apose == true )
    {
      return 100;
    }
    else
    {
      return 200;
    }
  }
  if( nbfois == 2 )
  {
    if( m_apose == true )
    {
      return 100;
    }
    else
    {
      return 300;
    }
  }
/// SI >= 3
  if( m_apose == true )
  {
    return 100;
  }
  else
  {
    return 500;
  }
}

void CValeurJeu::eliminerDoublons_123_234()
{
  if( m_test_123_234 == true )
  {
    eliminerDoublons_123et234();
  }
}

void CValeurJeu::eliminerDoublons_123et234()
{
/// elimine Doublons 1 2 3
  bool coeur123 = false;
  bool coeursuite1234 = false;
  if( m_aCartes[ COEUR ][ 0 ] == 1 && m_aCartes[ COEUR ][ 2 ] == 2 &&  m_aCartes[ COEUR ][ 4 ] == 3 )
  {
    coeur123 = true;
    if( m_aCartes[ COEUR ][ 6 ] == 4 )  coeursuite1234 = true;
  }

  bool pique123 = false;
  bool piquesuite1234 = false;
  if( m_aCartes[ PIQUE ][ 0 ] == 1 && m_aCartes[ PIQUE ][ 2 ] == 2 &&  m_aCartes[ PIQUE ][ 4 ] == 3 )
  {
    pique123 = true;
    if( m_aCartes[ PIQUE ][ 6 ] == 4 )  piquesuite1234 = true;
    if( coeur123 == true )
    {
      if( coeursuite1234 == true && piquesuite1234 == true )
      {
        if( m_aValeurs[ PIQUE ][ 0 ] >= 1100 )
          m_aValeurs[ PIQUE ][ 0 ] -= 1000;
        else m_aValeurs[ PIQUE ][ 0 ] = 1;
        m_aValeurs[ PIQUE ][ 2 ] = 2;
        m_aValeurs[ PIQUE ][ 4 ] = 3;
        m_aValeurs[ PIQUE ][ 6 ] = 4;
        pique123 = false;
        piquesuite1234 = false;
      }
      else
      {
        if( coeursuite1234 == true && piquesuite1234 == false )
        {
          if( m_aValeurs[ PIQUE ][ 0 ] >= 1100 )
            m_aValeurs[ PIQUE ][ 0 ] -= 1000;
          else m_aValeurs[ PIQUE ][ 0 ] = 1;
          m_aValeurs[ PIQUE ][ 2 ] = 2;
          m_aValeurs[ PIQUE ][ 4 ] = 3;
          pique123 = false;
        }
        else
        {
          if( coeursuite1234 == false && piquesuite1234 == true )
          {
            if( m_aValeurs[ COEUR ][ 0 ] >= 1100 )
              m_aValeurs[ COEUR ][ 0 ] -= 1000;
            else m_aValeurs[ COEUR ][ 0 ] = 1;
            m_aValeurs[ COEUR ][ 2 ] = 2;
            m_aValeurs[ COEUR ][ 4 ] = 3;
            coeur123 = false;
          }
          else
          {
            if( coeursuite1234 == false && piquesuite1234 == false )
            {
              if( m_aValeurs[ PIQUE ][ 0 ] >= 1100 )
                m_aValeurs[ PIQUE ][ 0 ] -= 1000;
              else m_aValeurs[ PIQUE ][ 0 ] = 1;
              m_aValeurs[ PIQUE ][ 2 ] = 2;
              m_aValeurs[ PIQUE ][ 4 ] = 3;
              pique123 = false;
            }
          }
        }
      }
    }
  }

  bool carreau123 = false;
  bool carreausuite1234 = false;
  if( m_aCartes[ CARREAU ][ 0 ] == 1 && m_aCartes[ CARREAU ][ 2 ] == 2 &&  m_aCartes[ CARREAU ][ 4 ] == 3 )
  {
    carreau123 = true;
    if( m_aCartes[ CARREAU ][ 6 ] == 4 )  carreausuite1234 = true;
    if( coeur123 == true )
    {
      if( coeursuite1234 == true && carreausuite1234 == true )
      {
        if( m_aValeurs[ CARREAU ][ 0 ] >= 1100 )
          m_aValeurs[ CARREAU ][ 0 ] -= 1000;
        else m_aValeurs[ CARREAU ][ 0 ] = 1;
        m_aValeurs[ CARREAU ][ 2 ] = 2;
        m_aValeurs[ CARREAU ][ 4 ] = 3;
        m_aValeurs[ CARREAU ][ 6 ] = 4;
        carreau123 = false;
        carreausuite1234 = false;
      }
      else
      {
        if( coeursuite1234 == true && carreausuite1234 == false )
        {
          if( m_aValeurs[ CARREAU ][ 0 ] >= 1100 )
            m_aValeurs[ CARREAU ][ 0 ] -= 1000;
          else m_aValeurs[ CARREAU ][ 0 ] = 1;
          m_aValeurs[ CARREAU ][ 2 ] = 2;
          m_aValeurs[ CARREAU ][ 4 ] = 3;
          carreau123 = false;
        }
        else
        {
          if( coeursuite1234 == false && carreausuite1234 == true )
          {
            if( m_aValeurs[ COEUR ][ 0 ] >= 1100 )
              m_aValeurs[ COEUR ][ 0 ] -= 1000;
            else  m_aValeurs[ COEUR ][ 0 ] = 1;
            m_aValeurs[ COEUR ][ 2 ] = 2;
            m_aValeurs[ COEUR ][ 4 ] = 3;
            coeur123 = false;
          }
          else
          {
            if( coeursuite1234 == false && carreausuite1234 == false )
            {
              if( m_aValeurs[ CARREAU ][ 0 ] >= 1100 )
                m_aValeurs[ CARREAU ][ 0 ] -= 1000;
              else m_aValeurs[ CARREAU ][ 0 ] = 1;
              m_aValeurs[ CARREAU ][ 2 ] = 2;
              m_aValeurs[ CARREAU ][ 4 ] = 3;
              carreau123 = false;
            }
          }
        }
      }
    }
    if( pique123 == true )
    {
      if( piquesuite1234 == true && carreausuite1234 == true )
      {
        if( m_aValeurs[ CARREAU ][ 0 ] >= 1100 )
          m_aValeurs[ CARREAU ][ 0 ] -= 1000;
        else m_aValeurs[ CARREAU ][ 0 ] = 1;
        m_aValeurs[ CARREAU ][ 2 ] = 2;
        m_aValeurs[ CARREAU ][ 4 ] = 3;
        m_aValeurs[ CARREAU ][ 6 ] = 4;
        carreau123 = false;
        carreausuite1234 = false;
      }
      else
      {
        if( piquesuite1234 == true && carreausuite1234 == false )
        {
          if( m_aValeurs[ CARREAU ][ 0 ] >= 1100 )
            m_aValeurs[ CARREAU ][ 0 ] -= 1000;
          else  m_aValeurs[ CARREAU ][ 0 ] = 1;
          m_aValeurs[ CARREAU ][ 2 ] = 2;
          m_aValeurs[ CARREAU ][ 4 ] = 3;
          carreau123 = false;
        }
        else
        {
          if( piquesuite1234 == false && carreausuite1234 == true )
          {
            if( m_aValeurs[ PIQUE ][ 0 ] >= 1100 )
              m_aValeurs[ PIQUE ][ 0 ] -= 1000;
            else  m_aValeurs[ PIQUE ][ 0 ] = 1;
            m_aValeurs[ PIQUE ][ 2 ] = 2;
            m_aValeurs[ PIQUE ][ 4 ] = 3;
            pique123 = false;
          }
          else
          {
            if( piquesuite1234 == false && carreausuite1234 == false )
            {
              if( m_aValeurs[ CARREAU ][ 0 ] >= 1100 )
                m_aValeurs[ CARREAU ][ 0 ] -= 1000;
              else  m_aValeurs[ CARREAU ][ 0 ] = 1;
              m_aValeurs[ CARREAU ][ 2 ] = 2;
              m_aValeurs[ CARREAU ][ 4 ] = 3;
              carreau123 = false;
            }
          }
        }
      }
    }
  }

  bool trefle123 = false;
  bool treflesuite1234 = false;
  if( m_aCartes[ TREFLE ][ 0 ] == 1 && m_aCartes[ TREFLE ][ 2 ] == 2 &&  m_aCartes[ TREFLE ][ 4 ] == 3 )
  {
    trefle123 = true;
    if( m_aCartes[ TREFLE ][ 6 ] == 4 )  treflesuite1234 = true;
    if( coeur123 == true )
    {
      if( coeursuite1234 == true && treflesuite1234 == true )
      {
        if( m_aValeurs[ TREFLE ][ 0 ] >= 1100 )
          m_aValeurs[ TREFLE ][ 0 ] -= 1000;
        else  m_aValeurs[ TREFLE ][ 0 ] = 1;
        m_aValeurs[ TREFLE ][ 2 ] = 2;
        m_aValeurs[ TREFLE ][ 4 ] = 3;
        m_aValeurs[ TREFLE ][ 6 ] = 4;
        trefle123 = false;
        treflesuite1234 = false;
      }
      else
      {
        if( coeursuite1234 == true && treflesuite1234 == false )
        {
          if( m_aValeurs[ TREFLE ][ 0 ] >= 1100 )
            m_aValeurs[ TREFLE ][ 0 ] -= 1000;
          else  m_aValeurs[ TREFLE ][ 0 ] = 1;
          m_aValeurs[ TREFLE ][ 2 ] = 2;
          m_aValeurs[ TREFLE ][ 4 ] = 3;
          trefle123 = false;
        }
        else
        {
          if( coeursuite1234 == false && treflesuite1234 == true )
          {
            if( m_aValeurs[ COEUR ][ 0 ] >= 1100 )
              m_aValeurs[ COEUR ][ 0 ] -= 1000;
            else  m_aValeurs[ COEUR ][ 0 ] = 1;
            m_aValeurs[ COEUR ][ 2 ] = 2;
            m_aValeurs[ COEUR ][ 4 ] = 3;
            coeur123 = false;
          }
          else
          {
            if( coeursuite1234 == false && treflesuite1234 == false )
            {
              if( m_aValeurs[ TREFLE ][ 0 ] >= 1100 )
                m_aValeurs[ TREFLE ][ 0 ] -= 1000;
              else  m_aValeurs[ TREFLE ][ 0 ] = 1;
              m_aValeurs[ TREFLE ][ 2 ] = 2;
              m_aValeurs[ TREFLE ][ 4 ] = 3;
              trefle123 = false;
            }
          }
        }
      }
    }
    if( pique123 == true )
    {
      if( piquesuite1234 == true && treflesuite1234 == true )
      {
        if( m_aValeurs[ TREFLE ][ 0 ] >= 1100 )
          m_aValeurs[ TREFLE ][ 0 ] -= 1000;
        else  m_aValeurs[ TREFLE ][ 0 ] = 1;
        m_aValeurs[ TREFLE ][ 2 ] = 2;
        m_aValeurs[ TREFLE ][ 4 ] = 3;
        m_aValeurs[ TREFLE ][ 6 ] = 4;
        trefle123 = false;
        treflesuite1234 = false;
      }
      else
      {
        if( piquesuite1234 == true && treflesuite1234 == false )
        {
          if( m_aValeurs[ TREFLE ][ 0 ] >= 1100 )
            m_aValeurs[ TREFLE ][ 0 ] -= 1000;
          else  m_aValeurs[ TREFLE ][ 0 ] = 1;
          m_aValeurs[ TREFLE ][ 2 ] = 2;
          m_aValeurs[ TREFLE ][ 4 ] = 3;
          trefle123 = false;
        }
        else
        {
          if( piquesuite1234 == false && treflesuite1234 == true )
          {
            if( m_aValeurs[ PIQUE ][ 0 ] >= 1100 )
              m_aValeurs[ PIQUE ][ 0 ] -= 1000;
            else  m_aValeurs[ PIQUE ][ 0 ] = 1;
            m_aValeurs[ PIQUE ][ 2 ] = 2;
            m_aValeurs[ PIQUE ][ 4 ] = 3;
            pique123 = false;
          }
          else
          {
            if( piquesuite1234 == false && treflesuite1234 == false )
            {
              if( m_aValeurs[ TREFLE ][ 0 ] >= 1100 )
                m_aValeurs[ TREFLE ][ 0 ] -= 1000;
              else  m_aValeurs[ TREFLE ][ 0 ] = 1;
              m_aValeurs[ TREFLE ][ 2 ] = 2;
              m_aValeurs[ TREFLE ][ 4 ] = 3;
              trefle123 = false;
            }
          }
        }
      }
    }
    if( carreau123 == true )
    {
      if( carreausuite1234 == true && treflesuite1234 == true )
      {
        if( m_aValeurs[ TREFLE ][ 0 ] >= 1100 )
          m_aValeurs[ TREFLE ][ 0 ] -= 1000;
        else  m_aValeurs[ TREFLE ][ 0 ] = 1;
        m_aValeurs[ TREFLE ][ 2 ] = 2;
        m_aValeurs[ TREFLE ][ 4 ] = 3;
        m_aValeurs[ TREFLE ][ 6 ] = 4;
        trefle123 = false;
        treflesuite1234 = false;
      }
      else
      {
        if( carreausuite1234 == true && treflesuite1234 == false )
        {
          if( m_aValeurs[ TREFLE ][ 0 ] >= 1100 )
            m_aValeurs[ TREFLE ][ 0 ] -= 1000;
          else  m_aValeurs[ TREFLE ][ 0 ] = 1;
          m_aValeurs[ TREFLE ][ 2 ] = 2;
          m_aValeurs[ TREFLE ][ 4 ] = 3;
          trefle123 = false;
        }
        else
        {
          if( carreausuite1234 == false && treflesuite1234 == true )
          {
            if( m_aValeurs[ CARREAU ][ 0 ] >= 1100 )
              m_aValeurs[ CARREAU ][ 0 ] -= 1000;
            else  m_aValeurs[ CARREAU ][ 0 ] = 1;
            m_aValeurs[ CARREAU ][ 2 ] = 2;
            m_aValeurs[ CARREAU ][ 4 ] = 3;
            carreau123 = false;
          }
          else
          {
            if( carreausuite1234 == false && treflesuite1234 == false )
            {
              if( m_aValeurs[ TREFLE ][ 0 ] >= 1100 )
                m_aValeurs[ TREFLE ][ 0 ] -= 1000;
              else  m_aValeurs[ TREFLE ][ 0 ] = 1;
              m_aValeurs[ TREFLE ][ 2 ] = 2;
              m_aValeurs[ TREFLE ][ 4 ] = 3;
              trefle123 = false;
            }
          }
        }
      }
    }
  }

/// elimine Doublons 2 3 4
  bool coeur234 = false;
  bool coeursuite2345 = false;
  if( m_aCartes[ COEUR ][ 2 ] == 2 && m_aCartes[ COEUR ][ 4 ] == 3 &&  m_aCartes[ COEUR ][ 6 ] == 4 )
  {
    coeur234 = true;
    if( m_aCartes[ COEUR ][ 8 ] == 5 )  coeursuite2345 = true;
  }

  bool pique234 = false;
  bool piquesuite2345 = false;
  if( m_aCartes[ PIQUE ][ 2 ] == 2 && m_aCartes[ PIQUE ][ 4 ] == 3 &&  m_aCartes[ PIQUE ][ 6 ] == 4 )
  {
    pique234 = true;
    if( m_aCartes[ PIQUE ][ 8 ] == 5 )  piquesuite2345 = true;
    if( coeur234 == true )
    {
      if( coeursuite2345 == true && piquesuite2345 == true )
      {
        m_aValeurs[ PIQUE ][ 2 ] = 2;
        m_aValeurs[ PIQUE ][ 4 ] = 3;
        m_aValeurs[ PIQUE ][ 6 ] = 4;
        m_aValeurs[ PIQUE ][ 8 ] = 5;
        pique234 = false;
        piquesuite2345 = false;
      }
      else
      {
        if( coeursuite2345 == true && piquesuite2345 == false )
        {
          m_aValeurs[ PIQUE ][ 2 ] = 2;
          m_aValeurs[ PIQUE ][ 4 ] = 3;
          m_aValeurs[ PIQUE ][ 6 ] = 4;
          pique234 = false;
        }
        else
        {
          if( coeursuite2345 == false && piquesuite2345 == true )
          {
            m_aValeurs[ COEUR ][ 2 ] = 2;
            m_aValeurs[ COEUR ][ 4 ] = 3;
            m_aValeurs[ COEUR ][ 6 ] = 4;
            coeur234 = false;
          }
          else
          {
            if( coeursuite2345 == false && piquesuite2345 == false )
            {
              m_aValeurs[ PIQUE ][ 2 ] = 2;
              m_aValeurs[ PIQUE ][ 4 ] = 3;
              m_aValeurs[ PIQUE ][ 6 ] = 4;
              pique234 = false;
            }
          }
        }
      }
    }
  }

  bool carreau234 = false;
  bool carreausuite2345 = false;
  if( m_aCartes[ CARREAU ][ 2 ] == 2 && m_aCartes[ CARREAU ][ 4 ] == 3 &&  m_aCartes[ CARREAU ][ 6 ] == 4 )
  {
    carreau234 = true;
    if( m_aCartes[ CARREAU ][ 8 ] == 5 )  carreausuite2345 = true;
    if( coeur234 == true )
    {
      if( coeursuite2345 == true && carreausuite2345 == true )
      {
        m_aValeurs[ CARREAU ][ 2 ] = 2;
        m_aValeurs[ CARREAU ][ 4 ] = 3;
        m_aValeurs[ CARREAU ][ 6 ] = 4;
        m_aValeurs[ CARREAU ][ 8 ] = 5;
        carreau234 = false;
        carreausuite2345 = false;
      }
      else
      {
        if( coeursuite2345 == true && carreausuite2345 == false )
        {
          m_aValeurs[ CARREAU ][ 2 ] = 2;
          m_aValeurs[ CARREAU ][ 4 ] = 3;
          m_aValeurs[ CARREAU ][ 6 ] = 4;
          carreau234 = false;
        }
        else
        {
          if( coeursuite2345 == false && carreausuite2345 == true )
          {
            m_aValeurs[ COEUR ][ 2 ] = 2;
            m_aValeurs[ COEUR ][ 4 ] = 3;
            m_aValeurs[ COEUR ][ 6 ] = 4;
            coeur234 = false;
          }
          else
          {
            if( coeursuite2345 == false && carreausuite2345 == false )
            {
              m_aValeurs[ CARREAU ][ 2 ] = 2;
              m_aValeurs[ CARREAU ][ 4 ] = 3;
              m_aValeurs[ CARREAU ][ 6 ] = 4;
              carreau234 = false;
            }
          }
        }
      }
    }
    if( pique234 == true )
    {
      if( piquesuite2345 == true && carreausuite2345 == true )
      {
        m_aValeurs[ CARREAU ][ 2 ] = 2;
        m_aValeurs[ CARREAU ][ 4 ] = 3;
        m_aValeurs[ CARREAU ][ 6 ] = 4;
        m_aValeurs[ CARREAU ][ 8 ] = 5;
        carreau234 = false;
        carreausuite2345 = false;
      }
      else
      {
        if( piquesuite2345 == true && carreausuite2345 == false )
        {
          m_aValeurs[ CARREAU ][ 2 ] = 2;
          m_aValeurs[ CARREAU ][ 4 ] = 3;
          m_aValeurs[ CARREAU ][ 6 ] = 4;
          carreau234 = false;
        }
        else
        {
          if( piquesuite2345 == false && carreausuite2345 == true )
          {
            m_aValeurs[ PIQUE ][ 2 ] = 2;
            m_aValeurs[ PIQUE ][ 4 ] = 3;
            m_aValeurs[ PIQUE ][ 6 ] = 4;
            pique234 = false;
          }
          else
          {
            if( piquesuite2345 == false && carreausuite2345 == false )
            {
              m_aValeurs[ CARREAU ][ 2 ] = 2;
              m_aValeurs[ CARREAU ][ 4 ] = 3;
              m_aValeurs[ CARREAU ][ 6 ] = 4;
              carreau234 = false;
            }
          }
        }
      }
    }
  }

  bool trefle234 = false;
  bool treflesuite2345 = false;
  if( m_aCartes[ TREFLE ][ 2 ] == 2 && m_aCartes[ TREFLE ][ 4 ] == 3 &&  m_aCartes[ TREFLE ][ 6 ] == 4 )
  {
    trefle234 = true;
    if( m_aCartes[ TREFLE ][ 8 ] == 5 )  treflesuite2345 = true;
    if( coeur234 == true )
    {
      if( coeursuite2345 == true && treflesuite2345 == true )
      {
        m_aValeurs[ TREFLE ][ 2 ] = 2;
        m_aValeurs[ TREFLE ][ 4 ] = 3;
        m_aValeurs[ TREFLE ][ 6 ] = 4;
        m_aValeurs[ TREFLE ][ 8 ] = 5;
        trefle234 = false;
        treflesuite2345 = false;
      }
      else
      {
        if( coeursuite2345 == true && treflesuite2345 == false )
        {
          m_aValeurs[ TREFLE ][ 2 ] = 2;
          m_aValeurs[ TREFLE ][ 4 ] = 3;
          m_aValeurs[ TREFLE ][ 6 ] = 4;
          trefle234 = false;
        }
        else
        {
          if( coeursuite2345 == false && treflesuite2345 == true )
          {
            m_aValeurs[ COEUR ][ 2 ] = 2;
            m_aValeurs[ COEUR ][ 4 ] = 3;
            m_aValeurs[ COEUR ][ 6 ] = 4;
            coeur234 = false;
          }
          else
          {
            if( coeursuite2345 == false && treflesuite2345 == false )
            {
              m_aValeurs[ TREFLE ][ 2 ] = 2;
              m_aValeurs[ TREFLE ][ 4 ] = 3;
              m_aValeurs[ TREFLE ][ 6 ] = 4;
              trefle234 = false;
            }
          }
        }
      }
    }
    if( pique234 == true )
    {
      if( piquesuite2345 == true && treflesuite2345 == true )
      {
        m_aValeurs[ TREFLE ][ 2 ] = 2;
        m_aValeurs[ TREFLE ][ 4 ] = 3;
        m_aValeurs[ TREFLE ][ 6 ] = 4;
        m_aValeurs[ TREFLE ][ 8 ] = 5;
        trefle234 = false;
        treflesuite2345 = false;
      }
      else
      {
        if( piquesuite2345 == true && treflesuite2345 == false )
        {
          m_aValeurs[ TREFLE ][ 2 ] = 2;
          m_aValeurs[ TREFLE ][ 4 ] = 3;
          m_aValeurs[ TREFLE ][ 6 ] = 4;
          trefle234 = false;
        }
        else
        {
          if( piquesuite2345 == false && treflesuite2345 == true )
          {
            m_aValeurs[ PIQUE ][ 2 ] = 2;
            m_aValeurs[ PIQUE ][ 4 ] = 3;
            m_aValeurs[ PIQUE ][ 6 ] = 4;
            pique234 = false;
          }
          else
          {
            if( piquesuite2345 == false && treflesuite2345 == false )
            {
              m_aValeurs[ TREFLE ][ 2 ] = 2;
              m_aValeurs[ TREFLE ][ 4 ] = 3;
              m_aValeurs[ TREFLE ][ 6 ] = 4;
              trefle234 = false;
            }
          }
        }
      }
    }
    if( carreau234 == true )
    {
      if( carreausuite2345 == true && treflesuite2345 == true )
      {
        m_aValeurs[ TREFLE ][ 2 ] = 2;
        m_aValeurs[ TREFLE ][ 4 ] = 3;
        m_aValeurs[ TREFLE ][ 6 ] = 4;
        m_aValeurs[ TREFLE ][ 8 ] = 5;
        trefle234 = false;
        treflesuite2345 = false;
      }
      else
      {
        if( carreausuite2345 == true && treflesuite2345 == false )
        {
          m_aValeurs[ TREFLE ][ 2 ] = 2;
          m_aValeurs[ TREFLE ][ 4 ] = 3;
          m_aValeurs[ TREFLE ][ 6 ] = 4;
          trefle234 = false;
        }
        else
        {
          if( carreausuite2345 == false && treflesuite2345 == true )
          {
            m_aValeurs[ CARREAU ][ 2 ] = 2;
            m_aValeurs[ CARREAU ][ 4 ] = 3;
            m_aValeurs[ CARREAU ][ 6 ] = 4;
            carreau234 = false;
          }
          else
          {
            if( carreausuite2345 == false && treflesuite2345 == false )
            {
              m_aValeurs[ TREFLE ][ 2 ] = 2;
              m_aValeurs[ TREFLE ][ 4 ] = 3;
              m_aValeurs[ TREFLE ][ 6 ] = 4;
              trefle234 = false;
            }
          }
        }
      }
    }
  }

/// elimine doublons 1 2 3  et  2 3 4
  if( coeur234 == true )
  {
    if( pique123 == true )
    {
      if( m_aValeurs[ PIQUE ][ 0 ] >= 1100 )
        m_aValeurs[ PIQUE ][ 0 ] -= 1000;
      else  m_aValeurs[ PIQUE ][ 0 ] = 1;
      m_aValeurs[ PIQUE ][ 2 ] = 2;
      m_aValeurs[ PIQUE ][ 4 ] = 3;
      pique123 = false;
    }
    if( carreau123 == true )
    {
      if( m_aValeurs[ CARREAU ][ 0 ] >= 1100 )
        m_aValeurs[ CARREAU ][ 0 ] -= 1000;
      else  m_aValeurs[ CARREAU ][ 0 ] = 1;
      m_aValeurs[ CARREAU ][ 2 ] = 2;
      m_aValeurs[ CARREAU ][ 4 ] = 3;
      carreau123 = false;
    }
    if( trefle123 == true )
    {
      if( m_aValeurs[ TREFLE ][ 0 ] >= 1100 )
        m_aValeurs[ TREFLE ][ 0 ] -= 1000;
      else  m_aValeurs[ TREFLE ][ 0 ] = 1;
      m_aValeurs[ TREFLE ][ 2 ] = 2;
      m_aValeurs[ TREFLE ][ 4 ] = 3;
      trefle123 = false;
    }
  }

  if( pique234 == true )
  {
    if( coeur123 == true )
    {
      if( m_aValeurs[ COEUR ][ 0 ] >= 1100 )
        m_aValeurs[ COEUR ][ 0 ] -= 1000;
      else  m_aValeurs[ COEUR ][ 0 ] = 1;
      m_aValeurs[ COEUR ][ 2 ] = 2;
      m_aValeurs[ COEUR ][ 4 ] = 3;
      coeur123 = false;
    }
    if( carreau123 == true )
    {
      if( m_aValeurs[ CARREAU ][ 0 ] >= 1100 )
        m_aValeurs[ CARREAU ][ 0 ] -= 1000;
      else  m_aValeurs[ CARREAU ][ 0 ] = 1;
      m_aValeurs[ CARREAU ][ 2 ] = 2;
      m_aValeurs[ CARREAU ][ 4 ] = 3;
      carreau123 = false;
    }
    if( trefle123 == true )
    {
      if( m_aValeurs[ TREFLE ][ 0 ] >= 1100 )
        m_aValeurs[ TREFLE ][ 0 ] -= 1000;
      else  m_aValeurs[ TREFLE ][ 0 ] = 1;
      m_aValeurs[ TREFLE ][ 2 ] = 2;
      m_aValeurs[ TREFLE ][ 4 ] = 3;
      trefle123 = false;
    }
  }

  if( carreau234 == true )
  {
    if( coeur123 == true )
    {
      if( m_aValeurs[ COEUR ][ 0 ] >= 1100 )
        m_aValeurs[ COEUR ][ 0 ] -= 1000;
      else  m_aValeurs[ COEUR ][ 0 ] = 1;
      m_aValeurs[ COEUR ][ 2 ] = 2;
      m_aValeurs[ COEUR ][ 4 ] = 3;
      coeur123 = false;
    }
    if( pique123 == true )
    {
      if( m_aValeurs[ PIQUE ][ 0 ] >= 1100 )
        m_aValeurs[ PIQUE ][ 0 ] -= 1000;
      else  m_aValeurs[ PIQUE ][ 0 ] = 1;
      m_aValeurs[ PIQUE ][ 2 ] = 2;
      m_aValeurs[ PIQUE ][ 4 ] = 3;
      pique123 = false;
    }
    if( trefle123 == true )
    {
      if( m_aValeurs[ TREFLE ][ 0 ] >= 1100 )
        m_aValeurs[ TREFLE ][ 0 ] -= 1000;
      else  m_aValeurs[ TREFLE ][ 0 ] = 1;
      m_aValeurs[ TREFLE ][ 2 ] = 2;
      m_aValeurs[ TREFLE ][ 4 ] = 3;
      trefle123 = false;
    }
  }

  if( trefle234 == true )
  {
    if( coeur123 == true )
    {
      if( m_aValeurs[ COEUR ][ 0 ] >= 1100 )
        m_aValeurs[ COEUR ][ 0 ] -= 1000;
      else  m_aValeurs[ COEUR ][ 0 ] = 1;
      m_aValeurs[ COEUR ][ 2 ] = 2;
      m_aValeurs[ COEUR ][ 4 ] = 3;
      coeur123 = false;
    }
    if( pique123 == true )
    {
      if( m_aValeurs[ PIQUE ][ 0 ] >= 1100 )
        m_aValeurs[ PIQUE ][ 0 ] -= 1000;
      else  m_aValeurs[ PIQUE ][ 0 ] = 1;
      m_aValeurs[ PIQUE ][ 2 ] = 2;
      m_aValeurs[ PIQUE ][ 4 ] = 3;
      pique123 = false;
    }
    if( carreau123 == true )
    {
      if( m_aValeurs[ CARREAU ][ 0 ] >= 1100 )
        m_aValeurs[ CARREAU ][ 0 ] -= 1000;
      else  m_aValeurs[ CARREAU ][ 0 ] = 1;
      m_aValeurs[ CARREAU ][ 2 ] = 2;
      m_aValeurs[ CARREAU ][ 4 ] = 3;
      carreau123 = false;
    }
  }
}
