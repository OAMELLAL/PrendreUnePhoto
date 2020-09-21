/*******************************************************
 * Copyright (C) 2020 AMELLAL Oussama <ouss.amellal@gmail.com>
 * 
 * This file is part of 'PrendreUnePhoto' project.
 * 
 * 'PrendreUnePhoto' project can not be copied and/or distributed without the express
 * permission of AMELLAL Oussama
 *******************************************************/
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "objectcamvideo.h"
#include "outils.h"

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "opencv2/opencv.hpp"

#include<QGridLayout>
#include<QCameraInfo>
#include<QMessageBox>
#include<QThread>
#include <QDateTime>
#include<QStandardPaths>
#include<QDir>
#include<QFileDialog>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    ,mObjectCamVideo(nullptr)
    , mQMenuFichier(nullptr)
{
    ui->setupUi(this);
    // this->setStyleSheet("background-color: blue;");// Changer la couleur de la fenêtre
    initialisation();
    FrameLock = new QMutex();
    mQActionOuvrirCamera->setEnabled(true);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::initialisation()
{
    this->resize(1500, 900);
    mQMenuFichier = menuBar()->addMenu("&Fichier");
    QGridLayout * QGridLayoutPrincipale = new QGridLayout;
    mQGraphicsSceneImage = new QGraphicsScene(this);
    mQGraphicsViewImage = new QGraphicsView(mQGraphicsSceneImage);
    QGridLayoutPrincipale->addWidget(mQGraphicsViewImage,0,0,12, 15);

    QGridLayout *QGridLayoutOutils = new QGridLayout();
    QGridLayoutPrincipale->addLayout(QGridLayoutOutils, 12, 0, 1, 15);
    mQPushButtonPrendreUnePhoto = new QPushButton(this);
    mQPushButtonPrendreUnePhoto->setText("Prendre une photo");
    QGridLayoutOutils->addWidget(mQPushButtonPrendreUnePhoto, 0, 0, Qt::AlignCenter);

    QWidget *widget = new QWidget();
    widget->setLayout(QGridLayoutPrincipale);
    setCentralWidget(widget);

    mQStatusBarPrinc = statusBar();
    mQLabelStatusBarPrinc = new QLabel(mQStatusBarPrinc);
    mQStatusBarPrinc->addPermanentWidget(mQLabelStatusBarPrinc);
    mQLabelStatusBarPrinc->setText("Pas d'enregitrement en cours.");

    ActionsInitialisation();

    mQListViewVidSauvg = new QListView(this);
    mQListViewVidSauvg->setViewMode(QListView::ListMode);
    mQListViewVidSauvg->setResizeMode(QListView::Adjust);
    mQListViewVidSauvg->setSpacing(3);
    mQListViewVidSauvg->setWrapping(false);
    list_model = new QStandardItemModel(this);
    mQListViewVidSauvg->setModel(list_model);
    QGridLayoutPrincipale->addWidget(mQListViewVidSauvg, 0, 15, 12, 6);
}
void MainWindow::ActionsInitialisation()
{

    mQActionOuvrirCamera = new QAction("&Ouvrir la caméra", this);
    mQMenuFichier->addAction(mQActionOuvrirCamera);

    mQActionOuvrirVideo = new QAction("&Ouvrir une vidéo", this);
    mQMenuFichier->addAction(mQActionOuvrirVideo);


    mQActionCaracCamera = new QAction("&Caractéristiques de la caméra", this);
    mQMenuFichier->addAction(mQActionCaracCamera);

    mQActionQuitter = new QAction("&Quitter", this);
    mQMenuFichier->addAction(mQActionQuitter);


    connect(mQActionOuvrirCamera, SIGNAL(triggered()), this, SLOT(OuvrirCamera()));
    connect(mQActionOuvrirVideo, SIGNAL(triggered()), this, SLOT(OuvrirVideo()));
    connect(mQActionCaracCamera, SIGNAL(triggered()), this, SLOT(AfficherCaracCamera()));
    connect(mQActionQuitter, SIGNAL(triggered()), QApplication::instance(), SLOT(quit()));
    connect(mQPushButtonPrendreUnePhoto, SIGNAL(clicked()), this, SLOT(PrendrePhoto()));

}

void MainWindow::OuvrirCamera()
{
    if(mObjectCamVideo != nullptr) {

        mQGraphicsSceneImage->clear();
        mQGraphicsViewImage->resetTransform();


        disconnect(mObjectCamVideo, &ObjectCamVideo::EnvoyerFrame, this, &MainWindow::RecevoirFrame);
        disconnect(mObjectCamVideo, &ObjectCamVideo::photoTaken, this, &MainWindow::AjoutPhotoSauvegarde);
        connect(ObjectCamVideoThread, &QThread::finished, this, &QObject::deleteLater);
    }

    ObjectCamVideoThread = new QThread();
    mObjectCamVideo = new ObjectCamVideo(0, FrameLock );
    mObjectCamVideo->ConnexionThread(ObjectCamVideoThread);
    mObjectCamVideo->moveToThread(ObjectCamVideoThread);
    connect(mObjectCamVideo, &ObjectCamVideo::EnvoyerFrame, this, &MainWindow::RecevoirFrame);
    ObjectCamVideoThread->start();
    connect(mObjectCamVideo, &ObjectCamVideo::photoTaken, this, &MainWindow::AjoutPhotoSauvegarde);


}

void MainWindow::OuvrirVideo()
{

    if(mObjectCamVideo != nullptr) {

        mQGraphicsSceneImage->clear();
        mQGraphicsViewImage->resetTransform();

        disconnect(mObjectCamVideo, &ObjectCamVideo::EnvoyerFrame, this, &MainWindow::RecevoirFrame);
        disconnect(mObjectCamVideo, &ObjectCamVideo::photoTaken, this, &MainWindow::AjoutPhotoSauvegarde);
        connect(ObjectCamVideoThread, &QThread::finished, this, &QObject::deleteLater);
    }


    QFileDialog dialog(this);
    dialog.setWindowTitle("Ouvrir une image");
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter(tr("Videos filtres (*.mp4 *.avi)"));
    QStringList CheminsFichiers;

    if (dialog.exec())
    {
        CheminsFichiers = dialog.selectedFiles();
        ObjectCamVideoThread = new QThread();
        mObjectCamVideo = new ObjectCamVideo(CheminsFichiers.at(0), FrameLock );

        mObjectCamVideo->ConnexionThread(ObjectCamVideoThread);
        mObjectCamVideo->moveToThread(ObjectCamVideoThread);
        connect(mObjectCamVideo, &ObjectCamVideo::EnvoyerFrame, this, &MainWindow::RecevoirFrame);
        ObjectCamVideoThread->start();
        connect(mObjectCamVideo, &ObjectCamVideo::photoTaken, this, &MainWindow::AjoutPhotoSauvegarde);
        qDebug() << CheminsFichiers.at(0);
    }
}
void MainWindow::RecevoirFrame(cv::Mat * mat)
{
    FrameLock->lock();
    FrameEnCours = *mat;
    FrameLock->unlock();
    QImage frame(
                FrameEnCours.data,
                FrameEnCours.cols,
                FrameEnCours.rows,
                FrameEnCours.step,
                QImage::Format_RGB888);
    QPixmap image = QPixmap::fromImage(frame);
    mQGraphicsSceneImage->clear();
    mQGraphicsViewImage->resetTransform();
    mQGraphicsSceneImage->addPixmap(image);
    mQGraphicsSceneImage->update();
    mQGraphicsViewImage->setSceneRect(image.rect());
}

void MainWindow::AjoutPhotoSauvegarde(QString name)
{
    QString photo_path = Outils::CheminPhoto(name, "jpg");
    QStandardItem *item = new QStandardItem();
    list_model->appendRow(item);
    QModelIndex index = list_model->indexFromItem(item);
    list_model->setData(index, QPixmap(photo_path).scaledToHeight(145), Qt::DecorationRole);
    list_model->setData(index, name, Qt::DisplayRole);
    mQListViewVidSauvg->scrollTo(index);

}

void MainWindow::PrendrePhoto()
{
    if(mObjectCamVideo != nullptr) {
        mObjectCamVideo->takePhoto();
    }
}

void MainWindow::AfficherCaracCamera()
{
    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    QString infoCams = QString("Caméras disponible : \n");
    for (const QCameraInfo &cameraInfo : cameras){
        infoCams +=  "Nom : " + cameraInfo.deviceName() + "\n";
        infoCams += "Description : " + cameraInfo.description() + "\n";
    }
    QMessageBox::information(this, "Caméras disponible", infoCams);

}
