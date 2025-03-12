#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTime>
#include <QMessageBox>
#include <QCloseEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QPen>
#include <QEventLoop>

#include "crami.h"

#include "sdlplayer.h"
#include "cafficheurimage.h"
#include "cbasejoueurs.h"
#include "miseaupoint.h"
#include "ctricher.h"
#include "cresultat.h"
#include "cretourpose.h"


CRami::CRami(QWidget *parent)
 : QWidget(parent)
{
/// creation Layout
    loadImage( ":/images/Plateau.png", &m_imgPlateau, 1469, 1128 );
    m_imgMemPlateau = m_imgPlateau;
    m_afficheurImage = new CAfficheurImage( this, 1469, 1128 );
    m_afficheurImage->setImage( m_imgMemPlateau );

    QWidget *widget;
    QVBoxLayout *vboxLayout;
    QHBoxLayout *hboxLayout1;

    widget = new QWidget(this);

    vboxLayout = new QVBoxLayout(widget);
    vboxLayout->setSpacing(6);
/// qt6    vboxLayout->setMargin(0);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));

    vboxLayout->addWidget(m_afficheurImage);

    hboxLayout1 = new QHBoxLayout();
    hboxLayout1->setSpacing(6);
/// qt6    hboxLayout1->setMargin(0);
    hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));

    QFont font;
    font.setPointSize(10);
/// qt6    font.setWeight(QFont::Weight( 50 ));
    font.setWeight(QFont::Medium);

    m_gpSudLabel = new QLabel(this);
    m_gpSudLabel->setFont(font);
    m_gpSudLabel->setFrameShape(QFrame::WinPanel);
    m_gpSudLabel->setFrameShadow(QFrame::Sunken);
    m_gpSudLabel->setText(" ");

    hboxLayout1->addWidget(m_gpSudLabel);

    m_gpEstLabel = new QLabel(this);
    m_gpEstLabel->setFont(font);
    m_gpEstLabel->setFrameShape(QFrame::WinPanel);
    m_gpEstLabel->setFrameShadow(QFrame::Sunken);
    m_gpEstLabel->setText(" ");

    hboxLayout1->addWidget(m_gpEstLabel);

    m_gpNordLabel = new QLabel(this);
    m_gpNordLabel->setFont(font);
    m_gpNordLabel->setFrameShape(QFrame::WinPanel);
    m_gpNordLabel->setFrameShadow(QFrame::Sunken);
    m_gpNordLabel->setText(" ");

    hboxLayout1->addWidget(m_gpNordLabel);

    m_gpOuestLabel = new QLabel(this);
    m_gpOuestLabel->setFont(font);
    m_gpOuestLabel->setFrameShape(QFrame::WinPanel);
    m_gpOuestLabel->setFrameShadow(QFrame::Sunken);
    m_gpOuestLabel->setText(" ");

    hboxLayout1->addWidget(m_gpOuestLabel);

    vboxLayout->addLayout(hboxLayout1);

    QPalette palette;
    QBrush brush(QColor(30, 255, 0, 255));
    brush.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::WindowText, brush);

    font.setPointSize(48);
/// qt6    font.setBold(true);
/// qt6    font.setWeight(QFont::Weight( 75 ));
    font.setWeight(QFont::Bold);

    m_nbCarteOuestLabel = new QLabel(this);
    m_nbCarteOuestLabel->setGeometry(QRect(73, 62, 86, 86));
    m_nbCarteOuestLabel->setPalette(palette);
    m_nbCarteOuestLabel->setFont(font);

    m_nbCarteNordLabel = new QLabel(this);
    m_nbCarteNordLabel->setGeometry(QRect(709, 62, 86, 86));
    m_nbCarteNordLabel->setPalette(palette);
    m_nbCarteNordLabel->setFont(font);

    m_nbCarteEstLabel = new QLabel(this);
    m_nbCarteEstLabel->setGeometry(QRect(1341, 62, 86, 86));
    m_nbCarteEstLabel->setPalette(palette);
    m_nbCarteEstLabel->setFont(font);

    m_nbCarteSudLabel = new QLabel(this);
    m_nbCarteSudLabel->setGeometry(QRect(1380, 1020, 86, 86));
    m_nbCarteSudLabel->setPalette(palette);
    m_nbCarteSudLabel->setFont(font);

/// Création Objets(Classes) Utiles

    m_pBaseJoueurs = new CBaseJoueurs();

/// Initialise Images et Son

    loadImage( ":/images/DessinCartes.png", &m_imgDessinCartes, 1196, 675 );

    m_mySDLPlayer = new SDLPlayer();

/// Init Variables
    afficherImageDebutJeu();

/// qt6    qsrand ( time (0) );
    srand ( time (0) );

    m_rectpioche = QRect(634,833,90,131);
    m_rectjetees = QRect(742,833,90,131);
    m_rectcartes = QRect(90,990,1230,131);
    m_rectposees = QRect(5,139,1459,680);

    m_periodeJeu = ATTENTE;
    m_f_EtatPartie = false;
    m_f_timerActif = false;
}

void CRami::debuterJeu()
{
    m_imgMemPlateau = m_imgPlateau;
    m_afficheurImage->afficherImage( m_imgMemPlateau );

    afficherDessinJoueurs();

    m_numordre = 0;
    m_index_ds_ordrejeu = 0;

    m_penalite_Sud = 0;
    m_gagnee_Sud = 0;
    m_penalite_Nord = 0;
    m_gagnee_Nord = 0;
    m_penalite_Est = 0;
    m_gagnee_Est = 0;
    m_penalite_Ouest = 0;
    m_gagnee_Ouest = 0;
    afficher_g_p_Sud();
    afficher_g_p_Nord();
    afficher_g_p_Est();
    afficher_g_p_Ouest();

    m_qlresultat.clear();

    melangerCartes();

    m_periodeJeu = DEBUT1PARTIE;
    m_f_EtatPartie = true;
    m_f_timerActif = true;
    m_timerId = startTimer( m_tempo_timer );
}

void CRami::timerEvent( QTimerEvent * event )
{
  if( event->timerId() == m_timerId )
  {
    killTimer( m_timerId );
    m_f_timerActif = false;
    switch( m_periodeJeu )
    {
      case DEBUT1PARTIE:
        Jeu_Debut_1_Partie();
        break;
      case JOUEURSJOUE:
        Ordi_Joueurs_Joue();
        break;
      case FIN1PARTIE:
        Jeu_Fin_1_Partie();
        break;

      default:
        break;
    }
    if( m_quijoue == SUD && m_periodeJeu == JOUEURSJOUE )
    {
      afficherMsgSB( "À joueur SUD de jouer" );
      /// et attend event souris...
    }
    else
    {
      if( m_f_Fin1partie == false )
      {
        m_f_timerActif = true;
        m_timerId = startTimer( m_tempo_timer );
      }
      /// sinon attend event souris...
    }
  }
  else
  {
    QWidget::timerEvent( event );
  }
}


void CRami::Jeu_Debut_1_Partie()
{
    afficherMsgSB( "Début d'une partie" );

    m_imgMemPlateau = m_imgPlateau;
    m_afficheurImage->afficherImage( m_imgMemPlateau );

    afficherDessinJoueurs();

    m_quigagne = PERSONNE;
    m_quijoue = PERSONNE;
    m_pBaseJoueurs->calculerOrdreJeu( m_numordre, m_nombredejoueur, m_ordrejeu );
    initialiserJeuJoueurs();
    initialiserCartesSelect_Sud();
    distribuerCartes();
    initialiserNbCartesJoueurs();
    afficherNbCartesJoueurs();
    afficherCartesJeuSud();
    afficherNbCartesSud();
    initialiserCartesJetees();
    initialiserCartesPosees();
    afficherDosCartePioche();

    m_f_a_pose_Sud = false;
    m_f_a_pose_Est = false;
    m_f_a_pose_Nord = false;
    m_f_a_pose_Ouest = false;

    initialiserVariablesPoseJoueurs();

    m_f_a_parle_Sud = false;
    m_f_a_parle_Est = false;
    m_f_a_parle_Nord = false;
    m_f_a_parle_Ouest = false;

    initialiserVarJoue_Sud();

    m_nombredetour = 0;
    m_f_Fin1partie = false;
    m_quijoue = m_ordrejeu[m_index_ds_ordrejeu];
    m_periodeJeu = JOUEURSJOUE;

    m_nbpaspose_Est = 0;
    m_nbpaspose_Nord = 0;
    m_nbpaspose_Ouest = 0;
}

void CRami::Ordi_Joueurs_Joue()
{
  switch( m_quijoue )
  {
    case EST:
      if( true == Ordi_Est_Jouer() )
      {
        jouerSonGagne( EST );
        m_quigagne = EST;
        m_periodeJeu = FIN1PARTIE;
        m_f_timerActif = true;
        m_timerId = startTimer( m_tempo_timer );
      }
      else
      {
        m_nombredetour += 1;
        if( m_nombredejoueur == 3 )
        {
          m_quijoue = OUEST;
        }
        else
        {
          m_quijoue = NORD;			/// si 4 joueurs
        }
      }
      break;
    case NORD:
      if( true == Ordi_Nord_Jouer() )
      {
        jouerSonGagne( NORD );
        m_quigagne = NORD;
        m_periodeJeu = FIN1PARTIE;
        m_f_timerActif = true;
        m_timerId = startTimer( m_tempo_timer );
      }
      else
      {
        m_nombredetour += 1;
        if( m_nombredejoueur == 2 )
        {
    /// @todo Routine indication peut poser
          initialiserVarJoue_Sud();
          m_quijoue = SUD;
        }
        else
        {
          m_quijoue = OUEST;			/// si 4 joueurs
        }
      }
      break;
    case OUEST:
      if( true == Ordi_Ouest_Jouer() )
      {
        jouerSonGagne( OUEST );
        m_quigagne = OUEST;
        m_periodeJeu = FIN1PARTIE;
        m_f_timerActif = true;
        m_timerId = startTimer( m_tempo_timer );
      }
      else
      {
        m_nombredetour += 1;
    /// @todo Routine indication peut poser
        initialiserVarJoue_Sud();
        m_quijoue = SUD;
      }
      break;
    default:
      break;
  }
}

void CRami::Jeu_Fin_1_Partie()
{
    afficherMsgSB( "Fin d'une partie." );

    int gS, ptS, gE, ptE, gN, ptN, gO, ptO;
    gS = ptS = gE = ptE = gN = ptN = gO = ptO = 0;
    QString nom;

    if( m_quigagne == SUD )
    {
      nom = "SUD";
      gS = 1;
    }
    if( m_quigagne == EST )
    {
      nom = "EST";
      gE = 1;
    }
    if( m_quigagne == NORD )
    {
      nom = "NORD";
      gN = 1;
    }
    if( m_quigagne == OUEST )
    {
      nom = "OUEST";
      gO = 1;
    }

    QString msgquigagne = nom + " A GAGNÉ !";

    faireTemporisation( m_tempo_finpartie );

    effacerCartePioche();
    effacerCarteJetee();
    effacerCartesPosees();
    effacerCarreJoueursOrdi();
    effacerCartesJeuSud();
    effacerNbCartesJoueurs();
    afficherMsgQuiGagne( msgquigagne );

    QString penalite;

    m_pBaseJoueurs->trierJeu( m_jeu_Est );
    ptE = calculerPenaliteCartesJeuEst();
    if( ptE != 0 )
    {
      nom = "EST";
      afficherMsgNom( nom, 10, 171 );
      afficherCartesJeuEst();
      penalite = QString( "%1" ).arg( ptE );
      afficherMsgPenalite( penalite, 10, 171 );
      faireTemporisation( m_tempo_attente );
    }

    m_pBaseJoueurs->trierJeu( m_jeu_Nord );
    ptN = calculerPenaliteCartesJeuNord();
    if( ptN != 0 )
    {
      nom = "NORD";
      afficherMsgNom( nom, 10, 333 );
      afficherCartesJeuNord();
      penalite = QString( "%1" ).arg( ptN );
      afficherMsgPenalite( penalite, 10, 333 );
      faireTemporisation( m_tempo_attente );
    }

    m_pBaseJoueurs->trierJeu( m_jeu_Ouest );
    ptO = calculerPenaliteCartesJeuOuest();
    if( ptO != 0 )
    {
      nom = "OUEST";
      afficherMsgNom( nom, 10, 495 );
      afficherCartesJeuOuest();
      penalite = QString( "%1" ).arg( ptO );
      afficherMsgPenalite( penalite, 10, 495 );
      faireTemporisation( m_tempo_attente );
    }

    m_pBaseJoueurs->trierJeu( m_jeu_Sud );
    ptS = calculerPenaliteCartesJeuSud();
    if( ptS != 0 )
    {
      nom = "SUD";
      afficherMsgNom( nom, 10, 657 );
      afficherCartesJeu( 250, 657, m_jeu_Sud, 13 );
      penalite = QString( "%1" ).arg( ptS );
      afficherMsgPenalite( penalite, 10, 657 );
      faireTemporisation( m_tempo_attente );
    }

    m_penalite_Sud += ptS;
    m_gagnee_Sud += gS;
    m_penalite_Est += ptE;
    m_gagnee_Est += gE;
    m_penalite_Nord += ptN;
    m_gagnee_Nord += gN;
    m_penalite_Ouest += ptO;
    m_gagnee_Ouest += gO;
    afficher_g_p_Sud();
    afficher_g_p_Est();
    afficher_g_p_Nord();
    afficher_g_p_Ouest();

    CResultat resultat;
    resultat.penaliteSud = ptS;
    resultat.penaliteEst = ptE;
    resultat.penaliteNord = ptN;
    resultat.penaliteOuest = ptO;
    m_qlresultat.append( resultat );

    rassemblerCartes();
    m_numordre += 1;
    if( m_numordre >= m_nombredejoueur )
    {
      m_numordre = 0;
    }

    afficherMsgSB( "Fin d'une partie. Cliquer pour continuer." );
    m_f_Fin1partie = true;
}


void CRami::afficher_g_p_Sud()
{
    QString message;
    message = QString("S : Gagnée=%1  Pénalité=%2").arg( m_gagnee_Sud ).arg( m_penalite_Sud );
    m_gpSudLabel->setText( message );
}


void CRami::afficher_g_p_Nord()
{
    if( m_nombredejoueur == 2 || m_nombredejoueur == 4 )
    {
      QString message;
      message = QString("N : Gagnée=%1  Pénalité=%2").arg( m_gagnee_Nord ).arg( m_penalite_Nord );
      m_gpNordLabel->setText( message );
    }
    else
    {
      m_gpNordLabel->setText( " " );
    }
}


void CRami::afficher_g_p_Est()
{
    if( m_nombredejoueur == 3 || m_nombredejoueur == 4 )
    {
      QString message;
      message = QString("E : Gagnée=%1  Pénalité=%2").arg( m_gagnee_Est ).arg( m_penalite_Est );
      m_gpEstLabel->setText( message );
    }
    else
    {
      m_gpEstLabel->setText( " " );
    }
}


void CRami::afficher_g_p_Ouest()
{
    if( m_nombredejoueur == 3 || m_nombredejoueur == 4 )
    {
      QString message;
      message = QString("O : Gagnée=%1  Pénalité=%2").arg( m_gagnee_Ouest ).arg( m_penalite_Ouest );
      m_gpOuestLabel->setText( message );
    }
    else
    {
      m_gpOuestLabel->setText( " " );
    }
}


void CRami::effacerCartesJeuSud()
{
    loadImage( ":/images/FondCartesJoueur.png", &m_imgFondCartesJoueur, 1459, 145 );

    m_imgCopie = m_imgMemPlateau;
    QPainter painter(&m_imgCopie);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawImage(5, 977, m_imgFondCartesJoueur);
    painter.end();
    m_imgMemPlateau = m_imgCopie;
    m_afficheurImage->afficherImage( m_imgMemPlateau );
}

void CRami::effacerCartePioche()
{
    loadImage( ":/images/FondPioche.png", &m_imgFondPioche, 104, 145 );

    m_imgCopie = m_imgMemPlateau;
    QPainter painter(&m_imgCopie);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawImage(628, 827, m_imgFondPioche);
    painter.end();
    m_imgMemPlateau = m_imgCopie;
    m_afficheurImage->afficherImage( m_imgMemPlateau );
}

void CRami::effacerCarteJetee()
{
    loadImage( ":/images/FondJetee.png", &m_imgFondJetee, 101, 144 );

    m_imgCopie = m_imgMemPlateau;
    QPainter painter(&m_imgCopie);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawImage(736, 827, m_imgFondJetee);
    painter.end();
    m_imgMemPlateau = m_imgCopie;
    m_afficheurImage->afficherImage( m_imgMemPlateau );
}

void CRami::effacerCartesPosees()
{
    loadImage( ":/images/FondCartesPosees.png", &m_imgFondCartesPosees, 1459, 681 );

    m_imgCopie = m_imgMemPlateau;
    QPainter painter(&m_imgCopie);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawImage(5, 140, m_imgFondCartesPosees);
    painter.end();
    m_imgMemPlateau = m_imgCopie;
    m_afficheurImage->afficherImage( m_imgMemPlateau );
}

void CRami::effacerCarreJoueursOrdi()
{
    loadImage( ":/images/FondJoueursOrdi.png", &m_imgFondJoueursOrdi, 1459, 129 );

    m_imgCopie = m_imgMemPlateau;
    QPainter painter(&m_imgCopie);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawImage(5, 5, m_imgFondJoueursOrdi);
    painter.end();
    m_imgMemPlateau = m_imgCopie;
    m_afficheurImage->afficherImage( m_imgMemPlateau );
}

