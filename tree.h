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
    
    Node(Record record, vector<Node*>::iterator itr){
        this->itr = itr;
        this->record = record;
        this->children[0] = 0;
        this->children[1] = 0;
        this->parent = 0;
        this->rangeTree = 0;
        this->max_leaf = 0;
    }
};

class RangeTree {
private:

    int n;
    int max_dim;
    int cur_dim;
    vector<Node*> records;
    Node* root;

    Node* create_RangeTree(int l, int r){
        if(l == r){
            Node* parent = new Node(records[l]->record, records.end());
            Node* child1 = records[l];
            parent->children[0] = child1;
            parent->max_leaf = child1->record.coor[cur_dim];
            child1->parent = parent;

            if(cur_dim+1 == max_dim) return parent;

            // Create range tree
            vector<Node*> subvector;
            for(int i=l; i<=r; i++) subvector.push_back(records[i]);
            parent->rangeTree = new RangeTree(subvector, max_dim, cur_dim+1);

            return parent;
        }
        if(l+1 == r){
            Node* child1 = records[l];
            Node* child2 = records[r];
            vector<int> coor(max_dim);
            coor[cur_dim] = child1->record.coor[cur_dim];
            Node* parent = new Node({coor,""}, records.end());
            parent->children[0] = child1;
            parent->children[1] = child2;
            parent->max_leaf = child2->record.coor[cur_dim];
            child1->parent = parent;
            child2->parent = parent;

            if(cur_dim+1 == max_dim) return parent;

            // Create range tree
            vector<Node*> subvector;
            for(int i=l; i<=r; i++) subvector.push_back(records[i]);
            parent->rangeTree = new RangeTree(subvector, max_dim, cur_dim+1);

            return parent;
        }
        int m = (l+r)/2;
        Node* child1 = create_RangeTree(l, m);
        Node* child2 = create_RangeTree(m+1, r);
        vector<int> coor(max_dim);
        coor[cur_dim] = child1->max_leaf;
        Node* parent = new Node({coor,""}, records.end());
        parent->children[0] = child1;
        parent->children[1] = child2;
        parent->max_leaf = child2->max_leaf;
        child1->parent = parent;
        child2->parent = parent;

        if(cur_dim+1 == max_dim) return parent;

        // Create range tree
        vector<Node*> subvector;
        for(int i=l; i<=r; i++) subvector.push_back(records[i]);
        parent->rangeTree = new RangeTree(subvector, max_dim, cur_dim+1);

        return parent;
    }

public:
    
    RangeTree(vector<Record> data, int max_dim, int cur_dim=0){
        sort(data.begin(), data.end(),[&cur_dim](const Record &r1, const Record &r2)->bool{
            return (r1.coor[cur_dim] < r2.coor[cur_dim]);
        });
        this->n = data.size();
        this->records.resize(n);
        for(int i=0; i<n; i++){
            this->records[i] = new Node(data[i], records.begin()+i);
        }
        this->max_dim = max_dim;
        this->cur_dim = cur_dim;
        this->root = create_RangeTree(0, n-1);
    }

    RangeTree(vector<Node*> data, int max_dim, int cur_dim=0){
        sort(data.begin(), data.end(),[&cur_dim](const Node* r1, const Node* r2)->bool{
            return (r1->record.coor[cur_dim] < r2->record.coor[cur_dim]);
        });
        this->n = data.size();
        this->records.resize(n);
        for(int i=0; i<n; i++){
            this->records[i] = new Node(data[i]->record, records.begin()+i);
        }
        this->max_dim = max_dim;
        this->cur_dim = cur_dim;
        this->root = create_RangeTree(0, n-1);
    }

    vector<Record> range_query(vector<pair<int,int>> ranges, int cur_dim=0){
        vector<Record> rs;

        RangeTree* rt = this;
        Node* tmp = root;
        for(int i=0; i<ranges.size(); i++){

            while(tmp->children[0]){
                /*int c = tmp->record.coor[i];
                int r_left = ranges[i].first;
                if (r_left <= c)
                    tmp = tmp->children[0];
                else
                    tmp = tmp->children[1];*/
                tmp = tmp->children[(ranges[i].first>tmp->record.coor[i])];
            }

            auto itr = tmp->itr;

            if(i < ranges.size()-1) {
                while((itr+1) != rt->records.end() and (*(itr+1))->record.coor[i] <= ranges[i].second) {
                    itr++;
                }

                auto left = tmp;
                auto right = *itr;
                while(left != right){
                    left = left->parent;
                    right = right->parent;
                }
                rt = left->rangeTree;
                tmp = rt->root;
            } else {
                while(itr!=rt->records.end() and (*itr)->record.coor[i] <= ranges[i].second) {
                    //TODO validar records
                    rs.push_back((*itr)->record);
                    itr++;
                }
            }
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
