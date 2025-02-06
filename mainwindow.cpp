#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <iostream>
#include<cstring>
#include <cmath>
#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <set>
#include <map>
#include<bits/stdc++.h>


char Inputs1[100], func1[100], resultXor1[100];
char Inputs2[100], func2[100], resultXor2[100];
int numInputs1 = 0, numInputs2 = 0;

void clear(){
    memset(Inputs1, 0, sizeof(Inputs1));
    memset(func1, 0, sizeof(func1));
    memset(resultXor1, 0, sizeof(resultXor1));
    memset(resultXor2, 0, sizeof(resultXor2));
}


class Heap {
private:
    std::string arr[100];
    int length;
    int size;

public:

    Heap(int cap) {
        length = 0;         // real length of heap
        size = cap;   // max size the heap can reach
    }

    void add(std::string value) {
        if (length == size) {
            std::cout << "Heap is full!" << std::endl;
            return;
        }
        arr[length] = value;
        length++;
    }

    std::string getNode(int index) {
        if (index < 0 || index >= length) {
            return "Invalid index";  // Check for valid index
        }
        return arr[index];
    }

    // Added method to get the index of a node by its value
    int getNodeIndex(const std::string& value) {
        for (int i = 0; i < length; ++i) {
            if (arr[i] == value) {
                return i;
            }
        }
        return -1;  // Return -1 if the value is not found
    }

    std::string left(int i) {
        int leftIndex = 2 * i + 1;
        if (leftIndex < length) {
            return arr[leftIndex];
        }
        return "None";
    }

    std::string right(int i) {
        int rightIndex = 2 * i + 2;
        if (rightIndex < length) {
            return arr[rightIndex];
        }
        return "None";
    }

    int getLeftIndex(int i) {
        int leftIndex = 2 * i + 1;
        if (leftIndex < length) {
            return leftIndex;
        }
        return -1;  // Return -1 if no left child
    }

    int getRightIndex(int i) {
        int rightIndex = 2 * i + 2;
        if (rightIndex < length) {
            return rightIndex;
        }
        return -1;  // Return -1 if no right child
    }

    std::string parent(int i) {
        if (i == 0) {
            return "None";
        }
        int parentIndex = (i - 1) / 2;
        return arr[parentIndex];
    }

    void printHeap() {
        std::cout << "Heap: ";
        for (int i = 0; i < length; i++) {
            std::cout << arr[i] << " ";
        }
        std::cout << std::endl;
    }

    void removeNode(int index) {
        // Shift all elements after the index to the left by 1 position
        for (int i = index; i < length - 1; ++i) {
            arr[i] = arr[i + 1];
        }
        --length;  // Decrease the heap size by 1
    }

    int getSize() {
        return length;
    }
};

Heap heap1(100);
Heap heap2(100);


struct Node {
    int var;     // Variable index (-1 for terminal nodes)
    int low;     // Low child (false branch)
    int high;    // High child (true branch)
};

bool isEqual(Node x , Node y){
    if (x.low == y.low && x.high == y.high && x.var == y.var) return true;
    else return false;
}

void redundant(std::vector<std::vector<int>>& results, int i) {
    int val = results[i][3], p = results[i][1];
    std::set<int> parents;


    results.erase(results.begin() + i);

    if (results[p][2] == i) results[p][2] = val;
    if (results[p][3] == i) results[p][3] = val;
}

void merge(std::vector<std::vector<int>>& results, int i) {
    int val = results[i - 1][0], p = results[i][1], tag = results[i][0];
    std::set<int> parents;


    if (results[p][2] == results[i][0]) results[p][2] = val;
    if (results[p][3] == results[i][0]) results[p][3] = val;

    if (results[i].size() > 4) {
        for (int j = 4; j < results[i].size(); j++) {
            parents.insert(results[i][j]);
        }

        for (int j = 0; j < results[i].size(); j++) {
            if (parents.count(results[j][0]) != 0) {
                if (results[j][2] == results[i][0]) results[j][2] = val;
                if (results[j][3] == results[i][0]) results[j][3] = val;
            }
        }
    }



    results.erase(results.begin() + i);


    results[i - 1].push_back(p);

    for (auto it : parents) {
        int y = it;

        results[i - 1].push_back(it);
    }

}

void mergehandling(std::vector<std::vector<int>>& results) {
    int N = 0, P = 1, L = 2, R = 3;

    for (int i = results.size() - 1; i > 0; i--) {

        int left = results[i][L], nleft = results[i - 1][L];
        int right = results[i][R], nright = results[i - 1][R];

        if (left == nleft && right == nright) {
            merge(results, i);

            // cout<<i<<endl;
        }
    }


    return;
}

