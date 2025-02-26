#include "cbasejoueurs.h"

#include "constantes.h"

#include "cvaleurjeu.h"
#include "cvaleurcarte.h"
#include "cretourpose.h"

CBaseJoueurs::CBaseJoueurs()
{
}

void CBaseJoueurs::trierJeu( quint8 * ajeu )
{
  for( int i = 0; i < 14; i++ )
  {
    if( ajeu[ i ] == PASCARTE )
    {
      ajeu[ i ] = RIENCARTE;
    }
  }

  int index0, index1;
  quint8 carte0, carte1;
  index1 = 0;
  while( ajeu[ index1 ] != MARKFIN )
  {
    index0 = index1;
    while( ajeu[ index0 ] != MARKFIN )
    {
      carte1 = ajeu[ index1 ];
      carte0 = ajeu[ index0 ];
      if( carte0 < carte1 )
      {
        ajeu[ index1 ] = carte0;
        ajeu[ index0 ] = carte1;
      }
      index0++;
    }
    index1++;
  }

  for( int i = 0; i < 14; i++ )
  {
    if( ajeu[ i ] == RIENCARTE )
    {
      ajeu[ i ] = PASCARTE;
    }
  }
}

void CBaseJoueurs::supprimerTrouJeu( quint8 * ajeu, const int nbcarte )
{
  int j = 0;
  quint8 copie[ 14 ];

  for( int i = 0; i < 14; i++ )
  {
    copie[ i ] = PASCARTE;
  }

  for( int i = 0; i < nbcarte; i++ )
  {
    if( ajeu[ i ] != PASCARTE )
    {
      copie[ j ] = ajeu[ i ];
      j++;
    }
  }

  for( int i = 0; i < 14; i++ )
  {
    ajeu[ i ] = copie[ i ];
  }
}

quint8 CBaseJoueurs::chercherCartePlusPetiteValeur( quint8 * ajeu, const bool a_pose, int & nbpaspose, int niveaujeu, quint8 *cjetees, quint8 *cposees )
{
  quint8 carte = ERREUR_CARTE;

  CValeurJeu valeurjeu( ajeu, a_pose, niveaujeu );

  if( false == a_pose )  /// Joueur n'a pas encore posé
  {
    carte = valeurjeu.getCartePlusPetiteValeur( nbpaspose, cjetees, cposees );
  }
  else	/// Joueur à déjà posé
  {
    carte = valeurjeu.getCartePlusGrandeValeur( false, cjetees, cposees );
  }

  return carte;
}

quint8 CBaseJoueurs::chercherCartePlusGrandeValeur( quint8 * ajeu, const bool a_pose, int niveaujeu, quint8 *cjetees, quint8 *cposees )
{
  quint8 carte = ERREUR_CARTE;

  CValeurJeu valeurjeu( ajeu, a_pose, niveaujeu );

  carte = valeurjeu.getCartePlusGrandeValeur( true, cjetees, cposees );

  return carte;
}

bool CBaseJoueurs::aSuite2( quint8 * ajeu, const bool a_pose )
{
  bool aS2 = false;
  CValeurJeu valeurjeu( ajeu, a_pose );
  aS2 = valeurjeu.aSuite2();
  return aS2;
}

bool CBaseJoueurs::aMeme2( quint8 * ajeu, const bool a_pose )
{
  bool aM2 = false;
  CValeurJeu valeurjeu( ajeu, a_pose );
  aM2 = valeurjeu.aMeme2();
  return aM2;
}

int CBaseJoueurs::testerSiPeutPoserSud( quint8 * ajeu, const bool a_pose, const quint8 cjprise, quint8 * listepose )
{
  int coderetour;
  coderetour = testerSiPeutPoserSudSuitesMemes( ajeu, a_pose, cjprise, listepose );
  if( RETOUR_OK != coderetour )
  {
    int retour = testerSiPeutPoserSudTierceMemes( ajeu, a_pose, cjprise, listepose );
    if( RETOUR_OK == retour )
      return retour;
  }
  return coderetour;
}

int CBaseJoueurs::testerSiPeutPoserSudSuitesMemes( quint8 * ajeu, const bool a_pose, const quint8 cjprise, quint8 * listepose )
{
  int coderetour = ERREUR_INCONNUE;

  CValeurJeu valeurjeu( ajeu, a_pose );

  coderetour = valeurjeu.peutPoserSudSuitesMemes( cjprise, listepose );

  return coderetour;
}

int CBaseJoueurs::testerSiPeutPoserSudTierceMemes( quint8 * ajeu, const bool a_pose, const quint8 cjprise, quint8 * listepose )
{
  int coderetour = ERREUR_INCONNUE;

  CValeurJeu valeurjeu( ajeu, a_pose );

  coderetour = valeurjeu.peutPoserSudTierceMemes( cjprise, listepose );

  return coderetour;
}

