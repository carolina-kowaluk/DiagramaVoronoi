//
//  DiagramaVoronoi.cpp
//  OpenGLTest
//
//  Created by Márcio Sarroglia Pinho on 23/08/23.
//  Copyright © 2023 Márcio Sarroglia Pinho. All rights reserved.
//

#include "DiagramaVoronoi.h"
#include "Envelope.h"
#include "Poligono.h"
#include "Ponto.h"

ifstream input;            // ofstream arq;

Voronoi::Voronoi()
{

}
Poligono Voronoi::LeUmPoligono()
{
    Poligono P;
    unsigned int qtdVertices;
    input >> qtdVertices;  // arq << qtdVertices
    for (int i=0; i< qtdVertices; i++)
    {
        double x,y;
        // Le um ponto
        input >> x >> y;
        Ponto(x, y).imprime();
        if(!input)
        {
            cout << "Fim inesperado da linha." << endl;
            break;
        }
        P.insereVertice(Ponto(x,y));
    }
    cout << "Poligono lido com sucesso!" << endl;
    return P;
}

void Voronoi::LePoligonos(const char *nome)
{
    input.open(nome, ios::in); //arq.open(nome, ios::out);
    if (!input)
    {
        cout << "Erro ao abrir " << nome << ". " << endl;
        exit(0);
    }
    string S;

    input >> qtdDePoligonos;
    cout << "qtdDePoligonos:" << qtdDePoligonos << endl;
    Ponto A, B;
    Diagrama[0] = LeUmPoligono();
    Diagrama[0].obtemLimites(Min, Max);// obtem o envelope do poligono
    envelopes[0] = Envelope(Min,Max);
    for (int i=1; i< qtdDePoligonos; i++)
    {
        Diagrama[i] = LeUmPoligono();
        Diagrama[i].obtemLimites(A, B); // obtem o envelope do poligono
        envelopes[i] = Envelope(A,B);

        Min = ObtemMinimo (A, Min);
        Max = ObtemMaximo (B, Max);
    }
    cout << "Lista de Poligonos lida com sucesso!" << endl;

}

Poligono Voronoi::getPoligono(int i)
{
    if (i >= qtdDePoligonos)
    {
        cout << "Nro de Poligono Inexistente" << endl;
        return Diagrama[0];
    }
    return Diagrama[i];
}
Envelope Voronoi::getEnvelope(int e)
{
    return envelopes[e];
}

void Voronoi::obtemLimites(Ponto &min, Ponto &max)
{
    min = this->Min;
    max = this->Max;
}
unsigned int Voronoi::getNPoligonos()
{
    return qtdDePoligonos;
}

int Voronoi::getPoligonoAtualInclConvexos(Ponto pontoAtual)
{
    // TODO: retornar poligono e colocar FOR externo percorrendo poligonos

    for (int p=0; p < qtdDePoligonos; p++)
    {
        int qtdNegativos =0;

        for (int i=0; i < Diagrama[p].getNVertices(); i++)
        {
            Ponto v1 = Diagrama[p].getVertice(i);
            Ponto v2 = Diagrama[p].getVertice((i + 1) % Diagrama[p].getNVertices());

            Ponto vetorA = Ponto(v2.x-v1.x, v2.y-v1.y, 0);
            Ponto vetorB = Ponto(pontoAtual.x-v1.x, pontoAtual.y-v1.y, 0);

            Ponto prodV;
            ProdVetorial(vetorA, vetorB, prodV);

            printf("\n");
            prodV.imprime();

            // quando der positivo, é a aresta que ele passou
            if (prodV.z < 0)
                qtdNegativos++;
        }
        if (qtdNegativos == Diagrama[p].getNVertices() || qtdNegativos == 0)
        {
            return p;
        }
    }

    return 0;
}

int Voronoi::getPoligonoAtualPorVizinhos(Ponto pontoAtual, int poligonoAtual)
{
    for (int i=0; i < Diagrama[poligonoAtual].getNVertices(); i++)
    {
        Ponto v1 = Diagrama[poligonoAtual].getVertice(i);
        Ponto v2 = Diagrama[poligonoAtual].getVertice((i + 1) % Diagrama[poligonoAtual].getNVertices());

        Ponto vetorA = Ponto(v2.x-v1.x, v2.y-v1.y, 0);
        Ponto vetorB = Ponto(pontoAtual.x-v1.x, pontoAtual.y-v1.y, 0);

        Ponto prodV;
        ProdVetorial(vetorA, vetorB, prodV);

        //printf("\n");
        //prodV.imprime();

            // quando der positivo, é a aresta que ele passou
        if (prodV.z > 0)
            return Diagrama[poligonoAtual].vizinhos[i];
    }
}


int Voronoi::getPoligonoAtualVizinhos(Ponto pontoAtual, Poligono poligonoAtual)
{
    printf("\n");
    for (int v=0; v<Diagrama[7].getNVizinhos(); v++)
    {
        printf("%d ",Diagrama[7].vizinhos[v]);
    }
    return 0;
}

void Voronoi::criaVizinhos()
{
    for (int pAtual=0; pAtual < getNPoligonos(); pAtual++)
    {
        printf("\nP atual: %d\n", pAtual);
        for (int vAtual=0; vAtual < Diagrama[pAtual].getNVertices(); vAtual++)
        {
            for (int pol=0; pol<getNPoligonos(); pol++)
            {
                if (pol != pAtual)
                {
                    for (int v=0; v<Diagrama[pol].getNVertices(); v++)
                    {
                        Ponto verticeA = Diagrama[pAtual].getVertice(vAtual);
                        Ponto verticeB = Diagrama[pol].getVertice(v);

                        if (verticeA == verticeB) // se tiver o mesmo vertice
                        {
                            //printf("acha vertice: %d ", pol);
                            Ponto proxV = Diagrama[pAtual].getVertice((vAtual+1) % Diagrama[pAtual].getNVertices());

                            if (proxV == Diagrama[pol].getVertice((v+1) % Diagrama[pol].getNVertices()) ||
                                proxV == Diagrama[pol].getVertice((v-1+Diagrama[pol].getNVertices()) % Diagrama[pol].getNVertices()))     // se for o poligono certo
                            {
                                //printf("%d, ", pol);
                                Diagrama[pAtual].insereVizinho(pol);
                            }

                        }

                    }
                }
            }
        }
    }
}

int Voronoi::getPoligonoAtualInclConcavos(Ponto p1, Ponto pontoAtual)
{
    //int cruzam[20];
    int contador=0;

    printf("Envelopes: \n");

    for (int i=0; i<19; i++)
    {
        if (envelopes[i].temColisao(envelopes[i], pontoAtual))
        {
            printf("\n p %d \n", i);
            int contaInterseccao = 0;
            for (int a=0; a < Diagrama[i].getNVertices(); a++) // percorre arestas
            {
                Ponto v1;
                Ponto v2;
                Diagrama[i].getAresta(a, v1, v2);



                if (HaInterseccao(p1, pontoAtual, v1, v2))
                {
                    printf("\nachou\n");
                    contaInterseccao++;
                }
            }

            if (contaInterseccao%2 != 0) // encontrou o poligono
            {
                Diagrama[i].imprime();
                //return Diagrama[i];
                printf("Cont: %d\n",getContadorInt());
                return i;
            }
        }
    }
}


