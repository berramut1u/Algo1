#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include <string.h>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <stack>
#include <chrono>
#include <random>
using namespace std;
using namespace std::chrono;


/////////////////// Player ///////////////////
class publisher {
public:
    string name;
    float na_sales;
    float eu_sales;
    float others_sales;
};

/////////////////// Node ///////////////////
class Node {
public:
    publisher key;
    int color; // "Red"=1 or "Black"=0
    Node* parent, * left, * right;

    Node(publisher);
    ~Node();
    int get_color();
    void set_color(int);
};

// define the destructor
Node::~Node() {
}

/////////////////// RB-Tree ///////////////////
class RB_tree {
private:
    Node* root;

    void deleteTree(Node* node);
    void traverse(Node* node, vector<publisher>& aggregated_sales);

public:
    publisher* best_seller[3];
    stack<string> tree_deep_stack;

    Node* get_root();
    Node* RB_insert(Node* root, Node* ptr);
    void insertValue(vector<string>);
    void RB_left_rotate(Node* ptr);
    void RB_right_rotate(Node* ptr);
    void RB_insert_fixup(Node* ptr);
    void preorder();
    void find_best_seller();
    Node* search(string publisher_name);

    RB_tree();
    ~RB_tree();
};

RB_tree::RB_tree() {
    this->root = NULL;
    this->best_seller[0] = NULL;
    this->best_seller[1] = NULL;
    this->best_seller[2] = NULL;
}

RB_tree::~RB_tree() {
    // clean up dynamically allocated memory
    delete best_seller[0];
    delete best_seller[1];
    delete best_seller[2];
    deleteTree(root); // call deleteTree to clean up nodes
}

void RB_tree::deleteTree(Node* node) {
    if (node != nullptr) {
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }
}

void print_best_sellers(int year, publisher* temp_publisher[3]) {
    cout.precision(5);
    cout << "End of the " + to_string(year) + " Year" << endl;
    cout << "Best seller in North America: " + temp_publisher[0]->name + " - " << temp_publisher[0]->na_sales << " million" << endl;
    cout << "Best seller in Europe: " + temp_publisher[1]->name + " - " << temp_publisher[1]->eu_sales << " million" << endl;
    cout << "Best seller rest of the World: " + temp_publisher[2]->name + " - " << temp_publisher[2]->others_sales << " million" << endl;
}

int GLOBAL_YEAR = 1980;
vector<string> unique_publishers;

RB_tree generate_RBT_tree_from_csv(string file_name) {
    RB_tree temp_RBtree;
    ifstream file(file_name);

    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << file_name << endl;
        return temp_RBtree;
    }

    string line;
    getline(file, line); 
    
    vector<vector<string>> allData;
    while (getline(file, line)) {
        stringstream ss(line);
        string cell;
        vector<string> row;
        
        while (getline(ss, cell, ',')) {
            row.push_back(cell);
        }
        
        if (row.size() >= 7) {
            try {
                int year = stoi(row[2]);
                allData.push_back(row);
            } catch (...) {
                continue;
            }
        }
        unique_publishers.push_back(row[3]);
    }
    file.close();

    sort(allData.begin(), allData.end(),
         [](const vector<string>& a, const vector<string>& b) {
             return stoi(a[2]) < stoi(b[2]);
         });

    int currentDecade = 1990;
    // start timing before the insertion loop
    auto start_time = high_resolution_clock::now();
    for (const auto& row : allData) {
        int year = stoi(row[2]);
        
        while (year > currentDecade) {
            if (temp_RBtree.get_root() != nullptr) {
                GLOBAL_YEAR = currentDecade;
                temp_RBtree.find_best_seller();
            }
            currentDecade += 10;
        }
        
    
        temp_RBtree.insertValue(row);
    }

    // end timing after all insertions are complete
    auto end_time = high_resolution_clock::now();
    auto total_time = duration_cast<microseconds>(end_time - start_time);

    if (temp_RBtree.get_root() != nullptr) {
        GLOBAL_YEAR = currentDecade;
        temp_RBtree.find_best_seller();
    }

    cout << "Time taken to insert all data into RBT: " << total_time.count() << " µs" << endl;
    return temp_RBtree;
}

