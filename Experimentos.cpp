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

vector<Record> range_query_lineal (vector<Record>& data, vector<pair<int,int>> ranges) {
    vector<Record> res;
    for (auto& r : data) {
        if (in_range(r.coor[0], ranges[0].first, ranges[0].second) && in_range(r.coor[1], ranges[1].first, ranges[1].second))
            res.push_back(r);
    }
    return res;
}

void test(RangeTree& rt, int n, vector<Record>& data) {
    double t_range = 0, t_lineal = 0;
    int i = 0;
    bool verify = true;
    cout << "Haciendo " << n << " consultas...\n";
    while (i++ < n) {
        int Bx = rand() % X_MAX;
        int Ex = Bx + (rand() % (X_MAX/2));
        int By = rand() % Y_MAX;
        int Ey = By + (rand() % (Y_MAX/2));

        chrono::high_resolution_clock::time_point t2, t1 = chrono::high_resolution_clock::now();
        auto q = rt.range_query({{Bx, Ex}, {By, Ey}});
        t2 = chrono::high_resolution_clock::now();
        t_range += chrono::duration_cast<chrono::duration<double>>(t2 - t1).count();

        t1 = chrono::high_resolution_clock::now();
        auto tmp = range_query_lineal(data, {{Bx, Ex}, {By, Ey}});
        t2 = chrono::high_resolution_clock::now();
        t_lineal += chrono::duration_cast<chrono::duration<double>>(t2 - t1).count();

        bool correct = true;
        for (auto &p : q) {
            if (!in_range(p.coor[0], Bx, Ex) || !in_range(p.coor[1], By, Ey)) {
                correct = false;
                break;
            }
        }

        verify = verify && correct;

        if (!correct || tmp.size() != q.size())
            cout << "Consulta " << i+1 << " no es correcta\n";
    }
    cout << "Tiempo promedio busqueda por rango: " << t_range/n << " segundos" << endl;
    cout << "Tiempo promedio busqueda lineal: " << t_lineal/n << " segundos" << endl;
    if (verify) cout << "Todas las consultas son correctas!!!\n\n";
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