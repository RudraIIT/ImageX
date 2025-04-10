#ifndef IMAGEAREA_H
#define IMAGEAREA_H

#include <QLabel>
#include <QImage>

class ImageArea : public QLabel
{
    Q_OBJECT

public:
    explicit ImageArea(QWidget *parent = nullptr);
    void setImage(const QImage &image);
    void resizeEvent(QResizeEvent *event);
    void updatePixmap();

private:
    QImage currentImage;
};

#endif // IMAGEAREA_H