bool CBaseJoueurs::testerSiPeutPoserOrdi( quint8 * ajeu, const bool a_pose, const quint8 cjprise, quint8 * listepose )
{
  bool peutPoser;
  peutPoser = testerSiPeutPoserOrdiMaxCartes( ajeu, a_pose, cjprise, listepose );
  if( false == peutPoser )
  {
    peutPoser = testerSiPeutPoserOrdiCartes( ajeu, a_pose, cjprise, listepose );
  }
  return peutPoser;
}

bool CBaseJoueurs::testerSiPeutPoserOrdiMaxCartes( quint8 * ajeu, const bool a_pose, const quint8 cjprise, quint8 * listepose )
{
  bool peutPoser;

  CValeurJeu valeurjeu( ajeu, a_pose );

  peutPoser = valeurjeu.peutPoserOrdiMaxCartes( cjprise, listepose );

  return peutPoser;
}

bool CBaseJoueurs::testerSiPeutPoserOrdiCartes( quint8 * ajeu, const bool a_pose, const quint8 cjprise, quint8 * listepose )
{
  bool peutPoser;

  CValeurJeu valeurjeu( ajeu, a_pose );

  peutPoser = valeurjeu.peutPoserOrdiCartes( cjprise, listepose );

  return peutPoser;
}

CRetourPose CBaseJoueurs::faireListesPoses( QList<quint8> & qlpose, QList<quint8> & qlflag, quint8 * acpj, int icpj, quint8 * ajeu, int nbcartes, quint8 * listepose )
{
  CRetourPose rp;
  int nbcs;
  int nbcm;
  int nbcms;
  quint8 carte;

  int indexLCP = 0; /// index dans liste pose
  quint8 type = 0;

  while( listepose[ indexLCP ] != MARKFIN )
  {
    type = listepose[ indexLCP++ ];
    if( type == MARKSUITE )         /// si Suite
    {
      nbcs = listepose[ indexLCP++ ];
 /// 1ere Carte
      carte = listepose[ indexLCP++ ];
      qlpose.append( carte );
      acpj[ icpj++ ] = carte;
      enlever1Carte( ajeu, carte );
      nbcartes--;
      carte = carte - 1;
      if( carte == 0 || carte == 20 || carte == 40 || carte == 60 ) /// si AS
      {
        carte = 0;
      }
      qlflag.append( carte );
 /// Cartes du milieu
      nbcms = nbcs - 2;
      while ( nbcms > 0 )
      {
        carte = listepose[ indexLCP++ ];
        qlpose.append( carte );
        acpj[ icpj++ ] = carte;
        qlflag.append( 0 );
        enlever1Carte( ajeu, carte );
        nbcartes--;
        nbcms--;
      }
 /// derniere Carte
      carte = listepose[ indexLCP++ ];
      qlpose.append( carte );
      acpj[ icpj++ ] = carte;
      enlever1Carte( ajeu, carte );
      nbcartes--;
      carte = carte + 1;
      if( carte == 14 || carte == 34 || carte == 54 || carte == 74 )  /// si R
      {
        carte = carte - 13;
      }
      if( carte == 2 || carte == 22 || carte == 42 || carte == 62 ) /// si AS
      {
        carte = 0;
      }
      qlflag.append( carte );
  /// Fin 1 suite
      qlpose.append( PASCARTE );
      qlflag.append( RIENCARTE );
    }
    else      /// si Meme
    {
      nbcm = listepose[ indexLCP++ ];
      if( nbcm == 3 )  /// Meme 3 cartes
      {
        carte = listepose[ indexLCP + 0 ];
        CValeurCarte cvcarte = calculerCouleurIndex1Carte( carte );
        quint8 valcarte = cvcarte.valeurCarte();
        int total4cartes = calculerTotal4Cartes( valcarte );
        int total3cartes = 0;
        total3cartes = total3cartes + listepose[ indexLCP + 0 ];
        total3cartes = total3cartes + listepose[ indexLCP + 1 ];
        total3cartes = total3cartes + listepose[ indexLCP + 2 ];
        quint8 autorise = total4cartes - total3cartes;
        while ( nbcm > 0 )
        {
          carte = listepose[ indexLCP++ ];
          qlpose.append( carte );
          acpj[ icpj++ ] = carte;
          qlflag.append( autorise );
          enlever1Carte( ajeu, carte );
          nbcartes--;
          nbcm--;
        }
      }
      else  /// Meme 4 cartes
      {
        while ( nbcm > 0 )
        {
          carte = listepose[ indexLCP++ ];
          qlpose.append( carte );
          acpj[ icpj++ ] = carte;
          qlflag.append( 0 );
          enlever1Carte( ajeu, carte );
          nbcartes--;
          nbcm--;
        }
      }
  /// Fin 1 Meme
      qlpose.append( PASCARTE );
      qlflag.append( RIENCARTE );
    }
  }

  rp.setIsOk( true );
  rp.setNombreCartesJeu( nbcartes );
  rp.setIndexCpj( icpj );
  return rp;
}