void CRami::melangerCartes()
{
    int copiejeu[NBTOTCARTES];
    for(int i = 0; i < NBTOTCARTES; i++)
    {
      copiejeu[i] = jeucartesorg[i];	/// recopie
    }

    int carte;
    int index;
    for(int nbcarte = 0; nbcarte < NBTOTCARTES; nbcarte++)
    {
      do
      {
        index = genererHasard_0_103();
        carte = copiejeu[index];
      }while(carte == -1);
      m_jeucartes[nbcarte] = carte;
      copiejeu[index] = -1;
    }
    m_jeucartes[104] = MARKFIN;
    m_jeucartes[105] = MARKFIN;
}

int CRami::genererHasard_0_103()
{
/// qt6    quint16 hasard = 10 + (int) (1040.0*qrand()/(RAND_MAX+1.0));	/// de 10 a 1040
    quint16 hasard = 10 + (int) (1040.0*rand()/(RAND_MAX+1.0));	/// de 10 a 1040
    hasard = hasard / 10;					/// de 1 à 104
    if ( hasard > 0 ) hasard -= 1;			/// de 0 à 103
    if ( hasard >= 104 ) hasard = 103;
    return	hasard;
}

void CRami::distribuerCartes()
{
/// COUPE
    quint8 copiejeu[NBTOTCARTES];
    for(int i = 0; i < 52; i++)
    {
      copiejeu[i] = m_jeucartes[i];
    }
    for(int i = 0; i < 52; i++)
    {
      m_jeucartes[i] = m_jeucartes[i + 52];
    }
    for(int i = 0; i < 52; i++)
    {
      m_jeucartes[i + 52] = copiejeu[i];
    }

/// DISTRIBUE
    m_index_carte_a_piocher = 0;
    if( m_nombredejoueur == 2 && m_numordre == 0 )
    {
      for(int i = 0; i < 13; i++)
      {
        m_jeu_Sud[i] = m_jeucartes[m_index_carte_a_piocher];
        m_jeucartes[m_index_carte_a_piocher] = PASCARTE;
        m_index_carte_a_piocher += 1;
        m_jeu_Nord[i] = m_jeucartes[m_index_carte_a_piocher];
        m_jeucartes[m_index_carte_a_piocher] = PASCARTE;
        m_index_carte_a_piocher += 1;
      }
    }
    if( m_nombredejoueur == 2 && m_numordre == 1 )
    {
      for(int i = 0; i < 13; i++)
      {
        m_jeu_Nord[i] = m_jeucartes[m_index_carte_a_piocher];
        m_jeucartes[m_index_carte_a_piocher] = PASCARTE;
        m_index_carte_a_piocher += 1;
        m_jeu_Sud[i] = m_jeucartes[m_index_carte_a_piocher];
        m_jeucartes[m_index_carte_a_piocher] = PASCARTE;
        m_index_carte_a_piocher += 1;
      }
    }
    if( m_nombredejoueur == 3 && m_numordre == 0 )
    {
      for(int i = 0; i < 13; i++)
      {
        m_jeu_Sud[i] = m_jeucartes[m_index_carte_a_piocher];
        m_jeucartes[m_index_carte_a_piocher] = PASCARTE;
        m_index_carte_a_piocher += 1;
        m_jeu_Est[i] = m_jeucartes[m_index_carte_a_piocher];
        m_jeucartes[m_index_carte_a_piocher] = PASCARTE;
        m_index_carte_a_piocher += 1;
        m_jeu_Ouest[i] = m_jeucartes[m_index_carte_a_piocher];
        m_jeucartes[m_index_carte_a_piocher] = PASCARTE;
        m_index_carte_a_piocher += 1;
      }
    }
    if( m_nombredejoueur == 3 && m_numordre == 1 )
    {
      for(int i = 0; i < 13; i++)
      {
        m_jeu_Est[i] = m_jeucartes[m_index_carte_a_piocher];
        m_jeucartes[m_index_carte_a_piocher] = PASCARTE;
        m_index_carte_a_piocher += 1;
        m_jeu_Ouest[i] = m_jeucartes[m_index_carte_a_piocher];
        m_jeucartes[m_index_carte_a_piocher] = PASCARTE;
        m_index_carte_a_piocher += 1;
        m_jeu_Sud[i] = m_jeucartes[m_index_carte_a_piocher];
        m_jeucartes[m_index_carte_a_piocher] = PASCARTE;
        m_index_carte_a_piocher += 1;
      }
    }
    if( m_nombredejoueur == 3 && m_numordre == 2 )
    {
      for(int i = 0; i < 13; i++)
      {
        m_jeu_Ouest[i] = m_jeucartes[m_index_carte_a_piocher];
        m_jeucartes[m_index_carte_a_piocher] = PASCARTE;
        m_index_carte_a_piocher += 1;
        m_jeu_Sud[i] = m_jeucartes[m_index_carte_a_piocher];
        m_jeucartes[m_index_carte_a_piocher] = PASCARTE;
        m_index_carte_a_piocher += 1;
        m_jeu_Est[i] = m_jeucartes[m_index_carte_a_piocher];
        m_jeucartes[m_index_carte_a_piocher] = PASCARTE;
        m_index_carte_a_piocher += 1;
      }
    }
    if( m_nombredejoueur == 4 && m_numordre == 0 )
    {
      for(int i = 0; i < 13; i++)
      {
        m_jeu_Sud[i] = m_jeucartes[m_index_carte_a_piocher];
        m_jeucartes[m_index_carte_a_piocher] = PASCARTE;
        m_index_carte_a_piocher += 1;
        m_jeu_Est[i] = m_jeucartes[m_index_carte_a_piocher];
        m_jeucartes[m_index_carte_a_piocher] = PASCARTE;
        m_index_carte_a_piocher += 1;
        m_jeu_Nord[i] = m_jeucartes[m_index_carte_a_piocher];
        m_jeucartes[m_index_carte_a_piocher] = PASCARTE;
        m_index_carte_a_piocher += 1;
        m_jeu_Ouest[i] = m_jeucartes[m_index_carte_a_piocher];
        m_jeucartes[m_index_carte_a_piocher] = PASCARTE;
        m_index_carte_a_piocher += 1;
      }
    }
    if( m_nombredejoueur == 4 && m_numordre == 1 )
    {
      for(int i = 0; i < 13; i++)
      {
        m_jeu_Est[i] = m_jeucartes[m_index_carte_a_piocher];
        m_jeucartes[m_index_carte_a_piocher] = PASCARTE;
        m_index_carte_a_piocher += 1;
        m_jeu_Nord[i] = m_jeucartes[m_index_carte_a_piocher];
        m_jeucartes[m_index_carte_a_piocher] = PASCARTE;
        m_index_carte_a_piocher += 1;
        m_jeu_Ouest[i] = m_jeucartes[m_index_carte_a_piocher];
        m_jeucartes[m_index_carte_a_piocher] = PASCARTE;
        m_index_carte_a_piocher += 1;
        m_jeu_Sud[i] = m_jeucartes[m_index_carte_a_piocher];
        m_jeucartes[m_index_carte_a_piocher] = PASCARTE;
        m_index_carte_a_piocher += 1;
      }
    }
    if( m_nombredejoueur == 4 && m_numordre == 2 )
    {
      for(int i = 0; i < 13; i++)
      {
        m_jeu_Nord[i] = m_jeucartes[m_index_carte_a_piocher];
        m_jeucartes[m_index_carte_a_piocher] = PASCARTE;
        m_index_carte_a_piocher += 1;
        m_jeu_Ouest[i] = m_jeucartes[m_index_carte_a_piocher];
        m_jeucartes[m_index_carte_a_piocher] = PASCARTE;
        m_index_carte_a_piocher += 1;
        m_jeu_Sud[i] = m_jeucartes[m_index_carte_a_piocher];
        m_jeucartes[m_index_carte_a_piocher] = PASCARTE;
        m_index_carte_a_piocher += 1;
        m_jeu_Est[i] = m_jeucartes[m_index_carte_a_piocher];
        m_jeucartes[m_index_carte_a_piocher] = PASCARTE;
        m_index_carte_a_piocher += 1;
      }
    }
    if( m_nombredejoueur == 4 && m_numordre == 3 )
    {
      for(int i = 0; i < 13; i++)
      {
        m_jeu_Ouest[i] = m_jeucartes[m_index_carte_a_piocher];
        m_jeucartes[m_index_carte_a_piocher] = PASCARTE;
        m_index_carte_a_piocher += 1;
        m_jeu_Sud[i] = m_jeucartes[m_index_carte_a_piocher];
        m_jeucartes[m_index_carte_a_piocher] = PASCARTE;
        m_index_carte_a_piocher += 1;
        m_jeu_Est[i] = m_jeucartes[m_index_carte_a_piocher];
        m_jeucartes[m_index_carte_a_piocher] = PASCARTE;
        m_index_carte_a_piocher += 1;
        m_jeu_Nord[i] = m_jeucartes[m_index_carte_a_piocher];
        m_jeucartes[m_index_carte_a_piocher] = PASCARTE;
        m_index_carte_a_piocher += 1;
      }
    }
}

void CRami::rassemblerCartes()
{
    quint8 carte;
    int indextas, indexjeu;
    indexjeu = 0;
    quint8 copiejeu[106];
    copiejeu[104] = MARKFIN;
    copiejeu[105] = MARKFIN;

///	CARTES A PIOCHER
    indextas = m_index_carte_a_piocher;
    while( true )
    {
      carte = m_jeucartes[ indextas ];
      if( carte == MARKFIN )	break;
      if( carte != PASCARTE )
      {
        copiejeu[ indexjeu ] = carte;
        indexjeu += 1;
      }
      indextas += 1;
    }

///	CARTES JETEES
    indextas = m_index_carte_jetee;
    while( m_cartesjetees[ indextas ] != MARKFIN )
    {
      indextas += 1;
    }
    indextas -= 1;
    while( indextas > m_index_carte_jetee )
    {
      carte = m_cartesjetees[ indextas ];
      if( carte == PASCARTE )	break;
      if( carte != PASCARTE )
      {
        copiejeu[ indexjeu ] = carte;
        indexjeu += 1;
      }
      indextas -= 1;
    }

///	CARTES POSEES
    indextas = 0;
    while( true )
    {
      carte = m_cartesposees[ indextas ];
      if( carte == MARKFIN )	break;
      if( carte != PASCARTE )
      {
        copiejeu[ indexjeu ] = carte;
        indexjeu += 1;
      }
      indextas += 1;
    }

///	CARTES JEU SUD
    indextas = 0;
    while( true )
    {
      carte = m_jeu_Sud[ indextas ];
      if( carte == MARKFIN )	break;
      if( carte != PASCARTE )
      {
        copiejeu[ indexjeu ] = carte;
        indexjeu += 1;
      }
      indextas += 1;
    }

///	CARTES JEU EST
    indextas = 0;
    while( true )
    {
      carte = m_jeu_Est[ indextas ];
      if( carte == MARKFIN )	break;
      if( carte != PASCARTE )
      {
        copiejeu[ indexjeu ] = carte;
        indexjeu += 1;
      }
      indextas += 1;
    }

///	CARTES JEU NORD
    indextas = 0;
    while( true )
    {
      carte = m_jeu_Nord[ indextas ];
      if( carte == MARKFIN )	break;
      if( carte != PASCARTE )
      {
        copiejeu[ indexjeu ] = carte;
        indexjeu += 1;
      }
      indextas += 1;
    }

///	CARTES JEU OUEST
    indextas = 0;
    while( true )
    {
      carte = m_jeu_Ouest[ indextas ];
      if( carte == MARKFIN )	break;
      if( carte != PASCARTE )
      {
        copiejeu[indexjeu] = carte;
        indexjeu += 1;
      }
      indextas += 1;
    }

    copierTableau( copiejeu, m_jeucartes, 104 );
}

void CRami::initialiserJeuJoueurs()
{
    for(int i = 0; i < 14; i++)
    {
      m_jeu_Ouest[i] = PASCARTE;
      m_jeu_Sud[i] = PASCARTE;
      m_jeu_Est[i] = PASCARTE;
      m_jeu_Nord[i] = PASCARTE;
    }
    m_jeu_Ouest[14] = MARKFIN;
    m_jeu_Sud[14] = MARKFIN;
    m_jeu_Est[14] = MARKFIN;
    m_jeu_Nord[14] = MARKFIN;

    m_jeu_Ouest[15] = MARKFIN;
    m_jeu_Sud[15] = MARKFIN;
    m_jeu_Est[15] = MARKFIN;
    m_jeu_Nord[15] = MARKFIN;
}

void CRami::initialiserCartesSelect_Sud()
{
    for(int i = 0; i < 14; i++)
    {
      m_cartesSelect_Sud[i] = PASCARTE;
    }
    m_nbcartesSelect_Sud = 0;
}

void CRami::initialiserNbCartesJoueurs()
{
    if(m_nombredejoueur == 2)
    {
      m_nbcartes_Sud = 13;
      m_nbcartes_Nord = 13;
      m_nbcartes_Est = 0;
      m_nbcartes_Ouest = 0;
    }
    if(m_nombredejoueur == 3)
    {
      m_nbcartes_Sud = 13;
      m_nbcartes_Nord = 0;
      m_nbcartes_Est = 13;
      m_nbcartes_Ouest = 13;
    }
    if(m_nombredejoueur == 4)
    {
      m_nbcartes_Sud = 13;
      m_nbcartes_Nord = 13;
      m_nbcartes_Est = 13;
      m_nbcartes_Ouest = 13;
    }
}

void CRami::afficherNbCartesJoueurs()
{
    QString msgN, msgE, msgO;

    if(m_nombredejoueur == 2)
    {
      msgE = QString("");
      msgN = QString("%1").arg( m_nbcartes_Nord );
      msgO = QString("");
    }
    if(m_nombredejoueur == 3)
    {
      msgE = QString("%1").arg( m_nbcartes_Est );
      msgN = QString("");
      msgO = QString("%1").arg( m_nbcartes_Ouest );
    }
    if(m_nombredejoueur == 4)
    {
      msgE = QString("%1").arg( m_nbcartes_Est );
      msgN = QString("%1").arg( m_nbcartes_Nord );
      msgO = QString("%1").arg( m_nbcartes_Ouest );
    }
    m_nbCarteNordLabel->setText( msgN );
    m_nbCarteEstLabel->setText( msgE );
    m_nbCarteOuestLabel->setText( msgO );
}

void CRami::afficherNbCartesEst()
{
    QString msg = QString("%1").arg( m_nbcartes_Est );
    m_nbCarteEstLabel->setText( msg );
}

void CRami::afficherNbCartesNord()
{
    QString msg = QString("%1").arg( m_nbcartes_Nord );
    m_nbCarteNordLabel->setText( msg );
}

void CRami::afficherNbCartesOuest()
{
    QString msg = QString("%1").arg( m_nbcartes_Ouest );
    m_nbCarteOuestLabel->setText( msg );
}

void CRami::afficherNbCartesSud()
{
    QString msg = QString("%1").arg( m_nbcartes_Sud );
    m_nbCarteSudLabel->setText( msg );
}

void CRami::effacerNbCartesJoueurs()
{
    QString msgN, msgE, msgO, msgS;

    msgE = QString("");
    msgN = QString("");
    msgO = QString("");
    msgS = QString("");
    m_nbCarteSudLabel->setText( msgS );
    m_nbCarteNordLabel->setText( msgN );
    m_nbCarteEstLabel->setText( msgE );
    m_nbCarteOuestLabel->setText( msgO );
}


void CRami::afficherCartesJeuEst()
{
    afficherCartesJeu( 250, 171, m_jeu_Est, 13 );
}


void CRami::afficherCartesJeuNord()
{
    afficherCartesJeu( 250, 333, m_jeu_Nord, 13 );
}


void CRami::afficherCartesJeuOuest()
{
    afficherCartesJeu( 250, 495, m_jeu_Ouest, 13 );
}

void CRami::afficherCartesJeu( const int x, const int y, const quint8 *aJeu, const int nbcarte)
{
    int dx = x;
    int dy = y;
    QPoint p;
    int carte;
    for( int i = 0; i < nbcarte; i++ )
    {
      carte = aJeu[i];
      if( carte > 0 )
      {
        p = calculerSourceXY1Carte( carte );
        afficherUneCarte( dx, dy, p.x(), p.y() );
      }
      dx= dx + 91;
    }
}

void CRami::afficherCartesJeuSud()
{
    effacerCartesJeuSud();
    int dx = 90;
    int dy;
    QPoint p;
    int carte;

    for( int i = 0; i < 14; i++ )
    {
      carte = m_jeu_Sud[i];
      if( carte > 0 )
      {
        dy = 990;
        if( m_cartesSelect_Sud[ i ] != PASCARTE )	dy = 978;
        p = calculerSourceXY1Carte( carte );
        afficherUneCarte( dx, dy, p.x(), p.y() );
      }
      dx= dx + 91;
    }
}

void CRami::afficherDosCartePioche()
{
    afficherUneCarte( 634, 833, 0, 540 );
}

void CRami::afficherUneCarte( const int dx, const int dy, const int carte )
{
    QPoint p;
    if( carte == DOSCARTE )
    {
      p.setX( 0 );
      p.setY( 540 );
    }
    else
    {
      p = calculerSourceXY1Carte( carte );
    }
    afficherUneCarte( dx, dy, p.x(), p.y() );
}

void CRami::effacerUneCarte( const int dx, const int dy )
{
    afficherUneCarte( dx, dy, 1104, 540 );
}

void CRami::afficherUneCarte( const int dx, const int dy, const int sx, const int sy )
{
//	m_imgCopie = m_imgMemPlateau;
//	QPainter painter(&m_imgCopie);
    QPainter painter(&m_imgMemPlateau);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawImage( dx, dy, m_imgDessinCartes, sx, sy, 90, 131 );
    painter.end();
//	m_imgMemPlateau = m_imgCopie;
    m_afficheurImage->afficherImage( m_imgMemPlateau );
}

