/*
 * Supprime l'arrière plan et se focalise sur les objets mobiles
 * l'image de référence est prise en appuyant sur 'espace'
 */
 
 
#define entree "Image de référence"
#define sortie "Sortie"
 
#include <opencv/highgui.h>
#include <opencv/cv.h>
#include <iostream>


using namespace std;

IplImage* refImageCapture(IplImage* sourceImage)
{
	/*Fonction permettant de capturer l'image de référence
	 * sans copier certains attributs de l'image provenant du flux video. */
	 
	IplImage* refImage ;
	CvScalar scalaire ; 

	refImage = cvCreateImage(cvGetSize(sourceImage),IPL_DEPTH_8U,3);
	
		for(int x=0; x<sourceImage->width; x++)
		{
			for(int y=0; y<sourceImage->height; y++)
			{
				scalaire=cvGet2D(sourceImage, y, x); //On lit le pixel x,y de l'image recupérée
				cvSet2D(refImage,y,x,scalaire); // et on le met sur l'image de référence
			}
		}
		
	return refImage;
}

 int main() {
	 
	int i=0;
	char key;
	
	// Images
	 IplImage *refImage;
	 IplImage *outImage;
	 
	// Capture vidéo
	CvCapture *capture;
	capture = cvCreateCameraCapture(0);
 
	// Vérifier si l'ouverture du flux est ok
	if (!capture) {
	   cout << "Ouverture du flux vidéo impossible !\n" << endl;
	   return 1;
	}
 
	// Définition des fenêtres
	cvNamedWindow(entree, CV_WINDOW_AUTOSIZE);
	cvNamedWindow(sortie, CV_WINDOW_AUTOSIZE);
		
	//On attend que le flux video soit propre
	while(i<10) 
	{
		outImage = cvQueryFrame(capture);
		cvWaitKey(10); 
		i++;
	}
	cout << "Image de référence crée\n" << endl;

	//Création de l'image de référence
	refImage = refImageCapture(outImage);

	// Boucle tant que l'utilisateur n'appuie pas sur la touche q (ou Q)
	while(key != 'q' && key != 'Q') {
		
	   // Aquisition image de sortie
		outImage = cvQueryFrame(capture);
		if(key == ' ') refImage = refImageCapture(outImage);

		//Traitement
		cvAbsDiff(refImage, outImage, outImage);
		cvThreshold(outImage, outImage, 100, 255, CV_THRESH_BINARY);
		
	   // On affiche les images
	   cvShowImage( sortie, outImage);
	   cvShowImage( entree, refImage);
	   
	   // On attend 50ms
	   key = cvWaitKey(50);
	}
 
	cvReleaseCapture(&capture);
	cvDestroyWindow(entree);
	cvDestroyWindow(sortie);
 
	return 0;
 
}
