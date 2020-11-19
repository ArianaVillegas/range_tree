#ifndef RTREE_TREE_H
#define RTREE_TREE_H

#include <vector>
#include <string>
#include <tuple>
#include <list>
#include <iostream>
#include <algorithm>

using namespace std;

class RangeTree;

struct Record{
    vector<int> coor;
    string des;
};

struct Node{
    vector<Node*>::iterator itr;
    Record record;
    Node* children[2];
    Node* parent;
    RangeTree* rangeTree;
    int max_leaf;
    int depth;
    
    Node(Record record, int depth, vector<Node*>::iterator itr){
        this->itr = itr;
        this->record = record;
        this->children[0] = 0;
        this->children[1] = 0;
        this->parent = 0;
        this->rangeTree = 0;
        this->max_leaf = 0;
        this->depth = depth;
    }
};

class RangeTree {
private:

    int n;
    int max_dim;
    int cur_dim;
    vector<Node*> records;
    Node* root;

    Node* create_RangeTree(int l, int r, int depth){
        Node *parent, *child1, *child2;
        vector<int> coor(max_dim);
        if(l == r){
            parent = new Node(records[l]->record, depth, records.end());
            child1 = records[l];
            child1->depth = depth+1;
            parent->children[0] = child1;
            parent->max_leaf = child1->record.coor[cur_dim];
            child1->parent = parent;
        } else if(l+1 == r){
            child1 = records[l];
            child2 = records[r];
            coor[cur_dim] = child1->record.coor[cur_dim];
            parent = new Node({coor,""}, depth, records.end());
            parent->children[0] = child1;
            parent->children[1] = child2;
            parent->max_leaf = child2->record.coor[cur_dim];
            child1->parent = child2->parent = parent;
            child1->depth = child2->depth = depth + 1;
        } else {
            int m = (l + r) / 2;
            child1 = create_RangeTree(l, m, depth + 1);
            child2 = create_RangeTree(m + 1, r, depth + 1);
            coor[cur_dim] = child1->max_leaf;
            parent = new Node({coor, ""}, depth, records.end());
            parent->children[0] = child1;
            parent->children[1] = child2;
            parent->max_leaf = child2->max_leaf;
            child1->parent = child2->parent = parent;
            child1->depth = child2->depth = depth + 1;
        }

        // Create range tree
        if(cur_dim+1 != max_dim) {
            vector<Record> subvector;
            for (int i = l; i <= r; i++) subvector.push_back(records[i]->record);
            parent->rangeTree = new RangeTree(subvector, max_dim, cur_dim + 1);
        }

        return parent;
    }

public:
    
    RangeTree(vector<Record> &data, int max_dim, int cur_dim=0, int depth=0){
        sort(data.begin(), data.end(),[&cur_dim](const Record &r1, const Record &r2)->bool{
            return (r1.coor[cur_dim] < r2.coor[cur_dim]);
        });
        this->n = data.size();
        this->records.resize(n);
        for(int i=0; i<n; i++){
            this->records[i] = new Node(data[i], depth, records.begin()+i);
        }
        this->max_dim = max_dim;
        this->cur_dim = cur_dim;
        this->root = create_RangeTree(0, n-1, 0);
    }

    vector<Record> range_query(vector<pair<int,int>> ranges, int cur_dim=0){
        vector<Record> rs;

        RangeTree* rt = this;
        Node* tmp = root;
        for(int i=0; i<ranges.size(); i++){

            while(tmp->children[0]){
                tmp = tmp->children[(ranges[i].first > tmp->record.coor[i])];
            }

            auto itr = tmp->itr;

            if(i < ranges.size()-1) {
                while((itr+1) != rt->records.end() and (*(itr+1))->record.coor[i] <= ranges[i].second) {
                    itr++;
                }

                auto left = tmp;
                auto right = *itr;
                if(left != right) {
                    while (left->depth < right->depth)
                        right = right->parent;
                    while (left->depth > right->depth)
                        left = left->parent;
                    while (left != right) {
                        left = left->parent;
                        right = right->parent;
                    }

                    rt = left->rangeTree;
                    tmp = rt->root;
                    continue;
                }
            }

            itr = tmp->itr;
            while(itr!=rt->records.end() and (*itr)->record.coor[i] <= ranges[i].second) {
                bool valid = 1;
                for (int i = 0; i < ranges.size(); i++) {
                    auto value = (*itr)->record.coor[i];
                    if (ranges[i].first > value or value > ranges[i].second) {
                        valid = 0;
                    }
                }
                if (valid) rs.push_back((*itr)->record);
                itr++;
            }
            break;
        }

        return rs;
    }

    /*void print(){
        for(auto Node:t){
            cout << Node.val_x << ' ' << Node.des << '\n';
        }
    }*/

};


#endif //RTREE_TREE_H
