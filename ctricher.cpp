#include "ctricher.h"
#include "constantes.h"

CTricher::CTricher(QWidget* parent)
: QDialog( parent ), Ui::Dialog()
{
    setupUi(this);
}

CTricher::CTricher(quint8 * aJeu, int nbcartes, QWidget* parent)
: QDialog( parent ), Ui::Dialog()
{
    setupUi(this);

    m_nbcartes = nbcartes;
    copierJeu( aJeu );
    initialiser();
    quint8 carte = PASCARTE;
    for( int i = 0; i < 14; i++ )
    {
      carte = aJeu[ i ];
      if( carte != PASCARTE )
      {
        calculerNumCarte( carte );
        if( m_icouleur == 0 )
        {
          cocherBoxCoeur( m_numcarte );
        }
        if( m_icouleur == 1 )
        {
          cocherBoxPique( m_numcarte );
        }
        if( m_icouleur == 2 )
        {
          cocherBoxCarreau( m_numcarte );
        }
        if( m_icouleur == 3 )
        {
          cocherBoxTrefle( m_numcarte );
        }
      }
    }
}

void CTricher::reject()
{
    QDialog::reject();
}

void CTricher::accept()
{
  for( int i = 0; i < 14; i++ )
  {
    m_aJeu[ i ] = 0;
  }
  int nbcartes = 0;
  if( nbcartes <= m_nbcartes && true == CO_1_0->isChecked() )
  {
    m_aJeu[ nbcartes ] = 1;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == CO_1_1->isChecked() )
  {
    m_aJeu[ nbcartes ] = 1;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == CO_2_0->isChecked() )
  {
    m_aJeu[ nbcartes ] = 2;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == CO_2_1->isChecked() )
  {
    m_aJeu[ nbcartes ] = 2;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == CO_3_0->isChecked() )
  {
    m_aJeu[ nbcartes ] = 3;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == CO_3_1->isChecked() )
  {
    m_aJeu[ nbcartes ] = 3;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == CO_4_0->isChecked() )
  {
    m_aJeu[ nbcartes ] = 4;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == CO_4_1->isChecked() )
  {
    m_aJeu[ nbcartes ] = 4;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == CO_5_0->isChecked() )
  {
    m_aJeu[ nbcartes ] = 5;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == CO_5_1->isChecked() )
  {
    m_aJeu[ nbcartes ] = 5;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == CO_6_0->isChecked() )
  {
    m_aJeu[ nbcartes ] = 6;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == CO_6_1->isChecked() )
  {
    m_aJeu[ nbcartes ] = 6;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == CO_7_0->isChecked() )
  {
    m_aJeu[ nbcartes ] = 7;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == CO_7_1->isChecked() )
  {
    m_aJeu[ nbcartes ] = 7;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == CO_8_0->isChecked() )
  {
    m_aJeu[ nbcartes ] = 8;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == CO_8_1->isChecked() )
  {
    m_aJeu[ nbcartes ] = 8;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == CO_9_0->isChecked() )
  {
    m_aJeu[ nbcartes ] = 9;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == CO_9_1->isChecked() )
  {
    m_aJeu[ nbcartes ] = 9;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == CO_10_0->isChecked() )
  {
    m_aJeu[ nbcartes ] = 10;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == CO_10_1->isChecked() )
  {
    m_aJeu[ nbcartes ] = 10;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == CO_11_0->isChecked() )
  {
    m_aJeu[ nbcartes ] = 11;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == CO_11_1->isChecked() )
  {
    m_aJeu[ nbcartes ] = 11;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == CO_12_0->isChecked() )
  {
    m_aJeu[ nbcartes ] = 12;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == CO_12_1->isChecked() )
  {
    m_aJeu[ nbcartes ] = 12;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == CO_13_0->isChecked() )
  {
    m_aJeu[ nbcartes ] = 13;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == CO_13_1->isChecked() )
  {
    m_aJeu[ nbcartes ] = 13;
    nbcartes++;
  }

  if( nbcartes <= m_nbcartes && true == PI_1_0->isChecked() )
  {
    m_aJeu[ nbcartes ] = 21;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == PI_1_1->isChecked() )
  {
    m_aJeu[ nbcartes ] = 21;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == PI_2_0->isChecked() )
  {
    m_aJeu[ nbcartes ] = 22;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == PI_2_1->isChecked() )
  {
    m_aJeu[ nbcartes ] = 22;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == PI_3_0->isChecked() )
  {
    m_aJeu[ nbcartes ] = 23;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == PI_3_1->isChecked() )
  {
    m_aJeu[ nbcartes ] = 23;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == PI_4_0->isChecked() )
  {
    m_aJeu[ nbcartes ] = 24;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == PI_4_1->isChecked() )
  {
    m_aJeu[ nbcartes ] = 24;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == PI_5_0->isChecked() )
  {
    m_aJeu[ nbcartes ] = 25;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == PI_5_1->isChecked() )
  {
    m_aJeu[ nbcartes ] = 25;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == PI_6_0->isChecked() )
  {
    m_aJeu[ nbcartes ] = 26;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == PI_6_1->isChecked() )
  {
    m_aJeu[ nbcartes ] = 26;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == PI_7_0->isChecked() )
  {
    m_aJeu[ nbcartes ] = 27;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == PI_7_1->isChecked() )
  {
    m_aJeu[ nbcartes ] = 27;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == PI_8_0->isChecked() )
  {
    m_aJeu[ nbcartes ] = 28;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == PI_8_1->isChecked() )
  {
    m_aJeu[ nbcartes ] = 28;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == PI_9_0->isChecked() )
  {
    m_aJeu[ nbcartes ] = 29;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == PI_9_1->isChecked() )
  {
    m_aJeu[ nbcartes ] = 29;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == PI_10_0->isChecked() )
  {
    m_aJeu[ nbcartes ] = 30;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == PI_10_1->isChecked() )
  {
    m_aJeu[ nbcartes ] = 30;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == PI_11_0->isChecked() )
  {
    m_aJeu[ nbcartes ] = 31;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == PI_11_1->isChecked() )
  {
    m_aJeu[ nbcartes ] = 31;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == PI_12_0->isChecked() )
  {
    m_aJeu[ nbcartes ] = 32;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == PI_12_1->isChecked() )
  {
    m_aJeu[ nbcartes ] = 32;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == PI_13_0->isChecked() )
  {
    m_aJeu[ nbcartes ] = 33;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == PI_13_1->isChecked() )
  {
    m_aJeu[ nbcartes ] = 33;
    nbcartes++;
  }

  if( nbcartes <= m_nbcartes && true == CA_1_0->isChecked() )
  {
    m_aJeu[ nbcartes ] = 41;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == CA_1_1->isChecked() )
  {
    m_aJeu[ nbcartes ] = 41;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == CA_2_0->isChecked() )
  {
    m_aJeu[ nbcartes ] = 42;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == CA_2_1->isChecked() )
  {
    m_aJeu[ nbcartes ] = 42;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == CA_3_0->isChecked() )
  {
    m_aJeu[ nbcartes ] = 43;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == CA_3_1->isChecked() )
  {
    m_aJeu[ nbcartes ] = 43;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == CA_4_0->isChecked() )
  {
    m_aJeu[ nbcartes ] = 44;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == CA_4_1->isChecked() )
  {
    m_aJeu[ nbcartes ] = 44;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == CA_5_0->isChecked() )
  {
    m_aJeu[ nbcartes ] = 45;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == CA_5_1->isChecked() )
  {
    m_aJeu[ nbcartes ] = 45;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == CA_6_0->isChecked() )
  {
    m_aJeu[ nbcartes ] = 46;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == CA_6_1->isChecked() )
  {
    m_aJeu[ nbcartes ] = 46;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == CA_7_0->isChecked() )
  {
    m_aJeu[ nbcartes ] = 47;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == CA_7_1->isChecked() )
  {
    m_aJeu[ nbcartes ] = 47;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == CA_8_0->isChecked() )
  {
    m_aJeu[ nbcartes ] = 48;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == CA_8_1->isChecked() )
  {
    m_aJeu[ nbcartes ] = 48;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == CA_9_0->isChecked() )
  {
    m_aJeu[ nbcartes ] = 49;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == CA_9_1->isChecked() )
  {
    m_aJeu[ nbcartes ] = 49;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == CA_10_0->isChecked() )
  {
    m_aJeu[ nbcartes ] = 50;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == CA_10_1->isChecked() )
  {
    m_aJeu[ nbcartes ] = 50;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == CA_11_0->isChecked() )
  {
    m_aJeu[ nbcartes ] = 51;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == CA_11_1->isChecked() )
  {
    m_aJeu[ nbcartes ] = 51;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == CA_12_0->isChecked() )
  {
    m_aJeu[ nbcartes ] = 52;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == CA_12_1->isChecked() )
  {
    m_aJeu[ nbcartes ] = 52;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == CA_13_0->isChecked() )
  {
    m_aJeu[ nbcartes ] = 53;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == CA_13_1->isChecked() )
  {
    m_aJeu[ nbcartes ] = 53;
    nbcartes++;
  }

  if( nbcartes <= m_nbcartes && true == TR_1_0->isChecked() )
  {
    m_aJeu[ nbcartes ] = 61;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == TR_1_1->isChecked() )
  {
    m_aJeu[ nbcartes ] = 61;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == TR_2_0->isChecked() )
  {
    m_aJeu[ nbcartes ] = 62;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == TR_2_1->isChecked() )
  {
    m_aJeu[ nbcartes ] = 62;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == TR_3_0->isChecked() )
  {
    m_aJeu[ nbcartes ] = 63;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == TR_3_1->isChecked() )
  {
    m_aJeu[ nbcartes ] = 63;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == TR_4_0->isChecked() )
  {
    m_aJeu[ nbcartes ] = 64;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == TR_4_1->isChecked() )
  {
    m_aJeu[ nbcartes ] = 64;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == TR_5_0->isChecked() )
  {
    m_aJeu[ nbcartes ] = 65;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == TR_5_1->isChecked() )
  {
    m_aJeu[ nbcartes ] = 65;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == TR_6_0->isChecked() )
  {
    m_aJeu[ nbcartes ] = 66;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == TR_6_1->isChecked() )
  {
    m_aJeu[ nbcartes ] = 66;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == TR_7_0->isChecked() )
  {
    m_aJeu[ nbcartes ] = 67;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == TR_7_1->isChecked() )
  {
    m_aJeu[ nbcartes ] = 67;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == TR_8_0->isChecked() )
  {
    m_aJeu[ nbcartes ] = 68;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == TR_8_1->isChecked() )
  {
    m_aJeu[ nbcartes ] = 68;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == TR_9_0->isChecked() )
  {
    m_aJeu[ nbcartes ] = 69;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == TR_9_1->isChecked() )
  {
    m_aJeu[ nbcartes ] = 69;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == TR_10_0->isChecked() )
  {
    m_aJeu[ nbcartes ] = 70;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == TR_10_1->isChecked() )
  {
    m_aJeu[ nbcartes ] = 70;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == TR_11_0->isChecked() )
  {
    m_aJeu[ nbcartes ] = 71;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == TR_11_1->isChecked() )
  {
    m_aJeu[ nbcartes ] = 71;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == TR_12_0->isChecked() )
  {
    m_aJeu[ nbcartes ] = 72;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == TR_12_1->isChecked() )
  {
    m_aJeu[ nbcartes ] = 72;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == TR_13_0->isChecked() )
  {
    m_aJeu[ nbcartes ] = 73;
    nbcartes++;
  }
  if( nbcartes <= m_nbcartes && true == TR_13_1->isChecked() )
  {
    m_aJeu[ nbcartes ] = 73;
    nbcartes++;
  }

  QDialog::accept();
}

