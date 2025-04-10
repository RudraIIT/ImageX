#include "MainWindow.h"
#include <QMenuBar>
#include <QMenu>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSlider>
#include "ImageArea.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Image X");
    resize(1280, 720);

    setupUI();
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI()
{
    QMenuBar *menuBar = new QMenuBar(this);
    setMenuBar(menuBar);

    QMenu *fileMenu = menuBar->addMenu("File");
    QAction *openAction = fileMenu->addAction("Open");
    connect(openAction, &QAction::triggered, this, &MainWindow::openImage);

    fileMenu->addAction("Save");
    fileMenu->addAction("Exit");

    QMenu *editMenu = menuBar->addMenu("Edit");
    editMenu->addAction("Undo");
    editMenu->addAction("Redo");

    QMenu *helpMenu = menuBar->addMenu("Help");
    helpMenu->addAction("About");
    helpMenu->addAction("Help");

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);

    QFrame *sidebar = new QFrame(this);
    sidebar->setFrameShape(QFrame::Box);
    sidebar->setMinimumWidth(150);
    sidebar->setMaximumWidth(250);

    QVBoxLayout *sidebarLayout = new QVBoxLayout(sidebar);

    QLabel *blurLabel = new QLabel("Blur", this);
    QSlider *blurSlider = new QSlider(Qt::Horizontal, this);
    blurSlider->setRange(0, 100);
    blurSlider->setValue(0);

    sidebarLayout->addWidget(blurLabel);
    sidebarLayout->addWidget(blurSlider);

    QLabel *brightnessLabel = new QLabel("Brightness", this);
    QSlider *brightnessSlider = new QSlider(Qt::Horizontal, this);
    brightnessSlider->setRange(-100, 100);
    brightnessSlider->setValue(0);

    sidebarLayout->addWidget(brightnessLabel);
    sidebarLayout->addWidget(brightnessSlider);

    QLabel *contrastLabel = new QLabel("Contrast", this);
    QSlider *contrastSlider = new QSlider(Qt::Horizontal, this);
    contrastSlider->setRange(0, 300);
    contrastSlider->setValue(100); 

    sidebarLayout->addWidget(contrastLabel);
    sidebarLayout->addWidget(contrastSlider);

    sidebarLayout->addStretch();

    imageArea = new ImageArea(this);  
    imageArea->setMinimumSize(600, 400);

    mainLayout->addWidget(sidebar);
    mainLayout->addWidget(imageArea, 1);   

    connect(blurSlider, &QSlider::valueChanged, this, [=](int value) {
        blurValue = value;
        applyFilters();
    });

    connect(brightnessSlider, &QSlider::valueChanged, this, [=](int value) {
        brightnessValue = value;
        applyFilters();
    });

    connect(contrastSlider, &QSlider::valueChanged, this, [=](int value) {
        contrastValue = value;
        applyFilters();
    });
}


void MainWindow::openImage()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open Image", "", "Images (*.png *.xpm *.jpg *.bmp)");

    if(!fileName.isEmpty()) {
        cv::Mat img = cv::imread(fileName.toStdString());
        if(img.empty()) return;

        originalImage = img.clone();

        cv::cvtColor(img, img, cv::COLOR_BGR2RGB);
        QImage qimg(img.data, img.cols, img.rows, img.step, QImage::Format_RGB888);

        imageArea->setImage(qimg.copy());
    }
}

// void MainWindow::applyBlur(int value)
// {
//     if(originalImage.empty()) return;

//     cv::Mat blurredImage;
//     int ksize = value % 2 == 0 ? value + 1 : value; 

//     cv::blur(originalImage, blurredImage, cv::Size(ksize, ksize));

//     cv::cvtColor(blurredImage, blurredImage, cv::COLOR_BGR2RGB);
//     QImage qimg(blurredImage.data, blurredImage.cols, blurredImage.rows, blurredImage.step, QImage::Format_RGB888);

//     imageArea->setImage(qimg.copy());
// }

// void MainWindow::applyBrightness(int value)
// {
//     if(originalImage.empty()) return;

//     cv::Mat brightenedImage;
//     originalImage.convertTo(brightenedImage, -1, 1, value);

//     cv::cvtColor(brightenedImage, brightenedImage, cv::COLOR_BGR2RGB);
//     QImage qimg(brightenedImage.data, brightenedImage.cols, brightenedImage.rows, brightenedImage.step, QImage::Format_RGB888);

//     imageArea->setImage(qimg.copy());

// }

// void MainWindow::applyContrast(int value)
// {
//     if(originalImage.empty()) return;

//     double alpha = value / 100.0;
//     int beta = 0;

//     cv::Mat contrastedImage;
//     originalImage.convertTo(contrastedImage, -1, alpha, beta);

//     cv::cvtColor(contrastedImage, contrastedImage, cv::COLOR_BGR2RGB);
//     QImage qimg(contrastedImage.data, contrastedImage.cols, contrastedImage.rows, contrastedImage.step, QImage::Format_RGB888);

//     imageArea->setImage(qimg.copy());
// }

void MainWindow::applyFilters()
{
    if(originalImage.empty()) return;

    cv::Mat filteredImage = originalImage.clone();

    if(blurValue > 0)
    {
        int ksize = blurValue * 2 + 1;
        cv::GaussianBlur(filteredImage, filteredImage, cv::Size(ksize, ksize), 0);
    }

    double alpha = contrastValue / 100.0;
    int beta = brightnessValue;

    filteredImage.convertTo(filteredImage, -1, alpha, beta);

    cv::cvtColor(filteredImage, filteredImage, cv::COLOR_BGR2RGB);
    QImage qimg(filteredImage.data, filteredImage.cols, filteredImage.rows, filteredImage.step, QImage::Format_RGB888);

    imageArea->setImage(qimg.copy());
}