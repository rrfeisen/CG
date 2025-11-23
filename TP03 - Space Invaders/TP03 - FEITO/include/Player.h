#ifndef PLAYER_H
#define PLAYER_H

#include "GameObject.h"
#include "Projectile.h"

/**
 * Classe que representa o jogador (canhão controlável).
 */
class Player : public GameObject {
private:
    float speed;              // Velocidade de movimento
    int screenWidth;          // Largura da tela para limites
    int screenHeight;         // Altura da tela
    int lives;                // Vidas restantes
    Projectile* shot;         // Ponteiro para o tiro atual
    
public:
    Player(int screenWidth, int screenHeight)
        : GameObject(screenWidth/2, 50, 30, 30),
          speed(8.0f),
          screenWidth(screenWidth),
          screenHeight(screenHeight),
          lives(3),
          shot(nullptr) {}
    
    ~Player() {
        if (shot != nullptr) {
            delete shot;
        }
    }
    
    /**
     * Move o jogador para a esquerda.
     */
    void moveLeft() {
        if (x > width/2) {
            x -= speed;
        }
    }
    
    /**
     * Move o jogador para a direita.
     */
    void moveRight() {
        if (x < screenWidth - width/2) {
            x += speed;
        }
    }
    
    /**
     * Dispara um projétil se não houver tiro ativo.
     * Retorna true se disparou com sucesso.
     */
    bool shoot() {
        if (shot == nullptr || !shot->isActive()) {
            if (shot != nullptr) {
                delete shot;
            }
            shot = new Projectile(x, y + 20, true, screenHeight);
            return true;
        }
        return false;
    }
    
    /**
     * Atualiza o tiro do jogador se existir.
     */
    void update() override {
        if (shot != nullptr && shot->isActive()) {
            shot->update();
        }
    }
    
    /**
     * Desenha o jogador como um triângulo verde.
     */
    void draw() const override {
        if (!active) return;
        
        glPushMatrix();
        glTranslatef(x, y, 0);
        glColor3f(0.0f, 1.0f, 0.0f);  // Verde
        
        // Desenha como triângulo apontando para cima
        glBegin(GL_TRIANGLES);
        glVertex2f(0, height/2);           // Topo
        glVertex2f(-width/2, -height/2);   // Base esquerda
        glVertex2f(width/2, -height/2);    // Base direita
        glEnd();
        
        glPopMatrix();
        
        // Desenha o tiro se existir
        if (shot != nullptr) {
            shot->draw();
        }
    }
    
    /**
     * Remove uma vida do jogador.
     */
    void loseLife() {
        lives--;
        // Reposiciona no centro após perder vida
        x = screenWidth / 2;
    }
    
    /**
     * Reseta a posição do jogador para o centro.
     */
    void reset() {
        x = screenWidth / 2;
        y = 50;
        if (shot != nullptr) {
            delete shot;
            shot = nullptr;
        }
    }
    
    // Getters
    int getLives() const { return lives; }
    Projectile* getShot() const { return shot; }
    
    /**
     * Destrói o tiro atual (usado após colisão).
     */
    void destroyShot() {
        if (shot != nullptr) {
            shot->setActive(false);
        }
    }
};

#endif // PLAYER_H