void CTricher::copierJeu( quint8 * ajeu )
{
  for( int i = 0; i < 14; i++ )
  {
    m_aJeu[ i ] = ajeu[ i ];
  }
}

void CTricher::calculerNumCarte( quint8 carte )
{
  if( carte != PASCARTE )
  {
    quint8 numcarte;
    int icouleur;
    if( carte >= 1 && carte <= 13 )
    {
      numcarte = carte;
      icouleur = 0;
    }
    if( carte >= 21 && carte <= 33 )
    {
      numcarte = carte - 20;
      icouleur = 1;
    }
    if( carte >= 41 && carte <= 53 )
    {
      numcarte = carte - 40;
      icouleur = 2;
    }
    if( carte >= 61 && carte <= 73 )
    {
      numcarte = carte - 60;
      icouleur = 3;
    }

    m_icouleur = icouleur;
    m_numcarte = numcarte;
  }
}

void CTricher::cocherBoxCoeur( quint8 carte )
{
  if( carte == 1 )
  {
    if( true == CO_1_0->isChecked() ) CO_1_1->setChecked( true );
    else CO_1_0->setChecked( true );
  }
  if( carte == 2 )
  {
    if( true == CO_2_0->isChecked() ) CO_2_1->setChecked( true );
    else CO_2_0->setChecked( true );
  }
  if( carte == 3 )
  {
    if( true == CO_3_0->isChecked() ) CO_3_1->setChecked( true );
    else CO_3_0->setChecked( true );
  }
  if( carte == 4 )
  {
    if( true == CO_4_0->isChecked() ) CO_4_1->setChecked( true );
    else CO_4_0->setChecked( true );
  }
  if( carte == 5 )
  {
    if( true == CO_5_0->isChecked() ) CO_5_1->setChecked( true );
    else  CO_5_0->setChecked( true );
  }
  if( carte == 6 )
  {
    if( true == CO_6_0->isChecked() ) CO_6_1->setChecked( true );
    else  CO_6_0->setChecked( true );
  }
  if( carte == 7 )
  {
    if( true == CO_7_0->isChecked() ) CO_7_1->setChecked( true );
    else  CO_7_0->setChecked( true );
  }
  if( carte == 8 )
  {
    if( true == CO_8_0->isChecked() ) CO_8_1->setChecked( true );
    else  CO_8_0->setChecked( true );
  }
  if( carte == 9 )
  {
    if( true == CO_9_0->isChecked() ) CO_9_1->setChecked( true );
    else  CO_9_0->setChecked( true );
  }
  if( carte == 10 )
  {
    if( true == CO_10_0->isChecked() ) CO_10_1->setChecked( true );
    else  CO_10_0->setChecked( true );
  }
  if( carte == 11 )
  {
    if( true == CO_11_0->isChecked() ) CO_11_1->setChecked( true );
    else  CO_11_0->setChecked( true );
  }
  if( carte == 12 )
  {
    if( true == CO_12_0->isChecked() ) CO_12_1->setChecked( true );
    else  CO_12_0->setChecked( true );
  }
  if( carte == 13 )
  {
    if( true == CO_13_0->isChecked() ) CO_13_1->setChecked( true );
    else  CO_13_0->setChecked( true );
  }
}

