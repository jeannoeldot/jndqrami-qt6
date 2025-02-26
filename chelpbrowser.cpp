#include <QHBoxLayout>
#include <QVBoxLayout>

#include "chelpbrowser.h"

CHelpBrowser::CHelpBrowser(const QString &path, const QString &page, QWidget *parent) :
    QWidget(parent)
{
    //	setAttribute( Qt::WA_DeleteOnClose );
    //	setAttribute( Qt::WA_GroupLeader );

    m_textBrowser = new QTextBrowser;

    m_homeButton = new QPushButton("&Début");
    m_backButton = new QPushButton("&Retour");
    m_closeButton = new QPushButton("Fermer");
    m_closeButton->setShortcut(Qt::Key_Escape);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(m_homeButton);
    buttonLayout->addWidget(m_backButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_closeButton);

    QVBoxLayout	*mainLayout = new QVBoxLayout;
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(m_textBrowser);
    setLayout(mainLayout);

    connect(m_homeButton, SIGNAL(clicked()), m_textBrowser, SLOT(home()));
    connect(m_backButton, SIGNAL(clicked()), m_textBrowser, SLOT(backward()));
    connect(m_closeButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(m_textBrowser, SIGNAL(sourceChanged(const QUrl &)), this, SLOT(updateWindowTitle()));

    m_textBrowser->setSearchPaths(QStringList() << path << ":/images");
    m_textBrowser->setSource(page);

    resize(500, 600);
}

void CHelpBrowser::updateWindowTitle()
{
    setWindowTitle(QString("Aide : %1").arg(m_textBrowser->documentTitle()));
}