void CRami::dessinerUneCarte( const int dx, const int dy, const int sx, const int sy )
{
//  m_imgCopie = m_imgMemPlateau;
//  QPainter painter(&m_imgCopie);
    QPainter painter(&m_imgMemPlateau);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawImage( dx, dy, m_imgDessinCartes, sx, sy, 90, 131 );
    painter.end();
//  m_imgMemPlateau = m_imgCopie;
//  m_afficheurImage->afficherImage( m_imgMemPlateau );
}

void CRami::afficherDessinJoueurs()
{
    if(m_nombredejoueur == 2)
    {
      loadImage( ":/images/DessinNord.png", &m_imgDessinNord, 174, 128 );
      m_imgCopie = m_imgMemPlateau;
      QPainter painterN(&m_imgCopie);
      painterN.setCompositionMode(QPainter::CompositionMode_SourceOver);
      painterN.drawImage(647, 5, m_imgDessinNord);
      painterN.end();
      m_imgMemPlateau = m_imgCopie;
      m_afficheurImage->afficherImage( m_imgMemPlateau );
    }

    if(m_nombredejoueur == 3)
    {
      loadImage( ":/images/DessinEst.png", &m_imgDessinEst, 174, 128 );
      m_imgCopie = m_imgMemPlateau;
      QPainter painterE(&m_imgCopie);
      painterE.setCompositionMode(QPainter::CompositionMode_SourceOver);
      painterE.drawImage(1280, 5, m_imgDessinEst);
      painterE.end();
      m_imgMemPlateau = m_imgCopie;
      m_afficheurImage->afficherImage( m_imgMemPlateau );

      loadImage( ":/images/DessinOuest.png", &m_imgDessinOuest, 174, 128 );
      m_imgCopie = m_imgMemPlateau;
      QPainter painterO(&m_imgCopie);
      painterO.setCompositionMode(QPainter::CompositionMode_SourceOver);
      painterO.drawImage(15, 5, m_imgDessinOuest);
      painterO.end();
      m_imgMemPlateau = m_imgCopie;
      m_afficheurImage->afficherImage( m_imgMemPlateau );
    }

    if(m_nombredejoueur == 4)
    {
      loadImage( ":/images/DessinEst.png", &m_imgDessinEst, 174, 128 );
      m_imgCopie = m_imgMemPlateau;
      QPainter painterE(&m_imgCopie);
      painterE.setCompositionMode(QPainter::CompositionMode_SourceOver);
      painterE.drawImage(1280, 5, m_imgDessinEst);
      painterE.end();
      m_imgMemPlateau = m_imgCopie;
      m_afficheurImage->afficherImage( m_imgMemPlateau );

      loadImage( ":/images/DessinNord.png", &m_imgDessinNord, 174, 128 );
      m_imgCopie = m_imgMemPlateau;
      QPainter painterN(&m_imgCopie);
      painterN.setCompositionMode(QPainter::CompositionMode_SourceOver);
      painterN.drawImage(647, 5, m_imgDessinNord);
      painterN.end();
      m_imgMemPlateau = m_imgCopie;
      m_afficheurImage->afficherImage( m_imgMemPlateau );

      loadImage( ":/images/DessinOuest.png", &m_imgDessinOuest, 174, 128 );
      m_imgCopie = m_imgMemPlateau;
      QPainter painterO(&m_imgCopie);
      painterO.setCompositionMode(QPainter::CompositionMode_SourceOver);
      painterO.drawImage(15, 5, m_imgDessinOuest);
      painterO.end();
      m_imgMemPlateau = m_imgCopie;
      m_afficheurImage->afficherImage( m_imgMemPlateau );
    }
}

void CRami::initialiserCartesPosees()
{
    m_index_carte_posee = 0;
    for( int i = 0; i < 108; i++ )
    {
      m_cartesposees[i] = PASCARTE;
    }
    m_cartesposees[108] = MARKFIN;
    m_cartesposees[109] = MARKFIN;
}

void CRami::initialiserCartesJetees()
{
    m_index_carte_jetee = 107;
    m_nbcartejetee = 0;
    for( int i = 0; i < 108; i++ )
    {
      m_cartesjetees[i] = PASCARTE;
    }
    m_cartesjetees[108] = MARKFIN;
    m_cartesjetees[109] = MARKFIN;
}

void CRami::initialiserVariablesPoseJoueurs()
{
    m_cartesposees_Sud.clear();
    m_flagCP_Sud.clear();

    m_cartesposees_Est.clear();
    m_flagCP_Est.clear();

    m_cartesposees_Nord.clear();
    m_flagCP_Nord.clear();

    m_cartesposees_Ouest.clear();
    m_flagCP_Ouest.clear();
}

quint8 CRami::chercherCartePlusPetiteValeur( quint8 * ajeu, const bool a_pose, int & nbpaspose )
{
  quint8 carte = ERREUR_CARTE;
  carte = m_pBaseJoueurs->chercherCartePlusPetiteValeur( ajeu, a_pose, nbpaspose, m_niveaujeu, m_cartesjetees, m_cartesposees );
  if( carte == ERREUR_CARTE )
  {
    QMessageBox msgBox(QMessageBox::Critical, NOM_PROG + " - Erreur",
                        QString(" ORDI JOUE...\n\n"
                        "carte == ERREUR_CARTE.\n"
                        "Rami va QUITTER!!!\n\n"
                        "DÉSOLÉ"),
                         { },
                         this);
//    msgBox.setInformativeText(" ");
    msgBox.exec();
    qApp->exit();
  }
  return carte;
}

quint8 CRami::chercherCartePlusGrandeValeur( quint8 * ajeu, const bool a_pose )
{
  quint8 carte = ERREUR_CARTE;
  carte = m_pBaseJoueurs->chercherCartePlusGrandeValeur( ajeu, a_pose, m_niveaujeu, m_cartesjetees, m_cartesposees );
  if( carte == ERREUR_CARTE )
  {
      QMessageBox msgBox(QMessageBox::Critical, NOM_PROG + " - Erreur",
                         QString(" ORDI JOUE...\n\n"
                                 "carte == ERREUR_CARTE.\n"
                                 "Rami va QUITTER!!!\n\n"
                                 "DÉSOLÉ"),
                         { },
                         this);
      //    msgBox.setInformativeText(" ");
      msgBox.exec();
      qApp->exit();
  }
  return carte;
}

bool CRami::Ordi_Est_Jouer()
{
  afficherRectOrangeEst();

  afficherMsgSB( "EST joue" );

  m_pBaseJoueurs->trierJeu( m_jeu_Est );

  bool verifPose = true;
  bool avecJetee = false;
  bool avecPioche = false;
  bool peutPoserCJ = false;
  bool peutPoser1C = false;
  bool a_Fini = false;

//  qDebug( "Nombre de tour : %d", m_nombredetour );

  if( m_nombredetour < m_nombredejoueur )
  {
/// 1er Tour
    afficherAnimationPiocheCarte( DOSCARTE, EST );
    jouerSonPiochePrendCarte( EST );
    piocher1Carte( m_jeu_Est, false );
    m_nbcartes_Est += 1;
    afficherNbCartesEst();
    m_pBaseJoueurs->trierJeu( m_jeu_Est );
    verifPose = false;

/// @todo MIS POUR TEST
#ifdef MISE_AU_POINT
    Ordi_Test_A_Piocher( m_jeu_Est );
#endif
///

  }
  else
  {
/// Pas 1er Tour : Peut Piocher, prendre carte Jetée, Poser.
    m_prise_ds_jetee_Est = PASCARTE;
    bool peutPoser = false;
    peutPoser = simulerPrendreCarteJetee( m_jeu_Est, m_f_a_pose_Est );

    if( true == peutPoser )
    {
/// Peut poser avec Carte Jetee Prise

/// @todo MIS POUR TEST
#ifdef MISE_AU_POINT
    Ordi_Test_A_PrisCJetee( m_jeu_Est );
#endif
///

      m_prise_ds_jetee_Est = prendreCarteJetee( m_jeu_Est );
      afficherAnimationPrendCarte( m_prise_ds_jetee_Est, EST );
      jouerSonPiochePrendCarte( EST );
      m_nbcartes_Est += 1;
      afficherNbCartesEst();
      m_pBaseJoueurs->trierJeu( m_jeu_Est );
      m_pBaseJoueurs->testerSiPeutPoserOrdi( m_jeu_Est, m_f_a_pose_Est, m_prise_ds_jetee_Est, m_listepose );
      avecJetee = true;
      verifPose = false;
    }
    else
    {
/// NE peut PAS poser avec Carte Jetee Prise
      peutPoserCJ = false;
      if( m_f_a_pose_Est == true )
      {
/// Tester si carte jetee prise va sur Pose
        bool okCJ = false;
        okCJ = simulerPoserCarteJetee( m_jeu_Est, m_f_a_pose_Est, m_nbcartes_Est );
        if( okCJ == true )
        {
          m_prise_ds_jetee_Est = prendreCarteJetee( m_jeu_Est );
          afficherAnimationPrendCarte( m_prise_ds_jetee_Est, EST );
          jouerSonPiochePrendCarte( EST );
          m_nbcartes_Est += 1;
          afficherNbCartesEst();
          m_pBaseJoueurs->trierJeu( m_jeu_Est );
          peutPoserCJ = true;
          avecJetee = true;
          verifPose = false;
        }
      }
      if( peutPoserCJ == false )
      {
/// PIOCHE
        afficherAnimationPiocheCarte( DOSCARTE, EST );
        jouerSonPiochePrendCarte( EST );
        piocher1Carte( m_jeu_Est, true );
        m_nbcartes_Est += 1;
        afficherNbCartesEst();
        m_pBaseJoueurs->trierJeu( m_jeu_Est );

/// @todo MIS POUR TEST
#ifdef MISE_AU_POINT
    Ordi_Test_A_Piocher( m_jeu_Est );
    insererCartesDansJeu( m_jeu_Est, m_nbcartes_Est );
#endif
///

/// Tester si peut poser avec Pioche
        peutPoser = m_pBaseJoueurs->testerSiPeutPoserOrdi( m_jeu_Est, m_f_a_pose_Est, PASCARTE, m_listepose );
        if( peutPoser == true )
        {
/// Peut poser avec Carte Piochee
          avecPioche = true;
          verifPose = false;
        }
        else
        {
/// NE peut PAS poser avec Carte Piochee
          peutPoser1C = false;
          if( m_f_a_pose_Est == true )
          {
/// Testera si 1 carte va sur Pose
            peutPoser1C = true;
            avecPioche = true;
            verifPose = false;
          }
        }
      }
    }

    if( avecJetee == true || avecPioche == true )
    {
/// Pose cartes sur POSE
      if( peutPoserCJ == false && peutPoser1C == false )
      {
        CRetourPose rp = m_pBaseJoueurs->faireListesPoses( m_cartesposees_Est, m_flagCP_Est, m_cartesposees, m_index_carte_posee, m_jeu_Est, m_nbcartes_Est, m_listepose );
        m_index_carte_posee = rp.indexDansCpj();
        m_nbcartes_Est = rp.nombreCartesJeu();

        afficherNbCartesEst();

        jouerSonPoseCartes();
        afficherCartesPoseesJoueurs();
        faireTemporisation( m_tempo_attente );

        m_pBaseJoueurs->trierJeu( m_jeu_Est );

        m_f_a_pose_Est = true;

/// @todo MIS POUR TEST
#ifdef MISE_AU_POINT
    Ordi_Test_A_Poser( m_jeu_Est );
#endif
///
      }

/// Pose 1 carte sur POSE
      quint8 cartetest = PASCARTE;
      int existe;
      int nbcartestest = m_nbcartes_Est;
      int index = 0;
      m_pBaseJoueurs->trierJeu( m_jeu_Est );

      while( m_nbcartes_Est > 1 && index < nbcartestest )
      {
        cartetest = m_jeu_Est[ index ];
        index += 1;
        existe = m_flagCP_Est.indexOf ( cartetest, 0 );
        if( existe != -1 )
        {
          CRetourPose rp = m_pBaseJoueurs->inserer1CListesPoses( m_cartesposees_Est, m_flagCP_Est, m_cartesposees, m_index_carte_posee, m_jeu_Est, m_nbcartes_Est, cartetest );
          m_index_carte_posee = rp.indexDansCpj();
          m_nbcartes_Est = rp.nombreCartesJeu();
          afficherNbCartesEst();
          jouerSonPose1Carte();
          afficherCartesPoseesJoueurs();
          faireTemporisation( m_tempo_attente );
          index = 0;
          nbcartestest = m_nbcartes_Est;
          m_pBaseJoueurs->trierJeu( m_jeu_Est );
          continue;
        }
        existe = m_flagCP_Nord.indexOf ( cartetest, 0 );
        if( existe != -1 )
        {
          CRetourPose rp = m_pBaseJoueurs->inserer1CListesPoses( m_cartesposees_Nord, m_flagCP_Nord, m_cartesposees, m_index_carte_posee, m_jeu_Est, m_nbcartes_Est, cartetest );
          m_index_carte_posee = rp.indexDansCpj();
          m_nbcartes_Est = rp.nombreCartesJeu();
          afficherNbCartesEst();
          jouerSonPose1Carte();
          afficherCartesPoseesJoueurs();
          faireTemporisation( m_tempo_attente );
          index = 0;
          nbcartestest = m_nbcartes_Est;
          m_pBaseJoueurs->trierJeu( m_jeu_Est );
          continue;
        }
        existe = m_flagCP_Ouest.indexOf ( cartetest, 0 );
        if( existe != -1 )
        {
          CRetourPose rp = m_pBaseJoueurs->inserer1CListesPoses( m_cartesposees_Ouest, m_flagCP_Ouest, m_cartesposees, m_index_carte_posee, m_jeu_Est, m_nbcartes_Est, cartetest );
          m_index_carte_posee = rp.indexDansCpj();
          m_nbcartes_Est = rp.nombreCartesJeu();
          afficherNbCartesEst();
          jouerSonPose1Carte();
          afficherCartesPoseesJoueurs();
          faireTemporisation( m_tempo_attente );
          index = 0;
          nbcartestest = m_nbcartes_Est;
          m_pBaseJoueurs->trierJeu( m_jeu_Est );
          continue;
        }
        existe = m_flagCP_Sud.indexOf ( cartetest, 0 );
        if( existe != -1 )
        {
          CRetourPose rp = m_pBaseJoueurs->inserer1CListesPoses( m_cartesposees_Sud, m_flagCP_Sud, m_cartesposees, m_index_carte_posee, m_jeu_Est, m_nbcartes_Est, cartetest );
          m_index_carte_posee = rp.indexDansCpj();
          m_nbcartes_Est = rp.nombreCartesJeu();
          afficherNbCartesEst();
          jouerSonPose1Carte();
          afficherCartesPoseesJoueurs();
          faireTemporisation( m_tempo_attente );
          index = 0;
          nbcartestest = m_nbcartes_Est;
          m_pBaseJoueurs->trierJeu( m_jeu_Est );
          continue;
        }
      }
      m_pBaseJoueurs->trierJeu( m_jeu_Est );
    }
  }

  quint8 carte = ERREUR_CARTE;
  if( verifPose == true )
  {
/// Tester si carte à jeter va sur Pose
    bool a_poseJS = false;
    bool si3J = ( m_nombredejoueur == 3 && m_f_a_pose_Ouest == true );
    bool si4J = ( m_nombredejoueur == 4 && m_f_a_pose_Nord == true );
    if( si3J == true || si4J == true )
    {
      a_poseJS = true;
    }
    carte = calculerCarteAJeter( a_poseJS, m_jeu_Est, m_f_a_pose_Est, m_nbcartes_Est, m_nbpaspose_Est );
  }
  else
  {
    if( m_f_a_pose_Est == true )
    {
      m_nbpaspose_Est = 0;
      if( m_nbcartes_Sud == 1 || m_nbcartes_Nord == 1 || m_nbcartes_Ouest == 1 )
      {
        carte = chercherCartePlusGrandeValeur( m_jeu_Est, m_f_a_pose_Est );
      }
      else
      {
        carte = chercherCartePlusPetiteValeur( m_jeu_Est, m_f_a_pose_Est, m_nbpaspose_Est );
      }
    }
    else
    {
      carte = chercherCartePlusPetiteValeur( m_jeu_Est, m_f_a_pose_Est, m_nbpaspose_Est );
    }
  }

/// Jeter 1 carte
  afficherAnimationJetteCarte( carte, EST );
  jouerSonJette1Carte();
  jeter1Carte( m_jeu_Est, carte );
  m_nbcartes_Est -= 1;
  afficherNbCartesEst();
  if( m_nbcartes_Est == 1 && m_f_a_parle_Est == false )
  {
    jouerSonReste1Carte( EST );
    m_f_a_parle_Est = true;
  }

/// @todo MIS POUR TEST
#ifdef MISE_AU_POINT
      Ordi_Test_A_Jeter1C( m_jeu_Est );
#endif
///


/// @todo MIS POUR TEST
#ifdef MISE_AU_POINT
  Ordi_Test_Fin();
#endif
///

  effacerRectOrangeEst();

  if( m_nbcartes_Est == 0 )  a_Fini = true;
  return a_Fini;
}


void CRami::Ordi_Test_A_PrisCJetee( const quint8 *jeu )
{
    effacerCartesJeuSud();

    afficherCartesJeu( 90, 990, jeu, 14 );

    QMessageBox msgBox(QMessageBox::Information, NOM_PROG + " - Test",
                       QString(" ORDI\n\n"
                               " a pris carte jetée.\n"),
                       { },
                       this);
    //    msgBox.setInformativeText(" ");
    msgBox.exec();
}

