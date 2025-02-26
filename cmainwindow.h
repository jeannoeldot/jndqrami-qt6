#ifndef CMAINWINDOW_H
#define CMAINWINDOW_H

#include <QMainWindow>

class CRami;
class CHelpBrowser;

class CMainWindow : public QMainWindow
{
    Q_OBJECT

public:
///    CMainWindow(const int w_ecran, const int h_ecran, QWidget *parent = 0);
/// Modifs qt6
    CMainWindow(QWidget *parent = 0);
    ~CMainWindow();

private slots:
    void slotDebutJeu2();
    void slotDebutJeu3();
    void slotDebutJeu4();
    void slotArretPartie();
    void slotQuitterProg();
    void slotPreferences();
    void slotStatistiques();
    void slotAide();
    void slotAbout();
    void slotAboutQt();

public slots:
    void afficherMsgStatusBar();

private:
    void createActions();
    void createMenus();
    void createStatusBar();

    void readSettings();
    void writeSettings();

    void afficherStatusBar();
    bool testerSiPartieEnCours();
    void setPreferences();

    QPoint m_pos;
    int		m_heightMenuBar;
    int		m_heightStatusBar;
    QString	m_messagestatusbar;

    CRami		*m_pcrami;
    CHelpBrowser *m_pchelpbrowser;

    int m_nbjoueurs;

    bool m_f_animecarte;
    bool m_f_son;
    int m_niveaujeu;
    int m_vitesse;
    int m_volume;

    QAction *m_debutjeu2Act;
    QAction *m_debutjeu3Act;
    QAction *m_debutjeu4Act;
    QAction *m_arretpartieAct;
    QAction *m_quitterAct;
    QAction *m_preferenceAct;
    QAction *m_statistiqueAct;
    QAction *m_aideAct;
    QAction *m_aboutAct;
    QAction *m_aboutQtAct;

    QMenu *m_jeuMenu;
    QMenu *m_optionsMenu;
    QMenu *m_aideMenu;

protected:
    void closeEvent( QCloseEvent *event );
};

#endif // CMAINWINDOW_H
