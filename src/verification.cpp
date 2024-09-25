#include <iostream>
#include <string>
#include <vector>
#include "main.cpp"


using namespace std;


class MerkleNode {

    string hash;
    MerkleNode *left;
    MerkleNode *right;

    MerkleNode(string hash, string prevHash){
        this->left = nullptr;
        this->right = nullptr;
        this->hash = hash;
        

    }

    MerkleNode(MerkleNode* left, MerkleNode* right){
        this->left = left;
        this->right = right;
        this->hash = computeHash(left->hash + right->hash);

    }




};




class MerkleTree {

    public: 

        MerkleNode* root;

        MerkleTree(){
            root = nullptr;
        }

        MerkleNode* makeTree(vector>string> &transactions){
            vector<MerkleNode*> nodes;

            for (const string& timestamp : transactions ){
                nodes.push_back(new MerkleNode(computeHash(timestamp)));
            }

            while (nodes.size() > 1){
                vector <MerkleNode*> nextLevel;

                for (int i = 0; i < nodes.size(); i += 2){
                    if (i + 1 < nodes.size()){
                        nextLevel.push_back(new MerkleNode(nodes.at(i), nodes.at(i+1)));

                    } else {
                        nextLevel.push_back(new MerkleNode(nodes.at(i), nodes.at(i)));
                    }
                }
                nodes = nextLevel;
            }
            root = nodes.at(0);
        }

        string rootHash(){
            if (root != nullptr){
                return root->hash;
            } else {
                return "";
            }
        }



    



};