#ifndef CHELPBROWSER_H
#define CHELPBROWSER_H

#include <QWidget>
#include <QTextBrowser>
#include <QPushButton>

class CHelpBrowser : public QWidget
{
    Q_OBJECT
public:
    explicit CHelpBrowser(const QString &path, const QString &page, QWidget *parent = 0);

signals:

public slots:

private slots:
    void updateWindowTitle();

private:
    QPushButton *m_backButton;
    QPushButton *m_homeButton;
    QTextBrowser *m_textBrowser;
    QPushButton *m_closeButton;
};

#endif // CHELPBROWSER_H
