#include "cpreferences.h"

#include "constantes.h"

CPreferences::CPreferences(QWidget* parent)
: QDialog( parent ), Ui::DlgPreferences()
{
    setupUi(this);
}

CPreferences::CPreferences(bool anime, bool son, int niveau, int vitesse, int volume, QWidget* parent)
: QDialog( parent ), Ui::DlgPreferences()
{
    setupUi(this);

    m_f_animecarte = anime;
    m_f_son = son;
    m_niveaujeu = niveau;
    m_vitesse = vitesse;
    m_volume= volume;
    initialiserValeurs();

    listWidget->setCurrentRow( 0 );
}

void CPreferences::reject()
{
    QDialog::reject();
}

void CPreferences::accept()
{
    if( true == animeCB->isChecked() )
    {
      m_f_animecarte = true;
    }
    else
    {
      m_f_animecarte = false;
    }

    if( true == sonsCB->isChecked() )
    {
      m_f_son = true;
      m_volume = volumeHSlider->sliderPosition();
    }
    else
    {
      m_f_son = false;
      m_volume = 12;
    }

    if(true == faibleRB->isChecked())  m_niveaujeu = NIVEAU_FAIBLE;
    if(true == moyenRB->isChecked()) m_niveaujeu = NIVEAU_MOYEN;
    if(true == fortRB->isChecked()) m_niveaujeu = NIVEAU_FORT;

    if(true == vlenteRB->isChecked())  m_vitesse = VITESSE_LENTE;
    if(true == vnormaleRB->isChecked()) m_vitesse = VITESSE_NORMALE;
    if(true == vrapideRB->isChecked()) m_vitesse = VITESSE_RAPIDE;

    QDialog::accept();
}

void CPreferences::initialiserValeurs()
{
    if( m_f_animecarte == true )
    {
      animeCB->setChecked( true );
    }
    else
    {
      animeCB->setChecked( false );
      animeGroupBox->setVisible( false );
    }

    if( m_f_son == true )
    {
      sonsCB->setChecked( true );
      volumeHSlider->setSliderPosition ( m_volume );
    }
    else
    {
      sonsCB->setChecked( false );
      volumeHSlider->setSliderPosition( 12 );
      volumeGroupBox->setVisible( false );
    }

    switch( m_niveaujeu )
    {
      case NIVEAU_FAIBLE :
        faibleRB->setChecked( true );
        moyenRB->setChecked( false );
        fortRB->setChecked( false );
        break;
      case NIVEAU_MOYEN :
        faibleRB->setChecked( false );
        moyenRB->setChecked( true );
        fortRB->setChecked( false );
        break;
      case NIVEAU_FORT :
        faibleRB->setChecked( false );
        moyenRB->setChecked( false );
        fortRB->setChecked( true );
        break;
    }

    switch( m_vitesse )
    {
      case VITESSE_LENTE :
        vlenteRB->setChecked( true );
        vnormaleRB->setChecked( false );
        vrapideRB->setChecked( false );
        break;
      case VITESSE_NORMALE :
        vlenteRB->setChecked( false );
        vnormaleRB->setChecked( true );
        vrapideRB->setChecked( false );
        break;
      case VITESSE_RAPIDE :
        vlenteRB->setChecked( false );
        vnormaleRB->setChecked( false );
        vrapideRB->setChecked( true );
        break;
    }
}
