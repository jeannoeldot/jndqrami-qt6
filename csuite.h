#ifndef CSUITE_H
#define CSUITE_H

class CSuite
{
public:
    CSuite();
    CSuite(const int nbcartes, const int nbpoints);
    void setNombrePoints(const int nbpoints);
    void setNombreCartes(const int nbcartes);
    void setIndex1ereCarte(const int index);
    void setCouleur(const int couleur);

    int nombrePoints();
    int nombreCartes();
    int index1ereCarte();
    int couleur();

    bool operator == (const CSuite & autre);
    CSuite operator + (const CSuite & autre);
    CSuite operator - (const CSuite & autre);

protected:
    int m_nbpoints;
    int m_nbcartes;
    int m_index1c;
    int m_couleur;

private:
    bool isEgal(const int une, const int autre);
};

#endif // CSUITE_H
