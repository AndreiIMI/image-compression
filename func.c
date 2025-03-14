#include "structs.h"

TArb Constr_Frunza(int r, int g, int b) // construieste un nod de tip 1 care are valori RGB
{
    TArb aux = (TArb) malloc(sizeof(TNod));
    if (!aux) 
        return NULL;

    aux->tipul_nodului = 1;
    aux->red = r;
    aux->green = g;
    aux->blue = b;
    aux->f1 = aux->f2 = aux->f3 = aux->f4 = NULL;

    return aux;
}

void Init_Radacina(TArb arb)    // initializeaza un nod de tip 0
{
    arb->tipul_nodului = 0;
    arb->f1 = Constr_Frunza(0, 0, 0);
    arb->f2 = Constr_Frunza(0, 0, 0);
    arb->f3 = Constr_Frunza(0, 0, 0);
    arb->f4 = Constr_Frunza(0, 0, 0);
}

void distruge(TArb r)   // distruge un nod
{
	if (!r) return;
	distruge (r->f1);
	distruge (r->f2);
    distruge (r->f3);
    distruge (r->f4);
	free (r);
}

void DistrArb(TArb *a)  // distruge intreg arborele
{
	if (!(*a)) return;
	distruge (*a);
	*a = NULL;
}

// formeaza arborele de compresie cu ajutorul matricei de pixeli

void compress_image(TArb arb, Pixel **grid, int size, int x, int y, int factor)
{
    unsigned long long red = 0, green = 0, blue = 0, mean = 0;
    for (int i = x; i < x + size; i++)
        for (int j = y; j < y + size; j++)
        {
            red = red + grid[i][j].red;
            green = green + grid[i][j].green;
            blue = blue + grid[i][j].blue;
        }
    red = red / (size * size);
    green = green / (size * size);
    blue = blue / (size * size);
    for (int i = x; i < x + size; i++)
        for (int j = y; j < y + size; j++)
        {
            mean = mean + (red - grid[i][j].red)*(red - grid[i][j].red);
            mean = mean + (green - grid[i][j].green)*(green - grid[i][j].green);
            mean = mean + (blue - grid[i][j].blue)*(blue - grid[i][j].blue);
        }
    mean = mean / (3 * size * size);
    if (mean > factor)
    {
        size = size / 2;
        Init_Radacina(arb); // divizam zona dupa care apelam recursiv cele patru zone noi create
        compress_image(arb->f1, grid, size, x, y, factor);
        compress_image(arb->f2, grid, size, x, y + size, factor);
        compress_image(arb->f3, grid, size, x + size, y + size, factor);
        compress_image(arb->f4, grid, size, x + size, y, factor);
    }
    else
    {
        Atribuire_Frunza(arb, red, green, blue);
    }  
}

void Atribuire_Frunza(TArb arb, int red, int green, int blue)
{
    arb->tipul_nodului = 1;
    arb->red = red;
    arb->green = green;
    arb->blue = blue;
    arb->f1 = arb->f2 = arb->f3 = arb->f4 = NULL;
}

int Niv_Arbore(TArb arb)    // calculeaza numarul de nivele din arbore
{
    int n1, n2, n3, n4;
    if (arb == NULL)
        return 0;
    n1 = Niv_Arbore(arb->f1);
    n2 = Niv_Arbore(arb->f2);
    n3 = Niv_Arbore(arb->f3);
    n4 = Niv_Arbore(arb->f4);
    int max = n1;
    if (n2 > max)
        max = n2;
    if (n3 > max)
        max = n3;
    if (n4 > max)
        max = n4;
    return 1 + max;
}

int Numara_Frunze(TArb arb) // calculeaza numarul de frunze din arbore
{
    int nr;
    if (arb == NULL)
        return 0;
    nr = Numara_Frunze(arb->f1);
    nr += Numara_Frunze(arb->f2);
    nr += Numara_Frunze(arb->f3);
    nr += Numara_Frunze(arb->f4);
    if (arb->tipul_nodului == 1)
        nr++;
    return nr;
}

int NivPrima_Frunza(TArb arb, int niv)  // returneaza nivelul pe care se afla prima frunza
{
    int n1, n2, n3, n4;
    if (arb == NULL)
        return -1;
    if (arb->tipul_nodului == 1)
        return niv;
    n1 = NivPrima_Frunza(arb->f1, niv + 1);
    n2 = NivPrima_Frunza(arb->f2, niv + 1);
    n3 = NivPrima_Frunza(arb->f3, niv + 1);
    n4 = NivPrima_Frunza(arb->f4, niv + 1);
    int min = 99999;
    if (n1 < min && n1 != -1)
        min = n1;
    if (n2 < min && n2 != -1)
        min = n2;
    if (n3 < min && n3 != -1)
        min = n3;
    if (n4 < min && n4 != -1)
        min = n4;
    return min;
}

