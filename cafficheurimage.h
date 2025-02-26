#ifndef CAFFICHEURIMAGE_H
#define CAFFICHEURIMAGE_H

#include <QWidget>
#include <QPaintEvent>

class CAfficheurImage : public QWidget
{
    Q_OBJECT
public:
    explicit CAfficheurImage(QWidget *parent = 0);
    explicit CAfficheurImage(QWidget *parent = 0, int w = 0, int h = 0 );

    QSize sizeHint() const;
    void afficherImage(const QImage &image);
    void setImage( const QImage &image );

protected:
    void paintEvent(QPaintEvent *);

private:
    QImage	m_image;
    int 	m_w;
    int	m_h;

signals:

public slots:

};

#endif // CAFFICHEURIMAGE_H
