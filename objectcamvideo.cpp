/*******************************************************
 * Copyright (C) 2020 AMELLAL Oussama <ouss.amellal@gmail.com>
 * 
 * This file is part of 'PrendreUnePhoto' project.
 * 
 * 'PrendreUnePhoto' project can not be copied and/or distributed without the express
 * permission of AMELLAL Oussama
 *******************************************************/
#include "objectcamvideo.h"
#include "outils.h"

#include<QThread>



ObjectCamVideo::ObjectCamVideo(int camera, QMutex *lock):
    mEnregisterement(false),FramelockObj(lock), mIndexcamera(camera), mCheminVideo("")
{
    frame_width = frame_height = 0;
}
ObjectCamVideo::ObjectCamVideo(QString videoPath, QMutex *lock):
    mEnregisterement(false), FramelockObj(lock), mIndexcamera(-1), mCheminVideo(videoPath)
{
    frame_width = frame_height = 0;

}
void ObjectCamVideo::EnregisterPhoto(cv::Mat &frame)
{
    QString photo_name = Outils::NommerImageSauveg();
    QString photo_path = Outils::CheminPhoto(photo_name, "jpg");
    cv::imwrite(photo_path.toStdString(), frame);
    emit photoTaken(photo_name);
    taking_photo = false;
}
void ObjectCamVideo::ConnexionThread(QThread *QThreadcamvid)
{
    connect(QThreadcamvid, SIGNAL(started()), this, SLOT(Traitement()));
}


void ObjectCamVideo::Traitement()
{
    mEnregisterement = true;
    cv::VideoCapture cap ;
    if(mCheminVideo != "")
    {
         cap.open(mCheminVideo.toStdString());
    }
    else
    {
        cap.open(mIndexcamera);
    }

    cv::Mat frameTemp;
    frame_width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    frame_height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    while(mEnregisterement) {
        cap >> frameTemp;
        if (frameTemp.empty()) {
            break;
        }
        if(taking_photo) {
            EnregisterPhoto(frameTemp);
        }

        cvtColor(frameTemp, frameTemp, cv::COLOR_BGR2RGB);
        FramelockObj->lock();
        mFrame = frameTemp;
        FramelockObj->unlock();
        emit EnvoyerFrame(&mFrame);
        cv::waitKey(50);
    }

    cap.release();
    mEnregisterement = false;
    mCheminVideo = "";
}