void CRami::Ordi_Test_A_Poser( const quint8 *jeu )
{
    effacerCartesJeuSud();

    afficherCartesJeu( 90, 990, jeu, 14 );

    QMessageBox msgBox(QMessageBox::Information, NOM_PROG + " - Test",
                       QString(" ORDI\n\n"
                               " a posé.\n"),
                       { },
                       this);
    //    msgBox.setInformativeText(" ");
    msgBox.exec();
}

void CRami::Ordi_Test_A_Poser1C( const quint8 *jeu )
{
    effacerCartesJeuSud();

    afficherCartesJeu( 90, 990, jeu, 14 );

    QMessageBox msgBox(QMessageBox::Information, NOM_PROG + " - Test",
                       QString(" ORDI\n\n"
                               " a posé 1 carte.\n"),
                       { },
                       this);
    //    msgBox.setInformativeText(" ");
    msgBox.exec();
}

void CRami::Ordi_Test_A_Piocher( const quint8 *jeu )
{
    effacerCartesJeuSud();

    afficherCartesJeu( 90, 990, jeu, 14 );

    QMessageBox msgBox(QMessageBox::Information, NOM_PROG + " - Test",
                       QString(" ORDI\n\n"
                               " a pioché.\n"),
                       { },
                       this);
    //    msgBox.setInformativeText(" ");
    msgBox.exec();
}

void CRami::Ordi_Test_A_Jeter1C( const quint8 *jeu )
{
    effacerCartesJeuSud();

    afficherCartesJeu( 90, 990, jeu, 14 );

    QMessageBox msgBox(QMessageBox::Information, NOM_PROG + " - Test",
                       QString(" ORDI\n\n"
                               " a jeté 1 carte.\n"),
                       { },
                       this);
    //    msgBox.setInformativeText(" ");
    msgBox.exec();
}

void CRami::Ordi_Test_Fin()
{
    afficherCartesJeuSud();
}



bool CRami::Ordi_Nord_Jouer()
{
  afficherRectOrangeNord();

  afficherMsgSB( "NORD joue" );

  m_pBaseJoueurs->trierJeu( m_jeu_Nord );

  bool verifPose = true;
  bool avecJetee = false;
  bool avecPioche = false;
  bool peutPoserCJ = false;
  bool peutPoser1C = false;
  bool a_Fini = false;

  if( m_nombredetour < m_nombredejoueur )
  {
/// 1er Tour
    afficherAnimationPiocheCarte( DOSCARTE, NORD );
    jouerSonPiochePrendCarte( NORD );
    piocher1Carte( m_jeu_Nord, false );
    m_nbcartes_Nord += 1;
    afficherNbCartesNord();
    m_pBaseJoueurs->trierJeu( m_jeu_Nord );
    verifPose = false;
  }
  else
  {
/// Pas 1er Tour : Peut Piocher, prendre carte Jetée, Poser.
    m_prise_ds_jetee_Nord = PASCARTE;
    bool peutPoser = false;
    peutPoser = simulerPrendreCarteJetee( m_jeu_Nord, m_f_a_pose_Nord );

    if( true == peutPoser )
    {
/// Peut poser avec Carte Jetee Prise
      m_prise_ds_jetee_Nord = prendreCarteJetee( m_jeu_Nord );
      afficherAnimationPrendCarte( m_prise_ds_jetee_Nord, NORD );
      jouerSonPiochePrendCarte( NORD );
      m_nbcartes_Nord += 1;
      afficherNbCartesNord();
      m_pBaseJoueurs->trierJeu( m_jeu_Nord );
      m_pBaseJoueurs->testerSiPeutPoserOrdi( m_jeu_Nord, m_f_a_pose_Nord, m_prise_ds_jetee_Nord, m_listepose );
      avecJetee = true;
      verifPose = false;
    }
    else
    {
/// NE peut PAS poser avec Carte Jetee Prise
      peutPoserCJ = false;
      if( m_f_a_pose_Nord == true )
      {
/// Tester si carte jetee prise va sur Pose
        bool okCJ = false;
        okCJ = simulerPoserCarteJetee( m_jeu_Nord, m_f_a_pose_Nord, m_nbcartes_Nord );
        if( okCJ == true )
        {
          m_prise_ds_jetee_Nord = prendreCarteJetee( m_jeu_Nord );
          afficherAnimationPrendCarte( m_prise_ds_jetee_Nord, NORD );
          jouerSonPiochePrendCarte( NORD );
          m_nbcartes_Nord += 1;
          afficherNbCartesNord();
          m_pBaseJoueurs->trierJeu( m_jeu_Nord );
          peutPoserCJ = true;
          avecJetee = true;
          verifPose = false;
        }
      }
      if( peutPoserCJ == false )
      {
/// PIOCHE
        afficherAnimationPiocheCarte( DOSCARTE, NORD );
        jouerSonPiochePrendCarte( NORD );
        piocher1Carte( m_jeu_Nord, true );
        m_nbcartes_Nord += 1;
        afficherNbCartesNord();
        m_pBaseJoueurs->trierJeu( m_jeu_Nord );
/// Tester si peut poser avec Pioche
        peutPoser = m_pBaseJoueurs->testerSiPeutPoserOrdi( m_jeu_Nord, m_f_a_pose_Nord, PASCARTE, m_listepose );
        if( peutPoser == true )
        {
/// Peut poser avec Carte Piochee
          avecPioche = true;
          verifPose = false;
        }
        else
        {
/// NE peut PAS poser avec Carte Piochee
          peutPoser1C = false;
          if( m_f_a_pose_Nord == true )
          {
/// Testera si 1 carte va sur Pose
            peutPoser1C = true;
            avecPioche = true;
            verifPose = false;
          }
        }
      }
    }

    if( avecJetee == true || avecPioche == true )
    {
/// Pose cartes sur POSE
      if( peutPoserCJ == false && peutPoser1C == false )
      {
        CRetourPose rp = m_pBaseJoueurs->faireListesPoses( m_cartesposees_Nord, m_flagCP_Nord, m_cartesposees, m_index_carte_posee, m_jeu_Nord, m_nbcartes_Nord, m_listepose );
        m_index_carte_posee = rp.indexDansCpj();
        m_nbcartes_Nord = rp.nombreCartesJeu();

        afficherNbCartesNord();

        jouerSonPoseCartes();
        afficherCartesPoseesJoueurs();
        faireTemporisation( m_tempo_attente );

        m_pBaseJoueurs->trierJeu( m_jeu_Nord );

        m_f_a_pose_Nord = true;
      }

/// Pose 1 carte sur POSE
      quint8 cartetest = PASCARTE;
      int existe;
      int nbcartestest = m_nbcartes_Nord;
      int index = 0;
      m_pBaseJoueurs->trierJeu( m_jeu_Nord );

      while( m_nbcartes_Nord > 1 && index < nbcartestest )
      {
        cartetest = m_jeu_Nord[ index ];
        index += 1;
        existe = m_flagCP_Nord.indexOf ( cartetest, 0 );
        if( existe != -1 )
        {
          CRetourPose rp = m_pBaseJoueurs->inserer1CListesPoses( m_cartesposees_Nord, m_flagCP_Nord, m_cartesposees, m_index_carte_posee, m_jeu_Nord, m_nbcartes_Nord, cartetest );
          m_index_carte_posee = rp.indexDansCpj();
          m_nbcartes_Nord = rp.nombreCartesJeu();
          afficherNbCartesNord();
          jouerSonPose1Carte();
          afficherCartesPoseesJoueurs();
          faireTemporisation( m_tempo_attente );
          index = 0;
          nbcartestest = m_nbcartes_Nord;
          m_pBaseJoueurs->trierJeu( m_jeu_Nord );
          continue;
        }
        existe = m_flagCP_Ouest.indexOf ( cartetest, 0 );
        if( existe != -1 )
        {
          CRetourPose rp = m_pBaseJoueurs->inserer1CListesPoses( m_cartesposees_Ouest, m_flagCP_Ouest, m_cartesposees, m_index_carte_posee, m_jeu_Nord, m_nbcartes_Nord, cartetest );
          m_index_carte_posee = rp.indexDansCpj();
          m_nbcartes_Nord = rp.nombreCartesJeu();
          afficherNbCartesNord();
          jouerSonPose1Carte();
          afficherCartesPoseesJoueurs();
          faireTemporisation( m_tempo_attente );
          index = 0;
          nbcartestest = m_nbcartes_Nord;
          m_pBaseJoueurs->trierJeu( m_jeu_Nord );
          continue;
        }
        existe = m_flagCP_Sud.indexOf ( cartetest, 0 );
        if( existe != -1 )
        {
          CRetourPose rp = m_pBaseJoueurs->inserer1CListesPoses( m_cartesposees_Sud, m_flagCP_Sud, m_cartesposees, m_index_carte_posee, m_jeu_Nord, m_nbcartes_Nord, cartetest );
          m_index_carte_posee = rp.indexDansCpj();
          m_nbcartes_Nord = rp.nombreCartesJeu();
          afficherNbCartesNord();
          jouerSonPose1Carte();
          afficherCartesPoseesJoueurs();
          faireTemporisation( m_tempo_attente );
          index = 0;
          nbcartestest = m_nbcartes_Nord;
          m_pBaseJoueurs->trierJeu( m_jeu_Nord );
          continue;
        }
        existe = m_flagCP_Est.indexOf ( cartetest, 0 );
        if( existe != -1 )
        {
          CRetourPose rp = m_pBaseJoueurs->inserer1CListesPoses( m_cartesposees_Est, m_flagCP_Est, m_cartesposees, m_index_carte_posee, m_jeu_Nord, m_nbcartes_Nord, cartetest );
          m_index_carte_posee = rp.indexDansCpj();
          m_nbcartes_Nord = rp.nombreCartesJeu();
          afficherNbCartesNord();
          jouerSonPose1Carte();
          afficherCartesPoseesJoueurs();
          faireTemporisation( m_tempo_attente );
          index = 0;
          nbcartestest = m_nbcartes_Nord;
          m_pBaseJoueurs->trierJeu( m_jeu_Nord );
          continue;
        }
      }
      m_pBaseJoueurs->trierJeu( m_jeu_Nord );
    }
  }

  quint8 carte = ERREUR_CARTE;
  if( verifPose == true )
  {
/// Tester si carte à jeter va sur Pose
    bool a_poseJS = false;
    bool si2J = ( m_nombredejoueur == 2 && m_f_a_pose_Sud == true );
    bool si4J = ( m_nombredejoueur == 4 && m_f_a_pose_Ouest == true );
    if( si2J == true || si4J == true )
    {
      a_poseJS = true;
    }
    carte = calculerCarteAJeter( a_poseJS, m_jeu_Nord, m_f_a_pose_Nord, m_nbcartes_Nord, m_nbpaspose_Nord );
  }
  else
  {
    if( m_f_a_pose_Nord == true )
    {
      m_nbpaspose_Nord = 0;
      if( m_nbcartes_Sud == 1 || m_nbcartes_Est == 1 || m_nbcartes_Ouest == 1 )
      {
        carte = chercherCartePlusGrandeValeur( m_jeu_Nord, m_f_a_pose_Nord );
      }
      else
      {
        carte = chercherCartePlusPetiteValeur( m_jeu_Nord, m_f_a_pose_Nord, m_nbpaspose_Nord );
      }
    }
    else
    {
      carte = chercherCartePlusPetiteValeur( m_jeu_Nord, m_f_a_pose_Nord, m_nbpaspose_Nord );
    }
  }

/// Jeter 1 carte
  afficherAnimationJetteCarte( carte, NORD );
  jouerSonJette1Carte();
  jeter1Carte( m_jeu_Nord, carte );
  m_nbcartes_Nord -= 1;
  afficherNbCartesNord();
  if( m_nbcartes_Nord == 1 && m_f_a_parle_Nord == false )
  {
    jouerSonReste1Carte( NORD );
    m_f_a_parle_Nord = true;
  }

/// @todo MIS POUR TEST
#ifdef MISE_AU_POINT
      Ordi_Test_A_Jeter1C( m_jeu_Nord );
#endif
///


/// @todo MIS POUR TEST
#ifdef MISE_AU_POINT
  Ordi_Test_Fin();
#endif
///


  effacerRectOrangeNord();

  if( m_nbcartes_Nord == 0 )  a_Fini = true;
  return a_Fini;
}


bool CRami::Ordi_Ouest_Jouer()
{
  afficherRectOrangeOuest();

  afficherMsgSB( "OUEST joue" );

  m_pBaseJoueurs->trierJeu( m_jeu_Ouest );

  bool verifPose = true;
  bool avecJetee = false;
  bool avecPioche = false;
  bool peutPoserCJ = false;
  bool peutPoser1C = false;
  bool a_Fini = false;

  if( m_nombredetour < m_nombredejoueur )
  {
/// 1er Tour
    afficherAnimationPiocheCarte( DOSCARTE, OUEST );
    jouerSonPiochePrendCarte( OUEST );
    piocher1Carte( m_jeu_Ouest, false );
    m_nbcartes_Ouest += 1;
    afficherNbCartesOuest();
    m_pBaseJoueurs->trierJeu( m_jeu_Ouest );
    verifPose = false;
  }
  else
  {
/// Pas 1er Tour : Peut Piocher, prendre carte Jetée, Poser.
    m_prise_ds_jetee_Ouest = PASCARTE;
    bool peutPoser = false;
    peutPoser = simulerPrendreCarteJetee( m_jeu_Ouest, m_f_a_pose_Ouest );

    if( true == peutPoser )
    {
/// Peut poser avec Carte Jetee Prise
      m_prise_ds_jetee_Ouest = prendreCarteJetee( m_jeu_Ouest );
      afficherAnimationPrendCarte( m_prise_ds_jetee_Ouest, OUEST );
      jouerSonPiochePrendCarte( OUEST );
      m_nbcartes_Ouest += 1;
      afficherNbCartesOuest();
      m_pBaseJoueurs->trierJeu( m_jeu_Ouest );
      m_pBaseJoueurs->testerSiPeutPoserOrdi( m_jeu_Ouest, m_f_a_pose_Ouest, m_prise_ds_jetee_Ouest, m_listepose );
      avecJetee = true;
      verifPose = false;
    }
    else
    {
/// NE peut PAS poser avec Carte Jetee Prise
      peutPoserCJ = false;
      if( m_f_a_pose_Ouest == true )
      {
/// Tester si carte jetee prise va sur Pose
        bool okCJ = false;
        okCJ = simulerPoserCarteJetee( m_jeu_Ouest, m_f_a_pose_Ouest, m_nbcartes_Ouest );
        if( okCJ == true )
        {
          m_prise_ds_jetee_Ouest = prendreCarteJetee( m_jeu_Ouest );
          afficherAnimationPrendCarte( m_prise_ds_jetee_Ouest, OUEST );
          jouerSonPiochePrendCarte( OUEST );
          m_nbcartes_Ouest += 1;
          afficherNbCartesOuest();
          m_pBaseJoueurs->trierJeu( m_jeu_Ouest );
          peutPoserCJ = true;
          avecJetee = true;
          verifPose = false;
        }
      }
      if( peutPoserCJ == false )
      {
/// PIOCHE
        afficherAnimationPiocheCarte( DOSCARTE, OUEST );
        jouerSonPiochePrendCarte( OUEST );
        piocher1Carte( m_jeu_Ouest, true );
        m_nbcartes_Ouest += 1;
        afficherNbCartesOuest();
        m_pBaseJoueurs->trierJeu( m_jeu_Ouest );
/// Tester si peut poser avec Pioche
        peutPoser = m_pBaseJoueurs->testerSiPeutPoserOrdi( m_jeu_Ouest, m_f_a_pose_Ouest, PASCARTE, m_listepose );
        if( peutPoser == true )
        {
/// Peut poser avec Carte Piochee
          avecPioche = true;
          verifPose = false;
        }
        else
        {
/// NE peut PAS poser avec Carte Piochee
          peutPoser1C = false;
          if( m_f_a_pose_Ouest == true )
          {
/// Testera si 1 carte va sur Pose
            peutPoser1C = true;
            avecPioche = true;
            verifPose = false;
          }
        }
      }
    }

    if( avecJetee == true || avecPioche == true )
    {
/// Pose cartes sur POSE
      if( peutPoserCJ == false && peutPoser1C == false )
      {
        CRetourPose rp = m_pBaseJoueurs->faireListesPoses( m_cartesposees_Ouest, m_flagCP_Ouest, m_cartesposees, m_index_carte_posee, m_jeu_Ouest, m_nbcartes_Ouest, m_listepose );
        m_index_carte_posee = rp.indexDansCpj();
        m_nbcartes_Ouest = rp.nombreCartesJeu();

        afficherNbCartesOuest();

        jouerSonPoseCartes();
        afficherCartesPoseesJoueurs();
        faireTemporisation( m_tempo_attente );

        m_pBaseJoueurs->trierJeu( m_jeu_Ouest );

        m_f_a_pose_Ouest = true;
      }

/// Pose 1 carte sur POSE
      quint8 cartetest = PASCARTE;
      int existe;
      int nbcartestest = m_nbcartes_Ouest;
      int index = 0;
      m_pBaseJoueurs->trierJeu( m_jeu_Ouest );

      while( m_nbcartes_Ouest > 1 && index < nbcartestest )
      {
        cartetest = m_jeu_Ouest[ index ];
        index += 1;
        existe = m_flagCP_Ouest.indexOf ( cartetest, 0 );
        if( existe != -1 )
        {
          CRetourPose rp = m_pBaseJoueurs->inserer1CListesPoses( m_cartesposees_Ouest, m_flagCP_Ouest, m_cartesposees, m_index_carte_posee, m_jeu_Ouest, m_nbcartes_Ouest, cartetest );
          m_index_carte_posee = rp.indexDansCpj();
          m_nbcartes_Ouest = rp.nombreCartesJeu();
          afficherNbCartesOuest();
          jouerSonPose1Carte();
          afficherCartesPoseesJoueurs();
          faireTemporisation( m_tempo_attente );
          index = 0;
          nbcartestest = m_nbcartes_Ouest;
          m_pBaseJoueurs->trierJeu( m_jeu_Ouest );
          continue;
        }
        existe = m_flagCP_Sud.indexOf ( cartetest, 0 );
        if( existe != -1 )
        {
          CRetourPose rp = m_pBaseJoueurs->inserer1CListesPoses( m_cartesposees_Sud, m_flagCP_Sud, m_cartesposees, m_index_carte_posee, m_jeu_Ouest, m_nbcartes_Ouest, cartetest );
          m_index_carte_posee = rp.indexDansCpj();
          m_nbcartes_Ouest = rp.nombreCartesJeu();
          afficherNbCartesOuest();
          jouerSonPose1Carte();
          afficherCartesPoseesJoueurs();
          faireTemporisation( m_tempo_attente );
          index = 0;
          nbcartestest = m_nbcartes_Ouest;
          m_pBaseJoueurs->trierJeu( m_jeu_Ouest );
          continue;
        }
        existe = m_flagCP_Est.indexOf ( cartetest, 0 );
        if( existe != -1 )
        {
          CRetourPose rp = m_pBaseJoueurs->inserer1CListesPoses( m_cartesposees_Est, m_flagCP_Est, m_cartesposees, m_index_carte_posee, m_jeu_Ouest, m_nbcartes_Ouest, cartetest );
          m_index_carte_posee = rp.indexDansCpj();
          m_nbcartes_Ouest = rp.nombreCartesJeu();
          afficherNbCartesOuest();
          jouerSonPose1Carte();
          afficherCartesPoseesJoueurs();
          faireTemporisation( m_tempo_attente );
          index = 0;
          nbcartestest = m_nbcartes_Ouest;
          m_pBaseJoueurs->trierJeu( m_jeu_Ouest );
          continue;
        }
        existe = m_flagCP_Nord.indexOf ( cartetest, 0 );
        if( existe != -1 )
        {
          CRetourPose rp = m_pBaseJoueurs->inserer1CListesPoses( m_cartesposees_Nord, m_flagCP_Nord, m_cartesposees, m_index_carte_posee, m_jeu_Ouest, m_nbcartes_Ouest, cartetest );
          m_index_carte_posee = rp.indexDansCpj();
          m_nbcartes_Ouest = rp.nombreCartesJeu();
          afficherNbCartesOuest();
          jouerSonPose1Carte();
          afficherCartesPoseesJoueurs();
          faireTemporisation( m_tempo_attente );
          index = 0;
          nbcartestest = m_nbcartes_Ouest;
          m_pBaseJoueurs->trierJeu( m_jeu_Ouest );
          continue;
        }
      }
      m_pBaseJoueurs->trierJeu( m_jeu_Ouest );
    }
  }

  quint8 carte = ERREUR_CARTE;
  if( verifPose == true )
  {
/// Tester si carte à jeter va sur Pose
    bool a_poseJS = false;
    bool si3J = ( m_nombredejoueur == 3 && m_f_a_pose_Sud == true );
    bool si4J = ( m_nombredejoueur == 4 && m_f_a_pose_Sud == true );
    if( si3J == true || si4J == true )
    {
      a_poseJS = true;
    }
    carte = calculerCarteAJeter( a_poseJS, m_jeu_Ouest, m_f_a_pose_Ouest, m_nbcartes_Ouest, m_nbpaspose_Ouest );
  }
  else
  {
    if( m_f_a_pose_Ouest == true )
    {
      m_nbpaspose_Ouest = 0;
      if( m_nbcartes_Sud == 1 || m_nbcartes_Est == 1 || m_nbcartes_Nord == 1 )
      {
        carte = chercherCartePlusGrandeValeur( m_jeu_Ouest, m_f_a_pose_Ouest );
      }
      else
      {
        carte = chercherCartePlusPetiteValeur( m_jeu_Ouest, m_f_a_pose_Ouest, m_nbpaspose_Ouest );
      }
    }
    else
    {
      carte = chercherCartePlusPetiteValeur( m_jeu_Ouest, m_f_a_pose_Ouest, m_nbpaspose_Ouest );
    }
  }

/// Jeter 1 carte
  afficherAnimationJetteCarte( carte, OUEST );
  jouerSonJette1Carte();
  jeter1Carte( m_jeu_Ouest, carte );
  m_nbcartes_Ouest -= 1;
  afficherNbCartesOuest();
  if( m_nbcartes_Ouest == 1 && m_f_a_parle_Ouest == false )
  {
    jouerSonReste1Carte( OUEST );
    m_f_a_parle_Ouest = true;
  }

/// @todo MIS POUR TEST
#ifdef MISE_AU_POINT
      Ordi_Test_A_Jeter1C( m_jeu_Ouest );
#endif
///


/// @todo MIS POUR TEST
#ifdef MISE_AU_POINT
  Ordi_Test_Fin();
#endif
///


  effacerRectOrangeOuest();

  if( m_nbcartes_Ouest == 0 )  a_Fini = true;
  return a_Fini;
}