TCoada* InitQ() // initializeaza o coada vida
{ 
  TCoada* c;
  c = (TCoada*)malloc(sizeof(TCoada));
  if ( ! c ) return NULL;
  c->inc = c->sf = NULL;
  return c;
}

int IntrQ(TCoada *c, TArb x)    // introduce un nod in coada
{ 
  TLista aux;
  aux = (TLista)malloc(sizeof(TCelula2));
  if (!aux) return 0;          
  aux->info = x;
  aux->urm = NULL;
  if (c->sf != NULL)
    c->sf->urm = aux;     
  else          
    c->inc = aux;        
  c->sf = aux;          
  return 1;          
}

int ExtrQ(TCoada *c, TArb* x)   // extrage un nod din coada
{
  TLista aux;
  aux = c->inc;
  *x = aux->info;
  if (c->inc == c->sf) {
    c->inc = NULL;
    c->sf = NULL;
  }
  else
    c->inc = aux->urm;
  free(aux);  
  return 1;              
}

void DistrQ(TCoada **c) // distruge coada
{
  TLista p, aux;
  p = (*c)->inc;
  while(p) {
    aux = p;
    p = p->urm;
    free(aux);
  }
  free(*c);
  *c = NULL;
}

void Generare_Arbore(TArb arb, FILE *in)    // genereaza un arbore bazat pe fisierul comprimat
{
        TCoada *coada;
        coada = InitQ();
        unsigned char type, red, green, blue;
        fread(&type, sizeof(unsigned char), 1, in);
        if (type == 0) {
            IntrQ(coada, arb);
            while (coada->inc != NULL)  // folosim o coada pentru a retine ordinea tatilor din arbore
            {
                TArb aux;
                ExtrQ(coada, &aux); // extragem primul tata din coada
                Init_Radacina(aux);
                fread(&type, sizeof(unsigned char), 1, in);
                if (type == 0)
                {
                    IntrQ(coada, aux->f1);  // daca este un nod tata, il introducem in coada
                }
                else
                {
                    fread(&red, sizeof(unsigned char), 1, in);
                    fread(&green, sizeof(unsigned char), 1, in);
                    fread(&blue, sizeof(unsigned char), 1, in);
                    Atribuire_Frunza(aux->f1, (int)red, (int)green, (int)blue); 
                }
                fread(&type, sizeof(unsigned char), 1, in);
                if (type == 0)
                {
                    IntrQ(coada, aux->f2);
                }
                else
                {
                    fread(&red, sizeof(unsigned char), 1, in);
                    fread(&green, sizeof(unsigned char), 1, in);
                    fread(&blue, sizeof(unsigned char), 1, in);
                    Atribuire_Frunza(aux->f2, (int)red, (int)green, (int)blue); 
                }
                fread(&type, sizeof(unsigned char), 1, in);
                if (type == 0)
                {
                    IntrQ(coada, aux->f3);
                }
                else
                {
                    fread(&red, sizeof(unsigned char), 1, in);
                    fread(&green, sizeof(unsigned char), 1, in);
                    fread(&blue, sizeof(unsigned char), 1, in);
                    Atribuire_Frunza(aux->f3, (int)red, (int)green, (int)blue); 
                }
                fread(&type, sizeof(unsigned char), 1, in);
                if (type == 0)
                {
                    IntrQ(coada, aux->f4);
                }
                else
                {
                    fread(&red, sizeof(unsigned char), 1, in);
                    fread(&green, sizeof(unsigned char), 1, in);
                    fread(&blue, sizeof(unsigned char), 1, in);
                    Atribuire_Frunza(aux->f4, (int)red, (int)green, (int)blue); 
                }
            }
        }
        else
        {
            fread(&red, sizeof(unsigned char), 1, in);
            fread(&green, sizeof(unsigned char), 1, in);
            fread(&blue, sizeof(unsigned char), 1, in);
            Atribuire_Frunza(arb, (int)red, (int)green, (int)blue);
        }
    DistrQ(&coada);
}

void decompress_image(TArb arb, Pixel **grid, int size, int x, int y)   // formeaza o matrice de pixeli bazata pe arbore
{
    if (arb == NULL)
        return;
    for (int i = x; i < x + size; i++)
        for (int j = y; j < y + size; j++)
        {
            grid[i][j].red = arb->red;
            grid[i][j].green = arb->green;
            grid[i][j].blue = arb->blue;
        }
    size = size / 2;
    decompress_image(arb->f1, grid, size, x, y);
    decompress_image(arb->f2, grid, size, x, y + size);
    decompress_image(arb->f3, grid, size, x + size, y + size);
    decompress_image(arb->f4, grid, size, x + size, y);
}