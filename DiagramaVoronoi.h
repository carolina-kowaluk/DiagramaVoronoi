//
//  DiagramaVoronoi.hpp
//  OpenGLTest
//
//  Created by Márcio Sarroglia Pinho on 23/08/23.
//  Copyright © 2023 Márcio Sarroglia Pinho. All rights reserved.
//

#ifndef DiagramaVoronoi_h
#define DiagramaVoronoi_h

#include <iostream>
#include <fstream>
using namespace std;

#include "Poligono.h"
#include "Envelope.h"
#include "Ponto.h"

class Voronoi
{
    Poligono Diagrama[1000];
    Envelope envelopes[5000];
    unsigned int qtdDePoligonos;
    Ponto Min, Max;
public:
    Voronoi();
    Poligono LeUmPoligono();
    void LePoligonos(const char *nome);
    Poligono getPoligono(int i);
    void obtemLimites(Ponto &min, Ponto &max);
    unsigned int getNPoligonos();
    Poligono getPoligonoAtualInclConcavos(Ponto p1, Ponto pontoAtual);
    bool getPoligonoAtualInclConvexos(Ponto pontoAtual);
};

#endif /* DiagramaVoronoi_h */
