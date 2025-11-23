#ifndef ALIENSWARM_H
#define ALIENSWARM_H

#include "Alien.h"
#include "Projectile.h"
#include <vector>
#include <cstdlib>
#include <ctime>

/**
 * Classe que gerencia o enxame de alienígenas.
 * Controla movimento em bloco, descida e tiros.
 */
class AlienSwarm {
private:
    std::vector<Alien*> aliens;           // Vetor de alienígenas
    std::vector<Projectile*> shots;       // Vetor de tiros dos aliens
    float swarmX, swarmY;                 // Posição do enxame
    float speedX;                         // Velocidade horizontal
    float speedY;                         // Velocidade de descida
    int direction;                        // Direção: 1 = direita, -1 = esquerda
    int rows, cols;                       // Grade de aliens
    int screenWidth, screenHeight;        // Dimensões da tela
    float shootTimer;                     // Timer para controlar tiros
    float shootInterval;                  // Intervalo entre tiros
    
public:
    AlienSwarm(int screenWidth, int screenHeight, int rows = 5, int cols = 10)
        : swarmX(100), swarmY(450), speedX(2.0f), speedY(20.0f),
          direction(1), rows(rows), cols(cols),
          screenWidth(screenWidth), screenHeight(screenHeight),
          shootTimer(0), shootInterval(60.0f) {
        
        srand(time(nullptr));
        createSwarm();
    }
    
    ~AlienSwarm() {
        for (Alien* alien : aliens) {
            delete alien;
        }
        for (Projectile* shot : shots) {
            delete shot;
        }
    }
    
    /**
     * Cria a grade inicial de alienígenas.
     */
    void createSwarm() {
        float spacingX = 40;
        float spacingY = 35;
        
        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < cols; col++) {
                float x = col * spacingX;
                float y = row * spacingY;
                aliens.push_back(new Alien(x, y, row));
            }
        }
    }
    
    /**
     * Atualiza o movimento do enxame e gerencia tiros.
     */
    void update() {
        // Move o enxame horizontalmente
        swarmX += speedX * direction;
        
        // Verifica se precisa descer e inverter direção
        float leftMost = getLeftMostAlienX();
        float rightMost = getRightMostAlienX();
        
        if ((direction > 0 && rightMost > screenWidth - 50) ||
            (direction < 0 && leftMost < 50)) {
            direction *= -1;
            swarmY -= speedY;  // Desce
        }
        
        // Atualiza tiros existentes
        for (size_t i = 0; i < shots.size(); i++) {
            if (shots[i]->isActive()) {
                shots[i]->update();
            } else {
                delete shots[i];
                shots.erase(shots.begin() + i);
                i--;
            }
        }
        
        // Sistema de tiro dos aliens
        shootTimer++;
        if (shootTimer >= shootInterval) {
            shootTimer = 0;
            shootFromRandomAlien();
        }
    }
    
    /**
     * Desenha todos os alienígenas e seus tiros.
     */
    void draw() const {
        for (const Alien* alien : aliens) {
            if (alien->isActive()) {
                glPushMatrix();
                glTranslatef(swarmX, swarmY, 0);
                alien->draw();
                glPopMatrix();
            }
        }
        
        for (const Projectile* shot : shots) {
            shot->draw();
        }
    }
    
    /**
     * Faz um alien aleatório da fileira inferior atirar.
     */
    void shootFromRandomAlien() {
        std::vector<Alien*> bottomAliens;
        
        // Encontra aliens da fileira inferior que estão ativos
        for (int col = 0; col < cols; col++) {
            for (int row = 0; row < rows; row++) {
                int idx = row * cols + col;
                if (idx < aliens.size() && aliens[idx]->isActive()) {
                    // Verifica se não há alien abaixo deste
                    bool isBottom = true;
                    for (int r = row - 1; r >= 0; r--) {
                        int checkIdx = r * cols + col;
                        if (checkIdx < aliens.size() && aliens[checkIdx]->isActive()) {
                            isBottom = false;
                            break;
                        }
                    }
                    if (isBottom) {
                        bottomAliens.push_back(aliens[idx]);
                        break;
                    }
                }
            }
        }
        
        if (!bottomAliens.empty()) {
            int idx = rand() % bottomAliens.size();
            Alien* shooter = bottomAliens[idx];
            float shotX = swarmX + shooter->getX();
            float shotY = swarmY + shooter->getY();
            shots.push_back(new Projectile(shotX, shotY, false, screenHeight));
        }
    }
    
    /**
     * Remove um alien específico (após colisão).
     */
    bool removeAlien(Alien* alien) {
        alien->setActive(false);
        return true;
    }
    
    /**
     * Verifica se todos os aliens foram destruídos.
     */
    bool allDestroyed() const {
        for (const Alien* alien : aliens) {
            if (alien->isActive()) {
                return false;
            }
        }
        return true;
    }
    
    /**
     * Verifica se algum alien alcançou a altura do jogador.
     */
    bool reachedPlayer(float playerY) const {
        float lowestY = getLowestAlienY();
        return lowestY <= playerY + 30;
    }
    
    // Getters para posições extremas
    float getLeftMostAlienX() const {
        float minX = screenWidth;
        for (const Alien* alien : aliens) {
            if (alien->isActive()) {
                float alienX = swarmX + alien->getX();
                if (alienX < minX) minX = alienX;
            }
        }
        return minX;
    }
    
    float getRightMostAlienX() const {
        float maxX = 0;
        for (const Alien* alien : aliens) {
            if (alien->isActive()) {
                float alienX = swarmX + alien->getX();
                if (alienX > maxX) maxX = alienX;
            }
        }
        return maxX;
    }
    
    float getLowestAlienY() const {
        float minY = screenHeight;
        for (const Alien* alien : aliens) {
            if (alien->isActive()) {
                float alienY = swarmY + alien->getY();
                if (alienY < minY) minY = alienY;
            }
        }
        return minY;
    }
    
    const std::vector<Alien*>& getAliens() const { return aliens; }
    const std::vector<Projectile*>& getShots() const { return shots; }
    float getSwarmX() const { return swarmX; }
    float getSwarmY() const { return swarmY; }
    
    void destroyShot(Projectile* shot) {
        shot->setActive(false);
    }
};

#endif // ALIENSWARM_H
