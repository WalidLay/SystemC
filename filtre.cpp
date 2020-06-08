#include <string>
#include "filtre.h"

/************************************************************
 * 
 * filtre moyenneur sur 8 pixels adjacents 
 * (pixel central + ses 8 voisins) : dans la mesure du possible, 
 * ce filtre doit effectuer son traitement au vol.
 * 
 * //in : imageIn [0..ligNb-1]0..colNb-1]
//out : imageOut[0..ligNb-1][0..colNb-1]
 
//mise a zero de imageOut
pour lig =0 to ligNb-1
  pour col = 0 to colNg-1
     imageOut[lig][col]=0
 
//convolution moyenne 3x3
pour lig =1 to ligNb-2
{
  pour col = 1 to colNg-2
  {
     imageOut[lig][col]=0
     pour j=-1 to +1
     {
       pour i=-1 to +1
       {
          imageOut[lig][col]=imageOut[lig][col]+imageIn[lig+i][col+j]
        }
     }
     imageOut[lig][col]=/9;
  }
 * **********************************************************/

void FILTRE::moyenneur(){
  for (int i=0 ; i < WIDTH ; i++){
    for (int j = 0 ; j < HEIGHT; j++){
    }
  }
}