void redundantHandling(std::vector<std::vector<int>>& results) {
    int N = 0, P = 1, L = 2, R = 3;


    //handle redundant
    // cout<<"redundant:"<<endl;
    for (int i = results.size() - 1; i > 0; i--) {

        int left = results[i][L];
        int right = results[i][R];

        if (left == right) {
            redundant(results, i);

            // cout<<i<<endl;
        }
    }


}

int get_index(std::vector<std::vector<int>>& results, int t) {
    for (int i = 0; i < results.size(); i++) {
        if (results[i][0] == t) return i;
    }

    return -1;
}

void changeFormat(std::vector<std::vector<int>>& results, std::unordered_map<char, int>& varMap, std::vector<Node>& nodes, std::string arr) {

    for (int i = 0; i < results.size(); i++) {
        int f, s, th;

        int tag = results[i][0];
        char ch = arr[tag];
        f = varMap[ch];

        std::cout << f << std::endl;

        tag = results[i][2];
        int index = get_index(results, tag);
        if (index == -1) {
            ch = arr[tag];
            index = ch - '0';
            s = index;
        }
        else
            s = index + 2;

        std::cout << s << std::endl;

        tag = results[i][3];
        index = get_index(results, tag);
        if (index == -1) {
            ch = arr[tag];
            index = ch - '0';
            th = index;
        }
        else
            th = index + 2;

        std::cout << th << std::endl;
        nodes.emplace_back(Node{ f, s, th });


    }
}


void simplifyROBDD(std::string& arr, std::vector<std::vector<int>>& results,std::unordered_map<char, int>& varMap) {


    // arr -> abbccc....
    // s is size of that array mesta5demeeno fe el loop

    int s = arr.size();
    int c = 0;
    int lc = 0, nc = 0;
    std::set<char> st;

    for (int i = 0; i < arr.size(); i++) {
        if (isalpha(arr[i])) {
            st.insert(arr[i]);
        }
    }

    for (auto it : st) {
        varMap[it] = c;
        c++;
    }


    std::map<int, int> parent;
    std::map<int, char> mp;
    int N = 0, P = 1, L = 2, R = 3;
    // L -> low value , R -> high value
    // N -> tag of each one
    // P -> parent tag

    //zabat el gadwal
    while (st.count(arr[nc]) != 0) {
        std::vector <int> row(4);

        row[N] = nc;
        lc++;
        row[L] = lc;
        parent[lc] = nc;
        lc++;
        row[R] = lc;
        parent[lc] = nc;

        if (parent.count(nc) != 0) {
            row[P] = parent[nc];
        }
        else {
            row[P] = -1;
        }

        results.push_back(row);

        // for(int i : row){
        //     cout<<"*"<<i;
        // }
        // cout<<endl;
        nc++;
    }

    int one = nc + 1;
    int zero = nc;


    // one we zero nafs el node

    for (int i = results.size() - 1; i >= 0; i--) {

        char left = arr[results[i][L]];
        char right = arr[results[i][R]];

        if (left == '1') {
            results[i][L] = one;
        }
        else if (left == '0') {
            results[i][L] = zero;
        }

        if (right == '1') {
            results[i][R] = one;
        }
        else if (right == '0') {
            results[i][R] = zero;
        }


    }

    arr[one] = '1';
    arr[zero] = '0';

    for (int i = 0; i < results.size(); i++) {
        for (int j : results[i]) {
            std::cout << "*" << j;
        }
        std::cout << std::endl;
    }
    std::cout<<'\n';

    //handle redundant
    // cout<<"redundant:"<<endl;
    redundantHandling(results);
    for (int i = 0; i < results.size(); i++) {
        for (int j : results[i]) {
            std::cout << "*" << j;
        }
        std::cout << std::endl;
    }
    std::cout<<'\n';
    //handle merge
    // cout<<"merge:"<<endl;
    mergehandling(results);
    for (int i = 0; i < results.size(); i++) {
        for (int j : results[i]) {
            std::cout << "*" << j;
        }
        std::cout << std::endl;
    }
    std::cout<<'\n';
    //2nd redundance check
    // cout<<"redundant:"<<endl;
    redundantHandling(results);
    for (int i = 0; i < results.size(); i++) {
        for (int j : results[i]) {
            std::cout << "*" << j;
        }
        std::cout << std::endl;
    }
    std::cout<<'\n';

    for (int i = 0; i < results.size(); i++) {
        for (int j : results[i]) {
            std::cout << "*" << j;
        }
        std::cout << std::endl;
    }


}