int CRami::calculerPenaliteCartesJeuEst()
{
    if( m_nbcartes_Est == 0 )	return 0;
    if( m_nbcartes_Est == 13 )	return 100;
    return m_pBaseJoueurs->calculerPenaliteCartesJeu( m_jeu_Est, 13 );
}

int CRami::calculerPenaliteCartesJeuNord()
{
    if( m_nbcartes_Nord == 0 )	return 0;
    if( m_nbcartes_Nord == 13 )	return 100;
    return m_pBaseJoueurs->calculerPenaliteCartesJeu( m_jeu_Nord, 13 );
}

int CRami::calculerPenaliteCartesJeuOuest()
{
    if( m_nbcartes_Ouest == 0 )	return 0;
    if( m_nbcartes_Ouest == 13 )	return 100;
    return m_pBaseJoueurs->calculerPenaliteCartesJeu( m_jeu_Ouest, 13 );
}

int CRami::calculerPenaliteCartesJeuSud()
{
    if( m_nbcartes_Sud == 0 )	return 0;
    if( m_nbcartes_Sud == 13 )	return 100;
    return m_pBaseJoueurs->calculerPenaliteCartesJeu( m_jeu_Sud, 13 );
}

void CRami::afficherRectOrangeEst()
{
    afficherRectangleOrange( 1280, 5 );
}

void CRami::afficherRectOrangeNord()
{
    afficherRectangleOrange( 647, 5 );
}

void CRami::afficherRectOrangeOuest()
{
     afficherRectangleOrange( 15, 5 );
}

void CRami::effacerRectOrangeEst()
{
    effacerRectangleOrange( 1280, 5 );
}

void CRami::effacerRectOrangeNord()
{
    effacerRectangleOrange( 647, 5 );
}

void CRami::effacerRectOrangeOuest()
{
    effacerRectangleOrange( 15, 5 );
}



/// HUMAIN JOUE
void CRami::mousePressEvent( QMouseEvent * event )
{
  if( m_periodeJeu == FIN1PARTIE && m_f_Fin1partie == true )
  {
    m_f_Fin1partie = false;
    m_periodeJeu = DEBUT1PARTIE;
    m_f_timerActif = true;
    m_timerId = startTimer( m_tempo_timer );
  }
  else
  {
    if( event->button() == Qt::LeftButton )		/// SI Gauche Pressed
    {
      m_feventmouse = false;
      if( m_periodeJeu == JOUEURSJOUE && m_quijoue == SUD )
      {
        m_posClicX = event->position().x();
        m_posClicY = event->position().y();
        m_keymodifier = event->modifiers();

        POSCLIC posclic = POSERREUR;

        if( m_rectpioche.contains( m_posClicX, m_posClicY ) )	posclic = POSPIOCHE;
        if( m_rectjetees.contains( m_posClicX, m_posClicY ) )	posclic = POSJETEES;
        if( m_rectcartes.contains( m_posClicX, m_posClicY ) )	posclic = POSCARTES;

        switch( posclic )
        {
          case POSPIOCHE :
            gauchePressed_Pioche();
            break;
          case POSJETEES :
            gauchePressed_Jetees();
            break;
          case POSCARTES :
            if( m_keymodifier == Qt::ControlModifier )
            {
              controlgauchePressed_Cartes();
            }
            else
            {
              gauchePressed_Cartes();
            }
            break;
          default :
            jouerSonErreur();
            break;
        }
      }
    }
//    else
//    {
//      QWidget::mousePressEvent( event );
//    }
  }
}

void CRami::gauchePressed_Jetees()
{
    if( m_nombredetour < m_nombredejoueur )	/// Si 1er Tour
    {
      jouerSonErreur();
      return;
    }
    if( m_fclicpioche == true )
    {
      jouerSonErreur();
      return;
    }
    if( m_fclicjetee == true )
    {
      jouerSonErreur();
      return;
    }
    m_feventmouse = true;
    m_fclicjetee = true;
    quint8 carte = m_cartesjetees[ m_index_carte_jetee + 2 ];
    afficherUneCarte( 742, 833, carte );
    m_prise_ds_jetee_Sud = m_cartesjetees[ m_index_carte_jetee + 1 ];
    m_movecopiePlateau = m_imgMemPlateau;
    m_moveposX = m_posClicX;
    m_moveposY = m_posClicY;

    QPoint p = calculerSourceXY1Carte( m_prise_ds_jetee_Sud );
    m_movesrcX = p.x();
    m_movesrcY = p.y();

    afficherUneCarte( m_posClicX-45, m_posClicY-65, m_movesrcX, m_movesrcY );
    m_movecarteSud = true;
}

void CRami::gauchePressed_Pioche()
{
    if( m_fclicpioche == true )
    {
      jouerSonErreur();
      return;
    }
    if( m_fclicjetee == true )
    {
      jouerSonErreur();
      return;
    }

    m_feventmouse = true;
    m_fclicpioche = true;
    m_prise_ds_pioche_Sud = DOSCARTE;
    m_movecopiePlateau = m_imgMemPlateau;
    m_moveposX = m_posClicX;
    m_moveposY = m_posClicY;

    QPoint p = calculerSourceXY1Carte( m_prise_ds_pioche_Sud );
    m_movesrcX = p.x();
    m_movesrcY = p.y();

    afficherUneCarte( m_posClicX-45, m_posClicY-65, m_movesrcX, m_movesrcY );
    m_movecarteSud = true;
}

void CRami::gauchePressed_Cartes()
{
  int posX = m_posClicX - OFFSET;
  m_indexcartecliquee = posX / 91;
  if( PASCARTE == m_jeu_Sud[ m_indexcartecliquee ] )
  {
    jouerSonErreur();
    return;
  }

  m_feventmouse = true;

  if( m_cartesSelect_Sud[ m_indexcartecliquee ] == PASCARTE )
  {
/// Clic sur carte "normale"
    m_fclic1carte = true;
    m_prise_ds_jeu_Sud = m_jeu_Sud[ m_indexcartecliquee ];
    m_jeu_Sud[ m_indexcartecliquee ] = PASCARTE;
    m_cartesSelect_Sud[ m_indexcartecliquee ] = PASCARTE;
    afficherCartesJeuSud();
    m_nbcartes_Sud -= 1;
    afficherNbCartesSud();
    m_cartesSelect_Sud[ m_indexcartecliquee ] = PASCARTE;
    m_movecopiePlateau = m_imgMemPlateau;
    m_moveposX = m_posClicX;
    m_moveposY = m_posClicY;

    QPoint p = calculerSourceXY1Carte( m_prise_ds_jeu_Sud );
    m_movesrcX = p.x();
    m_movesrcY = p.y();

    afficherUneCarte( m_posClicX-45, m_posClicY-65, m_movesrcX, m_movesrcY );
  }
  else
  {
/// Clic sur cartes selectionnees
    if( m_nombredetour < m_nombredejoueur )
    {
      jouerSonErreur();
      m_feventmouse = false;
      return;
    }
    m_fcliccartesselect = true;

    copierTableau( m_jeu_Sud, m_savejeuSud, 16 );
    m_savenbcartesSud = m_nbcartes_Sud;

    for( int i = 0; i < 13; i++ )
    {
      m_movecartesSelect[ i ] = PASCARTE;
    }

    int indexselect = 0;
    for( int i = 0; i < 14; i++ )
    {
      if( m_cartesSelect_Sud[ i ] != PASCARTE )
      {
        m_movecartesSelect[ indexselect ] = m_cartesSelect_Sud[ i ];
        indexselect ++;
        m_jeu_Sud[ i ] = PASCARTE;
        m_cartesSelect_Sud[ i ] = PASCARTE;
        m_nbcartes_Sud -= 1;
      }
    }

    afficherCartesJeuSud();
    afficherNbCartesSud();

    m_movecopiePlateau = m_imgMemPlateau;
    m_moveposX = m_posClicX;
    m_moveposY = m_posClicY;

    afficherCartesSelect_Sud( m_posClicX-45, m_posClicY-65 );
  }

  m_movecarteSud = true;
}

void CRami::controlgauchePressed_Cartes()
{
    int posX = m_posClicX - OFFSET;
    m_indexcartecliquee = posX / 91;
    quint8 carte = m_jeu_Sud[ m_indexcartecliquee ];
    if( PASCARTE == carte )
    {
      jouerSonErreur();
      return;
    }
    if( PASCARTE == m_cartesSelect_Sud[ m_indexcartecliquee ] )
    {
      if( m_nbcartesSelect_Sud < m_nbcartes_Sud - 1 )
      {
        m_cartesSelect_Sud[ m_indexcartecliquee ] = carte;
        m_nbcartesSelect_Sud += 1;
      }
    }
    else
    {
      m_cartesSelect_Sud[ m_indexcartecliquee ] = PASCARTE;
      m_nbcartesSelect_Sud -= 1;
    }
    afficherCartesJeuSud();
}

void CRami::mouseReleaseEvent( QMouseEvent * event )
{
  if( m_periodeJeu == JOUEURSJOUE && m_quijoue == SUD )
  {
    m_posClicX = event->position().x();
    m_posClicY = event->position().y();

    POSCLIC posclic = POSERREUR;

    if( m_rectposees.contains( m_posClicX, m_posClicY ) ) posclic = POSPOSEES;
    if( m_rectpioche.contains( m_posClicX, m_posClicY ) ) posclic = POSPIOCHE;
    if( m_rectjetees.contains( m_posClicX, m_posClicY ) ) posclic = POSJETEES;
    if( m_rectcartes.contains( m_posClicX, m_posClicY ) ) posclic = POSCARTES;


    if( ( event->button() == Qt::RightButton ) && m_movecarteSud == false )
    {

/// @todo MIS POUR TEST
#ifdef MISE_AU_POINT
insererCartesDansJeu( m_jeu_Sud, m_nbcartes_Sud );
afficherCartesJeuSud();
#endif
///
      switch( posclic )
      {
        case POSJETEES :
          droitReleased_Jetees();
          break;
        case POSCARTES :
          droitReleased_Cartes();
          break;
        default :
          droitReleased_Erreur();
          break;
      }
    }
    else
    {
      if( event->button() == Qt::LeftButton )		/// SI Gauche Pressed
      {
        if( m_feventmouse == true )
        {
          switch( posclic )
          {
            case POSPOSEES :
              gaucheReleased_Posees();
              break;
            case POSPIOCHE :
              gaucheReleased_Pioche();
              break;
            case POSJETEES :
              gaucheReleased_Jetees();
              break;
            case POSCARTES :
              gaucheReleased_Cartes();
              break;
            default :		/// = POSERREUR
              gaucheReleased_Erreur();
              break;
          }
        }
      }
    }
    }
// else
// {
//  QWidget::mouseReleaseEvent( event );
// }
//	m_feventmouse = false;
}

void CRami::droitReleased_Jetees()
{
    if( m_cartesjetees[ m_index_carte_jetee + 1 ] != MARKFIN )
    {
      QImage  imgCopie;
      imgCopie = m_imgMemPlateau;

      effacerCartePioche();
      effacerCarteJetee();

      int dx = 5;
      int dy = 833;
      QPoint p;
      quint8 carte;
      int ijetee = m_index_carte_jetee;
      for( int i = 0; i < 16; i++ )
      {
        carte = m_cartesjetees[ ++ijetee ];
        if( carte == MARKFIN )  break;
        if( carte > 0 )
        {
          p = calculerSourceXY1Carte( carte );
          afficherUneCarte( dx, dy, p.x(), p.y() );
        }
        dx = dx + 91;
      }

      faireTemporisation( 2000 );

      m_imgMemPlateau = imgCopie;
      m_afficheurImage->afficherImage( m_imgMemPlateau );
      m_feventmouse = false;
    }
}

void CRami::droitReleased_Cartes()
{
    m_pBaseJoueurs->trierJeu( m_jeu_Sud );
    afficherCartesJeuSud();
    m_feventmouse = false;
}

void CRami::droitReleased_Erreur()
{
    /// @todo implement me
        m_feventmouse = false;
}

