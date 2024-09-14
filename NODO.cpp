#include <iostream>

using namespace std;

template <class T>
struct nodo
{
    T valor;
    nodo<T>* next;

    // Constructor del nodo
    nodo(T v, nodo<T>* n = nullptr) // Cambiar 'null' a 'nullptr'
    {
        valor = v;
        next = n;
    }
};

template <class T>
struct LE
{
    nodo<T>* head = nullptr; // Usar 'nullptr' en lugar de 'NULL' para punteros

    // M�todo para agregar un nuevo nodo al principio de la lista
    void add(T v)
    {
        nodo<T>* nuevonodo = new nodo<T>(v); // A�adir tipo de nodo y corregir par�ntesis
        nuevonodo->next = head;
        head = nuevonodo;
    }

    // M�todo para eliminar un nodo con el valor dado
    void del(T v)
    {
        nodo<T>* pos = head;
        nodo<T>* prev = nullptr; // Puntero para mantener el nodo anterior

        while (pos != nullptr)
        {
            if (pos->valor == v)
            {
                if (prev == nullptr) // Si es el primer nodo
                {
                    head = pos->next;
                }
                else // Si no es el primer nodo
                {
                    prev->next = pos->next;
                }
                delete pos; // Eliminar nodo
                return;
            }
            prev = pos;
            pos = pos->next;
        }
    }

    // M�todo para buscar un nodo con el valor dado
    bool find(T v, nodo<T>*& pos)
    {
        pos = head;
        while (pos != nullptr)
        {
            if (pos->valor == v)
            {
                return true; // Se encontr� el valor
            }
            pos = pos->next;
        }
        return false; // No se encontr� el valor
    }

    // M�todo para imprimir todos los nodos de la lista
    void print()
    {
        nodo<T>* actual = head;
        while (actual != nullptr)
        {
            cout << actual->valor << " -> ";
            actual = actual->next;
        }
        cout << "nullptr" << endl; // Indica el final de la lista
    }
};

int main()
{
    LE<int> lista;
    lista.add(5);
    lista.add(10);
    lista.add(15);

    cout << "Lista despu�s de agregar nodos: ";
    lista.print();

    nodo<int>* encontrado = nullptr;
    if (lista.find(10, encontrado))
    {
        cout << "Valor 10 encontrado en la lista." << endl;
    }
    else
    {
        cout << "Valor 10 no encontrado en la lista." << endl;
    }

    lista.del(10);
    cout << "Lista despu�s de eliminar el nodo con valor 10: ";
    lista.print();

    LE<char>lista2;
    lista2.add('a');
    lista2.add('b');

    lista2.print();

    lista2.del('a');

    lista2.print();

    return 0;
}
