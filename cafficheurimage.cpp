#include <QPainter>

#include "cafficheurimage.h"

CAfficheurImage::CAfficheurImage(QWidget *parent) :
    QWidget(parent)
{
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

CAfficheurImage::CAfficheurImage( QWidget *parent, int w, int h )
 : QWidget(parent)
{
    m_w = w;
    m_h = h;
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

void CAfficheurImage::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawImage( QPoint(0, 0), m_image );
}

QSize CAfficheurImage::sizeHint() const
{
    return QSize(m_w, m_h);
}

void CAfficheurImage::afficherImage(const QImage &image)
{
    m_image = image;
    update();
}

void CAfficheurImage::setImage( const QImage &image )
{
    m_image = image;
}