void CRami::gaucheReleased_Erreur()
{
  jouerSonErreur();

  m_imgMemPlateau = m_movecopiePlateau;
  m_afficheurImage->afficherImage( m_imgMemPlateau );

  if( m_fclic1carte == true || m_fcliccartesselect == true )
  {
/// VIENT DE CARTES
    if( m_fcliccartesselect == true )
    {
      copierTableau( m_savejeuSud, m_jeu_Sud, 16 );
      m_nbcartes_Sud = m_savenbcartesSud;

/*
      m_pBaseJoueurs->supprimerTrouJeu( m_jeu_Sud, 14 );
      int indextrou = 0;
      while( m_jeu_Sud[ indextrou ] != PASCARTE )
      {
        indextrou++;
      }

      for( int i = 0; i < 13; i++ )
      {
        if( m_movecartesSelect[ i ] != PASCARTE )
        {
          m_jeu_Sud[ indextrou ] = m_movecartesSelect[ i ];
          indextrou ++;
          m_nbcartes_Sud += 1;
        }
      }
*/

      initialiserCartesSelect_Sud();
      afficherCartesJeuSud();
      afficherNbCartesSud();
      m_fcliccartesselect = false;
    }
    else		/// 1 carte
    {
      m_jeu_Sud[ m_indexcartecliquee ] = m_prise_ds_jeu_Sud;
      afficherCartesJeuSud();
      m_nbcartes_Sud += 1;
      afficherNbCartesSud();
      m_fclic1carte = false;
      m_prise_ds_jeu_Sud = PASCARTE;
    }
  }
  else
  {
    if( m_fclicpioche == true )
    {
/// VIENT DE PIOCHE
      m_fclicpioche = false;
      m_prise_ds_pioche_Sud = PASCARTE;
    }
    else
    {
/// VIENT DE JETEES
      quint8 carte = m_cartesjetees[ m_index_carte_jetee + 1 ];
      afficherUneCarte( 742, 833, carte );
      m_fclicjetee = false;
      m_prise_ds_jetee_Sud = PASCARTE;
    }
  }
    m_feventmouse = false;
    m_movecarteSud = false;
}

void CRami::gaucheReleased_Pioche()
{
    m_imgMemPlateau = m_movecopiePlateau;
    m_afficheurImage->afficherImage( m_imgMemPlateau );

    if( m_fclic1carte == true || m_fcliccartesselect == true )
    {
/// VIENT DE CARTES
        jouerSonErreur();
        if( m_fcliccartesselect == true )
        {
      copierTableau( m_savejeuSud, m_jeu_Sud, 16 );
      m_nbcartes_Sud = m_savenbcartesSud;

/*
            m_pBaseJoueurs->supprimerTrouJeu( m_jeu_Sud, 14 );
            int indextrou = 0;
            while( m_jeu_Sud[ indextrou ] != PASCARTE )
            {
                indextrou++;
            }

            for( int i = 0; i < 13; i++ )
            {
                if( m_movecartesSelect[ i ] != PASCARTE )
                {
                    m_jeu_Sud[ indextrou ] = m_movecartesSelect[ i ];
                    indextrou ++;
                    m_nbcartes_Sud += 1;
                }
            }
*/

            initialiserCartesSelect_Sud();
            afficherCartesJeuSud();
            afficherNbCartesSud();
            m_fcliccartesselect = false;
        }
        else		/// 1 carte
        {
            m_jeu_Sud[ m_indexcartecliquee ] = m_prise_ds_jeu_Sud;
            afficherCartesJeuSud();
            m_nbcartes_Sud += 1;
            afficherNbCartesSud();
            m_fclic1carte = false;
            m_prise_ds_jeu_Sud = PASCARTE;
        }
    }
    else
    {
        if( m_fclicpioche == true )
        {
/// VIENT DE PIOCHE
            m_fclicpioche = false;
            m_prise_ds_pioche_Sud = PASCARTE;
        }
        else
        {
/// VIENT DE JETEES
            jouerSonErreur();
            quint8 carte = m_cartesjetees[ m_index_carte_jetee + 1 ];
            afficherUneCarte( 742, 833, carte );
            m_fclicjetee = false;
            m_prise_ds_jetee_Sud = PASCARTE;
        }
    }
    m_feventmouse = false;
    m_movecarteSud = false;
}

void CRami::gaucheReleased_Cartes()
{
/*
quint8 jeuavant[16];
quint8 jeuapres[16];
copierTableau( m_jeu_Sud, jeuavant, 16 );
*/
    m_imgMemPlateau = m_movecopiePlateau;
    m_afficheurImage->afficherImage( m_imgMemPlateau );

    int nbdecalage;
    int posX = m_posClicX - OFFSET;
    int indexarrivee = posX / 91;

    if( m_fclic1carte == true || m_fcliccartesselect == true )
    {
/// VIENT DE CARTES
        if( m_fcliccartesselect == true )
        {
            jouerSonErreur();
      copierTableau( m_savejeuSud, m_jeu_Sud, 16 );
      m_nbcartes_Sud = m_savenbcartesSud;

/*			m_pBaseJoueurs->supprimerTrouJeu( m_jeu_Sud, 14 );
            int indextrou = 0;
            while( m_jeu_Sud[ indextrou ] != PASCARTE )
            {
                indextrou++;
            }

            for( int i = 0; i < 13; i++ )
            {
                if( m_movecartesSelect[ i ] != PASCARTE )
                {
                    m_jeu_Sud[ indextrou ] = m_movecartesSelect[ i ];
                    indextrou ++;
                    m_nbcartes_Sud += 1;
                }
            }
*/

            initialiserCartesSelect_Sud();
            afficherCartesJeuSud();
            afficherNbCartesSud();
            m_fcliccartesselect = false;
        }
        else		/// 1 carte
        {
/// VIENT DE 1 CARTE
            int indexdepart = m_indexcartecliquee;
            if( m_indexcartecliquee < indexarrivee )
            {
                nbdecalage = indexarrivee - indexdepart;
                for( int i = 0; i < nbdecalage; i++ )
                {
                    m_jeu_Sud[ indexdepart ] = m_jeu_Sud[ indexdepart + 1 ];
                    m_cartesSelect_Sud[ indexdepart ] = m_cartesSelect_Sud[ indexdepart + 1 ];
                    indexdepart += 1;
                }
            }
            if( m_indexcartecliquee > indexarrivee )
            {
                nbdecalage = indexdepart - indexarrivee;
                for( int i = 0; i < nbdecalage; i++ )
                {
                    m_jeu_Sud[ indexdepart ] = m_jeu_Sud[ indexdepart - 1 ];
                    m_cartesSelect_Sud[ indexdepart ] = m_cartesSelect_Sud[ indexdepart - 1 ];
                    indexdepart -= 1;
                }
            }
            m_jeu_Sud[ indexarrivee ] = m_prise_ds_jeu_Sud;
            afficherCartesJeuSud();
            m_nbcartes_Sud += 1;
            afficherNbCartesSud();
            sauvegarderEtatJeu();
            m_fclic1carte = false;
            m_prise_ds_jeu_Sud = PASCARTE;
        }
    }
    else	/// VIENT DE PIOCHE OU JETEES
    {
        int indextrou = 0;
        while( m_jeu_Sud[ indextrou ] != PASCARTE )
        {
            indextrou++;
        }

//copierTableau( m_jeu_Sud, jeuapres, 16 );

        if( true == m_fclicpioche )
        {
/// VIENT DE PIOCHE
    /// @todo Routine indication  peut poser
      if( indextrou < indexarrivee )
      {
        nbdecalage = indexarrivee - indextrou;
        for( int i = 0; i < nbdecalage; i++ )
        {
          m_jeu_Sud[ indextrou ] = m_jeu_Sud[ indextrou + 1 ];
          m_cartesSelect_Sud[ indextrou ] = m_cartesSelect_Sud[ indextrou + 1 ];
          indextrou += 1;
        }
      }
      if( indextrou > indexarrivee )
      {
        nbdecalage = indextrou - indexarrivee;
        for( int i = 0; i < nbdecalage; i++ )
        {
          m_jeu_Sud[ indextrou ] = m_jeu_Sud[ indextrou - 1 ];
          m_cartesSelect_Sud[ indextrou ] = m_cartesSelect_Sud[ indextrou - 1 ];
          indextrou -= 1;
        }
      }

            m_jeu_Sud[ indexarrivee ] = m_jeucartes[ m_index_carte_a_piocher ];

//copierTableau( m_jeu_Sud, jeuapres, 16 );

            m_cartesSelect_Sud[ indexarrivee ] = PASCARTE;
            m_jeucartes[ m_index_carte_a_piocher ] = PASCARTE;
            m_index_carte_a_piocher += 1;
            verifierPileCartesAPiocher();
            m_nbcartes_Sud += 1;
            afficherNbCartesSud();
            afficherCartesJeuSud();
        }
        else
        {
/// VIENT DE JETEES
      if( indextrou < indexarrivee )
      {
        nbdecalage = indexarrivee - indextrou;
        for( int i = 0; i < nbdecalage; i++ )
        {
          m_jeu_Sud[ indextrou ] = m_jeu_Sud[ indextrou + 1 ];
          m_cartesSelect_Sud[ indextrou ] = m_cartesSelect_Sud[ indextrou + 1 ];
          indextrou += 1;
        }
      }
      if( indextrou > indexarrivee )
      {
        nbdecalage = indextrou - indexarrivee;
        for( int i = 0; i < nbdecalage; i++ )
        {
          m_jeu_Sud[ indextrou ] = m_jeu_Sud[ indextrou - 1 ];
          m_cartesSelect_Sud[ indextrou ] = m_cartesSelect_Sud[ indextrou - 1 ];
          indextrou -= 1;
        }
      }

      m_jeu_Sud[ indexarrivee ] = m_cartesjetees[ m_index_carte_jetee + 1 ];
      m_cartesSelect_Sud[ indexarrivee ] = PASCARTE;
      m_index_carte_jetee += 1;
      m_cartesjetees[ m_index_carte_jetee ] = PASCARTE;
      afficherCartesJeuSud();
      m_nbcartes_Sud += 1;
      afficherNbCartesSud();
    }
  }
  m_feventmouse = false;
  m_movecarteSud = false;
}

void CRami::gaucheReleased_Posees()
{
  bool a_pioche_priscj = false;
  if( m_fclicpioche == true || m_fclicjetee == true )
  {
    a_pioche_priscj = true;
  }

  if( m_fclic1carte == true || m_fcliccartesselect == true )
  {
/// VIENT DE CARTES
    quint8 jeuverif[16];
    for( int i = 0; i < 14; i++ )
    {
      jeuverif[ i ] = 0;
    }
    jeuverif[14] = MARKFIN;
    jeuverif[15] = MARKFIN;
    int nbcarte_nul;
    int coderetour = ERREUR_INCONNUE;

    if( m_fcliccartesselect == true )
    {
      if( a_pioche_priscj == true )
      {
        copierTableau( m_movecartesSelect, jeuverif, 13 );
        m_pBaseJoueurs->trierJeu( jeuverif );
        coderetour = m_pBaseJoueurs->testerSiPeutPoserSud( jeuverif, m_f_a_pose_Sud, m_prise_ds_jetee_Sud, m_listepose/*, SUD*/ );
      }
      else
      {
        coderetour = PAS_PIOCHE_PRISCJ;
      }

      if( coderetour == RETOUR_OK )
      {
/// OK Pose
        nbcarte_nul = m_nbcartesSelect_Sud;
        CRetourPose rp = m_pBaseJoueurs->faireListesPoses( m_cartesposees_Sud, m_flagCP_Sud, m_cartesposees, m_index_carte_posee, jeuverif, nbcarte_nul, m_listepose );

        m_index_carte_posee = rp.indexDansCpj();

        m_imgMemPlateau = m_movecopiePlateau;
        m_afficheurImage->afficherImage( m_imgMemPlateau );

        afficherCartesPoseesJoueurs();

        m_f_a_pose_Sud = true;

        initialiserCartesSelect_Sud();
        m_fcliccartesselect = false;

        if( m_fclicjetee == true )
        {
          m_fcarte_jetee_prise_posee = true;
          m_prise_ds_jetee_Sud = PASCARTE;
        }
      }
      else
      {
        jouerSonErreur();
        afficherErreurPoseSud( coderetour );
        m_imgMemPlateau = m_movecopiePlateau;
        m_afficheurImage->afficherImage( m_imgMemPlateau );

        copierTableau( m_savejeuSud, m_jeu_Sud, 16 );
        m_nbcartes_Sud = m_savenbcartesSud;


/*        m_pBaseJoueurs->supprimerTrouJeu( m_jeu_Sud, 14 );
        int indextrou = 0;
        while( m_jeu_Sud[ indextrou ] != PASCARTE )
        {
          indextrou++;
        }

        for( int i = 0; i < 13; i++ )
        {
          if( m_movecartesSelect[ i ] != PASCARTE )
          {
              m_jeu_Sud[ indextrou ] = m_movecartesSelect[ i ];
              indextrou ++;
              m_nbcartes_Sud += 1;
          }
        }
*/

        initialiserCartesSelect_Sud();
        m_fcliccartesselect = false;

        if( m_fclicjetee == true )
        {
          if( m_fcarte_jetee_prise_posee == false )
          {
            m_cartesjetees[ m_index_carte_jetee ] = m_prise_ds_jetee_Sud;
            quint8 carte = m_prise_ds_jetee_Sud;
            afficherUneCarte( 742, 833, carte );
            m_index_carte_jetee -= 1;
            m_fclicjetee = false;
            m_prise_ds_jetee_Sud = PASCARTE;
            m_pBaseJoueurs->enlever1Carte( m_jeu_Sud, carte );
            m_nbcartes_Sud -= 1;
          }
        }

        afficherCartesJeuSud();
        afficherNbCartesSud();
      }
    }
    else		/// 1 carte
    {
/// VIENT DE 1 CARTE
      int coderetour = ERREUR_INCONNUE;

      if( a_pioche_priscj == false )
      {
        coderetour = PAS_PIOCHE_PRISCJ;
      }
      else
      {
        if( m_f_a_pose_Sud == false )
        {
          coderetour = PAS_ENCORE_POSE;
        }
        else
        {
          if( m_nbcartes_Sud < 1 )
          {
            coderetour = PAS_DERNIERE_CARTE;
          }
          else
          {
            coderetour = PAS_SUR_POSE;
            if( m_fclicjetee == true )
            {
              if( m_fcarte_jetee_prise_posee == false )
              {
                if( m_prise_ds_jeu_Sud != m_prise_ds_jetee_Sud )
                {
                  coderetour = PAS_CJP;
                }
              }
            }
            int positionsurrectposees = calculerPositionSurRectPosees();
            int existe;
            quint8 cartetest = m_prise_ds_jeu_Sud;
            jeuverif[ 0 ] = m_prise_ds_jeu_Sud;
            nbcarte_nul = 1;

            if ( positionsurrectposees == SURPOSE_SUD )
            {
                existe = m_flagCP_Sud.indexOf ( cartetest, 0 );
                if( existe != -1 && coderetour == PAS_SUR_POSE )
                {
                    CRetourPose rp = m_pBaseJoueurs->inserer1CListesPoses( m_cartesposees_Sud, m_flagCP_Sud, m_cartesposees, m_index_carte_posee, jeuverif, nbcarte_nul, cartetest );
                    m_index_carte_posee = rp.indexDansCpj();
                    coderetour = RETOUR_OK;
                }
                existe = m_flagCP_Est.indexOf ( cartetest, 0 );
                if( existe != -1 && coderetour == PAS_SUR_POSE )
                {
                    CRetourPose rp = m_pBaseJoueurs->inserer1CListesPoses( m_cartesposees_Est, m_flagCP_Est, m_cartesposees, m_index_carte_posee, jeuverif, nbcarte_nul, cartetest );
                    m_index_carte_posee = rp.indexDansCpj();
                    coderetour = RETOUR_OK;
                }
                existe = m_flagCP_Nord.indexOf ( cartetest, 0 );
                if( existe != -1 && coderetour == PAS_SUR_POSE )
                {
                    CRetourPose rp = m_pBaseJoueurs->inserer1CListesPoses( m_cartesposees_Nord, m_flagCP_Nord, m_cartesposees, m_index_carte_posee, jeuverif, nbcarte_nul, cartetest );
                    m_index_carte_posee = rp.indexDansCpj();
                    coderetour = RETOUR_OK;
                }
                existe = m_flagCP_Ouest.indexOf ( cartetest, 0 );
                if( existe != -1 && coderetour == PAS_SUR_POSE )
                {
                    CRetourPose rp = m_pBaseJoueurs->inserer1CListesPoses( m_cartesposees_Ouest, m_flagCP_Ouest, m_cartesposees, m_index_carte_posee, jeuverif, nbcarte_nul, cartetest );
                    m_index_carte_posee = rp.indexDansCpj();
                    coderetour = RETOUR_OK;
                }
            }
            else if ( positionsurrectposees == SURPOSE_EST )
            {
                existe = m_flagCP_Est.indexOf ( cartetest, 0 );
                if( existe != -1 && coderetour == PAS_SUR_POSE )
                {
                    CRetourPose rp = m_pBaseJoueurs->inserer1CListesPoses( m_cartesposees_Est, m_flagCP_Est, m_cartesposees, m_index_carte_posee, jeuverif, nbcarte_nul, cartetest );
                    m_index_carte_posee = rp.indexDansCpj();
                    coderetour = RETOUR_OK;
                }
                existe = m_flagCP_Nord.indexOf ( cartetest, 0 );
                if( existe != -1 && coderetour == PAS_SUR_POSE )
                {
                    CRetourPose rp = m_pBaseJoueurs->inserer1CListesPoses( m_cartesposees_Nord, m_flagCP_Nord, m_cartesposees, m_index_carte_posee, jeuverif, nbcarte_nul, cartetest );
                    m_index_carte_posee = rp.indexDansCpj();
                    coderetour = RETOUR_OK;
                }
                existe = m_flagCP_Ouest.indexOf ( cartetest, 0 );
                if( existe != -1 && coderetour == PAS_SUR_POSE )
                {
                    CRetourPose rp = m_pBaseJoueurs->inserer1CListesPoses( m_cartesposees_Ouest, m_flagCP_Ouest, m_cartesposees, m_index_carte_posee, jeuverif, nbcarte_nul, cartetest );
                    m_index_carte_posee = rp.indexDansCpj();
                    coderetour = RETOUR_OK;
                }
                existe = m_flagCP_Sud.indexOf ( cartetest, 0 );
                if( existe != -1 && coderetour == PAS_SUR_POSE )
                {
                    CRetourPose rp = m_pBaseJoueurs->inserer1CListesPoses( m_cartesposees_Sud, m_flagCP_Sud, m_cartesposees, m_index_carte_posee, jeuverif, nbcarte_nul, cartetest );
                    m_index_carte_posee = rp.indexDansCpj();
                    coderetour = RETOUR_OK;
                }
            }
            else if ( positionsurrectposees == SURPOSE_NORD )
            {
                existe = m_flagCP_Nord.indexOf ( cartetest, 0 );
                if( existe != -1 && coderetour == PAS_SUR_POSE )
                {
                    CRetourPose rp = m_pBaseJoueurs->inserer1CListesPoses( m_cartesposees_Nord, m_flagCP_Nord, m_cartesposees, m_index_carte_posee, jeuverif, nbcarte_nul, cartetest );
                    m_index_carte_posee = rp.indexDansCpj();
                    coderetour = RETOUR_OK;
                }
                existe = m_flagCP_Ouest.indexOf ( cartetest, 0 );
                if( existe != -1 && coderetour == PAS_SUR_POSE )
                {
                    CRetourPose rp = m_pBaseJoueurs->inserer1CListesPoses( m_cartesposees_Ouest, m_flagCP_Ouest, m_cartesposees, m_index_carte_posee, jeuverif, nbcarte_nul, cartetest );
                    m_index_carte_posee = rp.indexDansCpj();
                    coderetour = RETOUR_OK;
                }
                existe = m_flagCP_Sud.indexOf ( cartetest, 0 );
                if( existe != -1 && coderetour == PAS_SUR_POSE )
                {
                    CRetourPose rp = m_pBaseJoueurs->inserer1CListesPoses( m_cartesposees_Sud, m_flagCP_Sud, m_cartesposees, m_index_carte_posee, jeuverif, nbcarte_nul, cartetest );
                    m_index_carte_posee = rp.indexDansCpj();
                    coderetour = RETOUR_OK;
                }
                existe = m_flagCP_Est.indexOf ( cartetest, 0 );
                if( existe != -1 && coderetour == PAS_SUR_POSE )
                {
                    CRetourPose rp = m_pBaseJoueurs->inserer1CListesPoses( m_cartesposees_Est, m_flagCP_Est, m_cartesposees, m_index_carte_posee, jeuverif, nbcarte_nul, cartetest );
                    m_index_carte_posee = rp.indexDansCpj();
                    coderetour = RETOUR_OK;
                }
            }
            else if ( positionsurrectposees == SURPOSE_OUEST )
            {
                existe = m_flagCP_Ouest.indexOf ( cartetest, 0 );
                if( existe != -1 && coderetour == PAS_SUR_POSE )
                {
                    CRetourPose rp = m_pBaseJoueurs->inserer1CListesPoses( m_cartesposees_Ouest, m_flagCP_Ouest, m_cartesposees, m_index_carte_posee, jeuverif, nbcarte_nul, cartetest );
                    m_index_carte_posee = rp.indexDansCpj();
                    coderetour = RETOUR_OK;
                }
                existe = m_flagCP_Sud.indexOf ( cartetest, 0 );
                if( existe != -1 && coderetour == PAS_SUR_POSE )
                {
                    CRetourPose rp = m_pBaseJoueurs->inserer1CListesPoses( m_cartesposees_Sud, m_flagCP_Sud, m_cartesposees, m_index_carte_posee, jeuverif, nbcarte_nul, cartetest );
                    m_index_carte_posee = rp.indexDansCpj();
                    coderetour = RETOUR_OK;
                }
                existe = m_flagCP_Est.indexOf ( cartetest, 0 );
                if( existe != -1 && coderetour == PAS_SUR_POSE )
                {
                    CRetourPose rp = m_pBaseJoueurs->inserer1CListesPoses( m_cartesposees_Est, m_flagCP_Est, m_cartesposees, m_index_carte_posee, jeuverif, nbcarte_nul, cartetest );
                    m_index_carte_posee = rp.indexDansCpj();
                    coderetour = RETOUR_OK;
                }
                existe = m_flagCP_Nord.indexOf ( cartetest, 0 );
                if( existe != -1 && coderetour == PAS_SUR_POSE )
                {
                    CRetourPose rp = m_pBaseJoueurs->inserer1CListesPoses( m_cartesposees_Nord, m_flagCP_Nord, m_cartesposees, m_index_carte_posee, jeuverif, nbcarte_nul, cartetest );
                    m_index_carte_posee = rp.indexDansCpj();
                    coderetour = RETOUR_OK;
                }
            }
          }
        }
      }

      if( coderetour == RETOUR_OK )
      {
      /// OK Pose 1 carte
        m_imgMemPlateau = m_movecopiePlateau;
        m_afficheurImage->afficherImage( m_imgMemPlateau );

        afficherCartesPoseesJoueurs();

        if( m_fclicjetee == true )
        {
          m_fcarte_jetee_prise_posee = true;
          m_prise_ds_jetee_Sud = PASCARTE;
        }
      }
      else
      {
      /// Erreur Pose
        jouerSonErreur();
        afficherErreurPoseSud( coderetour );
        m_imgMemPlateau = m_movecopiePlateau;
        m_afficheurImage->afficherImage( m_imgMemPlateau );
        m_jeu_Sud[ m_indexcartecliquee ] = m_prise_ds_jeu_Sud;
        m_nbcartes_Sud += 1;
        m_fclic1carte = false;
        m_prise_ds_jeu_Sud = PASCARTE;

        if( m_fclicjetee == true )
        {
          if( m_fcarte_jetee_prise_posee == false )
          {
            m_cartesjetees[ m_index_carte_jetee ] = m_prise_ds_jetee_Sud;
            quint8 carte = m_prise_ds_jetee_Sud;
            afficherUneCarte( 742, 833, carte );
            m_index_carte_jetee -= 1;
            m_fclicjetee = false;
            m_prise_ds_jetee_Sud = PASCARTE;
            m_pBaseJoueurs->enlever1Carte( m_jeu_Sud, carte );
            m_nbcartes_Sud -= 1;
          }
        }

        afficherCartesJeuSud();
        afficherNbCartesSud();
      }
    }
  }
  else
  {
    jouerSonErreur();
    m_imgMemPlateau = m_movecopiePlateau;
    m_afficheurImage->afficherImage( m_imgMemPlateau );
    if( true == m_fclicpioche )
    {
    /// VIENT DE PIOCHE
      m_fclicpioche = false;
      m_prise_ds_pioche_Sud = PASCARTE;
      }
      else
      {
    /// VIENT DE JETEES
        quint8 carte = m_cartesjetees[ m_index_carte_jetee + 1 ];
        afficherUneCarte( 742, 833, carte );
        m_fclicjetee = false;
        m_prise_ds_jetee_Sud = PASCARTE;
      }
  }
    m_feventmouse = false;
    m_movecarteSud = false;
}

