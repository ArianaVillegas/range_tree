#include <iostream>
#include <time.h>
#include "tree.h"

using namespace std;

int main() {

    srand(time(0));
    int c1[20] = {23, 49, 41, 83, 51, 56, 61, 1, 32, 18, 59, 18, 24, 36, 80, 67, 67, 44, 48, 27};
    int c2[20] = {89, 89, 80, 67, 36, 1, 13, 15, 21, 70, 99, 62, 99, 5, 25, 55, 16, 8, 79, 52};
    vector<Record> data;
    for(int i=0; i<2000000; i++){
        int a = rand()%10000;
        int b = rand()%10000;
        data.push_back({{a,b},""});
        cout << a << " " << b << endl;
    }
    data.push_back({{16,80},""});
    data.push_back({{16,80},""});
    cout << "-------------------" << endl;
    RangeTree rt(data,2);
    //t.print();

    auto ans = rt.range_query({{16,16},{80,80}});
    for(auto r : ans){
        for (auto c : r.coor)
            cout << c << ' ';
        cout << r.des << endl;
    }

    return 0;
}
