#include "Terreno.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>

Terreno::Terreno()
{
    fatorOndulacao = 1.0f;
    inicializar();
}

void Terreno::inicializar()
{
    //semente aleatória para variação a cada execução do cenário
    srand(time(NULL));

    for (int i = 0; i < LADO_CONTROLE; i++)
    {
        for (int j = 0; j < LADO_CONTROLE; j++)
        {
            //mapeia os pontos nos eixos X e Z centrados na origem (0,0)
            float x = (i - (LADO_CONTROLE / 2.0f)) * 3.5f;
            float z = (j - (LADO_CONTROLE / 2.0f)) * 3.5f;

            //elevações aleatórias iniciais para o terreno
            alturasOriginais[i][j] = (rand() % 40) / 10.0f;
            float y = alturasOriginais[i][j] * fatorOndulacao;

            pontosControle[i][j] = Vector3(x, y, z);
        }
    }
}

float Terreno::calcularFatorial(int n)
{
    float f = 1.0f;
    for (int c = 2; c <= n; c++) f *= c;
    return f;
}

float Terreno::calcularCombinacao(int n, int k)
{
    if (k < 0 || k > n) return 0.0f;
    return calcularFatorial(n) / (calcularFatorial(k) * calcularFatorial(n - k));
}

float Terreno::calcularBernstein(int i, int n, float t)
{
    // Fórmula clássica: J_(i,n)(t) = C(n,i) * t^i * (1-t)^(n-i)
    return calcularCombinacao(n, i) * pow(t, i) * pow(1.0f - t, n - i);
}

float Terreno::interseccionarTriangulo(float px, float pz)
{
    float larguraTotal = (LADO_CONTROLE - 1) * 3.5f;
    float xMin = -(larguraTotal / 2.0f);
    float zMin = -(larguraTotal / 2.0f);

    float u = (px - xMin) / larguraTotal;
    float v = (pz - zMin) / larguraTotal;

    //rrava os limites (Clamp) para que o carro não caia no "vazio" matemático
    if (u < 0.0f) u = 0.0f; if (u > 1.0f) u = 1.0f;
    if (v < 0.0f) v = 0.0f; if (v > 1.0f) v = 1.0f;

    //avalia o polinômio analiticamente na coordenada paramétrica exata para obter a altura Y
    Vector3 pontoSuperficie = avaliarBezier(u, v);

    return pontoSuperficie.y;
}

Vector3 Terreno::avaliarBezier(float u, float v)
{
    Vector3 pontoSuperficie(0.0f, 0.0f, 0.0f);
    int n = LADO_CONTROLE - 1; //grau da curva = número de pontos menos um (9)

    //somatório duplo das superfícies tensoriais de Bézier
    for (int i = 0; i < LADO_CONTROLE; i++)
    {
        float ber_u = calcularBernstein(i, n, u);
        for (int j = 0; j < LADO_CONTROLE; j++)
        {
            float ber_v = calcularBernstein(j, n, v);

            //ccumula a influência combinada utilizando os operadores sobrecarregados do Vector3
            pontoSuperficie = pontoSuperficie + (pontosControle[i][j] * (ber_u * ber_v));
        }
    }
    return pontoSuperficie;
}

void Terreno::alterarOndulacao(float delta)
{
    fatorOndulacao += delta;

    //impede inversões de relevo
    if (fatorOndulacao < 0.0f) fatorOndulacao = 0.0f;

    //atualiza a matriz geométrica ativa
    for (int i = 0; i < LADO_CONTROLE; i++)
    {
        for (int j = 0; j < LADO_CONTROLE; j++)
        {
            pontosControle[i][j].y = alturasOriginais[i][j] * fatorOndulacao;
        }
    }
}

void Terreno::render(Camera* camera)
{

    //resolução de subdivisão para garantir suavidade superior a 4x4 por trecho
    const int RESOLUCAO = 24;
    Vector2 malhaTela[RESOLUCAO + 1][RESOLUCAO + 1];

    //cálculo e projeção dos pontos interpolados do espaço 3D para 2D de tela
    for (int i = 0; i <= RESOLUCAO; i++)
    {
        float u = (float)i / RESOLUCAO;
        for (int j = 0; j <= RESOLUCAO; j++)
        {
            float v = (float)j / RESOLUCAO;
            Vector3 p3d = avaliarBezier(u, v);
            malhaTela[i][j] = camera->projetar(p3d);
        }
    }

    //desenho das conexões de linhas que formam a malha poligonal vazada
    CV::color(1); //cinza
    for (int i = 0; i < RESOLUCAO; i++)
    {
        for (int j = 0; j < RESOLUCAO; j++)
        {
            CV::line(malhaTela[i][j], malhaTela[i + 1][j]);
            CV::line(malhaTela[i][j], malhaTela[i][j + 1]);
        }
    }

    CV::color(2); //vermelho
    for (int i = 0; i < LADO_CONTROLE; i++)
    {
        for (int j = 0; j < LADO_CONTROLE; j++)
        {
            Vector2 ptTela = camera->projetar(pontosControle[i][j]);

            //desenha círculos preenchidos nos vértices de controle p melhor visualização
            CV::circleFill(ptTela, 3, 6);
        }
    }
}
