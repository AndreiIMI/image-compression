#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct nod {  // structura pentru arborele cuaternal
    unsigned int tipul_nodului;
    unsigned int red, green, blue;
    struct nod *f1, *f2, *f3, *f4;
} TNod, *TArb;

typedef struct pixel {  // structura pentru matricea de pixeli
    unsigned int red, green, blue;
} Pixel;

typedef struct celula {
  TArb info;
  struct celula* urm;
} TCelula2, *TLista;

typedef struct coada {  // structura pentru coada
  TLista inc, sf;      
} TCoada;

TArb Constr_Frunza(int r, int g, int b);
void distruge(TArb r);
void DistrArb(TArb *a);
void compress_image(TArb arb, Pixel **grid, int size, int x, int y, int factor);
void Init_Radacina(TArb arb);
void Atribuire_Frunza(TArb arb, int red, int green, int blue);
int Niv_Arbore(TArb arb);
int Numara_Frunze(TArb arb);
int NivPrima_Frunza(TArb arb, int niv);
void DistrQ(TCoada **c);
int ExtrQ(TCoada *c, TArb* x);
int IntrQ(TCoada *c, TArb x);
TCoada* InitQ();
void Generare_Arbore(TArb arb, FILE *in);
void decompress_image(TArb arb, Pixel **grid, int size, int x, int y);