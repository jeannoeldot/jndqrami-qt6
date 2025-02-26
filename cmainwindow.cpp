#include <QDebug>

#include <QApplication>
#include <QMainWindow>
#include <QAction>
#include <QMenuBar>
#include <QStatusBar>
#include <QSettings>
#include <QMessageBox>
#include <QCloseEvent>


#include "cmainwindow.h"
#include "constantes.h"
#include "miseaupoint.h"
#include "chelpbrowser.h"
#include "crami.h"
#include "cresultat.h"
#include "cpreferences.h"
#include "cstatistique.h"


///CMainWindow::CMainWindow(const int w_ecran, const int h_ecran, QWidget *parent)
/// Modifs qt6
CMainWindow::CMainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(NOM_PROG);
    setWindowIcon(QIcon(":/icons/IconeProg.png"));

    createActions();
    createMenus();
    createStatusBar();

    m_pcrami = new CRami(this);
    setCentralWidget( m_pcrami );

    // int x_mw;
    // int y_mw;
    // if(h_ecran >= 1024)
    // {
    //   x_mw = 50;
    //   y_mw = 50;
    // }
    // else
    // {
    //   x_mw = 0;
    //   y_mw = 0;
    // }
/// Modifs qt6
    int x_mw;
    int y_mw;
///    h_ecran >= 1024
    x_mw = 50;
    y_mw = 50;

    m_pos = QPoint(x_mw, y_mw);

    readSettings();

    QSize size = QSize(918, 655+30+m_heightMenuBar+m_heightStatusBar);
    setMinimumSize(size);
    setMaximumSize(size);
    resize(size);
    move(m_pos);

    connect(m_pcrami, SIGNAL(statusBarModified()), this, SLOT(afficherMsgStatusBar()));

    m_statistiqueAct->setEnabled(false);

    setPreferences();

    m_pchelpbrowser = 0;
}

CMainWindow::~CMainWindow()
{

}

void CMainWindow::slotDebutJeu2()
{
    if(false == testerSiPartieEnCours())
    {
        m_nbjoueurs = 2;
        m_statistiqueAct->setEnabled(true);
        m_pcrami->setNombreJoueur( 2 );
         m_pcrami->debuterJeu();
    }
}
void CMainWindow::slotDebutJeu3()
{
    if(false == testerSiPartieEnCours())
    {
        m_nbjoueurs = 3;
        m_statistiqueAct->setEnabled(true);
        m_pcrami->setNombreJoueur( 3 );
        m_pcrami->debuterJeu();
    }
}
void CMainWindow::slotDebutJeu4()
{
    if(false == testerSiPartieEnCours())
    {
        m_nbjoueurs = 4;
        m_statistiqueAct->setEnabled(true);
        m_pcrami->setNombreJoueur( 4 );
        m_pcrami->debuterJeu();
    }
}

void CMainWindow::slotArretPartie()
{
    if( true == m_pcrami->getEtatPartie())
    {
        QMessageBox msgBox(QMessageBox::Question, NOM_PROG + " - Arrêter",
                            "   Arrêter la partie en cours ?   \n",
                             { },
                            this);
//        msgBox.setInformativeText(" Texte information ");
        msgBox.addButton(QMessageBox::No);
        msgBox.addButton(QMessageBox::Yes);
        int reponse = msgBox.exec();
        switch (reponse)
        {
            case QMessageBox::No:           /// Continuer
                break;
            case QMessageBox::Yes:          /// Arrêter
                m_pcrami->setArretPartie();
                m_statistiqueAct->setEnabled(false);
                break;
            default:
                break;
        }
    }
}

void CMainWindow::slotPreferences()
{
  CPreferences preferences(m_f_animecarte, m_f_son, m_niveaujeu, m_vitesse, m_volume, this);

    bool reponse = preferences.exec();
    if(true == reponse)
    {
      m_f_animecarte = preferences.isCarteAnimated();
      m_f_son = preferences.isSonEnabled();
      m_niveaujeu = preferences.niveauJeu();
      m_vitesse = preferences.vitesseJeu();
      m_volume = preferences.volumeSon();
      setPreferences();
    }
}

void CMainWindow::slotStatistiques()
{
    QList<CResultat> * p_qlresultats;
    p_qlresultats = m_pcrami->getListeResultats();

    CStatistique statistiques(p_qlresultats, m_nbjoueurs, this);
    statistiques.exec();
}