void CRami::gaucheReleased_Jetees()
{
    if( false == m_fclicpioche && false == m_fclicjetee )
    {
        gaucheReleased_Erreur();
    }
    else
    {
        m_imgMemPlateau = m_movecopiePlateau;
        m_afficheurImage->afficherImage( m_imgMemPlateau );

        if( m_fclic1carte == true || m_fcliccartesselect == true )
        {
    /// VIENT DE CARTES
            if( m_fcliccartesselect == true )
            {
                jouerSonErreur();
                copierTableau( m_savejeuSud, m_jeu_Sud, 16 );
                m_nbcartes_Sud = m_savenbcartesSud;

/*
                m_pBaseJoueurs->supprimerTrouJeu( m_jeu_Sud, 14 );
                int indextrou = 0;
                while( m_jeu_Sud[ indextrou ] != PASCARTE )
                {
                    indextrou++;
                }

                for( int i = 0; i < 13; i++ )
                {
                    if( m_movecartesSelect[ i ] != PASCARTE )
                    {
                        m_jeu_Sud[ indextrou ] = m_movecartesSelect[ i ];
                        indextrou ++;
                        m_nbcartes_Sud += 1;
                    }
                }
*/

                initialiserCartesSelect_Sud();
                afficherCartesJeuSud();
                afficherNbCartesSud();
                m_fcliccartesselect = false;
            }
            else		/// 1 carte
            {
    /// VIENT DE 1 CARTE
                m_cartesjetees[m_index_carte_jetee] = m_prise_ds_jeu_Sud;
                m_index_carte_jetee -= 1;
                afficherUneCarte( 742, 833, m_prise_ds_jeu_Sud );
                afficherCartesJeuSud();

                if( m_nbcartes_Sud == 0 )
                {
                    jouerSonGagne( SUD );
                    m_quigagne = SUD;
                    m_periodeJeu = FIN1PARTIE;
                }
                else
                {
                    m_nombredetour += 1;
                    if( m_nombredejoueur == 2 )
                    {
                        m_quijoue = NORD;
                    }
                    else
                    {
                        m_quijoue = EST;
                    }
                }
                m_f_timerActif = true;
                m_timerId = startTimer( m_tempo_timer );
            }
        }
        else
        {
          if( true == m_fclicpioche )
          {
    /// VIENT DE PIOCHE
            jouerSonErreur();
            m_fclicpioche = false;
            m_prise_ds_pioche_Sud = PASCARTE;
          }
          else
          {
    /// VIENT DE JETEES
            quint8 carte = m_cartesjetees[ m_index_carte_jetee + 1 ];
            afficherUneCarte( 742, 833, carte );
            m_fclicjetee = false;
            m_prise_ds_jetee_Sud = PASCARTE;
          }
        }
    }
    m_feventmouse = false;
    m_movecarteSud = false;

}

void CRami::mouseMoveEvent( QMouseEvent * event )
{
    if ( event->buttons() & Qt::LeftButton )
    {
      if( m_movecarteSud == true )
      {
        if( m_fcliccartesselect == true )
        {
          QPoint newposXY = event->pos();
          QPoint oldposXY( m_moveposX, m_moveposY );
          if( newposXY != oldposXY )
          {
            m_imgMemPlateau = m_movecopiePlateau;
            afficherCartesSelect_Sud( newposXY.x()-45, newposXY.y()-65 );
            m_moveposX = newposXY.x();
            m_moveposY = newposXY.y();
          }
        }
        else		/// si 1 carte
        {
          QPoint newposXY = event->pos();
          QPoint oldposXY( m_moveposX, m_moveposY );

          if( newposXY != oldposXY )
          {
            m_imgMemPlateau = m_movecopiePlateau;
            afficherUneCarte( newposXY.x()-45, newposXY.y()-65, m_movesrcX, m_movesrcY );
            m_moveposX = newposXY.x();
            m_moveposY = newposXY.y();
          }
        }
      }
//      else
//      {
//        QWidget::mouseMoveEvent( event );
//      }
    }
//    else
//    {
//      QWidget::mouseMoveEvent( event );
//    }
}

void CRami::afficherErreurPoseSud( const int numerreur )
{
    QString msg;
    switch( numerreur )
    {
      case PAS_TIERCE :
        msg = QString( "Une tierce est nécessaire pour poser" );
        break;
      case PAS_51 :
        msg = QString( "51 points sont nécessaires pour poser" );
        break;
      case PAS_CONFORME :
        msg = QString( "Ensemble de cartes à poser non conforme" );
        break;
      case PAS_CJP :
        msg = QString( "La carte prise doit figurer dans les cartes à poser" );
        break;
      case PAS_SUR_POSE :
        msg = QString( "Aucun emplacement pour poser cette carte" );
        break;
      case PAS_PIOCHE_PRISCJ :
        msg = QString( "Il faut piocher ou prendre la carte jetée avant de poser" );
        break;
      case PAS_ENCORE_POSE :
        msg = QString( "Il faut déjà avoir posé avant de poser 1 carte" );
        break;
      case PAS_DERNIERE_CARTE :
        msg = QString( "La dernière carte est à jeter pour terminer la partie" );
        break;
      default :
        msg = QString( "Erreur inconnue..." );
        break;
    }

    QImage	imgCopie;
    imgCopie = m_imgMemPlateau;

    effacerCartePioche();
    effacerCarteJetee();

    QPainter painter( &m_imgMemPlateau );
    QPen pen;
    pen.setBrush( QColor( 255, 128, 0, 255 ));
    painter.setPen( pen );
    QFont font;
    font.setPointSize(40);
/// qt6    font.setBold(true);
/// qt6    font.setWeight(QFont::Weight( 75 ));
    font.setWeight(QFont::Bold);
    painter.setFont( font );

    QFontMetrics qfm = QFontMetrics( font );
    int w_msg = qfm.horizontalAdvance( msg );
    int x_msg = W_AFF_ECRAN - w_msg;
    x_msg = x_msg / 2;

    painter.drawText( x_msg, 920, msg );
    painter.end();
    m_afficheurImage->afficherImage( m_imgMemPlateau );
    faireTemporisation( 3000 );

    m_imgMemPlateau = imgCopie;
    m_afficheurImage->afficherImage( m_imgMemPlateau );
}

void CRami::sauvegarderEtatJeu()
{
  /// @todo sauvegarderEtatJeu
    m_fsauvegardeEtatJeu = true;
}

void CRami::restaurerEtatJeu()
{
  /// @todo restaurerEtatJeu
    m_fsauvegardeEtatJeu = false;
}

void CRami::initialiserVarJoue_Sud()
{
    m_feventmouse = false;
    m_fclicpioche = false;
    m_fclicjetee = false;
    m_fclic1carte = false;
    m_fcliccartesselect = false;
    m_prise_ds_pioche_Sud = PASCARTE;
    m_prise_ds_jetee_Sud = PASCARTE;
    m_prise_ds_jeu_Sud = PASCARTE;
    m_fcarte_jetee_prise_posee = false;
    m_fsauvegardeEtatJeu = false;
    m_movecarteSud = false;
}

/// FONCTIONS UTILES
void CRami::jouerSonErreur()
{
    m_mySDLPlayer->playSound( "erreur", PERSONNE, m_volume );
}

void CRami::jouerSonReste1Carte( const quint8 qui )
{
    if( true == m_f_sonordi )
    {
      m_mySDLPlayer->playSound("reste1carte", qui, m_volume );
    }
}

void CRami::jouerSonGagne( const quint8 qui )
{
    if( true == m_f_sonordi )
    {
      m_mySDLPlayer->playSound("ordigagne", qui, m_volume );
    }
}

void CRami::jouerSonPiochePrendCarte( const quint8 qui )
{
    if( true == m_f_sonordi )
    {
      m_mySDLPlayer->playSound("jette1carte", qui, m_volume );
    }
}

void CRami::jouerSonPose1Carte()
{
    if( true == m_f_sonordi )
    {
      m_mySDLPlayer->playSound( "pose1carte", PERSONNE, m_volume );
    }
}

void CRami::jouerSonPoseCartes()
{
    if( true == m_f_sonordi )
    {
      m_mySDLPlayer->playSound( "posecartes", PERSONNE, m_volume );
    }
}

void CRami::jouerSonJette1Carte()
{
    if( true == m_f_sonordi )
    {
      m_mySDLPlayer->playSound( "jette1carte", PERSONNE, m_volume );
    }
}

QPoint CRami::calculerSourceXY1Carte( const int carte )
{
    QPoint p;
    int sx, sy;
    if( carte > 80 )                    // DOS
    {
      sx = 0;
      sy = 540;
    }
    if( carte > 60 && carte < 80 )      // TREFLE
    {
      sx = 92 * ( carte - 61 );
      sy = 135 * 3;
    }
    if( carte > 40 && carte < 60 )      // CARREAU
    {
      sx = 92 * ( carte - 41 );
      sy = 135 * 2;
    }
    if( carte > 20 && carte < 40 )      // PIQUE
    {
      sx = 92 * ( carte - 21 );
      sy = 135 * 1;
    }
    if( carte > 0 && carte < 20 )       // COEUR
    {
      sx = 92 * ( carte - 1 );
      sy = 135 * 0;
    }
    p.setX( sx );
    p.setY( sy );
    return p;
}

void CRami::afficherImageDebutJeu()
{
    loadImage( ":/images/DebutJeu.png", &m_imgDebutJeu, 820, 660 );
    m_imgCopie = m_imgMemPlateau;
    QPainter painterN(&m_imgCopie);
    painterN.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painterN.drawImage(324, 234, m_imgDebutJeu);
    painterN.end();
    m_imgMemPlateau = m_imgCopie;
    m_afficheurImage->afficherImage( m_imgMemPlateau );
}

void CRami::loadImage(const QString &fileName, QImage *image, const int width, const int height)
{
    image->load(fileName);

    QImage fixedImage(width, height, QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&fixedImage);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.fillRect(fixedImage.rect(), Qt::transparent);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawImage( 0, 0, *image );
    painter.end();

    *image = fixedImage;
}

void CRami::afficherRectangleOrange( const int x, const int y )
{
    m_imgCopie = m_imgMemPlateau;
    QPainter painter( &m_imgCopie );
    QPen pen;
    pen.setWidth( 3 );
    pen.setBrush( QColor( 255, 128, 0, 255 ));
    painter.setPen( pen );
    painter.drawRect( x, y, 174, 128 );
    painter.end();
    m_imgMemPlateau = m_imgCopie;
    m_afficheurImage->afficherImage( m_imgMemPlateau );
}

