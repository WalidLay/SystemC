#ifndef FILTRE_H
#define FILTRE_H

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

SC_MODULE(FILTRE){
   //I/O ports
   sc_in  <bool>          clk ;
   sc_in  <bool>          reset_n;
   sc_in  <bool>          h_in;
   sc_in  <bool>          v_in;
   sc_in <unsigned char>  p_in;

   sc_out<unsigned char>  p_out;
   sc_out <bool>          h_out;
   sc_out <bool>          v_out;


    //Le constructeur
    SC_CTOR(FILTRE){
        SC_METHOD(moyenneur);
        sensitive << clk.pos();
        async_reset_signal_is(reset_n,false);

        //A compléter
    }

   /***************************************************
    *  méthodes et champs internes
    **************************************************/
   private:
   void moyenneur();
   const std::string base_name;
   Image             image;

   //Les dimensions à utiliser
   static const int HEIGHT  = 576;
   static const int WIDTH   = 720;

   public:


};

#endif
