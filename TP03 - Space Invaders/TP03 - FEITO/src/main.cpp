#include <GL/glut.h>
#include <stdio.h>
#include "../include/Game.h"

// Constantes da janela
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// Instância global do jogo
Game* game = nullptr;

// Estado das teclas
bool keyState[256];
bool specialKeyState[256];

/**
 * Callback de renderização.
 * Limpa a tela e desenha todos os elementos do jogo.
 */
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    if (game != nullptr) {
        game->draw();
    }
    
    glutSwapBuffers();
}

/**
 * Callback de atualização (game loop).
 * Atualiza a lógica do jogo e processa input.
 */
void update(int value) {
    if (game != nullptr) {
        // Processa input do jogador
        if (specialKeyState[GLUT_KEY_LEFT]) {
            game->movePlayerLeft();
        }
        if (specialKeyState[GLUT_KEY_RIGHT]) {
            game->movePlayerRight();
        }
        if (keyState[' ']) {
            game->playerShoot();
        }
        
        // Atualiza estado do jogo
        game->update();
    }
    
    // Re-agenda a próxima atualização (~60 FPS)
    glutTimerFunc(16, update, 0);
    
    // Força redesenho
    glutPostRedisplay();
}

/**
 * Callback para tecla pressionada (caracteres normais).
 */
void handleKeysDown(unsigned char key, int x, int y) {
    keyState[key] = true;
    
    // ESC para sair
    if (key == 27) {
        exit(0);
    }
}

/**
 * Callback para tecla solta (caracteres normais).
 */
void handleKeysUp(unsigned char key, int x, int y) {
    keyState[key] = false;
}

/**
 * Callback para tecla especial pressionada (setas, F1-F12, etc).
 */
void handleSpecialKeysDown(int key, int x, int y) {
    specialKeyState[key] = true;
}

/**
 * Callback para tecla especial solta.
 */
void handleSpecialKeysUp(int key, int x, int y) {
    specialKeyState[key] = false;
}

/**
 * Callback de redimensionamento da janela.
 * Configura a projeção ortográfica 2D.
 */
void reshape(int w, int h) {
    // Previne divisão por zero
    if (h == 0) h = 1;
    
    // Usa a janela inteira para renderizar
    glViewport(0, 0, w, h);
    
    // Configura a matriz de projeção
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    // Define o sistema de coordenadas 2D
    // Origem (0,0) no canto inferior esquerdo
    gluOrtho2D(0.0, WINDOW_WIDTH, 0.0, WINDOW_HEIGHT);
    
    // Volta para a matriz ModelView
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/**
 * Inicializa GLUT e configurações do OpenGL.
 */
void init(int argc, char** argv) {
    glutInit(&argc, argv);
    
    // Usa Double Buffer (para animação) e RGB
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("TP03 - Space Invaders");
    
    // Cor de fundo (preto)
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    // Inicializa estados das teclas
    for (int i = 0; i < 256; i++) {
        keyState[i] = false;
        specialKeyState[i] = false;
    }
}

/**
 * Função principal.
 */
int main(int argc, char** argv) {
    // Inicializa GLUT
    init(argc, argv);
    
    // Cria instância do jogo
    game = new Game(WINDOW_WIDTH, WINDOW_HEIGHT);
    
    // Registra os callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(16, update, 0);  // Inicia o game loop
    
    // Callbacks de teclado (baseado em estado)
    glutKeyboardFunc(handleKeysDown);
    glutKeyboardUpFunc(handleKeysUp);
    glutSpecialFunc(handleSpecialKeysDown);
    glutSpecialUpFunc(handleSpecialKeysUp);
    
    printf("=== Space Invaders ===\n");
    printf("Controles:\n");
    printf("  Setas <- -> : Mover o canhao\n");
    printf("  Espaco      : Atirar\n");
    printf("  ESC         : Sair\n");
    printf("\nObjetivo: Destrua todos os aliens!\n");
    printf("Cuidado: Nao deixe os aliens chegarem ate voce!\n\n");
    
    // Loop principal do GLUT
    glutMainLoop();
    
    // Limpa memória
    if (game != nullptr) {
        delete game;
    }
    
    return 0;
}
