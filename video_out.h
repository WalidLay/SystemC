#ifndef VIDEO_OUT_H
#define VIDEO_OUT_H

#include <systemc.h>
#include "image.h"


/**************** DEFINITION DU MODULE **************/

/******************************************************
 * 
 * la méthode doit permettre de stocker dans un tableau les valeurs de pixel
 * une fois que le bon nombre de pixel est atteint (dès que href est égal à 1 -> signal de synchro -> tu mets)
 * il faut donc faire un compteur, initialisé à 0, qui une fois qu'il a atteint la valeur max=longueur*largeur, se remet à 0
 *  
 * *****************************************************/

SC_MODULE(VIDEO_OUT){
   //  I/O ports

   sc_in<bool>          clk ;
   sc_in<bool>          reset_n;
   sc_in<bool>          href;
   sc_in<bool>          vref;
   sc_in<unsigned char> pixel_in;
   
   /***************************************************
    *  constructeur
    **************************************************/
   VIDEO_OUT(sc_module_name n, const std::string & b_n = "out"):
      sc_module(n),
      base_name(b_n)
   {
      cout << "Instanciation de " << name() <<" ..." ;

      SC_METHOD(loop);
      sensitive << clk.pos();
      async_reset_signal_is(reset_n,false);

      pix_num = 0;
      im_num = 0;
      vref_old = 0;
      image.pixel = Tab;
      image.width = WIDTH;
      image.height = HEIGHT;

      cout << "... réussie" << endl;
   }

   SC_HAS_PROCESS(VIDEO_OUT);

   /***************************************************
    *  méthodes et champs internes
    **************************************************/
   private:
   void loop();
   const std::string   base_name;              // nom de base des images d'entrée
   Image               image;

   // Pour ce projet les dimensions de l'image sont imposées
   static const int HEIGHT  = 576;
   static const int WIDTH   = 720;
   unsigned char Tab[WIDTH*HEIGHT];

   public:
   int              pix_num;
   bool             vref_old;
   int              im_num;
};

#endif