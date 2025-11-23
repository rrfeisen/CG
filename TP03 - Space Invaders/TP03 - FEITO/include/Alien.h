#ifndef ALIEN_H
#define ALIEN_H

#include "GameObject.h"

/**
 * Classe que representa um alienígena individual.
 */
class Alien : public GameObject {
private:
    int type;  // Tipo do alien (para diferentes pontuações/visuais)

public:
    Alien(float x, float y, int type = 0)
        : GameObject(x, y, 25, 20), type(type) {}
    
    /**
     * Atualização vazia - o movimento é controlado pelo AlienSwarm.
     */
    void update() override {
        // Movimento é controlado pelo enxame
    }
    
    /**
     * Desenha o alienígena como um retângulo colorido.
     */
    void draw() const override {
        if (!active) return;
        
        glPushMatrix();
        glTranslatef(x, y, 0);
        
        // Cores diferentes baseadas no tipo
        switch(type % 3) {
            case 0: glColor3f(1.0f, 0.0f, 1.0f); break;  // Magenta
            case 1: glColor3f(0.0f, 1.0f, 1.0f); break;  // Ciano
            case 2: glColor3f(1.0f, 0.5f, 0.0f); break;  // Laranja
        }
        
        // Corpo do alien
        glBegin(GL_QUADS);
        glVertex2f(-width/2, -height/2);
        glVertex2f(width/2, -height/2);
        glVertex2f(width/2, height/2);
        glVertex2f(-width/2, height/2);
        glEnd();
        
        // Olhos
        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_QUADS);
        // Olho esquerdo
        glVertex2f(-width/4 - 3, height/4 - 3);
        glVertex2f(-width/4 + 3, height/4 - 3);
        glVertex2f(-width/4 + 3, height/4 + 3);
        glVertex2f(-width/4 - 3, height/4 + 3);
        // Olho direito
        glVertex2f(width/4 - 3, height/4 - 3);
        glVertex2f(width/4 + 3, height/4 - 3);
        glVertex2f(width/4 + 3, height/4 + 3);
        glVertex2f(width/4 - 3, height/4 + 3);
        glEnd();
        
        glPopMatrix();
    }
    
    int getType() const { return type; }
    
    /**
     * Retorna pontos baseados no tipo do alien.
     */
    int getPoints() const {
        return (type % 3 + 1) * 10;
    }
};

#endif // ALIEN_H
