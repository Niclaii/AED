#include <SFML/Graphics.hpp>
#include <iostream>

struct Nodo {
    int valor;
    Nodo* izquierda;
    Nodo* derecha;
    int altura;

    Nodo(int val) : valor(val), izquierda(nullptr), derecha(nullptr), altura(1) {}
};

int obtenerAltura(Nodo* nodo) {
    if (nodo == nullptr) {
        return 0;
    }
    else {
        return nodo->altura;
    }
}

int obtenerFactorBalance(Nodo* nodo) {
    if (nodo == nullptr) return 0;
    return obtenerAltura(nodo->izquierda) - obtenerAltura(nodo->derecha);
}

void actualizarAltura(Nodo* nodo) {
    if (nodo != nullptr) {
        int alturaIzquierda = obtenerAltura(nodo->izquierda);
        int alturaDerecha = obtenerAltura(nodo->derecha);
        nodo->altura = (alturaIzquierda > alturaDerecha ? alturaIzquierda : alturaDerecha) + 1;
    }
}

Nodo* rotarDerecha(Nodo* y) {
    Nodo* x = y->izquierda;
    Nodo* T2 = x->derecha;

    x->derecha = y;
    y->izquierda = T2;

    actualizarAltura(y);
    actualizarAltura(x);

    return x;
}

Nodo* rotarIzquierda(Nodo* x) {
    Nodo* y = x->derecha;
    Nodo* T2 = y->izquierda;

    y->izquierda = x;
    x->derecha = T2;

    actualizarAltura(x);
    actualizarAltura(y);

    return y;
}

Nodo* balancear(Nodo* nodo) {
    actualizarAltura(nodo);

    int factorBalance = obtenerFactorBalance(nodo);

    if (factorBalance > 1) {
        if (obtenerFactorBalance(nodo->izquierda) < 0)
            nodo->izquierda = rotarIzquierda(nodo->izquierda);
        return rotarDerecha(nodo);
    }

    if (factorBalance < -1) {
        if (obtenerFactorBalance(nodo->derecha) > 0)
            nodo->derecha = rotarDerecha(nodo->derecha);
        return rotarIzquierda(nodo);
    }

    return nodo;
}

Nodo* insertar(Nodo* nodo, int valor) {
    if (nodo == nullptr) {
        return new Nodo(valor);
    }
    if (valor < nodo->valor) {
        nodo->izquierda = insertar(nodo->izquierda, valor);
    }
    else if (valor > nodo->valor) {
        nodo->derecha = insertar(nodo->derecha, valor);
    }
    else {
        return nodo;
    }

    return balancear(nodo);
}

class VisualizadorAVL {
public:
    VisualizadorAVL(Nodo* raiz) : raiz(raiz) {
        if (!fuente.loadFromFile("arial.ttf")) {
            std::cerr << "No se pudo cargar la fuente 'arial.ttf'.\n";
        }
    }

    void dibujarNodo(sf::RenderWindow& ventana, float x, float y, sf::Color color, int valor) {
        sf::CircleShape circulo(20);
        circulo.setFillColor(color);
        circulo.setOutlineColor(sf::Color::White);
        circulo.setOutlineThickness(2);
        circulo.setPosition(x, y);
        ventana.draw(circulo);

        sf::Text texto;
        texto.setFont(fuente);
        texto.setString(std::to_string(valor));
        texto.setCharacterSize(16);
        texto.setFillColor(sf::Color::Black);
        sf::FloatRect rectTexto = texto.getLocalBounds();
        texto.setOrigin(rectTexto.left + rectTexto.width / 2.0f, rectTexto.top + rectTexto.height / 2.0f);
        texto.setPosition(x + 20, y + 20);
        ventana.draw(texto);
    }

    void dibujarArbol(Nodo* nodo, sf::RenderWindow& ventana, float x, float y, float offsetX, float offsetY) {
        if (nodo == nullptr) return;

        if (nodo->izquierda) {
            sf::Vertex linea[] = { sf::Vertex(sf::Vector2f(x + 20, y + 20)),
                                   sf::Vertex(sf::Vector2f(x - offsetX + 20, y + offsetY + 20)) };
            ventana.draw(linea, 2, sf::Lines);
            dibujarArbol(nodo->izquierda, ventana, x - offsetX, y + offsetY, offsetX / 2, offsetY);
        }

        if (nodo->derecha) {
            sf::Vertex linea[] = { sf::Vertex(sf::Vector2f(x + 20, y + 20)),
                                   sf::Vertex(sf::Vector2f(x + offsetX + 20, y + offsetY + 20)) };
            ventana.draw(linea, 2, sf::Lines);
            dibujarArbol(nodo->derecha, ventana, x + offsetX, y + offsetY, offsetX / 2, offsetY);
        }

        dibujarNodo(ventana, x, y, sf::Color::White, nodo->valor);
    }

    void mostrar(sf::RenderWindow& ventana) {
        dibujarArbol(raiz, ventana, 400, 50, 150, 100);
    }

private:
    Nodo* raiz;
    sf::Font fuente;
};

int main() {
    sf::RenderWindow ventana(sf::VideoMode(800, 600), "Visualizaci�n del �rbol AVL");

    Nodo* raiz = nullptr;

    int valores[] = { 10, 5, 15, 3, 7, 13, 18, 1, 4, 6, 8, 11, 19 };
    int numNodos = sizeof(valores) / sizeof(valores[0]);

    for (int i = 0; i < numNodos; ++i) {
        raiz = insertar(raiz, valores[i]);
    }

    VisualizadorAVL visualizador(raiz);

    while (ventana.isOpen()) {
        sf::Event evento;
        while (ventana.pollEvent(evento)) {
            if (evento.type == sf::Event::Closed) {
                ventana.close();
            }
        }

        ventana.clear(sf::Color::Black);
        visualizador.mostrar(ventana);
        ventana.display();
    }

    return 0;
}
