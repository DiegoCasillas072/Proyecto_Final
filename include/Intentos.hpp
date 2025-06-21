#pragma once

class Intentos {
    int valor;
    int maximo;
public:
    Intentos(int maximo) : valor(maximo), maximo(maximo) {}
    void reiniciar() { valor = maximo; }
    void perder() { if (valor > 0) valor--; }
    int get() const { return valor; }
    bool agotado() const { return valor <= 0; }
};