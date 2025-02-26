#include "cmeme.h"

CMeme::CMeme()
{
    m_nbpoints = 0;
    m_nbcartes = 0;
    m_index1erecarte = -1;
    m_index2emecarte = -1;
    m_index3emecarte = -1;
    m_index4emecarte = -1;
}

CMeme::CMeme(const int nbcartes, const int nbpoints)
{
    m_nbpoints = nbpoints;
    m_nbcartes = nbcartes;
}

int CMeme::index1ereCarte()
{
    return m_index1erecarte;
}

int CMeme::index2emeCarte()
{
    return m_index2emecarte;
}

int CMeme::index3emeCarte()
{
    return m_index3emecarte;
}

int CMeme::index4emeCarte()
{
    return m_index4emecarte;
}

int CMeme::nombreCartes()
{
    return m_nbcartes;
}

int CMeme::nombrePoints()
{
    return m_nbpoints;
}

void CMeme::setIndex1ereCarte(const int index1c)
{
    m_index1erecarte = index1c;
}

void CMeme::setIndex2emeCarte(const int index2c)
{
    m_index2emecarte = index2c;
}

void CMeme::setIndex3emeCarte(const int index3c)
{
    m_index3emecarte = index3c;
}

void CMeme::setIndex4emeCarte(const int index4c)
{
    m_index4emecarte = index4c;
}

void CMeme::setNombreCartes(const int nbcartes)
{
    m_nbcartes = nbcartes;
}

void CMeme::setNombrePoints(const int nbpoints)
{
    m_nbpoints = nbpoints;
}

bool CMeme::operator == (const CMeme & autre)
{
    if(false == isEgal(m_nbcartes, autre.m_nbcartes))
        return false;
    if(false == isEgal(m_nbpoints, autre.m_nbpoints))
        return false;
    return true;
}

bool CMeme::isEgal(const int une, const int autre)
{
    return une == autre;
}

CMeme CMeme::operator + (const CMeme & autre)
{
    return CMeme(m_nbcartes + autre.m_nbcartes, m_nbpoints + autre.m_nbpoints);
}

CMeme CMeme::operator - (const CMeme & autre)
{
  return CMeme(m_nbcartes - autre.m_nbcartes, m_nbpoints - autre.m_nbpoints);
}
