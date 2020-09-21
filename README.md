# PrendreUnePhoto
Ce programme est une démonstration de l'ouverture d'une vidéo ou d'une caméra dans une Thread différente du Thread principale.
La création du Thread qui traite le flux vidéo se fait en suivant les étapes suivantes : 

-Créer un QObject.  
-Connectez vos signaux.  
-Créez votre QThread.     
-Déplacez (moveToThread) le QObject vers le QThread et démarrez le thread.

 

This program is a demonstration of opening a video or a camera in a Thread other than the main Thread.
The creation of the Thread that processes the video stream is done by following the following steps:

-Create a  QObject.   
-Connect your signals.    
-Create your QThread.   
-Move (moveToThread)the  QObject to the QThread and start the thread.
