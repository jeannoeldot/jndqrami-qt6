#ifndef CVALEURCARTE_H
#define CVALEURCARTE_H

#include <QWidget>

class CValeurCarte
{
public:
    CValeurCarte() {
        m_couleur = -1;
        m_index = -1;
        m_valeurcarte = -1;
    }

    void setCouleur(const int couleur) { m_couleur = couleur; }
    void setIndex(const int index) { m_index = index; }
    void setValeurCarte(const quint8 valeurcarte) { m_valeurcarte = valeurcarte; }
    int couleur() const { return m_couleur; }
    int index() const { return m_index; }
    quint8 valeurCarte() const { return m_valeurcarte; }

private:
    int m_couleur;
    int m_index;
    int m_valeurcarte;
};

#endif // CVALEURCARTE_H
