#include <SFML/Graphics.hpp>
#include <cmath>
#include <algorithm>
#include <vector>

const unsigned int width = 800;
const unsigned int height = 800;

void draw_line(sf::Vector2f p1, sf::Vector2f p2, sf::RenderTarget& target) {
    sf::VertexArray line(sf::PrimitiveType::Points);

    float dx = p2.x - p1.x;
    float dy = p2.y - p1.y;
    int steps = static_cast<int>(std::max(std::abs(dx), std::abs(dy)));
    if (steps == 0) steps = 1;

    float xStep = dx / steps;
    float yStep = dy / steps;

    for (int i = 0; i <= steps; i++) {
        line.append(sf::Vertex{{std::round(p1.x), std::round(p1.y)}, sf::Color::Green});
        p1.x += xStep;
        p1.y += yStep;
    }

    target.draw(line);
}

// Converts from -1..1 coordinates to screen pixels
sf::Vector2f screen(sf::Vector2f p) {
    return {
        (p.x + 1) / 2 * width,
        (1 - (p.y + 1) / 2) * height
    };
}

sf::Vector2f project(sf::Vector3f p) {
    return {p.x / p.z, p.y / p.z};
}

sf::Vector3f rotate_zx(sf::Vector3f p, float angle) {
    const float c = std::cos(angle);
    const float s = std::sin(angle);
    return {
        p.x * c - p.z * s,
        p.y,
        p.x * s + p.z * c
    };
}

int main() {
    sf::RenderWindow window(sf::VideoMode({width, height}), "My Game");
    window.setFramerateLimit(60);
    float dt = 1.0f / 60, dz = 1;

    std::vector<sf::Vector3f> points = {
        { 0.25f,  0.25f,  0.25f},   // 0
        {-0.25f,  0.25f,  0.25f},   // 1
        { 0.25f, -0.25f,  0.25f},   // 2
        {-0.25f, -0.25f,  0.25f},   // 3
        { 0.25f,  0.25f, -0.25f},   // 4
        {-0.25f,  0.25f, -0.25f},   // 5
        { 0.25f, -0.25f, -0.25f},   // 6
        {-0.25f, -0.25f, -0.25f}    // 7
    };

    std::vector<std::pair<int, int>> edges = {
        {0, 1}, {1, 3}, {3, 2}, {2, 0},   // front face
        {4, 5}, {5, 7}, {7, 6}, {6, 4},   // back face
        {0, 4}, {1, 5}, {2, 6}, {3, 7}    // connecting edges
    };

    float angle = 0;

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        angle += 0.5f * 3.14159f * dt;

        // rotate, move, project and convert every point to screen pixels once
        std::vector<sf::Vector2f> projected;
        for (const sf::Vector3f& item : points) {
            sf::Vector3f moved = rotate_zx(item, angle);
            moved.z += dz;
            projected.push_back(screen(project(moved)));
        }

        window.clear(sf::Color::Black);

        for (auto& [a, b] : edges) {
            draw_line(projected[a], projected[b], window);
        }

        window.display();
    }

    return 0;
}