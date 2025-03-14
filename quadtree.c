#include "structs.h"

int main(int argc, char *argv[]) 
{
    TArb arb = Constr_Frunza(0, 0, 0);
    FILE *in = fopen(argv[argc - 2], "r");
    FILE *out = fopen(argv[argc - 1], "wt");
    int n, m;
    Pixel **grid;

    // tratarea cerintei 3

    if (strcmp("-d", argv[1]) == 0)
    {
        fread(&n, sizeof(unsigned int), 1, in);
        Generare_Arbore(arb, in);   // generam arbore bazat pe fisierul comprimat
        grid = malloc(n * sizeof(Pixel*));
        for (int i = 0; i < n; i++)
            grid[i] = calloc(n, sizeof(Pixel));
        decompress_image(arb, grid, n, 0, 0);   // construim matricea de pixeli
        // completam fisierul .ppm cu informatiile necesare
        fprintf(out, "P6\n");
        fprintf(out, "%d %d\n", n, n);
        fprintf(out, "255\n");
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                {
                    unsigned char red = grid[i][j].red, green = grid[i][j].green, blue = grid[i][j].blue;
                    fwrite(&red, sizeof(unsigned char), 1, out);
                    fwrite(&green, sizeof(unsigned char), 1, out);
                    fwrite(&blue, sizeof(unsigned char), 1, out);
                }
        // eliberam matricea
        for (int i = 0; i < n; i++)
            free(grid[i]);
        free(grid);
        fclose(in);
        fclose(out);
        DistrArb(&arb);
        return 0;
    }

    // tratare cerinte 1 & 2

    // citim matricea de pixeli din fisierul de intrare
    int factor = atoi(argv[2]);
    fseek(in, 3, SEEK_SET);
    fscanf(in, "%d", &n);
    fscanf(in, "%d", &m);
    grid = malloc(n * sizeof(Pixel*));
    for (int i = 0; i < m; i++)
        grid[i] = calloc(m, sizeof(Pixel));
    fseek(in, 5, SEEK_CUR);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
        {
            unsigned char red, green, blue;
            fread(&red, sizeof(unsigned char), 1, in);
            fread(&green, sizeof(unsigned char), 1, in);
            fread(&blue, sizeof(unsigned char), 1, in);
            grid[i][j].red = red;
            grid[i][j].green = green;
            grid[i][j].blue = blue;
        }
    fclose(in);

    int size = n;
    compress_image(arb, grid, size, 0, 0, factor);  // formam arborele de comprimare
    if (strcmp("-c1", argv[1]) == 0)
    {
        fprintf(out, "%d\n", Niv_Arbore(arb));  // printam numarul de nivele din arbore
        fprintf(out, "%d\n", Numara_Frunze(arb));   // printam numarul de frunze (noduri de tip 1)
        int frun = NivPrima_Frunza(arb, 0); // gasim nivelul pe care se afla prima frunza
        int section = 1;
        for (int i = 0; i < frun; i++)
        section = section * 2;
        int area = n / section;
        fprintf(out, "%d\n", area); // printam lungimea celei mai mari zone
    }

    if (strcmp("-c2", argv[1]) == 0)
    {
        fwrite(&n, sizeof(unsigned int), 1, out);   // scriem lungimea in fisier

        // facem parcurgerea pe latime a arborelui

        TCoada *coada;
        coada = InitQ();
        IntrQ(coada, arb);  // introducem radacina in coada
        while (coada->inc != NULL)
        {
            TArb aux;
            ExtrQ(coada, &aux);
            if (aux->tipul_nodului == 0)
            {
                // introducem cei 4 fii in coada
                unsigned char type = aux->tipul_nodului;
                fwrite(&type, sizeof(unsigned char), 1, out);
                IntrQ(coada, aux->f1);
                IntrQ(coada, aux->f2);
                IntrQ(coada, aux->f3);
                IntrQ(coada, aux->f4);
            }
            else
            {
                // scriem culorile in format RGB in fisier
                unsigned char type = aux->tipul_nodului;
                unsigned char red = aux->red, green = aux->green, blue = aux->blue;
                fwrite(&type, sizeof(unsigned char), 1, out);
                fwrite(&red, sizeof(unsigned char), 1, out);
                fwrite(&green, sizeof(unsigned char), 1, out);
                fwrite(&blue, sizeof(unsigned char), 1, out);
            }              
        }
        DistrQ(&coada);
    }
    // eliberam matricea
    for (int i = 0; i < m; i++)
        free(grid[i]);
    free(grid);
    fclose(out);
    DistrArb(&arb); // distrugem arborele de comprimare
    return 0;
}