void CTricher::cocherBoxPique( quint8 carte )
{
  if( carte == 1 )
  {
    if( true == PI_1_0->isChecked() ) PI_1_1->setChecked( true );
    else PI_1_0->setChecked( true );
  }
  if( carte == 2 )
  {
    if( true == PI_2_0->isChecked() ) PI_2_1->setChecked( true );
    else PI_2_0->setChecked( true );
  }
  if( carte == 3 )
  {
    if( true == PI_3_0->isChecked() ) PI_3_1->setChecked( true );
    else PI_3_0->setChecked( true );
  }
  if( carte == 4 )
  {
    if( true == PI_4_0->isChecked() ) PI_4_1->setChecked( true );
    else PI_4_0->setChecked( true );
  }
  if( carte == 5 )
  {
    if( true == PI_5_0->isChecked() ) PI_5_1->setChecked( true );
    else  PI_5_0->setChecked( true );
  }
  if( carte == 6 )
  {
    if( true == PI_6_0->isChecked() ) PI_6_1->setChecked( true );
    else  PI_6_0->setChecked( true );
  }
  if( carte == 7 )
  {
    if( true == PI_7_0->isChecked() ) PI_7_1->setChecked( true );
    else  PI_7_0->setChecked( true );
  }
  if( carte == 8 )
  {
    if( true == PI_8_0->isChecked() ) PI_8_1->setChecked( true );
    else  PI_8_0->setChecked( true );
  }
  if( carte == 9 )
  {
    if( true == PI_9_0->isChecked() ) PI_9_1->setChecked( true );
    else  PI_9_0->setChecked( true );
  }
  if( carte == 10 )
  {
    if( true == PI_10_0->isChecked() ) PI_10_1->setChecked( true );
    else  PI_10_0->setChecked( true );
  }
  if( carte == 11 )
  {
    if( true == PI_11_0->isChecked() ) PI_11_1->setChecked( true );
    else  PI_11_0->setChecked( true );
  }
  if( carte == 12 )
  {
    if( true == PI_12_0->isChecked() ) PI_12_1->setChecked( true );
    else  PI_12_0->setChecked( true );
  }
  if( carte == 13 )
  {
    if( true == PI_13_0->isChecked() ) PI_13_1->setChecked( true );
    else  PI_13_0->setChecked( true );
  }
}

