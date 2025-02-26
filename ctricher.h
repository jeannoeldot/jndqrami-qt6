#ifndef CTRICHER_H
#define CTRICHER_H

#include "ui_DlgInsererCartes.h"

class CTricher : public QDialog, private Ui::Dialog
{
    Q_OBJECT

public:
    CTricher(QWidget *parent = 0);
    CTricher( quint8 * aJeu, int nbcartes, QWidget *parent = 0 );

    quint8 m_aJeu[16];

protected slots:
    virtual void    reject();
    virtual void    accept();

private:
    void copierJeu( quint8 * ajeu );
    void calculerNumCarte( quint8 carte );
    void cocherBoxCoeur( quint8 carte );
    void cocherBoxPique( quint8 carte );
    void cocherBoxCarreau( quint8 carte );
    void cocherBoxTrefle( quint8 carte );
    void initialiser();

    int m_icouleur;
    quint8 m_numcarte;
    int m_nbcartes;

signals:

public slots:

};

#endif // CTRICHER_H