CRetourPose CBaseJoueurs::inserer1CListesPoses( QList<quint8> & qlpose, QList<quint8> & qlflag, quint8 * acpj, int icpj, quint8 * ajeu, int nbcartes, quint8 carte )
{
  CRetourPose rp;
  quint8 c_a_tester = carte;

  int  indexF0 = qlflag.indexOf ( c_a_tester, 0 );
  if( indexF0 != -1 )   /// si trouvé
  {
    int  indexF1 = qlflag.indexOf ( c_a_tester, indexF0 + 1 );
    int  indexF2 = qlflag.indexOf ( c_a_tester, indexF0 + 2 );
    if( indexF1 == indexF0 + 1 && indexF2 == indexF0 + 2 )
    {
/// cas 3 memes
      qlpose.insert( indexF0, c_a_tester );
      qlflag.insert( indexF0, 0 );
      qlflag.replace( indexF0 + 1, 0 );
      qlflag.replace( indexF0 + 2, 0 );
      qlflag.replace( indexF0 + 3, 0 );
    }
    else
    {
/// cas suites
      indexF1 = qlflag.indexOf ( RIENCARTE, indexF0 + 1 );
      int diff = indexF1 - indexF0;
      if( diff > 1 )
      {
/// cas suites : début
        qlpose.insert( indexF0, c_a_tester );
        c_a_tester = c_a_tester - 1;
        if( c_a_tester == 0 || c_a_tester == 20 || c_a_tester == 40 || c_a_tester == 60 ) /// si AS
        {
          c_a_tester = 0;
        }
        qlflag.insert( indexF0, c_a_tester );
        qlflag.replace( indexF0 + 1, 0 );

      }
      else
      {
/// cas suites : fin
        qlpose.insert( indexF0 + 1, c_a_tester );
        c_a_tester = c_a_tester + 1;
        if( c_a_tester == 14 || c_a_tester == 34 || c_a_tester == 54 || c_a_tester == 74 )  /// si R
        {
          c_a_tester = c_a_tester - 13;
        }
        if( c_a_tester == 2 || c_a_tester == 22 || c_a_tester == 42 || c_a_tester == 62 ) /// si AS
        {
          c_a_tester = 0;
        }
        qlflag.insert( indexF0 + 1, c_a_tester );
        qlflag.replace( indexF0, 0 );
      }
    }
    acpj[ icpj++ ] = carte;

/// Enleve carte ds jeu
    enlever1Carte( ajeu, carte );
    nbcartes -= 1;
  }

  rp.setIsOk( true );
  rp.setNombreCartesJeu( nbcartes );
  rp.setIndexCpj( icpj );
  return rp;
}

CValeurCarte CBaseJoueurs::calculerCouleurIndex1Carte( quint8 carte )
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

int CBaseJoueurs::calculerTotal4Cartes( quint8 valeurcarte )
{
  int total = 0;
  int val = valeurcarte - 1;
  const int table[ 13 ] = { 124,128,132,136,140,144,148,152,156,160,164,168,172 };
  total = table[ val ];
  return total;
}

int CBaseJoueurs::calculerIndex1Carte( quint8 * ajeu, quint8 carte )
{
  int i = 0;
  while( ajeu[ i ] != MARKFIN )
  {
    if( ajeu[ i ] == carte )  break;
    i++;
  }
  return i;
}

void CBaseJoueurs::enlever1Carte( quint8 * ajeu, quint8 carte )
{
  int i = 0;
  while( ajeu[ i ] != MARKFIN )
  {
    if( ajeu[ i ] == carte )
    {
      ajeu[ i ] = PASCARTE;
      break;
    }
    i++;
  }
}

int CBaseJoueurs::calculerPenaliteCartesJeu( const quint8 *aJeu, const int nbcarte )
{
  int penalite = 0;
  int numcarte, carte;
  for( int i = 0; i < nbcarte; i++ )
  {
    numcarte = aJeu[i];
    carte = numcarte;
    if( carte > 60 && carte < 80 )
    {
      numcarte = numcarte - 61;
    }
    if( carte > 40 && carte < 60 )
    {
      numcarte = numcarte - 41;
    }
    if( carte > 20 && carte < 40 )
    {
      numcarte = numcarte - 21;
    }
    if( carte > 0 && carte < 20 )
    {
      numcarte = numcarte - 1;
    }
    if( carte > 0 )
    {
      penalite = penalite + tablepenalitecarte[ numcarte ];
    }
  }
  return penalite;
}

void CBaseJoueurs::calculerOrdreJeu( const int numordre, const int nombredejoueur, quint8 *ordrejeu )
{
  int o = numordre;
  o = o * 4;
  if( nombredejoueur == 2)
  {
    for(int i = 0; i < 4; i++)
    {
      ordrejeu[i] = ordre2joueurs[o];
      o += 1;
    }
  }
  if( nombredejoueur == 3)
  {
    for(int i = 0; i < 4; i++)
    {
      ordrejeu[i] = ordre3joueurs[o];
      o += 1;
    }
  }
  if( nombredejoueur == 4)
  {
    for(int i = 0; i < 4; i++)
    {
      ordrejeu[i] = ordre4joueurs[o];
      o += 1;
    }
  }
}
