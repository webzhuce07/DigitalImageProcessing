#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utility.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_openImageButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), ".", tr("Image Files(*.png *jpg *bmp)"));
    image = cv::imread(fileName.toStdString());
    QImage img = cvMat2QImage(image);
    ui->displayLabel->setPixmap(QPixmap::fromImage(img));

}

void MainWindow::on_processButton3_clicked()
{
    if(!image.data)
        return;
    cv::Mat result = image.clone();
    saltByIterator(result, 150);
    QImage img = cvMat2QImage(result);
    ui->displayLabel->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::on_processButton2_clicked()
{
    if(!image.data)
        return;
    cv::Mat result = image.clone();
    saltByPointer(result, 150);
    QImage img = cvMat2QImage(result);
    ui->displayLabel->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::on_processButton1_clicked()
{
    if(!image.data)
        return;
    cv::Mat result = image.clone();
    saltDirectly(result, 150);
    QImage img = cvMat2QImage(result);
    ui->displayLabel->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::saltDirectly(cv::Mat &image, int n)
{
   for(int k = 0; k < n; k++){
       //rand()Is a random number generating function
       int i = rand() % image.cols;
       int j = rand() % image.rows;
       if(image.channels() == 1){  //gray image
           image.at<uchar>(j,i) = 255;
       }else if(image.channels() == 3) {//color image
           image.at<cv::Vec3b>(j,i)[0] = 255;
           image.at<cv::Vec3b>(j,i)[1] = 255;
           image.at<cv::Vec3b>(j,i)[2] = 255;
       }
   }
}

void MainWindow::saltByPointer(cv::Mat &image, int n)
{
    for(int k = 0; k < n; k++){
        //rand()Is a random number generating function
        int i = rand() % image.cols;
        int j = rand() % image.rows;
        uchar* data = image.ptr<uchar>(j);
        for(int c = 0; c < image.channels(); c++ )
            data[i*image.channels()+ c] = 255;
     }
}

void MainWindow::saltByIterator(cv::Mat &image, int n)
{
    for(int k = 0; k < n; k++){
        //rand()Is a random number generating function
        int i = rand() % image.cols;
        int j = rand() % image.rows;
        cv::Mat_<cv::Vec3b>::iterator it = image.begin<cv::Vec3b>();
        it += (j * image.cols + i);
        for(int c= 0; c < image.channels(); c++)
            (*it)[c] = 255;
     }
}