void CMainWindow::slotAide()
{
    if(!m_pchelpbrowser)
    {
        m_pchelpbrowser = new CHelpBrowser(":/html", "Rami.html");
    }

    m_pchelpbrowser->show();
    m_pchelpbrowser->activateWindow();
}

void CMainWindow::slotAbout()
{
    QMessageBox::about(this, "À propos de " + NOM_PROG,
                        QString("<h2>%1 %2</h2>"
                        "<p><h3>Simple petit jeu de cartes...</h3>"
                        "<p>Copyright %3."
                        "<p>Ce programme est distribué selon les termes de la GPL v2."
                        "<p>")
            .arg(NOM_PROG)
            .arg(VERSION_PROG)
            .arg(COPYRIGHT)
             );
}

void CMainWindow::slotAboutQt()
{
    QMessageBox::aboutQt(this, "About Qt");
}

bool CMainWindow::testerSiPartieEnCours()
{
    if( true == m_pcrami->getEtatPartie())
    {
        QMessageBox msgBox(QMessageBox::Information, NOM_PROG + " - Jouer",
                           QString("Partie en cours..."
                           "\n\nFaire d'abord Menu Jeu : Arrêter ou F5"
                           "\net ensuite Menu Jeu : Jouer ou F2, F3, F4\n"),
                           { },
                           this);
        msgBox.exec();
        return true;
    }
    else
    {
        return false;
    }
}

void CMainWindow::slotQuitterProg()
{
    QMessageBox msgBox(QMessageBox::Question, NOM_PROG + " - Quitter",
                       "   Quitter le jeu Rami ?   \n",
                       { },
                       this);
    //        msgBox.setInformativeText(" Texte information ");
    msgBox.addButton(QMessageBox::No);
    msgBox.addButton(QMessageBox::Yes);
    int reponse = msgBox.exec();
    switch (reponse)
    {
        case QMessageBox::No:           /// Continuer
            break;
        case QMessageBox::Yes:          /// Arrêter
            writeSettings();
            /// qt6            qApp->quit();
            qApp->exit();
            break;
        default:
            break;
    }
}

void CMainWindow::closeEvent(QCloseEvent *event)
{
    QString msg = statusBar()->currentMessage();
    m_messagestatusbar = "Fermer le jeu?";
    afficherStatusBar();

    QMessageBox msgBox(QMessageBox::Question, NOM_PROG + " - Fermer",
                       "   Fermer le jeu Rami ?   \n",
                       { },
                       this);
    //        msgBox.setInformativeText(" Texte information ");
    msgBox.addButton(QMessageBox::No);
    msgBox.addButton(QMessageBox::Yes);
    int reponse = msgBox.exec();
    switch (reponse)
    {
        case QMessageBox::No:           /// Continuer
            statusBar()->showMessage(msg);
            event->ignore();
            break;
        case QMessageBox::Yes:          /// Arrêter
            writeSettings();
            event->accept();
            break;
        default:
            break;
    }
}

void CMainWindow::createActions()
{
    m_debutjeu2Act = new QAction("2 Joueurs", this);
    m_debutjeu2Act->setShortcut(Qt::Key_F2);
    m_debutjeu2Act->setStatusTip("Débuter une partie à 2 Joueurs");
    connect(m_debutjeu2Act, SIGNAL(triggered()), this, SLOT(slotDebutJeu2()));

    m_debutjeu3Act = new QAction("3 Joueurs", this);
    m_debutjeu3Act->setShortcut(Qt::Key_F3);
    m_debutjeu3Act->setStatusTip("Débuter une partie à 3 Joueurs");
    connect(m_debutjeu3Act, SIGNAL(triggered()), this, SLOT(slotDebutJeu3()));

    m_debutjeu4Act = new QAction("4 Joueurs", this);
    m_debutjeu4Act->setShortcut(Qt::Key_F4);
    m_debutjeu4Act->setStatusTip("Débuter une partie à 4 Joueurs");
    connect(m_debutjeu4Act, SIGNAL(triggered()), this, SLOT(slotDebutJeu4()));

    m_arretpartieAct = new QAction("Arrêter", this);
    m_arretpartieAct->setShortcut(Qt::Key_F5);
    m_arretpartieAct->setStatusTip("Arrêter la partie");
    connect(m_arretpartieAct, SIGNAL(triggered()), this, SLOT(slotArretPartie()));

    m_quitterAct = new QAction("&Quitter", this);
    m_quitterAct->setShortcut(QKeySequence("Ctrl+Q"));
    m_quitterAct->setStatusTip("Quitter Rami");
    connect(m_quitterAct, SIGNAL(triggered()), this, SLOT(slotQuitterProg()));

    m_preferenceAct = new QAction("&Préférences...", this);
    m_preferenceAct->setShortcut(QKeySequence("Ctrl+P"));
    m_preferenceAct->setStatusTip("Choisir les préférences");
    connect(m_preferenceAct, SIGNAL(triggered()), this, SLOT(slotPreferences()));

    m_statistiqueAct = new QAction("&Statistiques...", this);
    m_statistiqueAct->setShortcut(QKeySequence("Ctrl+S"));
    m_statistiqueAct->setStatusTip("Afficher les statistiques");
    connect(m_statistiqueAct, SIGNAL(triggered()), this, SLOT(slotStatistiques()));

    m_aideAct = new QAction("Aide", this);
    m_aideAct->setShortcut(Qt::Key_F1);
    m_aideAct->setStatusTip("Affiche l'aide de Rami");
    connect(m_aideAct, SIGNAL(triggered()), this, SLOT(slotAide()));

    m_aboutAct = new QAction("À Propos de : Rami", this);
    m_aboutAct->setStatusTip("À Propos de : Rami");
    connect(m_aboutAct, SIGNAL(triggered()), this, SLOT(slotAbout()));

    m_aboutQtAct = new QAction("À Propos de Qt", this);
    m_aboutQtAct->setStatusTip("À Propos de Qt");
    connect(m_aboutQtAct, SIGNAL(triggered()), this, SLOT(slotAboutQt()));
}

