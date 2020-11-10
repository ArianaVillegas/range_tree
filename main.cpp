#include <iostream>
#include <time.h>
#include "tree.h"

using namespace std;

int main() {

    srand(time(0));
    int c1[13] = {34, 69, 39, 97, 20, 52, 37, 1, 84, 29, 82, 7, 4};
    int c2[13] = {82, 6, 56, 42, 5, 12, 17, 25, 60, 57, 50, 55, 46};
    vector<Record> data;
    for(int i=0; i<2000; i++){
        int a = rand()%100;
        int b = rand()%100;
        data.push_back({{a,b},""});
        cout << a << " " << b << endl;
    }
    cout << "-------------------" << endl;
    RangeTree rt(data,2);
    //t.print();

    auto ans = rt.range_query({{5,5},{55,100}});
    for(auto r : ans){
        for (auto c : r.coor)
            cout << c << ' ';
        cout << r.des << endl;
    }

    return 0;
}
