#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <math.h>
#include "Vector3.h"
#include "Vector2.h"

class Camera
{
private:
    float anguloX;     //rotação no eixo X
    float anguloY;     //rotação no eixo
    float distanciaZ;  //distância de afastamento do cenário
    float cameraZoom;          //distância focal (fator de zoom da projeção)
    int* screenWidth;
    int* screenHeight;

    //rotação de um ponto no eixo X
    Vector3 rotacionarX(Vector3 p, float anguloGraus)
    {
        Vector3 resp;
        float rad = anguloGraus * 3.14159265359f / 180.0f;

        resp.x = p.x;
        resp.y = p.y * cos(rad) - p.z * sin(rad);
        resp.z = p.y * sin(rad) + p.z * cos(rad);

        return resp;
    }

    //rotação de um ponto no eixo Y
    Vector3 rotacionarY(Vector3 p, float anguloGraus)
    {
        Vector3 resp;
        float rad = anguloGraus * 3.14159265359f / 180.0f;

        resp.x = p.x * cos(rad) + p.z * sin(rad);
        resp.y = p.y;
        resp.z = -p.x * sin(rad) + p.z * cos(rad);

        return resp;
    }

public:
    Camera(int* w, int* h)
    {
        anguloX = 25.0f;
        anguloY = 0.0f;
        distanciaZ = 30.0f;
        cameraZoom = 500.0f;         //proporção do zoom de perspectiva
        screenWidth = w;
        screenHeight = h;
    }

    //alteração de ângulos com o teclado
    void alterarAngulos(float dx, float dy)
    {
        anguloX += dx;
        anguloY += dy;
    }

    //aproximação ou afastamento na visualização
    void alterarDistancia(float dz)
    {
        distanciaZ += dz;
        if (distanciaZ < 2.0f) distanciaZ = 2.0f; //câmera não atravessa a origem
    }

    //projeção do espaço 3d no espaço na tela
    Vector2 projetar(Vector3 pontoMundo)
    {
        //aplicação das rotações globais do cenário
        Vector3 p = rotacionarY(pontoMundo, anguloY);
        p = rotacionarX(p, anguloX);

        //aplicação da translação da câmera (afastando o cenário no eixo Z)
        p.z += distanciaZ;

        //clip para evitar divisões por zero ou inversões caso o ponto fique atrás da câmera
        if (p.z <= 0.1f) p.z = 0.1f;

        //execução da projeção em perspectiva centralizada nas dimensões da Canvas2D
        Vector2 pontoTela;
        pontoTela.x = (p.x * cameraZoom) / p.z + (*screenWidth / 2.0f);
        pontoTela.y = (p.y * cameraZoom) / p.z + (*screenHeight / 2.0f);

        return pontoTela;
    }
};

#endif
