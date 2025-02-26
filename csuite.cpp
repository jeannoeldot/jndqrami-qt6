#include "csuite.h"

CSuite::CSuite()
{
    m_nbpoints = 0;
    m_nbcartes = 0;
    m_index1c = -1;
    m_couleur = -1;
}

CSuite::CSuite(const int nbcartes, const int nbpoints)
{
    m_nbpoints = nbpoints;
    m_nbcartes = nbcartes;
}

void CSuite::setNombrePoints(const int nbpoints)
{
    m_nbpoints = nbpoints;
}

void CSuite::setNombreCartes(const int nbcartes)
{
    m_nbcartes = nbcartes;
}

void CSuite::setIndex1ereCarte(const int index)
{
    m_index1c = index;
}

void CSuite::setCouleur(const int couleur)
{
  m_couleur = couleur;
}

int CSuite::nombrePoints()
{
    return m_nbpoints;
}

int CSuite::nombreCartes()
{
    return m_nbcartes;
}

int CSuite::index1ereCarte()
{
    return m_index1c;
}

int CSuite::couleur()
{
    return m_couleur;
}

bool CSuite::operator == (const CSuite & autre)
{
    if(false == isEgal(m_nbcartes, autre.m_nbcartes))
        return false;
    if(false == isEgal(m_nbpoints, autre.m_nbpoints))
        return false;
    return true;
}

bool CSuite::isEgal(const int une, const int autre)
{
    return une == autre;
}

CSuite CSuite::operator + (const CSuite & autre)
{
    return CSuite(m_nbcartes + autre.m_nbcartes, m_nbpoints + autre.m_nbpoints);
}

CSuite CSuite::operator - (const CSuite & autre)
{
    return CSuite(m_nbcartes - autre.m_nbcartes, m_nbpoints - autre.m_nbpoints);
}