void CRami::effacerRectangleOrange( const int x, const int y )
{
    m_imgCopie = m_imgMemPlateau;
    QPainter painter( &m_imgCopie );
    QPen pen;
    pen.setWidth( 3 );
    pen.setBrush( QColor( 0, 102, 0, 255 ));
    painter.setPen( pen );
    painter.drawRect( x, y, 174, 128 );
    painter.end();
    m_imgMemPlateau = m_imgCopie;
    m_afficheurImage->afficherImage( m_imgMemPlateau );
}

void CRami::afficherMsgQuiGagne( const QString &msg )
{
    m_imgCopie = m_imgMemPlateau;
    QPainter painter( &m_imgCopie );
    QPen pen;
    pen.setBrush( QColor( 255, 128, 0, 255 ));
    painter.setPen( pen );
    QFont font;
    font.setPointSize(48);
/// qt6    font.setBold(true);
/// qt6    font.setWeight(QFont::Weight( 75 ));
    font.setWeight(QFont::Bold);
    painter.setFont( font );

    QFontMetrics qfm = QFontMetrics( font );
    int w_msg = qfm.horizontalAdvance( msg );
    int x_msg = W_AFF_ECRAN - w_msg;
    x_msg = x_msg / 2;

    painter.drawText( x_msg, 920, msg );
    painter.end();
    m_imgMemPlateau = m_imgCopie;
    m_afficheurImage->afficherImage( m_imgMemPlateau );
}

void CRami::afficherMsgNom( const QString &msg, const int x, const int y )
{
    m_imgCopie = m_imgMemPlateau;
    QPainter painter( &m_imgCopie );
    QPen pen;
    pen.setBrush( QColor( 255, 128, 0, 255 ));
    painter.setPen( pen );
    QFont font;
    font.setPointSize(48);
/// qt6    font.setBold(true);
/// qt6    font.setWeight(QFont::Weight( 75 ));
    font.setWeight(QFont::Bold);
    painter.setFont( font );
    painter.drawText( x, y + 66, msg );
    painter.end();
    m_imgMemPlateau = m_imgCopie;
    m_afficheurImage->afficherImage( m_imgMemPlateau );
}

void CRami::afficherMsgPenalite( const QString &msg, const int x, const int y )
{
    m_imgCopie = m_imgMemPlateau;
    QPainter painter( &m_imgCopie );
    QPen pen;
    pen.setBrush( QColor( 255, 128, 0, 255 ));
    painter.setPen( pen );
    QFont font;
    font.setPointSize(48);
/// qt6    font.setBold(true);
/// qt6    font.setWeight(QFont::Weight( 75 ));
    font.setWeight(QFont::Bold);
    painter.setFont( font );
    painter.drawText( x, y + 126, msg );
    painter.end();
    m_imgMemPlateau = m_imgCopie;
    m_afficheurImage->afficherImage( m_imgMemPlateau );
}

void CRami::afficherMsgSB( const QString &msg )
{
    m_messagestatusbar = msg;
    emit statusBarModified();
}

void CRami::afficherAnimationPrendCarte( const int carte, const quint8 joueur )
{
    afficherAnimationPiocheCarte( carte, joueur );
}

void CRami::afficherAnimationPiocheCarte( const int carte, const quint8 joueur )
{
    if( true == m_f_animecarte )
    {
      int incX, incY, posX, posY;
      QImage imgCopie;
      if( joueur == EST )
      {
          incX = 48;
          posX = 733 - 48;
      }
      if( joueur == NORD )
      {
          incX = 0;
          posX = 733;
      }
      if( joueur == OUEST )
      {
          incX = -48;
          posX = 733 + 48;
      }
      incY = 62;
      posY = 902 + 62;

      imgCopie = m_imgMemPlateau;

      for( int i = 0; i < 13; i++ )
      {
        m_imgMemPlateau = imgCopie;
        posX = posX + incX;
        posY = posY - incY;
        if( posY >= 90 )
        {
          afficherUneCarte( posX - 45, posY - 65, carte );
          faireTemporisation( m_tempo_anime );
        }
      }

      m_imgMemPlateau = imgCopie;
      m_afficheurImage->afficherImage( m_imgMemPlateau );
    }
}

void CRami::afficherAnimationJetteCarte( const int carte, const quint8 joueur )
{
    if( true == m_f_animecarte )
    {
      int incX, incY, posX, posY;
      QImage imgCopie;
      if( joueur == EST )
      {
          incX = -48;
          posX = 1357 + 48;
      }
      if( joueur == NORD )
      {
          incX = 0;
          posX = 733;
      }
      if( joueur == OUEST )
      {
          incX = 48;
          posX = 109 - 48;
      }
      incY = 62;
      posY = 90 - 62;

      imgCopie = m_imgMemPlateau;

      for( int i = 0; i < 13; i++ )
      {
        m_imgMemPlateau = imgCopie;
        posX = posX + incX;
        posY = posY + incY;
        if( posY <= 902 )
        {
          afficherUneCarte( posX - 45, posY - 65, carte );
          faireTemporisation( m_tempo_anime );
        }
      }

      m_imgMemPlateau = imgCopie;
      m_afficheurImage->afficherImage( m_imgMemPlateau );
    }
}

void CRami::faireTemporisation( const long temporisation )
{
    double aa = 0.0;
    double bb = 0.0;

    for( long i = 0; i < temporisation; i++ )
    {
        for( int i = 0; i < TEMPO_VALEUR_BOUCLE; i++ )
        {
            for( int i = 0; i < TEMPO_VALEUR_BOUCLE; i++ )
            {
                aa = 11.11;
                bb = 11.11;
                aa = aa * bb;
                bb = bb * 1.1;
                aa = aa * bb;
                bb = bb / 2.2;
                aa = aa * bb;
                bb = bb * 3.3;
                aa = aa * bb;
                bb = bb / 4.4;
                aa = aa * bb;
                qApp->processEvents( QEventLoop::ExcludeUserInputEvents );
            }
        }
    }

    // long n;
    // QTime t;

    // t.start();

    // while ( true )
    // {
    //   n = t.elapsed();
    //   if ( n > temporisation )	break;
    //   qApp->processEvents( QEventLoop::ExcludeUserInputEvents );
    // }
}


/// FONCTIONS PUBLIQUES
void CRami::setNombreJoueur( const int nbj )
{
    m_nombredejoueur = nbj;
}

void CRami::setArretPartie()
{
    if(true == m_f_timerActif)
    {
        killTimer( m_timerId );
    }
    afficherImageDebutJeu();
    m_periodeJeu = ATTENTE;
    m_f_EtatPartie = false;
    m_f_Fin1partie = false;
}

bool CRami::getEtatPartie()
{
    return m_f_EtatPartie;
}

QString CRami::getMessageStatusBar()
{
    return	m_messagestatusbar;
}

void CRami::setSiAnimeCarte( const bool f_animecarte )
{
    m_f_animecarte = f_animecarte;
}

void CRami::setNiveauJeu( const int niveau )
{
    m_niveaujeu = niveau;
}

void CRami::setVolume( const int volume )
{
    m_volume = volume;
}

void CRami::setSiSonOrdi( const bool f_son )
{
    m_f_sonordi = f_son;
}

void CRami::setVitesse( const int vitesse )
{
    m_vitesse = vitesse;
    switch( vitesse )
    {
      case VITESSE_LENTE :
        m_tempo_anime = TEMPO_ANIME * 2;
        m_tempo_finpartie = TEMPO_FINPARTIE * 2;
        m_tempo_attente = TEMPO_ATTENTE * 2;
        m_tempo_timer = TEMPO_TIMER * 2;
        break;
      case VITESSE_NORMALE :
        m_tempo_anime = TEMPO_ANIME;
        m_tempo_finpartie = TEMPO_FINPARTIE;
        m_tempo_attente = TEMPO_ATTENTE;
        m_tempo_timer = TEMPO_TIMER;
        break;
      case VITESSE_RAPIDE :
        m_tempo_anime = TEMPO_ANIME / 2;
        m_tempo_finpartie = TEMPO_FINPARTIE / 2;
        m_tempo_attente = TEMPO_ATTENTE / 2;
        m_tempo_timer = TEMPO_TIMER / 2;
        break;
    }
}

void CRami::afficherCartesSelect_Sud( const int posX, const int posY )
{
    int dx = posX;
    int dy = posY;
    QPoint p;
    int carte;

    for( int i = 0; i < 13; i++ )
    {
      carte = m_movecartesSelect[ i ];
      if( carte == 0 )	break;
      p = calculerSourceXY1Carte( carte );
      dessinerUneCarte( dx, dy, p.x(), p.y() );
      dx= dx + 91;
    }
    m_afficheurImage->afficherImage( m_imgMemPlateau );
}

void CRami::verifierPileCartesAPiocher()
{
    if( m_jeucartes[ m_index_carte_a_piocher ] != MARKFIN )
    {
      return;
    }

    quint8 carte = m_cartesjetees[ m_index_carte_jetee + 1 ];
    m_index_carte_jetee += 1;
    m_cartesjetees[ m_index_carte_jetee ] = PASCARTE;

    while( m_cartesjetees[ m_index_carte_jetee + 1 ] != MARKFIN )
    {
      m_index_carte_jetee += 1;
      m_index_carte_a_piocher -= 1;
      m_jeucartes[ m_index_carte_a_piocher ] = m_cartesjetees[ m_index_carte_jetee ];
      m_cartesjetees[ m_index_carte_jetee ] = PASCARTE;
    }

    m_cartesjetees[ m_index_carte_jetee ] = carte;
    m_index_carte_jetee -= 1;
}

void CRami::piocher1Carte( quint8 * ajeu, bool verifPile )
{
    ajeu[ 13 ] = m_jeucartes[ m_index_carte_a_piocher ];
    m_jeucartes[ m_index_carte_a_piocher ] = PASCARTE;
    m_index_carte_a_piocher += 1;
    if( verifPile == true )
    {
      verifierPileCartesAPiocher();
    }
}

void CRami::jeter1Carte( quint8 * ajeu, quint8 carte )
{
    m_pBaseJoueurs->enlever1Carte( ajeu, carte );

    m_cartesjetees[m_index_carte_jetee] = carte;
    m_index_carte_jetee -= 1;
    afficherUneCarte( 742, 833, carte );
}




/// @todo MIS POUR TEST
void CRami::insererCartesDansJeu( quint8 * aJeu, int nbcartes )
{
  CTricher ic( aJeu, nbcartes, this );

  bool rep = ic.exec();
  if( true == rep )
  {
    for( int i = 0; i < 14; i++ )
    {
      aJeu[ i ] = ic.m_aJeu[ i ];
    }
  }
}
///

void CRami::afficherCartesPoseesJoueurs()
{
    effacerCartesPosees();
    afficherCartesPosees( m_cartesposees_Est, 5, 171 );
    afficherCartesPosees( m_cartesposees_Nord, 5, 333 );
    afficherCartesPosees( m_cartesposees_Ouest, 5, 495 );
    afficherCartesPosees( m_cartesposees_Sud, 5, 657 );
}

void CRami::afficherCartesPoseesSud()
{
    afficherCartesPosees( m_cartesposees_Sud, 5, 657 );
}

void CRami::afficherCartesPoseesEst()
{
    afficherCartesPosees( m_cartesposees_Est, 5, 171 );
}

void CRami::afficherCartesPoseesNord()
{
    afficherCartesPosees( m_cartesposees_Nord, 5, 333 );
}

void CRami::afficherCartesPoseesOuest()
{
    afficherCartesPosees( m_cartesposees_Ouest, 5, 495 );
}

void CRami::afficherCartesPosees( QList<quint8> & qlpose, const int posX, const int posY )
{
    int nbitems;
    int w_cartes;
    int x_cartes;
    int x_vides;

    int w_pose = W_AFF_ECRAN;

    nbitems = qlpose.count();

    w_cartes = ( nbitems - 1 ) * 91;
    if( w_cartes > w_pose )
    {
      x_cartes = ( w_pose - 91 ) / ( nbitems - 2 );
      x_vides = x_cartes;
    }
    else
    {
      x_vides = 91;
      x_cartes = 91;
    }

    int dx = posX;
    int dy = posY;
    QPoint p;
    int carte;

    QListIterator<quint8> it1( qlpose );
    while( it1.hasNext() )
    {
      carte = it1.next();

      if( carte == PASCARTE )
      {
        dx= dx + x_vides;
      }
      else
      {
        p = calculerSourceXY1Carte( carte );
        afficherUneCarte( dx, dy, p.x(), p.y() );
        dx= dx + x_cartes;
      }
    }
}

quint8 CRami::prendreCarteJetee( quint8 * ajeu )
{
    quint8 cjp = PASCARTE;

    quint8 carte = m_cartesjetees[ m_index_carte_jetee + 2 ];
    afficherUneCarte( 742, 833, carte );

    cjp = m_cartesjetees[ m_index_carte_jetee + 1 ];
    ajeu[ 13 ] = cjp;

    m_index_carte_jetee += 1;
    m_cartesjetees[ m_index_carte_jetee ] = PASCARTE;

    return cjp;
}

bool CRami::simulerPrendreCarteJetee( quint8 * ajeu, const bool a_pose )
{
    quint8 cjp = m_cartesjetees[ m_index_carte_jetee + 1 ];

    bool estPresente = verifierPresence1Carte( ajeu, cjp );
    if( estPresente == true )   return false;

    bool peutPoser = false;
    quint8	copiejeu[16];
    copierTableau( ajeu, copiejeu, 16 );

    ajeu[13] = cjp;
    m_pBaseJoueurs->trierJeu( ajeu );
    peutPoser = m_pBaseJoueurs->testerSiPeutPoserOrdi( ajeu, a_pose, cjp, m_listepose );

    copierTableau( copiejeu, ajeu, 16 );

    return peutPoser;
}

bool CRami::simulerPoserCarteJetee( quint8 * ajeu, const bool a_pose, const int nbcartes )
{
    quint8 cpj = m_cartesjetees[ m_index_carte_jetee + 1 ];

    bool estPresente = verifierPresence1Carte( ajeu, cpj );
    if( estPresente == true )   return false;

    bool aPoser = false;
    int existe;

    existe = -1;
    if( existe == -1 ) existe = m_flagCP_Est.indexOf ( cpj, 0 );
    if( existe == -1 ) existe = m_flagCP_Nord.indexOf ( cpj, 0 );
    if( existe == -1 ) existe = m_flagCP_Ouest.indexOf ( cpj, 0 );
    if( existe == -1 ) existe = m_flagCP_Sud.indexOf ( cpj, 0 );

    if( existe != -1 )
    {
      if( nbcartes == 3 )
      {
        bool aS2 = m_pBaseJoueurs->aSuite2( ajeu, a_pose );
        bool aM2 = m_pBaseJoueurs->aMeme2( ajeu, a_pose );
        if( aS2 == true || aM2 == true )
        {
          if( m_nbcartes_Sud <= 2 || m_nbcartes_Est <= 2 || m_nbcartes_Nord <= 2 || m_nbcartes_Ouest <= 2 )
          {
            if( m_niveaujeu == NIVEAU_FAIBLE )
            {
              aPoser = true;
            }
            else
            {
              int hasard = genererHasard_0_103();
              if( hasard >= 52 )
              {
                aPoser = true;
              }
            }
          }
        }
        else
        {
          aPoser = true;
        }
      }
      else
      {
        aPoser = true;
      }
    }

    return aPoser;
}

quint8 CRami::calculerCarteAJeter( const bool a_poseJS, quint8 * ajeu, const bool a_pose, const int nbcartes, int & nbpaspose )
{
    quint8 carte;
    carte = chercherCartePlusPetiteValeur( ajeu, a_pose, nbpaspose );

    if( a_poseJS == true )  /// si Joueur Suivant a posé
    {
      int copieNbCartes = nbcartes;
      quint8	copiejeu[16];
      copierTableau( ajeu, copiejeu, 16 );

      int existe;
      quint8 carteA;
      bool okCarte = false;
      carteA = carte;

      while( copieNbCartes > 0 && okCarte == false )
      {
        int rien = 0;
        carte = chercherCartePlusPetiteValeur( ajeu, a_pose, rien );

        existe = -1;
        if( existe == -1 )    existe = m_flagCP_Est.indexOf ( carte, 0 );
        if( existe == -1 )    existe = m_flagCP_Nord.indexOf ( carte, 0 );
        if( existe == -1 )    existe = m_flagCP_Ouest.indexOf ( carte, 0 );
        if( existe == -1 )    existe = m_flagCP_Sud.indexOf ( carte, 0 );

        if( existe != -1 )
        {
          m_pBaseJoueurs->enlever1Carte( ajeu, carte );
          copieNbCartes -= 1;
        }
        else
        {
          okCarte = true;
        }
      }

      copierTableau( copiejeu, ajeu, 16 );
      if( copieNbCartes == 0 )    carte = carteA;
    }

    return carte;
}

void CRami::copierTableau( quint8 * aorg, quint8 * acopie, int nbitems )
{
    for( int i = 0; i < nbitems; i++ )
    {
      acopie[ i ] = aorg[ i ];
    }
}

bool CRami::verifierPresence1Carte( quint8 * jeu, quint8 carte )
{
    for( int i = 0; i < 14; i++ )
    {
      if( carte == jeu[ i ] )   return true;
    }
    return false;
}

int CRami::calculerPositionSurRectPosees()
{
    int pos = SURPOSE_SUD;
    if ( m_posClicY >= 171 && m_posClicY <= 332 )  pos = SURPOSE_EST;
    else if ( m_posClicY >= 333 && m_posClicY <= 494 )  pos = SURPOSE_NORD;
    else if ( m_posClicY >= 495 && m_posClicY <= 656 )  pos = SURPOSE_OUEST;
  //  else if ( m_posClicY >= 657 && m_posClicY <= 818 )  pos = SURPOSE_SUD;
    return pos;
}


QList<CResultat> * CRami::getListeResultats()
{
    return ( &m_qlresultat );
}
