#ifndef CPREFERENCES_H
#define CPREFERENCES_H

#include "ui_DlgPreferences.h"

class CPreferences : public QDialog, private Ui::DlgPreferences
{
    Q_OBJECT

public:
    CPreferences(QWidget* parent = 0);
    CPreferences(bool anime, bool son, int niveau, int vitesse, int volume, QWidget* parent = 0);

    bool isCarteAnimated() const { return m_f_animecarte; }
    bool isSonEnabled() const { return m_f_son; }
    int niveauJeu() const { return m_niveaujeu; }
    int vitesseJeu() const { return m_vitesse; }
    int volumeSon() const { return m_volume; }

private:
    void initialiserValeurs();

    bool m_f_animecarte;
    bool m_f_son;
    int m_niveaujeu;
    int m_vitesse;
    int m_volume;

protected slots:
    virtual void  reject();
    virtual void  accept();

protected:
};

#endif // CPREFERENCES_H
