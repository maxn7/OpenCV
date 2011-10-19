
#include <opencv/cv.h>
#include <opencv/highgui.h>


using namespace std;



int main()
{
    CvScalar scalaire ;

    IplImage *img;

    img=cvLoadImage("view.jpg",1); // 1 RGB, 0 niveau de gris

    // fenêtres
   // cvNamedWindow("input", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("output", CV_WINDOW_AUTOSIZE);

    cvShowImage("input", img);


       //On parcourt toute l'image
    for(int x=0; x<img->width; x++)
    {
        for(int y=0; y<img->height; y++)
        {

            //On récupère le pixel de coordonnées (x,y)
            scalaire=cvGet2D(img, y, x);


            /*if(scalaire.val[2] > 200 && scalaire.val[1] > 200)
            {
                scalaire.val[2] = 200 ;
                scalaire.val[1] = 200 ;
            }


            else
            {
            scalaire.val[2] = 255 ;
            scalaire.val[1] = 255 ;
            }*/



            if(scalaire.val[2] + scalaire.val[1] + scalaire.val[0] < 400 )
            {
                scalaire.val = {0,0,0} ;
            }

            cvSet2D(img,y,x,scalaire);




        }
    }

 cvShowImage("output", img);

    //On attend que l'utilisateur appuie sur une touche (0 = indéfiniment).
    cvWaitKey(0);

    //Destruction de la fenêtre.
    cvDestroyWindow("input");

    //Libération de l'IplImage (on lui passe un IplImage**).
    cvReleaseImage(&img);

    //Fini ^^
    return 0;

}
