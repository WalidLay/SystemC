#include <systemc.h>
#include "pgcd_signal.h"
#include <stdint.h>

//Un signal de type uint 
struct Sig {

    sc_uint<8> signal;

    //un constructeur
    Sig(sc_uint<8> _signal=0){
        signal = _signal;
    }

    //surcharge de fonction
    Sig operator ++ (int){
        Sig s;
        s.signal = signal ++;
    }

    bool operator == (const Sig &other) const {
      return (signal == other.signal);
    }

   bool operator - (const Sig &other) const {
       return (signal - other.signal);
    }

   bool operator > (const Sig &other) const {
       return(signal > other.signal);
    }

    void inc(){
        signal ++;
    }

};

//Module séquentiel générant ces signaux
SC_MODULE(gen1){
    sc_in<bool> clk;
    sc_in<bool> rst;
    sc_out<Sig> Pout;

    //Incrémentation du signal
    void gen(){
        Sig s;
        for(;;){
            Pout = s;
            wait();
            s++;
        }
    }

    //le constructeur
    SC_CTOR(gen1){
        SC_CTHREAD(gen,clk.pos());
        async_reset_signal_is(rst,true);
    }
};

SC_MODULE(PGCD){

    //les entrées sorties
    sc_in<Sig> S0;
    sc_in<Sig> S1;
    sc_out<Sig> result;

    //la fonction pgcd
    // Sig pgcd(){
    //     Sig temp;
    //     Sig max;
    //     Sig min;

    //     max = (S0 > S1) ? S0:S1;
    //     min = (S0 > S1) ? S1:S0;

    //     if (min==0){
    //         return max;
    //     }

    //     while (max - min){
    //         temp = max - min;
    //         max = (min > temp)?min:temp;
    //        min = (min > temp)?temp:min;
    //     }
    //     result = min; 
    // }

    void CALCUL(){
        Sig s;
        s = pgcd(S0.read(),S1.read());
        s = result;
    }

    

    //Le constructeur
    SC_CTOR(PGCD){
        SC_METHOD(CALCUL) ;
        sensitive << S0 << S1 ;
    }

};

int sc_main (int argc, char * argv[])
{

    sc_trace_file *pgcd_file;
    pgcd_file = sc_create_vcd_trace_file("calculs_de_pgcd");
    pgcd_file->set_time_unit(1,SC_NS);

    //On instancie deux générateurs de signaux
    gen1 gen_S0("gen_S0");
    gen1 gen_S1("gen_S1");

    //On instancie la méthode de calcul des PGCD
    PGCD calc_pgcd("calc_pgcd");

    //les signaux devront être généraux à chaque posedge de clk
    sc_clock clk("clk", 10, SC_NS);
    sc_signal<bool> rst;
    sc_signal<Sig> S0, S1, Result;

    gen_S0.clk(clk);
    gen_S0.rst(rst);
    gen_S0.Pout(S0);

    gen_S1.clk(clk);
    gen_S1.rst(rst);
    gen_S1.Pout(S1);

    //Opération du pgcd
    calc_pgcd.S0(S0);
    calc_pgcd.S1(S1);
    calc_pgcd.result(Result);

    //Traces
    TRACE(clk);
    TRACE(S0);
    TRACE(S1);
    TRACE(Result);

    sc_start(600, SC_NS);

    sc_close_vcd_trace_file(pgcd_file);
    return 0;
}