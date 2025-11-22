// Código base (inicial) para implementação do TP03 de CG
// (Space Invaders)
#include <GL/glut.h>
#include <stdio.h> // Para printf

// Tamanho da janela
const int width = 800;
const int height = 600;

// ----- TODO: Mover estado para classes (POO) -----
// Globais provisorias para o estado do jogo.
// O objetivo e substituir isso por classes!

// Estado das teclas
bool keyState[256];
bool specialKeyState[256];

// Exemplo: Posicao do Jogador
float playerX = width / 2;
float playerY = 50;
float playerSize = 30;

// Exemplo: Tiro do Jogador
bool playerShotActive = false;
float playerShotX = 0;
float playerShotY = 0;
// --------------------------------------------------


// Funcao para desenhar o HUD (Vidas e Pontos)
void drawHUD() {
    // ----- TODO: Implementar -----
    // Exemplo de como desenhar texto:
    // (Lembre-se de salvar/restaurar a matriz de projecao)
    // glColor3f(1.0f, 1.0f, 1.0f);
    // glRasterPos2f(10, height - 20); // Posicao
    // const char* text = "Score: 0";
    // for (const char* c = text; *c != '\0'; c++) {
    //     glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    // }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // ----- TODO: Chamar metodos de desenho (draw) das classes -----

    // Exemplo de desenho do jogador (substituir por player.draw())
    glPushMatrix();
    glTranslatef(playerX, playerY, 0);
    glColor3f(0.0f, 1.0f, 0.0f); // Verde
    glBegin(GL_QUADS);
        glVertex2f(-playerSize / 2, -playerSize / 2);
        glVertex2f( playerSize / 2, -playerSize / 2);
        glVertex2f( playerSize / 2,  playerSize / 2);
        glVertex2f(-playerSize / 2,  playerSize / 2);
    glEnd();
    glPopMatrix();
    
    // Exemplo de desenho do tiro (substituir por shot.draw())
    if (playerShotActive) {
        glPushMatrix();
        glTranslatef(playerShotX, playerShotY, 0);
        glColor3f(1.0f, 1.0f, 1.0f); // Branco
        glBegin(GL_QUADS);
            glVertex2f(-3, -8);
            glVertex2f( 3, -8);
            glVertex2f( 3,  8);
            glVertex2f(-3,  8);
        glEnd();
        glPopMatrix();
    }

    // ----- TODO: Desenhar Aliens (swarm.draw()) -----

    // Desenha o HUD por ultimo
    drawHUD();
    
    glutSwapBuffers();
}

void update(int value) {
    // ----- TODO: Chamar metodos de atualizacao (update) das classes -----
    
    // Logica de input (mover para a classe Player)
    float playerSpeed = 8.0f;
    if (specialKeyState[GLUT_KEY_LEFT] && playerX > playerSize / 2) {
        playerX -= playerSpeed;
    }
    if (specialKeyState[GLUT_KEY_RIGHT] && playerX < width - playerSize / 2) {
        playerX += playerSpeed;
    }
    
    // Logica do tiro (mover para a classe Player/Game)
    if (keyState[' '] && !playerShotActive) {
        playerShotActive = true;
        playerShotX = playerX;
        playerShotY = playerY + 20;
        // playSound(gSomDisparo); // (Opcional, se usar SDL_mixer)
    }
    
    // Atualiza posicao do tiro (mover para classe Projectile)
    if (playerShotActive) {
        playerShotY += 15.0f;
        // Desativa o tiro se sair da tela
        if (playerShotY > height) {
            playerShotActive = false;
        }
    }
    
    // ----- TODO: Chamar swarm.update() -----
    // ----- TODO: Chamar game.checkCollisions() -----
    // ----- TODO: Chamar game.checkGameState() -----


    // Re-agendar a proxima atualizacao
    glutTimerFunc(16, update, 0); // ~60 FPS
    
    // Forcar redesenho
    glutPostRedisplay();
}

// Callbacks de Teclado (Estado)
void handleKeysDown(unsigned char key, int x, int y) {
    keyState[key] = true;
}

void handleKeysUp(unsigned char key, int x, int y) {
    keyState[key] = false;
}

void handleSpecialKeysDown(int key, int x, int y) {
    specialKeyState[key] = true;
}

void handleSpecialKeysUp(int key, int x, int y) {
    specialKeyState[key] = false;
}

// Funcao para configurar a projecao 2D
void reshape(int w, int h) {
    // Previne divisao por zero
    if (h == 0) h = 1;
    
    // Usa a janela inteira para renderizar
    glViewport(0, 0, w, h);
    
    // Configura a matriz de projecao
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    // Define o sistema de coordenadas 2D
    // Origem (0,0) no canto inferior esquerdo
    gluOrtho2D(0.0, width, 0.0, height);
    
    // Volta para a matriz ModelView
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Init(int argc, char** argv)
{
    glutInit(&argc, argv);
    // Usa Double Buffer (para animacao) e RGB
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(100, 100); // Posicao
    glutCreateWindow("TP03 - Space Invaders");
    
    // Cor de fundo (preto)
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    // Inicializa estados das teclas
    for(int i=0; i<256; i++) {
        keyState[i] = false;
        specialKeyState[i] = false;
    }
}

int main(int argc, char** argv) {
    Init(argc, argv);
    
    // Registra os callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(16, update, 0); // Inicia o game loop
    
    // Callbacks de teclado (baseado em estado)
    glutKeyboardFunc(handleKeysDown);
    glutKeyboardUpFunc(handleKeysUp);
    glutSpecialFunc(handleSpecialKeysDown);
    glutSpecialUpFunc(handleSpecialKeysUp);
    
    // glutIgnoreKeyRepeat(1); // Opcional: ignora repeticao
    
    // ----- TODO: Chamar game.init() -----
    // initAudio(); // (Opcional, se usar som)
    
    glutMainLoop();
    
    // cleanupAudio(); // (Opcional, se usar som)
    return 0;
}