///////////////////////////////////////////////////////////////////////////////////////////////////////
// ROBDD Node Structure
void printNodes(const std::vector<Node>& nodes) {
    std::cout << "Index\tVar\tLow\tHigh" << std::endl;
    for (size_t i = 0; i < nodes.size(); ++i) {
        std::cout << i << "\t" << nodes[i].var << "\t" << nodes[i].low << "\t" << nodes[i].high << std::endl;
    }
}


// Function to generate DOT file
void generateDOT(const std::string& filename, int root, const std::vector<Node>& nodes, const std::unordered_map<char, int>& varMap) {
    std:: ofstream dotFile(filename);
    if (!dotFile) {
        std::cerr << "Error: Cannot create DOT file." << std::endl;
        return;
    }

    dotFile << "digraph ROBDD {\n";

    // Write nodes
    for (size_t i = 0; i < nodes.size(); ++i) {
        if (nodes[i].var == -1) { // Terminal nodes
            dotFile << "    " << i << " [label=\"" << (nodes[i].low == 0 ? "0" : "1") << "\", shape=box];\n";
        }
        else { // Variable nodes
            for (const auto& [varName, varIndex] : varMap) {
                if (varIndex == nodes[i].var) {
                    dotFile << "    " << i << " [label=\"" << varName << "\"];\n";
                }
            }
        }
    }

    // Write edges
    for (size_t i = 0; i < nodes.size(); ++i) {
        if (nodes[i].var != -1) { // Skip terminal nodes
            dotFile << "    " << i << " -> " << nodes[i].low << " [label=\"0\", style=dashed];\n";
            dotFile << "    " << i << " -> " << nodes[i].high << " [label=\"1\", color=green];\n";
        }
    }

    dotFile << "}\n";
    dotFile.close();

    std::cout << "DOT file generated: " << filename << std::endl;
}


static bool evaluateBoolExpr(char premises[], bool values[], char inputs[], int noInputs)
{
    bool result = false; // Initialize result for OR operation
    bool andOp = true; // Tracks conjunctions (AND operations)
    bool isImplication = false; // Flag for implication operation

    for (int i = 0; premises[i] != '\0'; i++)
    {
        char ch = premises[i];

        if (ch == ' ') continue;

        if (ch == '+') {
            result = result || andOp;
            andOp = true; // Reset for next conjunction
        }
        else if (ch == '.') {
            // Continue to the next character to handle conjunction
        }
        else if (ch == '~') {
            char nextChar = premises[++i];
            bool negationResult = true; // Default to true for negation
            for (int j = 0; j < noInputs; j++) {
                if (nextChar == inputs[j]) {
                    negationResult = !values[j];
                }
            }
            andOp = andOp && negationResult;
        }
        else {
            for (int j = 0; j < noInputs; j++) {
                if (ch == inputs[j]) {
                    andOp = andOp && values[j];
                }
            }
        }
    }

    // Final result considering OR operation if no implication was found
    if (!isImplication) {
        result = result || andOp;
    }

    return result;

}



// Function to print the truth table and evaluate all premises
std::string printTruthTableAndApplyShannonExpansion(int noInputs, int noPremises, char inputs[], char premises[], Heap& heap)
{
    // Loop through all combinations (2^numVars possibilities)
    int numCombinations = 1 << noInputs;  // This is 2^numVars

    // Add each variable in the desired pattern: 1 time for the first, 2 times for the second, etc.
    int repeatCount = 1;
    for (int i = 0; i < noInputs; i++) {
        for (int j = 0; j < repeatCount; j++) {
            heap.add(std::string(1, inputs[i]));  // Add the variable itself to the heap
        }
        repeatCount *= 2;  // Double the repeat count for the next variable
    }



    int rows = pow(2, noInputs);
    bool values[1000];

    std::cout << "\nTruth Table:\n";

    // Print headers for the truth table
    for (int i = 0; i < noInputs; i++) {
        std::cout << inputs[i] << "\t";
    }
    for (int j = 0; j < noPremises; j++) {
        std::cout << "s" << j + 1 << "\t";
    }
    std::cout << std::endl << std::endl;

    // Loop through all possible input combinations
    for (int i = 0; i < rows; i++)
    {
        // Generate truth values for each input variable
        for (int j = 0; j < noInputs; j++) {
            values[j] = (i >> (noInputs - j - 1)) & 1;
            std::cout << (values[j] ? "1" : "0") << "\t";
        }


        // Evaluate each premise
        for (int k = 0; k < noPremises; k++) {
            bool result = evaluateBoolExpr(premises, values, inputs, noInputs);
            std::cout << (result ? "1" : "0") << "\t";
            heap.add(result ? "1" : "0");
        }
        std::cout << std::endl << std::endl;
    }

    //heap.printHeap();

    std::string heapContents = "";
    for (int i = 0; i < heap.getSize(); i++) {
        heapContents += heap.getNode(i);
        heapContents += " ";
    }

    // cout << "\nHeap contents:\n" << heapContents << endl;
    return heapContents;

}

