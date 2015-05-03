#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

using namespace std;

void usage()
{

    cout << "Il sorgente stima l'entropia di un file per lunghezza di simboli diversi\n ";
    cout << "Successivametne valuta i rapporti di compressione di ordine m , CR=m/H(m) \n";
    cout << "\n\n\n";
}

void usage_error()
{
    cout << "Errore : Numero pametri insufficienti \n";
    cout << "Comandi validi : \n";
    cout << "(1) entropy nome_file  max_length_sym";
    cout << "(2) entropy nome_file  ";

}

float m_entropy_estimate(FILE * file , int m)
{

    int bit_count=0; // per tenere traccia dei bit di un simbolo già valutati
    int length_file=0; // lunghezza del file in numero di simboli
    int sym_value=0; // valore del simbolo corrente
    int array_dim=pow(2,m); // dimensione array dei possibili simboli
    int * counters=new int[array_dim]; // array di contatori

    // eseguo la lettura file

    char s='\0'; // simbolo a 8 bit
    while(s!=EOF) // lettura del file
    {
        s=fgetc(file);
        for(int pbit=7; pbit>=0 ; pbit--) // lettura sui singoli bit
		{
            // recupero valore del bit
            int pow_two=((s>>pbit)&1)<<bit_count;
            sym_value += pow_two;
            bit_count++;

            if(bit_count==m)
            {
                counters[sym_value]++;
                bit_count=0;
                length_file++;
                sym_value=0;
            }

		}

    }

    if (bit_count!=0)
    {
        // 0000 padding
        counters[sym_value]++;
        length_file++;
    }

    float h=0;
	float cumulative=0;
    int num_counters=0;

	for(int i=0; i<(array_dim) ; i++)
	{
		if(counters[i]!=0)
		{
			float pi=(float)counters[i]/(float)length_file;
			h+=pi*log2f(1/pi);
			cumulative+=pi;
		}

		num_counters+=counters[i];
	}

    return h;

}




int main(int argc, char **argv)
{
    usage();

    int max_sym_length=8;

    if(argc<2 || argc>3)
    {
        usage_error();
        return 0;

    }

    if(argc==3)
    {
        max_sym_length=atoi(argv[2]);
    }


    FILE * in;
    float h;
    float cr;

    for (int m=1;m<=max_sym_length;m++)
    {
        cout<< "========ESTIMATE ENTROPY =========\n\n";
        in=fopen(argv[1],"r");
        h=m_entropy_estimate(in,m);
        cr=m/h;
        fclose(in);

        cout << "Lunghezza simbolo :" << m << " \nEntropia : " <<h << "\nTasso Compressione : "<< cr <<"\n";
        cout<< "\n\n========ESTIMATE ENTROPY =========\n\n\n\n";

    }


    return 0;

}
