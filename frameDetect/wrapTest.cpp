#include "opencv2/highgui/highgui.hpp"
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <unistd.h>
 
#include <cmath>

 
#define PI 3.14159265
 
using namespace cv;
using namespace std;
 
int main(int argc, char** argv) {
    int alpha_=90., beta_=90., gamma_=90.;
    int f_ = 500, dist_ = 500;
    bool video = false;
 
    Mat source, destination;
    VideoCapture capture(0);
 
    if (argc >= 2) {
        // Si au moins un argument est précisé, on essaye de charger l'image
        cout << "Lecture de " << argv[1] << endl;
        source = imread(argv[1]);
    } else {
        // Sinon on essaye d'ouvrir une capture de vidéo (webcam)
        cout << "Ouverture du flux vidéo..." << endl;
        if(!capture.isOpened()) {
            cout << "... échec :(" << endl;
            return -1;
        }
        cout << "... ça marche :)" << endl;
        video = true;
    }
 
    string wndname1 = "Source";
    string wndname2 = "Warp Perspective";
    string tbarname1 = "Alpha";
    string tbarname2 = "Beta";
    string tbarname3 = "Gamma";
    string tbarname4 = "f";
    string tbarname5 = "Distance";
    namedWindow(wndname1, 1);
    namedWindow(wndname2, 1);
    createTrackbar(tbarname1, wndname2, &alpha_, 180);
    createTrackbar(tbarname2, wndname2, &beta_, 180);
    createTrackbar(tbarname3, wndname2, &gamma_, 180);
    createTrackbar(tbarname4, wndname2, &f_, 2000);
    createTrackbar(tbarname5, wndname2, &dist_, 2000);
 
 
    while(true) {
        // Les trackbar d'opencv ne donnent que des valeurs entières entre 0
        // et le max (dernier paramètre de createTrackbar)
        // Il faut donc adapter les paramètres
        double f, dist;
        double alpha, beta, gamma;
        alpha = ((double)alpha_ - 90.)*PI/180;
        beta = ((double)beta_ - 90.)*PI/180;
        gamma = ((double)gamma_ - 90.)*PI/180;
        f = (double) f_;
        dist = (double) dist_;
 
        if (video) {
            // Capture l'image de la caméra
            capture >> source;
        }
 
        // Affiche l'image
        imshow(wndname1, source);
 
        Size taille = source.size();
        double w = (double)taille.width, h = (double)taille.height;
 
        // Cette matrice projette l'image de la caméra (2D) dans l'espace (3D)
        // et la centre
        Mat A1 = (Mat_<double>(4,3) <<
            1, 0, -w/2,
            0, 1, -h/2,
            0, 0,    0,
            0, 0,    1);
        // On définit les 3 matrices de rotation (une par axe)
        Mat RX = (Mat_<double>(4, 4) <<
            1,          0,           0, 0,
            0, cos(alpha), -sin(alpha), 0,
            0, sin(alpha),  cos(alpha), 0,
            0,          0,           0, 1);
        Mat RY = (Mat_<double>(4, 4) <<
            cos(beta), 0, -sin(beta), 0,
                    0, 1,          0, 0,
            sin(beta), 0,  cos(beta), 0,
                    0, 0,          0, 1);
        Mat RZ = (Mat_<double>(4, 4) <<
            cos(gamma), -sin(gamma), 0, 0,
            sin(gamma),  cos(gamma), 0, 0,
            0,          0,           1, 0,
            0,          0,           0, 1);
        // La matrice de rotation finale
        Mat R = RX * RY * RZ;
        // Matrice de translation : on place le plan à la distance "dist" de
        // la caméra
        Mat T = (Mat_<double>(4, 4) <<
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, dist,
            0, 0, 0, 1);
        // Matrice de la caméra : projette l'image en 2D et la centre
        Mat A2 = (Mat_<double>(3,4) <<
            f, 0, w/2, 0,
            0, f, h/2, 0,
            0, 0,   1, 0);
        // La matrice de transformation est tout simplement le produit des
        // matrices
        Mat transfo = A2 * (T * (R * A1));
 
        // Applique la transformation
        warpPerspective(source, destination, transfo, taille, INTER_CUBIC | WARP_INVERSE_MAP);
        // Affiche le résultat
        imshow(wndname2, destination);
 
        if (video) {
            // Quitte lorsqu'une touche est pressée
            if(waitKey(10) >= 0)
                break;
        } else {
            // Quitte lorsque la touche echap est pressée
            if(waitKey(0) == 27)
                break;
        }
    }
 
    return 0;
}
