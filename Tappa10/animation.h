#pragma once
#include <SFML/Graphics.hpp>

class Animation {
public:
    Animation(sf::Texture* texture, int cols, int rows, int frameWidth, int frameHeight, float switchTime)
        : m_texture(texture), m_cols(cols), m_rows(rows), m_frameWidth(frameWidth), m_frameHeight(frameHeight), m_switchTime(switchTime) {
        m_totalTime = 0.0f;
        m_currentFrame = 0;
        updateUV();
    }

    void update(float deltaTime) {
        m_totalTime += deltaTime;
        if (m_totalTime >= m_switchTime) {
            m_totalTime -= m_switchTime;
            m_currentFrame = (m_currentFrame + 1) % (m_cols * m_rows);
            updateUV();
        }
    }

    void applyToSprite(sf::Sprite& sprite) const {
        sprite.setTexture(*m_texture);
        sprite.setTextureRect(m_uvRect);
    }

    int getFrameWidth() const { return m_frameWidth; }
    int getFrameHeight() const { return m_frameHeight; }
    int getTotalFrames() const { return m_cols * m_rows; }

    void setTexture(sf::Texture* texture) { m_texture = texture; }


private:
    void updateUV() {
        int row = m_currentFrame / m_cols;
        int col = m_currentFrame % m_cols;
        m_uvRect.position.x = col * m_frameWidth;
        m_uvRect.position.y = row * m_frameHeight;
        m_uvRect.size.x = m_frameWidth;
        m_uvRect.size.y = m_frameHeight;
    }

    sf::Texture* m_texture;
    sf::IntRect m_uvRect;
    int m_cols, m_rows;
    float m_switchTime;
    float m_totalTime;
    int m_currentFrame;
    int m_frameWidth, m_frameHeight;
};