void CMainWindow::createMenus()
{
    m_jeuMenu = menuBar()->addMenu("&Jeu");
    m_jeuMenu->addAction(m_debutjeu2Act);
    m_jeuMenu->addAction(m_debutjeu3Act);
    m_jeuMenu->addAction(m_debutjeu4Act);
    m_jeuMenu->addSeparator();
    m_jeuMenu->addAction(m_arretpartieAct);
    m_jeuMenu->addSeparator();
    m_jeuMenu->addAction(m_quitterAct);

    m_optionsMenu = menuBar()->addMenu("&Options");
    m_optionsMenu->addAction(m_preferenceAct);
    m_optionsMenu->addAction(m_statistiqueAct);

    m_aideMenu = menuBar()->addMenu("&Aide");
    m_aideMenu->addAction(m_aideAct);
    m_aideMenu->addSeparator();
    m_aideMenu->addAction(m_aboutAct);
    m_aideMenu->addAction(m_aboutQtAct);

    m_heightMenuBar = menuBar()->height();
}

void CMainWindow::createStatusBar()
{
    QFont font;
    font.setBold(true);
    statusBar()->setFont(font);
    statusBar()->showMessage("Prêt");
    m_heightStatusBar = statusBar()->height();
}

void CMainWindow::afficherStatusBar()
{
    statusBar()->showMessage(m_messagestatusbar);
}

void CMainWindow::afficherMsgStatusBar()
{
    m_messagestatusbar = m_pcrami->getMessageStatusBar();
    afficherStatusBar();
}

void CMainWindow::setPreferences()
{
    m_pcrami->setSiAnimeCarte(m_f_animecarte);
    m_pcrami->setSiSonOrdi(m_f_son);
    m_pcrami->setNiveauJeu(m_niveaujeu);
    m_pcrami->setVitesse(m_vitesse);
    m_pcrami->setVolume(m_volume);
}

void CMainWindow::readSettings()
{
    QSettings settings(ORGANISATION, "JNDQT6-" + NOM_PROG);
    QPoint pos = settings.value("position", m_pos).toPoint();
    m_pos = pos;
    bool animecarte = settings.value("animecarte", true).toBool();
    m_f_animecarte = animecarte;
    bool son = settings.value("son", true).toBool();
    m_f_son = son;
    int niveaujeu = settings.value("niveaujeu", NIVEAU_FORT).toInt();
    m_niveaujeu = niveaujeu;
    int vitesse = settings.value("vitesse", VITESSE_NORMALE).toInt();
    m_vitesse = vitesse;
    int volume = settings.value("volume", 12).toInt();
    m_volume = volume;
}

void CMainWindow::writeSettings()
{
    QSettings settings(ORGANISATION, "JNDQT6-" + NOM_PROG);
    settings.setValue("position", pos());
    settings.setValue("animecarte", m_f_animecarte);
    settings.setValue("son", m_f_son);
    settings.setValue("niveaujeu", m_niveaujeu);
    settings.setValue("vitesse", m_vitesse);
    settings.setValue("volume", m_volume);
}
