/*******************************************************
 * Copyright (C) 2020 AMELLAL Oussama <ouss.amellal@gmail.com>
 * 
 * This file is part of 'PrendreUnePhoto' project.
 * 
 * 'PrendreUnePhoto' project can not be copied and/or distributed without the express
 * permission of AMELLAL Oussama
 *******************************************************/
#include "outils.h"
#include<QDateTime>
#include<QStandardPaths>
#include<QDir>




QString Outils::CheminEnregistrPhotos()
{
    //Chemin des vidéos sauvegardé : Bureau
    QString CheminBureau = QStandardPaths::standardLocations(QStandardPaths::DesktopLocation)[0];
    QDir BureauDir(CheminBureau);
    BureauDir.mkpath("PrendreUnePhoto");
    return BureauDir.absoluteFilePath("PrendreUnePhoto");
}

QString Outils::CheminPhoto(QString nom, QString type)
{
    return QString("%1/%2.%3").arg(Outils::CheminEnregistrPhotos(), nom, type);
}

QString Outils::NommerImageSauveg()
{
    QDateTime time = QDateTime::currentDateTime();
    return time.toString("HH-mm-ss-zzz_dd-MM-yyyy");
}
