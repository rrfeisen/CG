#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "GameObject.h"

/**
 * Classe que representa um projétil (tiro) no jogo.
 * Pode ser do jogador (sobe) ou do alien (desce).
 */
class Projectile : public GameObject {
private:
    float speed;           // Velocidade do projétil
    bool fromPlayer;       // true se é tiro do jogador, false se é do alien
    int screenHeight;      // Altura da tela para verificar limites

public:
    Projectile(float x, float y, bool fromPlayer, int screenHeight)
        : GameObject(x, y, 6, 16), fromPlayer(fromPlayer), screenHeight(screenHeight) {
        speed = fromPlayer ? 15.0f : 8.0f;
    }
    
    /**
     * Atualiza a posição do projétil.
     * Move para cima (jogador) ou para baixo (alien).
     */
    void update() override {
        if (!active) return;
        
        if (fromPlayer) {
            y += speed;  // Move para cima
            if (y > screenHeight) {
                active = false;  // Desativa se sair da tela
            }
        } else {
            y -= speed;  // Move para baixo
            if (y < 0) {
                active = false;  // Desativa se sair da tela
            }
        }
    }
    
    /**
     * Desenha o projétil como um retângulo pequeno.
     */
    void draw() const override {
        if (!active) return;
        
        glPushMatrix();
        glTranslatef(x, y, 0);
        
        if (fromPlayer) {
            glColor3f(1.0f, 1.0f, 0.0f);  // Amarelo para tiro do jogador
        } else {
            glColor3f(1.0f, 0.0f, 0.0f);  // Vermelho para tiro do alien
        }
        
        glBegin(GL_QUADS);
        glVertex2f(-width/2, -height/2);
        glVertex2f(width/2, -height/2);
        glVertex2f(width/2, height/2);
        glVertex2f(-width/2, height/2);
        glEnd();
        
        glPopMatrix();
    }
    
    bool isFromPlayer() const { return fromPlayer; }
};

#endif // PROJECTILE_H