// (for performance comparison)
RB_tree generate_RBT_tree_by_game_name(string file_name) {

    RB_tree temp_RBtree;
    ifstream file(file_name);

    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << file_name << endl;
        return temp_RBtree;
    }

    string line;
    getline(file, line); 
    
    vector<vector<string>> allData;
    vector<string> game_names; //to store game names for search testing

    while (getline(file, line)) {
        stringstream ss(line);
        string cell;
        vector<string> row;
        
        while (getline(ss, cell, ',')) {
            row.push_back(cell);
        }
        
        if (row.size() >= 7) {
            allData.push_back(row);
            game_names.push_back(row[1]); // store game name
        }
    }
    file.close();

    // sort by game name
    sort(allData.begin(), allData.end(),
         [](const vector<string>& a, const vector<string>& b) {
             return a[1] < b[1];  
         });


    auto total_start_time = high_resolution_clock::now();

    // insert all data
    for (const auto& row : allData) {
        vector<string> game_row = row;
        game_row[3] = row[1];  // replace publisher name with game name
        temp_RBtree.insertValue(game_row);
    }

    
    auto total_end_time = high_resolution_clock::now();
    auto total_duration = duration_cast<microseconds>(total_end_time - total_start_time);

    cout << "Total time for game name-based tree generation: " << total_duration.count() << " µs" << endl;
      
    return temp_RBtree;
}


////////////////////////////////////////////
//----------------- MAIN -----------------//
////////////////////////////////////////////
int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " <CSV filename>\n";
        return EXIT_FAILURE;
    }

    string fname = argv[1];

    
    RB_tree RBtree = generate_RBT_tree_from_csv(fname);

    RB_tree game_tree = generate_RBT_tree_by_game_name(fname);

    // random searches with nanosecond precision
    const int num_searches = 50;  
    nanoseconds total_search_time(0);


    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, unique_publishers.size() - 1);

    int successful_searches = 0;
    for (int i = 0; i < num_searches; i++) {
        string random_publisher = unique_publishers[dis(gen)];
        
        auto start = high_resolution_clock::now();
        Node* result = RBtree.search(random_publisher);
        auto end = high_resolution_clock::now();
        
        if (result != nullptr) {
            total_search_time += duration_cast<nanoseconds>(end - start);
            successful_searches++;
        }
    }


    if (successful_searches > 0) {
        double avg_time = static_cast<double>(total_search_time.count()) / successful_searches;
        cout << "Average RBT search time: " << avg_time << " ns" << endl;
    } else {
        cout << "No successful searches performed" << endl;
    }

    RBtree.preorder();

    return EXIT_SUCCESS;
}

/////////////////// Node ///////////////////

Node::Node(publisher key) {
    this->key = key;
    this->color = 1; // "RED";
    this->parent = NULL;
    this->left = NULL;
    this->right = NULL;
}

/////////////////// RB-Tree ///////////////////

Node* RB_tree::get_root() {
    return this->root;
}

Node* RB_tree::RB_insert(Node* root, Node* ptr) {
    if (root == nullptr) {
        return ptr;
    }

    if (ptr->key.name < root->key.name) {
        root->left = RB_insert(root->left, ptr);
        root->left->parent = root;
    } else if (ptr->key.name > root->key.name) {
        root->right = RB_insert(root->right, ptr);
        root->right->parent = root;
    } else {
        // update existing node's sales data
        root->key.na_sales += ptr->key.na_sales;
        root->key.eu_sales += ptr->key.eu_sales;
        root->key.others_sales += ptr->key.others_sales;
        delete ptr;  // clean up the unused node
        return root;  // return existing node
    }
    return root;
}

void RB_tree::insertValue(vector<string> n) {
    publisher temp;
    temp.name = n[3];
    
    temp.na_sales = 0.0f;
    temp.eu_sales = 0.0f;
    temp.others_sales = 0.0f;

    try {
        if (!n[4].empty()) temp.na_sales = stof(n[4]);
        if (!n[5].empty()) temp.eu_sales = stof(n[5]);
        if (!n[6].empty()) temp.others_sales = stof(n[6]);
    } catch (const exception& e) {
    }

    Node* newNode = new Node(temp);
    
    if (root == NULL) {
        root = newNode;
        root->color = 0;  // root must be black
    } else {
        root = RB_insert(root, newNode);
        if (newNode->parent != nullptr) {  // if node was actually inserted
            RB_insert_fixup(newNode);
        }
    }
}


void RB_tree::RB_left_rotate(Node* ptr) {
    Node* y = ptr->right;
    if (y == NULL) {
        return;
    }
    
    ptr->right = y->left;
    if (y->left != NULL) {
        y->left->parent = ptr;
    }
    y->parent = ptr->parent;
    if (ptr->parent == NULL) {
        this->root = y;
    } else if (ptr == ptr->parent->left) {
        ptr->parent->left = y;
    } else {
        ptr->parent->right = y;
    }
    y->left = ptr;
    ptr->parent = y;
}

