/*******************************************************
 * Copyright (C) 2020 AMELLAL Oussama <ouss.amellal@gmail.com>
 * 
 * This file is part of 'PrendreUnePhoto' project.
 * 
 * 'PrendreUnePhoto' project can not be copied and/or distributed without the express
 * permission of AMELLAL Oussama
 *******************************************************/
 #ifndef OBJECTCAMVIDEO_H
#define OBJECTCAMVIDEO_H

#include <QObject>
#include<QThread>
#include<QMutex>

#include "opencv2/opencv.hpp"

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>


class ObjectCamVideo : public QObject
{
    Q_OBJECT
    int frame_width, frame_height;
    bool mEnregisterement;
    QMutex *FramelockObj;
    cv::Mat mFrame;
    int mIndexcamera;
    QString mCheminVideo;

    bool taking_photo;

private :
    void EnregisterPhoto(cv::Mat &frame);

public:
    ObjectCamVideo(int camera, QMutex *lock);// Pour ouvrir une caméra
    ObjectCamVideo(QString videoPath, QMutex *lock);// Pour ouvrir une vidéo
    void ConnexionThread(QThread * QThreadcamvid); //Connexion entre un signal(QThread) et le slot 'Traitement'
    void takePhoto() {taking_photo = true; }

signals:
    void EnvoyerFrame(cv::Mat *data);
    void photoTaken(QString name);
public slots:
    void Traitement();
};

#endif // OBJECTCAMVIDEO_H
