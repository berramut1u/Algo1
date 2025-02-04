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

Node::~Node() {
    // tree itself will handle the memory cleanup
}

/////////////////// BST-Tree ///////////////////
class BST_tree {
private:
    Node* root;

    void deleteTree(Node* node) {
        if (node != nullptr) {
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
        }
    }
public:
    publisher* best_seller[3]; // To store the best sellers for each region
    stack<string> tree_deep_stack;

    Node* get_root();
    Node* BST_insert(Node* root, Node* ptr);
    void insertValue(vector<string>);
    void find_best_seller(); 
    void traverse(Node* node, vector<publisher>& aggregated_sales); //helper function for find_best_seller
    Node* search(const string& publisher_name); //helper function for search


    BST_tree();
    ~BST_tree();
};

BST_tree::BST_tree() {
    this->root = NULL;
    this->best_seller[0] = NULL;
    this->best_seller[1] = NULL;
    this->best_seller[2] = NULL;
}

BST_tree::~BST_tree() {
    // cleaning dynamically allocated memory
    delete best_seller[0];
    delete best_seller[1];
    delete best_seller[2];
    deleteTree(root);
}

void print_best_sellers(int year, publisher* temp_publisher[3]) {
    cout.precision(5);
    cout << "End of the " + to_string(year) + " Year" << endl;
    cout << "Best seller in North America: " + temp_publisher[0]->name + " - " << temp_publisher[0]->na_sales << " million" << endl;
    cout << "Best seller in Europe: " + temp_publisher[1]->name + " - " << temp_publisher[1]->eu_sales << " million" << endl;
    cout << "Best seller rest of the World: " + temp_publisher[2]->name + " - " << temp_publisher[2]->others_sales << " million" << endl;
}

int GLOBAL_YEAR = 1980;  // start from the earliest year
vector<string> unique_publishers; //vector to store the publishers

BST_tree generate_BST_tree_from_csv(const string file_name) {
    BST_tree temp_BSTtree;
    ifstream file(file_name);
    
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << file_name << endl;
        return temp_BSTtree;
    }

    string line;
    if (!getline(file, line)) {
        cerr << "Error: File appears to be empty or has no header." << endl;
        file.close();
        return temp_BSTtree;
    }

    getline(file, line); 
    
    vector<vector<string>> allData;

     // read all data first
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
            if (temp_BSTtree.get_root() != nullptr) {
                GLOBAL_YEAR = currentDecade;
                temp_BSTtree.find_best_seller();
            }
            currentDecade += 10;
        }

        temp_BSTtree.insertValue(row);
    }
    // end timing after all insertions are complete
    auto end_time = high_resolution_clock::now();
    auto total_time = duration_cast<microseconds>(end_time - start_time);

    // handle final decade
    if (temp_BSTtree.get_root() != NULL) {
        GLOBAL_YEAR = currentDecade;
        temp_BSTtree.find_best_seller();
    }


    cout << "Time taken to insert all data into BST: " << total_time.count() << " µs" << endl;
    return temp_BSTtree;
}

// for the f part of the report
BST_tree generate_BST_tree_by_game_name(string file_name) {
    
    BST_tree temp_BSTtree;
    ifstream file(file_name);

    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << file_name << endl;
        return temp_BSTtree;
    }

    string line;
    getline(file, line); 
    
    vector<vector<string>> allData;
    vector<string> game_names;

    while (getline(file, line)) {
        stringstream ss(line);
        string cell;
        vector<string> row;
        
        while (getline(ss, cell, ',')) {
            row.push_back(cell);
        }
        
        if (row.size() >= 7) {
            allData.push_back(row);
            game_names.push_back(row[1]);
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
        temp_BSTtree.insertValue(game_row);
    }

    auto total_end_time = high_resolution_clock::now();
    auto total_duration = duration_cast<microseconds>(total_end_time - total_start_time);

    cout << "Total time for game name-based tree generation: " << total_duration.count() << " µs" << endl;
    
    return temp_BSTtree;
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

    BST_tree BSTtree = generate_BST_tree_from_csv(fname);

    BST_tree game_tree = generate_BST_tree_by_game_name(fname);

    
   // random search
    const int num_searches = 50;  
    nanoseconds total_search_time(0);

    // initialize random number generator
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, unique_publishers.size() - 1);

    int successful_searches = 0;
    for (int i = 0; i < num_searches; i++) {
        // get a random publisher
        string random_publisher = unique_publishers[dis(gen)];
        
        // measure search time
        auto start = high_resolution_clock::now();
        Node* result = BSTtree.search(random_publisher);
        auto end = high_resolution_clock::now();
        
        
        // only count successful searches
        if (result != nullptr) {
            total_search_time += duration_cast<nanoseconds>(end - start);
            successful_searches++;
        }
        
    }


    if (successful_searches > 0) {
        double avg_time = static_cast<double>(total_search_time.count()) / successful_searches;
        cout << "Average BST search time: " << avg_time << " ns" << endl;
    } else {
        cout << "No successful searches performed" << endl;
    }

    return EXIT_SUCCESS;
}