void CTricher::cocherBoxCarreau( quint8 carte )
{
  if( carte == 1 )
  {
    if( true == CA_1_0->isChecked() ) CA_1_1->setChecked( true );
    else CA_1_0->setChecked( true );
  }
  if( carte == 2 )
  {
    if( true == CA_2_0->isChecked() ) CA_2_1->setChecked( true );
    else CA_2_0->setChecked( true );
  }
  if( carte == 3 )
  {
    if( true == CA_3_0->isChecked() ) CA_3_1->setChecked( true );
    else CA_3_0->setChecked( true );
  }
  if( carte == 4 )
  {
    if( true == CA_4_0->isChecked() ) CA_4_1->setChecked( true );
    else CA_4_0->setChecked( true );
  }
  if( carte == 5 )
  {
    if( true == CA_5_0->isChecked() ) CA_5_1->setChecked( true );
    else  CA_5_0->setChecked( true );
  }
  if( carte == 6 )
  {
    if( true == CA_6_0->isChecked() ) CA_6_1->setChecked( true );
    else  CA_6_0->setChecked( true );
  }
  if( carte == 7 )
  {
    if( true == CA_7_0->isChecked() ) CA_7_1->setChecked( true );
    else  CA_7_0->setChecked( true );
  }
  if( carte == 8 )
  {
    if( true == CA_8_0->isChecked() ) CA_8_1->setChecked( true );
    else  CA_8_0->setChecked( true );
  }
  if( carte == 9 )
  {
    if( true == CA_9_0->isChecked() ) CA_9_1->setChecked( true );
    else  CA_9_0->setChecked( true );
  }
  if( carte == 10 )
  {
    if( true == CA_10_0->isChecked() ) CA_10_1->setChecked( true );
    else  CA_10_0->setChecked( true );
  }
  if( carte == 11 )
  {
    if( true == CA_11_0->isChecked() ) CA_11_1->setChecked( true );
    else  CA_11_0->setChecked( true );
  }
  if( carte == 12 )
  {
    if( true == CA_12_0->isChecked() ) CA_12_1->setChecked( true );
    else  CA_12_0->setChecked( true );
  }
  if( carte == 13 )
  {
    if( true == CA_13_0->isChecked() ) CA_13_1->setChecked( true );
    else  CA_13_0->setChecked( true );
  }
}

