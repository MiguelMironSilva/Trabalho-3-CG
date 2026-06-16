/*********************************************************************
//  Trabalho 3 da disciplina de Computação Gráfica
//  Autor: Miguel Miron Silva
//         06/2026
// *********************************************************************/

#include <GL/glut.h>
#include <GL/freeglut_ext.h> //callback da wheel do mouse.

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "gl_canvas2d.h"
#include "Camera.h"
#include "Terreno.h"
#include "Veiculo.h"

//largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int screenWidth = 500, screenHeight = 500;

//instanciamento de classes
Camera* camera = NULL;
Terreno* terreno = NULL;
Veiculo* carro = NULL;

int opcao  = 49;//variavel global para selecao do que sera exibido na canvas.
int mouseX, mouseY; //variaveis globais do mouse para poder exibir dentro da render().


void DrawMouseScreenCoords()
{
    char str[100];
    sprintf(str, "Mouse: (%d,%d)", mouseX, mouseY);
    CV::text(10,300, str);
    sprintf(str, "Screen: (%d,%d)", screenWidth, screenHeight);
    CV::text(10,320, str);
}


//funcao chamada continuamente. Deve-se controlar o que desenhar por meio de variaveis globais
//Todos os comandos para desenho na canvas devem ser chamados dentro da render().
//Deve-se manter essa função com poucas linhas de codigo.
void render()
{
   CV::color(1,0,0);

   terreno->render(camera);

   carro->adaptarAoTerreno(terreno);
   carro->render(camera);

   Sleep(10); //nao eh controle de FPS. Somente um limitador de FPS.
}

//funcao chamada toda vez que uma tecla for pressionada.
void keyboard(int key)
{
    //teclas para controle manual da orientação espacial da câmera
    switch (key)
    {
        case 27: //esc para sair do programa
            delete camera;
            exit(0);
            break;

        //rotação vertical e lateral
        case 'w': case 'W':
            camera->alterarAngulos(-2.0f, 0.0f);
            break;
        case 's': case 'S':
            camera->alterarAngulos(2.0f, 0.0f);
            break;
        case 'a': case 'A':
            camera->alterarAngulos(0.0f, -2.0f);
            break;
        case 'd': case 'D':
            camera->alterarAngulos(0.0f, 2.0f);
            break;

        //ajuste do zoom
        case '+':
            camera->alterarDistancia(-1.0f);
            break;
        case '-':
            camera->alterarDistancia(1.0f);
            break;

        //ajuste da planitude/ondulação do terreno
        case '1': //mais plano
            terreno->alterarOndulacao(-0.1f);
            break;
        case '2': //mais ondulado
            terreno->alterarOndulacao(0.1f);
            break;

        //controles do veiculo
        case 200: carro->virarEsquerda(); break; //seta para a Esquerda
        case 201: carro->moverFrente(); break;   //seta para Cima
        case 202: carro->virarDireita(); break;  //seta para a Direita
        case 203: carro->moverTras(); break;     //seta para Baixo

    }
}

//funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key)
{

}

//funcao para tratamento de mouse: cliques, movimentos e arrastos
void mouse(int button, int state, int wheel, int direction, int x, int y)
{
   mouseX = x; //guarda as coordenadas do mouse para exibir dentro da render()
   mouseY = y;

   //printf("\nmouse %d %d %d %d %d %d", button, state, wheel, direction,  x, y);

}

int main(void)
{

   camera = new Camera(&screenWidth, &screenHeight);
   terreno = new Terreno();
   carro = new Veiculo(0.0f, 0.0f);

   CV::init(&screenWidth, &screenHeight, "Trabalho 3 Miguel");
   printf("Utilize as teclas W, A, S e D para mudar de ângulo");
   printf("Utilize as teclas + e - para modular a distancia");
   printf("Utilize as teclas 1 e 2 para mudar a ondulação");
   printf("Utilize as setas para move o carro");
   CV::run();
}