/////////////////// Node ///////////////////

Node::Node(publisher key) {
    this->key = key;
    this->parent = NULL;
    this->left = NULL;
    this->right = NULL;
}

Node* BST_tree::get_root() {
    return this->root;
}

Node* BST_tree::BST_insert(Node* root, Node* ptr) {
    if (root == NULL) {
        return ptr;
    }

    Node* current = root;
    Node* parent = nullptr;

    while (current != NULL) {
        parent = current;
        if (ptr->key.name < current->key.name) {
            current = current->left;
        } else if (ptr->key.name > current->key.name) {
            current = current->right;
        } else {
            // if it duplicated, aggregate sales and return
            current->key.na_sales += ptr->key.na_sales;
            current->key.eu_sales += ptr->key.eu_sales;
            current->key.others_sales += ptr->key.others_sales;
            delete ptr;  // no need for the new node if it's a duplicate
            return root;
        }
    }

    // insert the new node
    if (ptr->key.name < parent->key.name) {
        parent->left = ptr;
    } else {
        parent->right = ptr;
    }

    ptr->parent = parent;
    return root;
}


void BST_tree::insertValue(vector<string> n) {
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
    } else {
        root = BST_insert(root, newNode);
    }
}


void BST_tree::traverse(Node* node, vector<publisher>& aggregated_sales) {
    if (node == nullptr) {
        return;
    }

    // adding  current node's data to aggregated sales
    aggregated_sales.push_back(node->key);

    // traverse left and right subtrees
    traverse(node->left, aggregated_sales);
    traverse(node->right, aggregated_sales);
}

void BST_tree::find_best_seller() {
    // skip if tree is empty
    if (root == NULL) {
        for (int i = 0; i < 3; i++) {
            if (best_seller[i] != NULL) {
                delete best_seller[i];
            }
            best_seller[i] = NULL;
        }
        return;
    }

    vector<publisher> aggregated_sales;
    traverse(root, aggregated_sales);
    
    if (aggregated_sales.empty()) {
        return;
    }

    // clean up existing best_seller entries
    for (int i = 0; i < 3; i++) {
        if (best_seller[i] != NULL) {
            delete best_seller[i];
            best_seller[i] = NULL;
        }
    }

    // find the best sellers
    publisher* best_na = &aggregated_sales[0];
    publisher* best_eu = &aggregated_sales[0];
    publisher* best_others = &aggregated_sales[0];

    for (auto& entry : aggregated_sales) {
        if (entry.na_sales > best_na->na_sales) {
            best_na = &entry;
        }
        if (entry.eu_sales > best_eu->eu_sales) {
            best_eu = &entry;
        }
        if (entry.others_sales > best_others->others_sales) {
            best_others = &entry;
        }
    }

    // store best sellers
    best_seller[0] = new publisher{best_na->name, best_na->na_sales, best_na->eu_sales, best_na->others_sales};
    best_seller[1] = new publisher{best_eu->name, best_eu->na_sales, best_eu->eu_sales, best_eu->others_sales};
    best_seller[2] = new publisher{best_others->name, best_others->na_sales, best_others->eu_sales, best_others->others_sales};

    // print the best sellers with decade information
    print_best_sellers(GLOBAL_YEAR, best_seller);
}

// function to search for a publisher by name
Node* BST_tree::search(const string& publisher_name) {
    Node* current = root;
    while (current != nullptr) {
        if (publisher_name < current->key.name) {
            current = current->left;
        } else if (publisher_name > current->key.name) {
            current = current->right;
        } else {
            return current;  
        }
    }
    return nullptr;  
}