#ifndef __TERRENO_H__
#define __TERRENO_H__

#include "Camera.h"
#include "gl_canvas2d.h"

//tamanho mínimo da malha de controle
#define LADO_CONTROLE 10

class Terreno
{
private:
    Vector3 pontosControle[LADO_CONTROLE][LADO_CONTROLE];
    float alturasOriginais[LADO_CONTROLE][LADO_CONTROLE];
    float fatorOndulacao;

    //funções para o cálculo de Bézier
    float calcularFatorial(int n);
    float calcularCombinacao(int n, int k);
    float calcularBernstein(int i, int n, float t);

public:
    Terreno();

    //distribuição espacial dos pontos de controle
    void inicializar();

    //coordenadas 3D da superfície no espaço paramétrico (u, v)
    Vector3 avaliarBezier(float u, float v);

    //altera a escala vertical do relevo baseada no input no main
    void alterarOndulacao(float delta);

    //adapta a altura e a rotação do veículo
    float interseccionarTriangulo(float px, float pz);

    //renderiza a malha curva avaliada e destaca os nós de controle
    void render(Camera* camera);
};

#endif
