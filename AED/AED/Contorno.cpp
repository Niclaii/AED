#include <SFML/Graphics.hpp>
#include <iostream>

struct TreeNode {
    int valor;
    TreeNode* left;
    TreeNode* right;

    TreeNode(int val) : valor(val), left(nullptr), right(nullptr) {}
};

TreeNode* insert(TreeNode* nodo, int valor) {
    if (nodo == nullptr) {
        return new TreeNode(valor);
    }
    if (valor < nodo->valor) {
        nodo->left = insert(nodo->left, valor);
    }
    else {
        nodo->right = insert(nodo->right, valor);
    }
    return nodo;
}

class Vector {
public:
    Vector() : capacidad(10), tamanio(0) {
        elementos = new int[capacidad];
    }

    ~Vector() {
        delete[] elementos;
    }

    void add(int value) {
        if (tamanio == capacidad) {
            redimensionar();
        }
        elementos[tamanio] = value;
        tamanio++;
    }

    bool contains(int value) {
        for (int i = 0; i < tamanio; ++i) {
            if (elementos[i] == value) {
                return true;
            }
        }
        return false;
    }

    void clear() {
        tamanio = 0;
    }

private:
    int* elementos;
    int capacidad;
    int tamanio;

    void redimensionar() {
        capacidad *= 2;
        int* nuevosElementos = new int[capacidad];
        for (int i = 0; i < tamanio; ++i) {
            nuevosElementos[i] = elementos[i];
        }
        delete[] elementos;
        elementos = nuevosElementos;
    }
};

class BinaryTreeVisualizer {
public:
    BinaryTreeVisualizer(TreeNode* raiz) : raiz(raiz) {
        if (!font.loadFromFile("arial.ttf")) {
            std::cerr << "No se pudo cargar la fuente 'arial.ttf'. Se usar�n c�rculos sin texto.\n";
            fontCargado = false;
        }
        else {
            fontCargado = true;
        }
    }

    void almacenarBorde(TreeNode* nodo, int nivel, bool esIzquierda, bool esDerecha) {
        if (!nodo) return;
        if (nivel >= tamaniosPorNivel.size()) {
            tamaniosPorNivel.push_back(nodo);
            borde.add(nodo->valor);
        }
        else {
            if (esIzquierda || esDerecha) {
                borde.add(nodo->valor);
            }
        }
        if (nodo->left) {
            almacenarBorde(nodo->left, nivel + 1, esIzquierda, false);
        }
        if (nodo->right) {
            almacenarBorde(nodo->right, nivel + 1, false, esDerecha);
        }
    }

    void almacenarHojas(TreeNode* nodo) {
        if (!nodo) return;
        if (!nodo->left && !nodo->right) {
            borde.add(nodo->valor);
            return;
        }
        almacenarHojas(nodo->left);
        almacenarHojas(nodo->right);
    }

    void dibujarNodo(sf::RenderWindow& window, float x, float y, sf::Color color, int valor) {
        sf::CircleShape circle(20);
        circle.setFillColor(color);
        circle.setOutlineColor(sf::Color::White);
        circle.setOutlineThickness(2);
        circle.setPosition(x, y);
        window.draw(circle);

        if (fontCargado) {
            sf::Text text(std::to_string(valor), font, 16);
            text.setFillColor(sf::Color::Black);
            sf::FloatRect textRect = text.getLocalBounds();
            text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
            text.setPosition(x + circle.getRadius(), y + circle.getRadius());
            window.draw(text);
        }
    }

    void dibujarNodos(TreeNode* nodo, sf::RenderWindow& window, float x, float y, float offsetX, float offsetY) {
        if (!nodo) return;
        if (borde.contains(nodo->valor)) {
            dibujarNodo(window, x, y, sf::Color::Green, nodo->valor);
        }
        else {
            dibujarNodo(window, x, y, sf::Color::White, nodo->valor);
        }
        if (nodo->left) {
            sf::Vertex line[] = { sf::Vertex(sf::Vector2f(x + 20, y + 20), sf::Color::White),
                                 sf::Vertex(sf::Vector2f(x - offsetX + 20, y + offsetY + 20), sf::Color::White) };
            window.draw(line, 2, sf::Lines);
            dibujarNodos(nodo->left, window, x - offsetX, y + offsetY, offsetX / 2, offsetY);
        }
        if (nodo->right) {
            sf::Vertex line[] = { sf::Vertex(sf::Vector2f(x + 20, y + 20), sf::Color::White),
                                 sf::Vertex(sf::Vector2f(x + offsetX + 20, y + offsetY + 20), sf::Color::White) };
            window.draw(line, 2, sf::Lines);
            dibujarNodos(nodo->right, window, x + offsetX, y + offsetY, offsetX / 2, offsetY);
        }
    }

    void display(sf::RenderWindow& window) {
        dibujarNodos(raiz, window, 400, 50, 150, 100);
    }

    void prepararDibujos() {
        borde.clear();
        tamaniosPorNivel.clear();
        almacenarBorde(raiz, 0, true, true);
        almacenarHojas(raiz);
    }

private:
    TreeNode* raiz;
    sf::Font font;
    bool fontCargado;
    Vector borde;
    std::vector<TreeNode*> tamaniosPorNivel;
};

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Visualizaci�n del �rbol Binario");
    TreeNode* raiz = nullptr;
    int valores[] = { 10, 5, 15, 3, 7, 13, 18, 1, 4, 6, 8, 11, 19 };
    int numnodos = sizeof(valores) / sizeof(valores[0]);
    for (int i = 0; i < numnodos; ++i) {
        raiz = insert(raiz, valores[i]);
    }
    BinaryTreeVisualizer visualizer(raiz);
    visualizer.prepararDibujos();
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        window.clear(sf::Color::Black);
        visualizer.display(window);
        window.display();
    }
    return 0;
}