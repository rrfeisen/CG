#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <GL/glut.h>

/**
 * Classe base abstrata para todos os objetos do jogo.
 * Define interface comum para atualização e renderização.
 */
class GameObject {
protected:
    float x, y;           // Posição do objeto
    float width, height;  // Dimensões do objeto
    bool active;          // Se o objeto está ativo no jogo

public:
    GameObject(float x = 0, float y = 0, float w = 10, float h = 10)
        : x(x), y(y), width(w), height(h), active(true) {}
    
    virtual ~GameObject() {}
    
    // Métodos virtuais puros (devem ser implementados pelas classes filhas)
    virtual void update() = 0;
    virtual void draw() const = 0;
    
    // Getters
    float getX() const { return x; }
    float getY() const { return y; }
    float getWidth() const { return width; }
    float getHeight() const { return height; }
    bool isActive() const { return active; }
    
    // Setters
    void setActive(bool a) { active = a; }
    void setPosition(float newX, float newY) { x = newX; y = newY; }
    
    /**
     * Retorna os limites da bounding box para detecção de colisão AABB.
     * Assume que a âncora do objeto está no centro.
     */
    void getBounds(float& minX, float& maxX, float& minY, float& maxY) const {
        minX = x - width / 2;
        maxX = x + width / 2;
        minY = y - height / 2;
        maxY = y + height / 2;
    }
    
    /**
     * Verifica colisão AABB com outro GameObject.
     * Retorna true se houver colisão.
     */
    bool checkCollision(const GameObject& other) const {
        float minX1, maxX1, minY1, maxY1;
        float minX2, maxX2, minY2, maxY2;
        
        getBounds(minX1, maxX1, minY1, maxY1);
        other.getBounds(minX2, maxX2, minY2, maxY2);
        
        // Verifica se NÃO há colisão (retorna o inverso)
        if (maxX1 < minX2 || minX1 > maxX2 || maxY1 < minY2 || minY1 > maxY2) {
            return false;
        }
        return true;
    }
};

#endif // GAMEOBJECT_H