void CTricher::cocherBoxTrefle( quint8 carte )
{
  if( carte == 1 )
  {
    if( true == TR_1_0->isChecked() ) TR_1_1->setChecked( true );
    else TR_1_0->setChecked( true );
  }
  if( carte == 2 )
  {
    if( true == TR_2_0->isChecked() ) TR_2_1->setChecked( true );
    else TR_2_0->setChecked( true );
  }
  if( carte == 3 )
  {
    if( true == TR_3_0->isChecked() ) TR_3_1->setChecked( true );
    else TR_3_0->setChecked( true );
  }
  if( carte == 4 )
  {
    if( true == TR_4_0->isChecked() ) TR_4_1->setChecked( true );
    else TR_4_0->setChecked( true );
  }
  if( carte == 5 )
  {
    if( true == TR_5_0->isChecked() ) TR_5_1->setChecked( true );
    else  TR_5_0->setChecked( true );
  }
  if( carte == 6 )
  {
    if( true == TR_6_0->isChecked() ) TR_6_1->setChecked( true );
    else  TR_6_0->setChecked( true );
  }
  if( carte == 7 )
  {
    if( true == TR_7_0->isChecked() ) TR_7_1->setChecked( true );
    else  TR_7_0->setChecked( true );
  }
  if( carte == 8 )
  {
    if( true == TR_8_0->isChecked() ) TR_8_1->setChecked( true );
    else  TR_8_0->setChecked( true );
  }
  if( carte == 9 )
  {
    if( true == TR_9_0->isChecked() ) TR_9_1->setChecked( true );
    else  TR_9_0->setChecked( true );
  }
  if( carte == 10 )
  {
    if( true == TR_10_0->isChecked() ) TR_10_1->setChecked( true );
    else  TR_10_0->setChecked( true );
  }
  if( carte == 11 )
  {
    if( true == TR_11_0->isChecked() ) TR_11_1->setChecked( true );
    else  TR_11_0->setChecked( true );
  }
  if( carte == 12 )
  {
    if( true == TR_12_0->isChecked() ) TR_12_1->setChecked( true );
    else  TR_12_0->setChecked( true );
  }
  if( carte == 13 )
  {
    if( true == TR_13_0->isChecked() ) TR_13_1->setChecked( true );
    else  TR_13_0->setChecked( true );
  }
}

