#include <QTableWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTableWidgetItem>


#include "cstatistique.h"
#include "cresultat.h"

CStatistique::CStatistique( QList<CResultat> *qlresultats, int nbjoueurs, QWidget *parent )
 : QDialog( parent )
{
    m_qlresultats = qlresultats;
    m_nbjoueurs = nbjoueurs;

    m_widthItem = 110;

    m_tableWidget = new QTableWidget( m_qlresultats->count(), m_nbjoueurs );
    switch( m_nbjoueurs )
    {
      case 2 :
        m_tableWidget->setHorizontalHeaderLabels( QStringList() << " SUD " << "NORD " );
        break;
      case 3 :
        m_tableWidget->setHorizontalHeaderLabels( QStringList() << " SUD " << " EST " << "OUEST" );
        break;
      case 4 :
        m_tableWidget->setHorizontalHeaderLabels( QStringList() << " SUD " << " EST " << "NORD " << "OUEST" );
        break;
    }

    addRows();

    m_tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_tableWidget->setEditTriggers( QAbstractItemView::NoEditTriggers );

    m_okButton = new QPushButton("OK");
    m_okButton->setDefault(true);
    connect( m_okButton, SIGNAL( clicked() ), this, SLOT( accept() ) );

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget( m_okButton );

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget( m_tableWidget );
    mainLayout->addLayout( buttonLayout );
    setLayout( mainLayout );

    setWindowTitle( "Rami - Résultats Parties" );
    setMinimumWidth( m_widthItem * m_nbjoueurs );
}

void CStatistique::addRows()
{
    switch( m_nbjoueurs )
    {
      case 2 :
        for( int row = 0; row < m_qlresultats->count(); ++row )
        {
          CResultat resultat = m_qlresultats->at( row );

          QTableWidgetItem *item0 = new QTableWidgetItem( QString::number( resultat.penaliteSud ) );
          item0->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
          m_tableWidget->setItem(row, 0, item0);

          QTableWidgetItem *item1 = new QTableWidgetItem( QString::number( resultat.penaliteNord ) );
          item1->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
          m_tableWidget->setItem(row, 1, item1);
        }
        break;
      case 3 :
        for( int row = 0; row < m_qlresultats->count(); ++row )
        {
          CResultat resultat = m_qlresultats->at( row );

          QTableWidgetItem *item0 = new QTableWidgetItem( QString::number( resultat.penaliteSud ) );
          item0->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
          m_tableWidget->setItem(row, 0, item0);

          QTableWidgetItem *item1 = new QTableWidgetItem( QString::number( resultat.penaliteEst ) );
          item1->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
          m_tableWidget->setItem(row, 1, item1);

          QTableWidgetItem *item2 = new QTableWidgetItem( QString::number( resultat.penaliteOuest ) );
          item2->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
          m_tableWidget->setItem(row, 2, item2);
        }
        break;
      case 4 :
        for( int row = 0; row < m_qlresultats->count(); ++row )
        {
          CResultat resultat = m_qlresultats->at( row );

          QTableWidgetItem *item0 = new QTableWidgetItem( QString::number( resultat.penaliteSud ) );
          item0->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
          m_tableWidget->setItem(row, 0, item0);

          QTableWidgetItem *item1 = new QTableWidgetItem( QString::number( resultat.penaliteEst ) );
          item1->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
          m_tableWidget->setItem(row, 1, item1);

          QTableWidgetItem *item2 = new QTableWidgetItem( QString::number( resultat.penaliteNord ) );
          item2->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
          m_tableWidget->setItem(row, 2, item2);

          QTableWidgetItem *item3 = new QTableWidgetItem( QString::number( resultat.penaliteOuest ) );
          item3->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
          m_tableWidget->setItem(row, 3, item3);
        }
        break;
    }
}
