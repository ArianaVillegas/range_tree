#include <iostream>
#include <time.h>
#include "tree.h"

using namespace std;

int main() {

    srand(time(0));
    vector<Record> data;
    for(int i=0; i<20; i++){
        data.push_back({rand()%100,rand()%100,""});
    }
    data.push_back({2,2,""});
    data.push_back({13,4,""});
    RangeTree rt(data,2);
    //t.print();

    auto ans = rt.range_query({{5,37},{2,80}});
    for(auto node : ans){
        cout << node.coor[0] << ' ' << node.coor[1] << ' ' << node.des << '\n';
    }

    return 0;
}
