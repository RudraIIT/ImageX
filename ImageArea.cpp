#include "ImageArea.h"

ImageArea::ImageArea(QWidget *parent)
    : QLabel(parent)
{
    setAlignment(Qt::AlignCenter);
    setStyleSheet("background-color: #222; color: white; font-size: 24px;");
}

void ImageArea::setImage(const QImage &image)
{
    currentImage = image;
    updatePixmap();
}

void ImageArea::resizeEvent(QResizeEvent *event)
{
    QLabel::resizeEvent(event);
    updatePixmap();
}

void ImageArea::updatePixmap()
{
    if(!currentImage.isNull())
    {
        setPixmap(QPixmap::fromImage(currentImage.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation)));
    }
}