void CTricher::initialiser()
{
    CO_1_0->setChecked( false );
    CO_1_1->setChecked( false );
    CO_2_0->setChecked( false );
    CO_2_1->setChecked( false );
    CO_3_0->setChecked( false );
    CO_3_1->setChecked( false );
    CO_4_0->setChecked( false );
    CO_4_1->setChecked( false );
    CO_5_0->setChecked( false );
    CO_5_1->setChecked( false );
    CO_6_0->setChecked( false );
    CO_6_1->setChecked( false );
    CO_7_0->setChecked( false );
    CO_7_1->setChecked( false );
    CO_8_0->setChecked( false );
    CO_8_1->setChecked( false );
    CO_9_0->setChecked( false );
    CO_9_1->setChecked( false );
    CO_10_0->setChecked( false );
    CO_10_1->setChecked( false );
    CO_11_0->setChecked( false );
    CO_11_1->setChecked( false );
    CO_12_0->setChecked( false );
    CO_12_1->setChecked( false );
    CO_13_0->setChecked( false );
    CO_13_1->setChecked( false );

    PI_1_0->setChecked( false );
    PI_1_1->setChecked( false );
    PI_2_0->setChecked( false );
    PI_2_1->setChecked( false );
    PI_3_0->setChecked( false );
    PI_3_1->setChecked( false );
    PI_4_0->setChecked( false );
    PI_4_1->setChecked( false );
    PI_5_0->setChecked( false );
    PI_5_1->setChecked( false );
    PI_6_0->setChecked( false );
    PI_6_1->setChecked( false );
    PI_7_0->setChecked( false );
    PI_7_1->setChecked( false );
    PI_8_0->setChecked( false );
    PI_8_1->setChecked( false );
    PI_9_0->setChecked( false );
    PI_9_1->setChecked( false );
    PI_10_0->setChecked( false );
    PI_10_1->setChecked( false );
    PI_11_0->setChecked( false );
    PI_11_1->setChecked( false );
    PI_12_0->setChecked( false );
    PI_12_1->setChecked( false );
    PI_13_0->setChecked( false );
    PI_13_1->setChecked( false );

    CA_1_0->setChecked( false );
    CA_1_1->setChecked( false );
    CA_2_0->setChecked( false );
    CA_2_1->setChecked( false );
    CA_3_0->setChecked( false );
    CA_3_1->setChecked( false );
    CA_4_0->setChecked( false );
    CA_4_1->setChecked( false );
    CA_5_0->setChecked( false );
    CA_5_1->setChecked( false );
    CA_6_0->setChecked( false );
    CA_6_1->setChecked( false );
    CA_7_0->setChecked( false );
    CA_7_1->setChecked( false );
    CA_8_0->setChecked( false );
    CA_8_1->setChecked( false );
    CA_9_0->setChecked( false );
    CA_9_1->setChecked( false );
    CA_10_0->setChecked( false );
    CA_10_1->setChecked( false );
    CA_11_0->setChecked( false );
    CA_11_1->setChecked( false );
    CA_12_0->setChecked( false );
    CA_12_1->setChecked( false );
    CA_13_0->setChecked( false );
    CA_13_1->setChecked( false );

    TR_1_0->setChecked( false );
    TR_1_1->setChecked( false );
    TR_2_0->setChecked( false );
    TR_2_1->setChecked( false );
    TR_3_0->setChecked( false );
    TR_3_1->setChecked( false );
    TR_4_0->setChecked( false );
    TR_4_1->setChecked( false );
    TR_5_0->setChecked( false );
    TR_5_1->setChecked( false );
    TR_6_0->setChecked( false );
    TR_6_1->setChecked( false );
    TR_7_0->setChecked( false );
    TR_7_1->setChecked( false );
    TR_8_0->setChecked( false );
    TR_8_1->setChecked( false );
    TR_9_0->setChecked( false );
    TR_9_1->setChecked( false );
    TR_10_0->setChecked( false );
    TR_10_1->setChecked( false );
    TR_11_0->setChecked( false );
    TR_11_1->setChecked( false );
    TR_12_0->setChecked( false );
    TR_12_1->setChecked( false );
    TR_13_0->setChecked( false );
    TR_13_1->setChecked( false );
}
