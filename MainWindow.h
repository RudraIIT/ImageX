#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QFrame>
#include <opencv2/opencv.hpp>
#include "ImageArea.h"  
#include <stack>

enum FilterType
{
    Normal,
    Grayscale,
    Sepia,
    Invert,
    Cool,
    Warm
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
    public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    private slots:
    void openImage();
    // void applyBlur(int value);
    // void applyBrightness(int value);
    // void applyContrast(int value);
    void applyFilters();
    void saveImage();
    void undo();
    void redo();
    void pushToUndoStack();
    void onSliderRelease();

    private:
    void setupUI();
    ImageArea *imageArea;
    QAction *openAction;
    cv::Mat originalImage;
    int blurValue = 0;
    int brightnessValue = 0;
    int contrastValue = 100;
    FilterType currentFilter = Normal;
    std::stack<QImage> undoStack;
    std::stack<QImage> redoStack;
};


#endif