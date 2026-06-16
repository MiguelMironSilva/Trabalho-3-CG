#include "Veiculo.h"
#include <math.h>

Veiculo::Veiculo(float startX, float startZ)
{
    pos = Vector3(startX, 0, startZ);
    yaw = 0.0f;
    velocidade = 0.3f;

    largura = 1.0f;
    comprimento = 1.5f;
}

void Veiculo::moverFrente()
{
    pos.x += sin(yaw) * velocidade;
    pos.z += cos(yaw) * velocidade;
}

void Veiculo::moverTras()
{
    pos.x -= sin(yaw) * velocidade;
    pos.z -= cos(yaw) * velocidade;
}

void Veiculo::virarEsquerda()
{
    yaw += 0.1f;
}

void Veiculo::virarDireita()
{
    yaw -= 0.1f;
}

Vector3 Veiculo::calcularPosicaoRoda(float offsetLocalX, float offsetLocalZ)
{
    //matriz de rotação 2D aplicada ao vetor local do carro
    float globalX = pos.x + (offsetLocalX * cos(yaw) + offsetLocalZ * sin(yaw));
    float globalZ = pos.z + (-offsetLocalX * sin(yaw) + offsetLocalZ * cos(yaw));

    return Vector3(globalX, 0.0f, globalZ);
}

void Veiculo::adaptarAoTerreno(Terreno* terreno)
{
    //coordenadas X e Z de cada roda com base na direção atual
    rodaFrente = calcularPosicaoRoda(0.0f, comprimento);
    rodaEsq    = calcularPosicaoRoda(-largura, -comprimento);
    rodaDir    = calcularPosicaoRoda(largura, -comprimento);

    //intersecção vertical projetando as rodas contra os triângulos da malha
    rodaFrente.y = terreno->interseccionarTriangulo(rodaFrente.x, rodaFrente.z);
    rodaEsq.y    = terreno->interseccionarTriangulo(rodaEsq.x, rodaEsq.z);
    rodaDir.y    = terreno->interseccionarTriangulo(rodaDir.x, rodaDir.z);

    //atualização do centro do carro para a média das alturas
    pos.y = (rodaFrente.y + rodaEsq.y + rodaDir.y) / 3.0f;
}

void Veiculo::render(Camera* camera)
{
    //desenho das 3 Rodas (círculos projetados em perspectiva)
    CV::color(4); // Índice para a cor Azul
    Vector2 tFrente = camera->projetar(rodaFrente);
    Vector2 tEsq    = camera->projetar(rodaEsq);
    Vector2 tDir    = camera->projetar(rodaDir);

    CV::circleFill(tFrente, 5, 8);
    CV::circleFill(tEsq, 5, 8);
    CV::circleFill(tDir, 5, 8);


    //vetor que aponta da roda esquerda para a direita
    Vector3 eixoX = rodaDir - rodaEsq;
    eixoX.normalize();

    //vetor central que aponta para a frente do carro
    Vector3 pontoMedioTraseiro = (rodaDir + rodaEsq) / 2.0f;
    Vector3 eixoZ = rodaFrente - pontoMedioTraseiro;
    eixoZ.normalize();

    //produto vetorial para encontrar a Normal da superfície do carro (Eixo Y local)
    Vector3 eixoY = eixoZ.cross(eixoX);
    eixoY.normalize();

    // ortogonalidade perfeita recalculando o eixo Z
    eixoZ = eixoX.cross(eixoY);
    eixoZ.normalize();

    //4 vértices do plano do carro usando a nova base vetorial rotacionada
    Vector3 v1 = pos - (eixoX * largura) + (eixoZ * comprimento);
    Vector3 v2 = pos + (eixoX * largura) + (eixoZ * comprimento);
    Vector3 v3 = pos + (eixoX * largura) - (eixoZ * comprimento);
    Vector3 v4 = pos - (eixoX * largura) - (eixoZ * comprimento);

    //vértices do plano do carro
    float pX[4], pY[4];
    Vector2 t1 = camera->projetar(v1); pX[0] = t1.x; pY[0] = t1.y;
    Vector2 t2 = camera->projetar(v2); pX[1] = t2.x; pY[1] = t2.y;
    Vector2 t3 = camera->projetar(v3); pX[2] = t3.x; pY[2] = t3.y;
    Vector2 t4 = camera->projetar(v4); pX[3] = t4.x; pY[3] = t4.y;

    CV::color(3); //verde
    CV::polygonFill(pX, pY, 4); //corpo plano rotacionado
}
