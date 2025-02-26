#ifndef CMEME_H
#define CMEME_H

class CMeme
{
public:
    CMeme();
    CMeme(const int nbcartes, const int nbpoints);

    int index1ereCarte();
    int index2emeCarte();
    int index3emeCarte();
    int index4emeCarte();
    int nombreCartes();
    int nombrePoints();
    void setIndex1ereCarte(const int index1c);
    void setIndex2emeCarte(const int index2c);
    void setIndex3emeCarte(const int index3c);
    void setIndex4emeCarte(const int index4c);
    void setNombreCartes(const int nbcartes);
    void setNombrePoints(const int nbpoints);

    bool operator == (const CMeme & autre);
    CMeme operator + (const CMeme & autre);
    CMeme operator - (const CMeme & autre);

protected:
    int m_index1erecarte;
    int m_index2emecarte;
    int m_index3emecarte;
    int m_index4emecarte;
    int m_nbcartes;
    int m_nbpoints;

private:
    bool isEgal(const int une, const int autre);
};

#endif // CMEME_H