void RB_tree::RB_right_rotate(Node* ptr) {
    Node* y = ptr->left;
    if (y == NULL) {
        return;
    }

    ptr->left = y->right;
    if (y->right != NULL) {
        y->right->parent = ptr;
    }
    y->parent = ptr->parent;
    if (ptr->parent == NULL) {
        this->root = y;
    } else if (ptr == ptr->parent->right) {
        ptr->parent->right = y;
    } else {
        ptr->parent->left = y;
    }
    y->right = ptr;
    ptr->parent = y;
}

void RB_tree::RB_insert_fixup(Node* ptr) {
    while (ptr != root && ptr->parent != nullptr && ptr->parent->color == 1) {
        Node* parent = ptr->parent;
        Node* grandparent = parent->parent;
        
        if (grandparent == nullptr) break;

        if (parent == grandparent->left) {
            Node* uncle = grandparent->right;
            
            if (uncle != nullptr && uncle->color == 1) {
                parent->color = 0;
                uncle->color = 0;
                grandparent->color = 1;
                ptr = grandparent;
            } else {
                if (ptr == parent->right) {
                    ptr = parent;
                    RB_left_rotate(ptr);
                    parent = ptr->parent;  // update parent after rotation
                }
                parent->color = 0;
                grandparent->color = 1;
                RB_right_rotate(grandparent);
            }
        } else {
            Node* uncle = grandparent->left;
            
            if (uncle != nullptr && uncle->color == 1) {
                parent->color = 0;
                uncle->color = 0;
                grandparent->color = 1;
                ptr = grandparent;
            } else {
                if (ptr == parent->left) {
                    ptr = parent;
                    RB_right_rotate(ptr);
                    parent = ptr->parent;  // update parent after rotation
                }
                parent->color = 0;
                grandparent->color = 1;
                RB_left_rotate(grandparent);
            }
        }
    }
    root->color = 0;  // ensure root is always black
}

void RB_tree::preorder() {
    stack<pair<Node*, int>> nodeStack;  // pair of Node and its depth
    Node* current = root;
    int depth = 0;
    
    while (current != nullptr || !nodeStack.empty()) {
        while (current != nullptr) {
            string depth_indicator(depth, '-');
            
            cout << depth_indicator << current->key.name 
                << "(" << (current->color == 0 ? "Black" : "Red") << ")" << endl;
                 
            nodeStack.push({current, depth});
            current = current->left;
            depth++;  
        }
        
        if (!nodeStack.empty()) {
            auto top = nodeStack.top();
            nodeStack.pop();
            current = top.first;
            depth = top.second;  // restore depth for the right child traversal
            current = current->right;
            depth++;  // increase depth when going right
        }
    }
}



int Node::get_color() {
    return this->color;
}

void Node::set_color(int color) {
    this->color = color;
}

void RB_tree::find_best_seller() {
    if (root == nullptr) {
        for (int i = 0; i < 3; i++) {
            delete best_seller[i];
            best_seller[i] = nullptr;
        }
        return;
    }

    vector<publisher> aggregated_sales;
    traverse(root, aggregated_sales);
    
    if (aggregated_sales.empty()) return;

    // clean up existing best_seller entries
    for (int i = 0; i < 3; i++) {
        if (best_seller[i] != nullptr) {
            delete best_seller[i];
            best_seller[i] = nullptr;
        }
    }

    // find best sellers for each region
    publisher* best_na = &aggregated_sales[0];
    publisher* best_eu = &aggregated_sales[0];
    publisher* best_others = &aggregated_sales[0];

    for (auto& pub : aggregated_sales) {
        if (pub.na_sales > best_na->na_sales) {
            best_na = &pub;
        }
        if (pub.eu_sales > best_eu->eu_sales) {
            best_eu = &pub;
        }
        if (pub.others_sales > best_others->others_sales) {
            best_others = &pub;
        }
    }

    // store best sellers
    best_seller[0] = new publisher{best_na->name, best_na->na_sales, best_na->eu_sales, best_na->others_sales};
    best_seller[1] = new publisher{best_eu->name, best_eu->na_sales, best_eu->eu_sales, best_eu->others_sales};
    best_seller[2] = new publisher{best_others->name, best_others->na_sales, best_others->eu_sales, best_others->others_sales};

    print_best_sellers(GLOBAL_YEAR, best_seller);
}

void RB_tree::traverse(Node* node, vector<publisher>& aggregated_sales) {
    if (node == nullptr) return;
    
    // only include publishers that existed before or during the current year
    aggregated_sales.push_back(node->key);
    
    traverse(node->left, aggregated_sales);
    traverse(node->right, aggregated_sales);
}

Node* RB_tree::search(string publisher_name) {
    Node* current = root;
    while (current != nullptr) {
        if (publisher_name < current->key.name) {
            current = current->left;
        } else if (publisher_name > current->key.name) {
            current = current->right;
        } else {
            return current;  // found the publisher
        }
    }
    return nullptr;  // publisher not found
}