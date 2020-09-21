/*******************************************************
 * Copyright (C) 2020 AMELLAL Oussama <ouss.amellal@gmail.com>
 * 
 * This file is part of 'PrendreUnePhoto' project.
 * 
 * 'PrendreUnePhoto' project can not be copied and/or distributed without the express
 * permission of AMELLAL Oussama
 *******************************************************/
#ifndef OUTILS_H
#define OUTILS_H

#include<QString>

class Outils
{
public:

    static QString NommerImageSauveg();
    static QString CheminEnregistrPhotos();
    static QString CheminPhoto(QString name, QString postfix);
};

#endif // OUTILS_H