void checkForXor(const char func[], char result[], const char inputs[], int numInputs) {
    int i = 0;
    int j = 0;

    while (func[i] != '\0') {
        if (func[i] == '^' && i > 0 && func[i + 1] != '\0') {

            char var1 = func[i - 1];  // Variable before ^
            char var2 = func[i + 1];  // Variable after ^


            bool found1 = false, found2 = false;
            for (int k = 0; k < numInputs; k++) {
                if (inputs[k] == var1) found1 = true;
                if (inputs[k] == var2) found2 = true;
            }

            if (found1 && found2) {
                j = j - 1;
                char before = (j > 0 && result[j - 1] == '.') ? result[j - 2] : '\0';
                char after = (func[i + 2] == '.') ? func[i + 3] : '\0';

                if (before != '\0' && after != '\0') {

                    j -= 2;
                    result[j++] = before;
                    result[j++] = '.';
                    result[j++] = var1;
                    result[j++] = '.';
                    result[j++] = '~';
                    result[j++] = var2;
                    result[j++] = '.';
                    result[j++] = after;
                    result[j++] = '+';
                    result[j++] = before;
                    result[j++] = '.';
                    result[j++] = '~';
                    result[j++] = var1;
                    result[j++] = '.';
                    result[j++] = var2;
                    result[j++] = '.';
                    result[j++] = after;
                    j++;
                }
                else if (before != '\0') {

                    j -= 2;
                    result[j++] = before;
                    result[j++] = '.';
                    result[j++] = var1;
                    result[j++] = '.';
                    result[j++] = '~';
                    result[j++] = var2;
                    result[j++] = '+';
                    result[j++] = before;
                    result[j++] = '.';
                    result[j++] = '~';
                    result[j++] = var1;
                    result[j++] = '.';
                    result[j++] = var2;
                }
                else if (after != '\0') {

                    result[j++] = var1;
                    result[j++] = '.';
                    result[j++] = '~';
                    result[j++] = var2;
                    result[j++] = '.';
                    result[j++] = after;
                    result[j++] = '+';
                    result[j++] = '~';
                    result[j++] = var1;
                    result[j++] = '.';
                    result[j++] = var2;
                    result[j++] = '.';
                    result[j++] = after;
                    j++;
                }
                else {

                    result[j++] = var1;
                    result[j++] = '.';
                    result[j++] = '~';
                    result[j++] = var2;
                    result[j++] = '+';
                    result[j++] = '~';
                    result[j++] = var1;
                    result[j++] = '.';
                    result[j++] = var2;
                }

                i += 2;
                continue;
            }
        }


        result[j++] = func[i++];
    }

    result[j] = '\0';
}


std::string getInputs(char inputs[], int& numInputs, char func[],char resultXor[],std::string s, Heap& heap) {


    std::strcpy(func, s.c_str());
    func[strlen(func)] = '\0';
    for (int i = 0 ; i<strlen(func); i++){
        std::cout<<func[i];

    }
    std::cout<<strlen(func);
    std::cout<<'\n';
    for (int i = 0 ; i<strlen(inputs); i++){
        std::cout<<inputs[i];

    }
    std::cout<<strlen(inputs);
    std::cout<<'\n';

    std::cout<<numInputs;
    checkForXor(func, resultXor, inputs, numInputs);

    std::string heapcontents = printTruthTableAndApplyShannonExpansion(numInputs, 1, inputs, resultXor, heap);

    //applyShannonExpansion(func, inputs, numInputs, heap);
    //applyDeMorgan(func, result);
    //cout << "After applying De Morgan's laws: " << result << "\n";

    return  heapcontents;
}




