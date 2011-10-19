/*
 * Code written by Lya (GeckoGeek.fr)
 */
 
 
#define fenetre "7Robot"
#define sortie "Apres traitement"
 
#include <opencv/highgui.h>
#include <opencv/cv.h>
 
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

using namespace std;



 int main() {
	 
	int i=0,x,y;
	// Touche clavier
	char key;
	// Image
	 IplImage *image;
	// Image de sortie 
	 IplImage *outImage;
	// Capture vidéo
	CvCapture *capture;
	//Pixel
	CvPoint pixel;
 
	// Ouvrir le flux vidéo
	//capture = cvCreateFileCapture("/path/to/your/video/test.avi"); // chemin pour un fichier
	capture = cvCreateCameraCapture(CV_CAP_ANY);
 
	// Vérifier si l'ouverture du flux est ok
	if (!capture) {
	   printf("Ouverture du flux vidéo impossible !\n");
	   return 1;
	}
 
	// Définition de la fenêtre
	cvNamedWindow(fenetre, CV_WINDOW_AUTOSIZE);
	cvNamedWindow(sortie, CV_WINDOW_AUTOSIZE);
 
	// Boucle tant que l'utilisateur n'appuie pas sur la touche q (ou Q)
	while(key != 'q' && key != 'Q') {
 
	   // On récupère une image
	   image = cvQueryFrame(capture);
		
	   // Taille image de sortie
	   outImage = cvCreateImage(cvGetSize(image),IPL_DEPTH_8U,3);
	   x = image->width;
	   y = image->height;
	   
	   //Smoothing
	   cvSmooth(image,outImage,CV_GAUSSIAN,13,13);
	   
	   //Points aléatoires
		
		for(i=0;i<100;i++)
		{
		   pixel.x = (int) floor(rand()%x);
		   pixel.y = (int) floor(rand()%y);
		   cvDrawCircle(outImage, pixel, 1, CV_RGB(255, 0, 0), -1);
		}
	   
	   
	   // On affiche l'image dans une fenêtre
	   cvShowImage( fenetre, image);
	   cvShowImage( sortie, outImage);
	   
	   // On attend 10ms
	   key = cvWaitKey(10);
 
	}
 
	cvReleaseCapture(&capture);
	cvDestroyWindow(fenetre);
 
	return 0;
 
}
