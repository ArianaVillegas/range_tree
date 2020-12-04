#include <iostream>
#include <ctime>
#include <chrono>
#include "tree.h"
using namespace std;

#define X_MAX 5000
#define Y_MAX 5000
#define N_POINTS 100000

bool in_range(int p, int B, int E) {
    return (B <= p && p <= E);
}

void test(RangeTree& rt, int n, vector<Record>& data) {
    int Bx = rand() % X_MAX;
    int Ex = Bx + n;
    int By = rand() % Y_MAX;
    int Ey = By + n;

    chrono::high_resolution_clock::time_point t2, t1 = chrono::high_resolution_clock::now();
    auto q = rt.range_query({{Bx, Ex}, {By, Ey}});
    t2 = chrono::high_resolution_clock::now();

    cout << "\nTamanio de rango de prueba: " << n << endl;
    cout << "Tiempo de consulta: " << chrono::duration_cast<chrono::duration<double>>(t2 - t1).count() << " segundos\n";

    bool correct = true;
    for (auto& p : q) {
        if (!in_range(p.coor[0], Bx, Ex) || !in_range(p.coor[1], By, Ey)) {
            correct = false;
            break;
        }
    }
    if (correct) cout << "Resultado SI esta dentro del rango!!!\n";
    else cout << "X Resultado NO esta dentro del rango X\n";

    int count = 0;
    for (auto& p : data) {
        if (in_range(p.coor[0], Bx, Ex) && in_range(p.coor[1], By, Ey)) count++;
    }
    cout << "Cantidad de puntos en el rango: " << count;
    cout << "\nCantidad de puntos obtenidos: " << q.size() << endl;

}

int main() {
    srand(time(nullptr));
    vector<Record> data;
    for(int i=0; i<N_POINTS; i++){
        int x = rand()%X_MAX;
        int y = rand()%Y_MAX;
        data.push_back({{x,y},""});
    }
    cout << "Insertando..." << endl;
    chrono::high_resolution_clock::time_point t2, t1 = chrono::high_resolution_clock::now();
    RangeTree rt(data,2);
    t2 = chrono::high_resolution_clock::now();
    cout << "Tiempo de insercion: " << chrono::duration_cast<chrono::duration<double>>(t2 - t1).count() << " segundos" << endl;

    test(rt, 10, data);
    test(rt, 100, data);
    test(rt, 1000, data);
    test(rt, 10000, data);
    return 0;
}