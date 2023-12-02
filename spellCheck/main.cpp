#include<iostream>
#include<SFML/Graphics.hpp>

void eventLoop(sf::Window& w);

int main() {
    sf::Window window(sf::VideoMode(800, 600), "Main");
    while (window.isOpen()) {
        eventLoop(window);
    }
    return 0;
}

void eventLoop(sf::Window& w) {
    sf::Event e;
    while(w.pollEvent(e)) {
        if (e.type == sf::Event::Closed) {
            w.close();
            return;
        }
    }
}