#ifndef __VEICULO_H__
#define __VEICULO_H__

#include "Vector3.h"
#include "Vector2.h"
#include "Camera.h"
#include "Terreno.h"
#include "gl_canvas2d.h"

class Veiculo
{
private:
    Vector3 pos;           //posição central do veículo (X, Y, Z)
    float yaw;             //angulo de rotação direcional (esquerda/direita)
    float velocidade;

    //dimensões do veículo
    float largura;
    float comprimento;

    //coordenadas das 3 rodas
    Vector3 rodaFrente;
    Vector3 rodaEsq;
    Vector3 rodaDir;

    //converte uma coordenada local do carro para o espaço global
    Vector3 calcularPosicaoRoda(float offsetLocalX, float offsetLocalZ);

public:
    Veiculo(float startX, float startZ);

    //controles de teclado para movimento
    void moverFrente();
    void moverTras();
    void virarEsquerda();
    void virarDireita();

    //cálculo da intersecção geométrica com o terreno e ajuste da rotação do corpo
    void adaptarAoTerreno(Terreno* terreno);

    //renderização das rodas e projeção do corpo poligonal rotacionado
    void render(Camera* camera);
};

#endif
