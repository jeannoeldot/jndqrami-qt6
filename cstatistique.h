#ifndef CSTATISTIQUE_H
#define CSTATISTIQUE_H

#include <QDialog>
#include <QList>

class QPushButton;
class QTableWidget;
class CResultat;

class CStatistique : public QDialog
{
    Q_OBJECT

public:
    CStatistique( QList<CResultat> *qlresultats, int nbjoueurs, QWidget *parent = 0 );

private:
    void addRows();

    QList<CResultat> *m_qlresultats;
    int m_nbjoueurs;
    QPushButton *m_okButton;
    QTableWidget *m_tableWidget;
    int m_widthItem;

signals:

public slots:

};

#endif // CSTATISTIQUE_H
