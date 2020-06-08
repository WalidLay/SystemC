/**********************************************
Travail à faire

    Écrivez une structure Pixel qui encapsule trois 
    composantes nonn signées (R,G,B) au format (5:6:5).
    Faites évoluer un signal transportant un Pixel en 
    incrémentant successivement chaque composante.
    Générez-les traces montrant cette évolution

**********************************/

#include <systemc.h>
#include "pixel.h"

// un type utilisateur
struct Pixel {
   
   sc_uint<5> R;
   sc_uint<6> G;
   sc_uint<5> B;

   // un constructeur particulier avec des valeurs par défaut
   Pixel( int _R=0, int _G=0, int _B=0){
         R = _R;
         G = _G;
         B = _B;
   };

bool operator == (const Pixel &other) const {
      return (R == other.R && G == other.G && B == other.B);
   }
   
   Pixel operator + (const Pixel &other) const {
       Pixel s;
       s.R = R +  other.R;
       s.G = G +  other.G;
       s.B = B +  other.B;
      return s;
   }

   Pixel operator ++ (int){
       Pixel s;
       s.R = R ++;
       s.G = G ++;
       s.B = B ++;
      return s;
   }

   friend ostream& operator << ( ostream& o, const Pixel& P ) {
      o 
         << "[" 
         << P.R << "," 
         << P.G << ","
         << P.B
         << "]" ;
      return o;
   }

   //pour encasuler : cacher les opérations que je fais et ne pas les détailler dans le main
   void inc(){
         R++;
         G++;
         B++;
   }
};

// surcharge de la fonction sc_trace pour le type utilisateur
void sc_trace( sc_trace_file* _f, const Pixel& _pix, const std::string& _s ) {
    sc_trace( _f, _pix.R, _s + "_R" );
    sc_trace( _f, _pix.G, _s + "_G" );
    sc_trace( _f, _pix.B, _s + "_B" );
}

/*******************************************************************
En reprenant le classe Pixel définie dans la section sur les signaux écrivez les modèles suivants :

    Deux modules combinatoires faisant la somme saturante de deux pixels.
        Le premier en utilisant une SC_METHOD
        Le second en utilisant un SC_THREAD
    3 modules séquentiels qui génère successivement, à la cadence d’une horloge externe et de façon cyclique, toutes les couleurs possibles :
        Vous utiliserez SC_METHOD, SC_THREAD et SC_CTHREAD
        L’état initial sera la couleur (0,0,0) et pourra être forcé grâce à un signal de remise à zéro asynchrone.

**********************************************/

//Module séquentiel utilisant Cthread pour générer des pixels
SC_MODULE(gen1_CT){
    //Les variables entrées/sorties
    sc_in<bool> clk {"clk"};
    sc_in<bool> rst {"rst"};
    sc_out<Pixel> Pout {"Pout"};

    //la méthode
    void gen(){
        Pixel P;
        for(;;){
            Pout = P;
            wait();
            P++;
        }
    }
    //Le constructeur
    SC_CTOR(gen1_CT){
        SC_CTHREAD(gen,clk.pos());
        async_reset_signal_is(rst,true);
    }

};

//Module séquentiel utilisant thread pour générer des pixels
SC_MODULE(gen2_CT){
    //Les variables entrées/sorties
    sc_in<bool> clk {"clk"};
    sc_in<bool> rst {"rst"};
    sc_out<Pixel> Pout {"Pout"};

    //la méthode
    void gen(){
        Pixel P;
        for(;;){
            Pout = P;
            wait();
            P++;
        }
    }
    //Le constructeur
    SC_CTOR(gen2_CT){
        SC_THREAD(gen);
        sensitive << clk.pos();
        async_reset_signal_is(rst,true);
    }

};


//Module séquentiel utilisant METHOD pour générer des pixels
SC_MODULE(gen3_CT){
    //Les variables entrées/sorties
    sc_in<bool> clk {"clk"};
    sc_in<bool> rst {"rst"};
    sc_out<Pixel> Pout {"Pout"};

    //la méthode : attention, pas de boucle ni de wait(), proche du langage RTL
    Pixel P;
    void gen(){
        if(rst)
            P=0;
            else
            P++;            
            Pout = P;
        }

    //Le constructeur
    SC_CTOR(gen3_CT){
        SC_METHOD(gen);
        sensitive << clk.pos();
        async_reset_signal_is(rst,true);
    }

};

SC_MODULE(SUM_COMB){

    //Les entrées / sorties
    sc_in <Pixel> P0;
    sc_in <Pixel> P1;
    sc_out<Pixel> Ps;


    //La fonction somme
    void sum(){
        Ps = P0.read() + P1.read();
    }

    //Le constructeur
    SC_CTOR(SUM_COMB){
        SC_METHOD(sum);
        sensitive << P0 << P1 ;
    }

};


// Le test
int sc_main (int argc, char * argv[])
{
   sc_trace_file *tfile ;
   tfile = sc_create_vcd_trace_file("ma_trace_variable");
   tfile->set_time_unit(1,SC_NS);

/********************    PROCESSUS         ***************/

    //2 instances de generateur cthread
    //gen1_CT gen_i1("gen_i1");
    //gen1_CT gen_i2("gen_i2");

    //2 instances de générateur thread
    //gen2_CT gen2_i1("gen2_i1");
    //gen2_CT gen2_i2("gen2_i2");

    //2 instances de générateur METHOD
    gen3_CT gen3_i1("gen3_i1");
    gen3_CT gen3_i2("gen3_i2");

    // une instance de sommateur
    SUM_COMB sum_comb_i ("sum_comb_i");


    // les signaux
    sc_clock clk("clk", 10, SC_NS);
    sc_signal<bool> rst;
    sc_signal<Pixel> sig1, sig2, sig_sum;

    // connexions avec cthread
    //gen_i1.clk(clk);
    //gen_i1.rst(rst);
    //gen_i1.Pout(sig1);

    //gen_i2.clk(clk);
    //gen_i2.rst(rst);
    //gen_i2.Pout(sig2);

    //Connexion avec gen2 utilisant thread
    //gen2_i1.clk(clk);
    //gen2_i1.rst(rst);
    //gen2_i1.Pout(sig1);

    //gen2_i2.clk(clk);
    //gen2_i2.rst(rst);
    //gen2_i2.Pout(sig2);

    //Connexion avec gen3 utilisant method
    gen3_i1.clk(clk);
    gen3_i1.rst(rst);
    gen3_i1.Pout(sig1);

    gen3_i2.clk(clk);
    gen3_i2.rst(rst);
    gen3_i2.Pout(sig2);


/*********** La somme ***************/

    sum_comb_i.P0(sig1);
    sum_comb_i.P1(sig2);
    sum_comb_i.Ps(sig_sum);

/*****************************************/
    // Traces
//#define TRACE(x) sc_trace(tfile, x, #x)    
   TRACE(clk);
   TRACE(rst);
   TRACE(sig1);
   TRACE(sig2);
   TRACE(sig_sum);

    sc_start(200, SC_NS);
    rst = 1;
    sc_start(200, SC_NS);
    rst = 0;
    sc_start(200, SC_NS);

   sc_close_vcd_trace_file(tfile);
   return 0;
}
