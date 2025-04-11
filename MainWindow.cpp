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

    QAction *saveAction = fileMenu->addAction("Save");
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveImage);

    fileMenu->addAction("Exit");

    QMenu *editMenu = menuBar->addMenu("Edit");
    QAction *undoAction = editMenu->addAction("Undo");
    connect(undoAction, &QAction::triggered, this, &MainWindow::undo);
    QAction *redoAction = editMenu->addAction("Redo");
    connect(redoAction, &QAction::triggered, this, &MainWindow::redo);

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
    QPushButton *normalBtn = new QPushButton("Normal");
    QPushButton *grayscaleBtn = new QPushButton("Grayscale");
    QPushButton *sepiaBtn = new QPushButton("Sepia");
    QPushButton *invertBtn = new QPushButton("Invert");
    QPushButton *coolBtn = new QPushButton("Cool");
    QPushButton *warmBtn = new QPushButton("Warm");
    contrastSlider->setRange(0, 300);
    contrastSlider->setValue(100); 

    sidebarLayout->addWidget(contrastLabel);
    sidebarLayout->addWidget(contrastSlider);

    sidebarLayout->addWidget(normalBtn);
    sidebarLayout->addWidget(grayscaleBtn);
    sidebarLayout->addWidget(sepiaBtn);
    sidebarLayout->addWidget(invertBtn);
    sidebarLayout->addWidget(coolBtn);
    sidebarLayout->addWidget(warmBtn);

    sidebarLayout->addStretch();

    imageArea = new ImageArea(this);  
    imageArea->setMinimumSize(600, 400);

    mainLayout->addWidget(sidebar);
    mainLayout->addWidget(imageArea, 1);   

    connect(blurSlider, &QSlider::valueChanged, this, [=](int value) {
        blurValue = value;
        applyFilters();
    });

    connect(blurSlider, &QSlider::sliderReleased, this, &MainWindow::onSliderRelease);

    connect(brightnessSlider, &QSlider::valueChanged, this, [=](int value) {
        brightnessValue = value;
        applyFilters();
    });

    connect(brightnessSlider, &QSlider::sliderReleased, this, &MainWindow::onSliderRelease);

    connect(contrastSlider, &QSlider::valueChanged, this, [=](int value) {
        contrastValue = value;
        applyFilters();
    });

    connect(contrastSlider, &QSlider::sliderReleased, this, &MainWindow::onSliderRelease);

    connect(normalBtn, &QPushButton::clicked, this, [=]() {
        currentFilter = Normal;
        applyFilters();
    });

    connect(grayscaleBtn, &QPushButton::clicked, this, [=]() {
        currentFilter = Grayscale;
        applyFilters();
    });

    connect(sepiaBtn, &QPushButton::clicked, this, [=]() {
        currentFilter = Sepia;
        applyFilters();
    });

    connect(invertBtn, &QPushButton::clicked, this, [=]() {
        currentFilter = Invert;
        applyFilters();
    });

    connect(coolBtn, &QPushButton::clicked, this, [=]() {
        currentFilter = Cool;
        applyFilters();
    });

    connect(warmBtn, &QPushButton::clicked, this, [=]() {
        currentFilter = Warm;
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

    pushToUndoStack();
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

    switch(currentFilter) {
        case Grayscale: {
            cv::cvtColor(filteredImage, filteredImage, cv::COLOR_BGR2GRAY);
            cv::cvtColor(filteredImage, filteredImage, cv::COLOR_GRAY2BGR);
            break;
        }
        
        case Sepia: {
            cv::Mat kernel = (cv::Mat_<float>(3,3) <<
                0.272, 0.534, 0.131,
                0.349, 0.686, 0.168,
                0.393, 0.769, 0.189);
            cv::transform(filteredImage, filteredImage, kernel);
            break;
        }
        
        case Invert: {
            cv::bitwise_not(filteredImage, filteredImage);
            break;
        }
        
        case Cool: {
            filteredImage += cv::Scalar(0,0,20);
            break;
        }
        
        case Warm: {
            filteredImage += cv::Scalar(20,20,0);
            break;
        }
        
        default:
            break;
    }

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

void MainWindow::saveImage()
{
    if(imageArea->getImage().isNull()) return;

    QString fileName = QFileDialog::getSaveFileName(this, "Save Image", "", "Images (*.png *.xpm *.jpg *.bmp)");

    if(!fileName.isEmpty()) {
        imageArea->getImage().save(fileName);
    }
}

void MainWindow::pushToUndoStack() {
    QImage currentImage = imageArea->getImage();

    if(!currentImage.isNull()) {
        undoStack.push(currentImage);
        std::cout<< "Pushed to undo stack" << std::endl;
        while(!redoStack.empty()) {
            redoStack.pop();
        }
    }
}

void MainWindow::undo() {
    if(undoStack.empty()) {
        std::cout << "No more actions to undo." << std::endl;
        return;
    }

    redoStack.push(imageArea->getImage());
    QImage currentImage = undoStack.top();
    undoStack.pop();
    
    imageArea->setImage(currentImage);
}

void MainWindow::redo() {
    if(redoStack.empty()) return;

    undoStack.push(imageArea->getImage());
    QImage currentImage = redoStack.top();
    redoStack.pop();
    
    imageArea->setImage(currentImage);
}

void MainWindow::onSliderRelease()
{
    pushToUndoStack();
}