std::string solution(std::string str1,std::string str2) {





    std::vector<std::vector<int>> results1;
    std::unordered_map<char, int> varMap1;
    std::vector<std::vector<int>> results2;
    std::unordered_map<char, int> varMap2;

    //  = {
    //     {"a", 0},
    //     {"b", 1},
    //     {"c", 2}
    // };
    std::vector<Node> nodes1;
    nodes1.emplace_back(Node{ -1, 0, 0 });
    nodes1.emplace_back(Node{ -1, 1, 1 });

    std::vector<Node> nodes2;
    nodes2.emplace_back(Node{ -1, 0, 0 });
    nodes2.emplace_back(Node{ -1, 1, 1 });


    // Process Function 1
    std::cout << "Process Function 1:\n";
    std::string s1 = getInputs(Inputs1, numInputs1, func1, resultXor1, str1, heap1);
    s1.erase(remove(s1.begin(), s1.end(), ' '), s1.end());

    //  Process Function 2
    std::cout << "\nProcess Function 2:\n";
    std::string s2 = getInputs(Inputs2, numInputs2, func2, resultXor2, str2, heap2);
    s2.erase(remove(s2.begin(), s2.end(), ' '), s2.end());

    // string s1 = "abbcccc01000111";
    // string s1 = "abbcccc00010111";
    // string s1 = "abbcccc01010111";
    // string s1 = "abb0111";

    std::cout << s1 << std::endl;
    std::cout << s2 << std::endl;
    simplifyROBDD(s1, results1, varMap1);
    std::cout<<"the results arrays"<<'\n';
    for (int i = 0 ; i<results1.size(); i++){
        for (int j ; j<results1[i].size();j++){
            std::cout<<results1[i][j]<<'\t';
        }
        std::cout<<'\n';
    }
    for (int i = 0 ; i<results2.size(); i++){
        for (int j ; j<results2[i].size();j++){
            std::cout<<results2[i][j]<<'\t';
        }
        std::cout<<'\n';
    }
    changeFormat(results1, varMap1, nodes1, s1);
    printNodes(nodes1);
    // Generate DOT file for the ROBDD
    generateDOT("robdd1.dot", 3, nodes1, varMap1);

    // Render DOT file to an image using Graphviz
    system("dot -Tpng robdd1.dot -o robdd1.png");
    bool eqv = true;
    std::cout << "ROBDD graph saved as robdd.png" << std::endl;


    // results.clear();
    // varMap.clear();
    // nodes.clear();

    simplifyROBDD(s2, results2, varMap2);
    std::cout<<"the results arrays"<<'\n';
    for (int i = 0 ; i<results1.size(); i++){
        for (int j ; j<results1[i].size();j++){
            std::cout<<results1[i][j]<<'\t';
        }
        std::cout<<'\n';
    }
    for (int i = 0 ; i<results2.size(); i++){
        for (int j ; j<results2[i].size();j++){
            std::cout<<results2[i][j]<<'\t';
        }
        std::cout<<'\n';
    }
    changeFormat(results2, varMap2, nodes2, s2);
    printNodes(nodes2);
    // Generate DOT file for the ROBDD
    generateDOT("robdd2.dot", 3, nodes2, varMap2);

    system("dot -Tpng robdd2.dot -o robdd2.png");

    std::cout<<"the results arrays"<<'\n';

    for (int i = 0 ; i<results1.size(); i++){
        for (int j ; j<results1[i].size();j++){
            std::cout<<results1[i][j]<<'\t';
        }
        std::cout<<'\n';
    }
    for (int i = 0 ; i<results2.size(); i++){
        for (int j ; j<results2[i].size();j++){
            std::cout<<results2[i][j]<<'\t';
        }
        std::cout<<'\n';
    }

    if (nodes1.size() != nodes2.size()){
        eqv =false;
    }else{
        for (int i = 0 ; i<nodes1.size()  ; i++ ){
            if(!isEqual(nodes1[i],nodes2[i])) eqv=false;
        }
    }
    std::cout << "ROBDD graph saved as robdd2.png" << std::endl;

    std:: string result;
    if (eqv){
        result="equivalent";
    }else{
        result="not equivalent";
    }
    return result;


}
void extractVariables(const std::string& expr,char result[], int& numInputs) {
    std::unordered_map<char, bool> variables;
    for (char c : expr) {
        if (isalpha(c)) {
            variables[c] = false; // Ensure uppercase and unique
        }
    }
    int i = 0;
    std::vector<char> arr;
    for (const auto& pair : variables) {
        arr.push_back(pair.first);

    }
    sort(arr.begin(),arr.end());
    for (char x : arr){
        result[i] = x;
        i++;
    }
    for(int i = 0 ; i<strlen(result) ; i++){
        std::cout<<result[i];
    }
    numInputs=i;

}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

std::string first_eqn,second_eqn;

void MainWindow::on_pushButton_clicked()
{
    first_eqn = ui->firsteqn->text().toStdString();
    second_eqn = ui->secondeqn->text().toStdString();
    clear();
    extractVariables(first_eqn,Inputs1,numInputs1);
    extractVariables(second_eqn,Inputs2,numInputs2);

    std::string r1 = solution(first_eqn,second_eqn);


    ui->result->setText(QString::fromStdString(r1));

}
