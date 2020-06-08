#include <string>
#include "video_out.h"

/******************************************************
 * 
 * la méthode doit permettre de stocker dans un tableau les valeurs de pixel
 * une fois que le bon nombre de pixel est atteint (dès que href est égal à 1 -> signal de synchro -> tu mets)
 * il faut donc faire un compteur, initialisé à 0, qui une fois qu'il a atteint la valeur max=longueur*largeur, se remet à 0
 *  
 * *****************************************************/

void VIDEO_OUT::loop(){

    //le reset
    if(!reset_n.read()){
        pix_num = 0;
        im_num  = 0;
    }

    //une nouvelle image
    if(vref.read() && !vref_old){
        pix_num = 0;
    }

    //Chargement dans un tableau dès que href=1
    if (href.read()){
        Tab[pix_num] = pixel_in.read();
        pix_num ++;
    }

    //A la fin d'une image
    if(pix_num == WIDTH*HEIGHT){
        std::string name = base_name + std::to_string(im_num) + ".png";
        image_write(&image,name.c_str());
        pix_num = 0;
        im_num++;
        if (im_num==4){
            im_num=0;
        }
    }

    //Détection d'une nouvelle image
    vref_old = vref.read();
}