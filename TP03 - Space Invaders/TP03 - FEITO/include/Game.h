#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "AlienSwarm.h"
#include <string>

/**
 * Enumeração para os estados do jogo.
 */
enum GameState {
    PLAYING,
    GAME_OVER,
    WIN
};

/**
 * Classe principal que gerencia o estado do jogo.
 * Controla pontuação, colisões e transições de estado.
 */
class Game {
private:
    Player* player;
    AlienSwarm* swarm;
    int score;
    GameState state;
    int screenWidth;
    int screenHeight;
    
public:
    Game(int width, int height)
        : score(0), state(PLAYING), screenWidth(width), screenHeight(height) {
        player = new Player(width, height);
        swarm = new AlienSwarm(width, height);
    }
    
    ~Game() {
        delete player;
        delete swarm;
    }
    
    /**
     * Atualiza o estado do jogo.
     */
    void update() {
        if (state != PLAYING) return;
        
        player->update();
        swarm->update();
        checkCollisions();
        checkGameState();
    }
    
    /**
     * Desenha todos os elementos do jogo.
     */
    void draw() const {
        player->draw();
        swarm->draw();
        drawHUD();
        
        if (state == GAME_OVER) {
            drawGameOver();
        } else if (state == WIN) {
            drawWin();
        }
    }
    
    /**
     * Verifica colisões entre projéteis e entidades.
     */
    void checkCollisions() {
        // Colisão: Tiro do jogador vs Aliens
        Projectile* playerShot = player->getShot();
        if (playerShot != nullptr && playerShot->isActive()) {
            const std::vector<Alien*>& aliens = swarm->getAliens();
            
            for (Alien* alien : aliens) {
                if (!alien->isActive()) continue;
                
                // Cria GameObject temporário para o alien na posição do swarm
                float alienWorldX = swarm->getSwarmX() + alien->getX();
                float alienWorldY = swarm->getSwarmY() + alien->getY();
                
                // Verifica colisão usando bounding boxes
                float shotMinX, shotMaxX, shotMinY, shotMaxY;
                float alienMinX, alienMaxX, alienMinY, alienMaxY;
                
                playerShot->getBounds(shotMinX, shotMaxX, shotMinY, shotMaxY);
                
                alienMinX = alienWorldX - alien->getWidth() / 2;
                alienMaxX = alienWorldX + alien->getWidth() / 2;
                alienMinY = alienWorldY - alien->getHeight() / 2;
                alienMaxY = alienWorldY + alien->getHeight() / 2;
                
                if (!(shotMaxX < alienMinX || shotMinX > alienMaxX ||
                      shotMaxY < alienMinY || shotMinY > alienMaxY)) {
                    // Colisão detectada!
                    swarm->removeAlien(alien);
                    player->destroyShot();
                    score += alien->getPoints();
                    break;
                }
            }
        }
        
        // Colisão: Tiro dos aliens vs Jogador
        const std::vector<Projectile*>& alienShots = swarm->getShots();
        for (Projectile* shot : alienShots) {
            if (!shot->isActive()) continue;
            
            if (shot->checkCollision(*player)) {
                player->loseLife();
                player->reset();
                swarm->destroyShot(shot);
                
                if (player->getLives() <= 0) {
                    state = GAME_OVER;
                }
                break;
            }
        }
    }
    
    /**
     * Verifica condições de vitória e derrota.
     */
    void checkGameState() {
        if (state != PLAYING) return;
        
        // Vitória: todos os aliens destruídos
        if (swarm->allDestroyed()) {
            state = WIN;
        }
        
        // Derrota: aliens alcançaram o jogador
        if (swarm->reachedPlayer(player->getY())) {
            state = GAME_OVER;
        }
        
        // Derrota: sem vidas
        if (player->getLives() <= 0) {
            state = GAME_OVER;
        }
    }
    
    /**
     * Desenha o HUD (pontuação e vidas).
     */
    void drawHUD() const {
        glColor3f(1.0f, 1.0f, 1.0f);
        
        // Pontuação
        glRasterPos2f(10, screenHeight - 20);
        std::string scoreText = "Score: " + std::to_string(score);
        for (char c : scoreText) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
        }
        
        // Vidas
        glRasterPos2f(screenWidth - 150, screenHeight - 20);
        std::string livesText = "Lives: " + std::to_string(player->getLives());
        for (char c : livesText) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
        }
    }
    
    /**
     * Desenha mensagem de Game Over.
     */
    void drawGameOver() const {
        glColor3f(1.0f, 0.0f, 0.0f);
        glRasterPos2f(screenWidth/2 - 80, screenHeight/2);
        std::string text = "GAME OVER";
        for (char c : text) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
        }
        
        glRasterPos2f(screenWidth/2 - 120, screenHeight/2 - 40);
        std::string scoreText = "Final Score: " + std::to_string(score);
        for (char c : scoreText) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
        }
    }
    
    /**
     * Desenha mensagem de vitória.
     */
    void drawWin() const {
        glColor3f(0.0f, 1.0f, 0.0f);
        glRasterPos2f(screenWidth/2 - 80, screenHeight/2);
        std::string text = "YOU WIN!";
        for (char c : text) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
        }
        
        glRasterPos2f(screenWidth/2 - 120, screenHeight/2 - 40);
        std::string scoreText = "Final Score: " + std::to_string(score);
        for (char c : scoreText) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
        }
    }
    
    // Controles do jogador
    void movePlayerLeft() { if (state == PLAYING) player->moveLeft(); }
    void movePlayerRight() { if (state == PLAYING) player->moveRight(); }
    bool playerShoot() { return (state == PLAYING) ? player->shoot() : false; }
    
    GameState getState() const { return state; }
    int getScore() const { return score; }
};

#endif // GAME_H
