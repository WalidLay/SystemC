#include <systemc.h>
#include "pgcd.h"
#include <stdint.h>

SC_MODULE(PGCD_FUNC) {
   // L'horloge
   sc_in<bool> clk;
   // Les autres I/O
   sc_in<bool> valid;
   sc_in<sc_uint<8>> a;
   sc_in<sc_uint<8>> b;
   sc_in<bool> rst;
   sc_out<bool> ready;
   sc_out<sc_uint<8>> result;

   void calcul_pgcd()
   {
       for(;;){
           ready=0;
           if(valid.read()){
               result = pgcd(a.read(),b.read());
               ready=1;            
           }
           wait();
       }
   }

   SC_CTOR(PGCD_FUNC){
      SC_CTHREAD(calcul_pgcd, clk.pos());
      async_reset_signal_is(rst,true);
   }
};


SC_MODULE(PGCD_TIMED) {
   // L'horloge
   sc_in<bool> clk;
   // Les autres I/O
   sc_in<bool> valid;
   sc_in<sc_uint<8>> a;
   sc_in<sc_uint<8>> b;
   sc_in<bool> rst;
   sc_out<bool> ready;
   sc_out<sc_uint<8>> result;

   void calcul_pgcd()
   {
       for(;;){
           ready=0;
           if(valid.read()){
               result = timed_pgcd(a.read(),b.read());
               ready=1;            
           }
           wait();
       }
   }

   SC_CTOR(PGCD_TIMED){
      SC_CTHREAD(calcul_pgcd, clk.pos());
      async_reset_signal_is(rst,true);
   }

   int timed_pgcd (int a, int b){
    int max = (a>b)?a:b;//compare a>b et répond a, sinon b
    int min = (a>b)?b:a;//idem au dessus

    if (min==0){
        return max;
    }

    while (max-min){
        int d = max - min;
        max = (min>d)?min:d;
        min = (min>d)?d:min;
        wait(1);
    }
    return min;
} 
};

/*************************************************************************************
 * Etat initial: valid est à 0, état reste en état idle (en attente) et A et B restent les mêmes
 * 
 * Puis "valid" passe à 1 -> "etat" passe à Compute, puis max et min commence à être calculé (fonction datapath)
 * 
 * Il faut aussi définir une fonction de contrôle "ctrl" permettant de faire passer ready de 0 à 1, ainsi que 
 * l'état d'attente vers l'état de calcul
 * 
 * Il n'existe pas d'assignment sur systemC donc on doit créer une fonction liée à 
 * une sc_method permettant de faire une affectation différée à la fin du calcul vers PGCD
 * ************************************************************************************/

SC_MODULE(PGCD_RTL) {
   // L'horloge
   sc_in<bool> clk;
   // Les autres I/O
   sc_in<bool> valid;
   sc_in<sc_uint<8>> a;
   sc_in<sc_uint<8>> b;
   sc_in<bool> rst;
   sc_out<bool> ready;
   sc_out<sc_uint<8>> result;

   sc_signal<sc_uint<8>> max, min;

   //Pour faire le cheminement des données
   void datapath(){//si valid 
       if(valid)
            max = std::max(a,b);
            min = std::min(a,b);
       else
       {
           max = std::max(min,max-min);
           min = std::min(min,max-min);
       }
   }

   void ctrl(){
       enum etat = {idle,compute};//idle : en attente // compute : calcul
       ready = false;
       if(valid)
            etat = compute;
            if(max==min){
                etat = idle;
                ready = true;
            }
   }

    //Pour l'affectation différée
   void sortie(){
       result = max;
   }

   SC_CTOR(PGCD_RTL){
       SC_METHOD(datapath);
       sensitive << clk.pos();
       SC_METHOD(ctrl);
       sensitive << clk.pos();
       SC_METHOD(sortie);
       sensitive << clk.pos();
   }

};

int sc_main (int argc, char * argv[]){


    //trace des signaux
    sc_trace_file *trace_pgcd ;
    trace_pgcd  = sc_create_vcd_trace_file("trace_pgcd");
    trace_pgcd->set_time_unit(1,SC_NS);

  
    // l'horloge
    sc_time T(10,SC_NS);
    sc_clock clk("clk",T, 0.5, SC_ZERO_TIME, false);

    //Les signaux
    sc_signal<bool> rst ;
    sc_signal<bool> valid ;
    sc_signal<bool> ready;
    sc_signal<sc_uint<8>> a;
    sc_signal<sc_uint<8>> b;
    sc_signal<sc_uint<8>> resultat ;

    //instanciation du module PGCD
    PGCD_TIMED instance_pgcd("instance_pgcd");
    //PGCD_RTL instance_pgce("instance_pgcd");

    //Connexion avec les éléments des modules
    instance_pgcd.clk(clk);
    instance_pgcd.ready(ready);
    instance_pgcd.valid(valid);
    instance_pgcd.rst(rst);
    instance_pgcd.result(resultat);
    instance_pgcd.a(a);
    instance_pgcd.b(b);

    //TRACES
    #define TRACE(x) sc_trace(trace_pgcd, x, #x)  
    TRACE(clk);
    TRACE(valid);
    TRACE(ready);
    TRACE(a);
    TRACE(b);
    TRACE(resultat);

    for(int i=0; i<256; i++)
        for(int j=0; j<256; j++)
        {
            a = i;
            b = j;
            valid = true;
            sc_start(T);
            valid = false;
            while(!ready.read()) {
                sc_start(T);
            }
            sc_start(T);
            // on pourait comparer ici le résultat renvoyé par
            // le module avec un resultat calculé par la fonction

            int c;
            c = pgcd(i,j);


        }

    sc_close_vcd_trace_file(trace_pgcd);

    return 0;
}