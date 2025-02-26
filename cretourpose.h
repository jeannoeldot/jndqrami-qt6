#ifndef CRETOURPOSE_H
#define CRETOURPOSE_H

class CRetourPose
{
public:
    CRetourPose() {
        m_isOk = false;
        m_nbcartesjeu = -1;
        m_indexcpj = -1;
    }

    void setIndexCpj(const int index) { m_indexcpj = index; }
    void setIsOk(const bool ok) { m_isOk = ok; }
    void setNombreCartesJeu(const int nbcartes) { m_nbcartesjeu = nbcartes; }
    bool isOk() const { return m_isOk; }
    int nombreCartesJeu() const { return m_nbcartesjeu; }
    int indexDansCpj() const { return m_indexcpj; }

private:
    bool m_isOk;
    int m_nbcartesjeu;
    int m_indexcpj;
};

#endif // CRETOURPOSE_H
