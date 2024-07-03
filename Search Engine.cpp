#include <fstream>
#include "AVL_Tree.h" // Assuming this is a custom AVL tree implementation
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

// Struct to represent a document item
struct DocumentItem {
    
    string documentName;
    int count = 0; // Count of occurrences of a word in this document
};

// Struct to represent a word item
struct WordItem {
    
    string word_name = ""; // The word itself
    vector<DocumentItem> documents; // List of documents containing this word
};

// Struct to represent word output
struct WordOutput {
    
    string documentName; // Name of the document
    string word; // Word
    int count; // Count of occurrences of the word in the document
};

// Function to convert a string to lowercase
string toLowercase(string data) {
    
    transform(data.begin(), data.end(), data.begin(),
                   [](unsigned char c) -> unsigned char { return std::tolower(c); });
    return data;
}

// Function to check if a document is already in the vector

bool check_document(vector<DocumentItem> vec, string file_name, int & idx){
    for (int i = 0; i < vec.size(); i++){
        if (vec[i].documentName == file_name){
            idx = i;
            return true;
        }
    }
    return false;
}

// Function to remove punctuation and digits from a word
void removePunctuationAndDigits(string& word) {
    
    string result;
    for (char letter : word) {
        if (!isdigit(letter) && !ispunct(letter)) {
            result += letter; // Append the character to the result string
        }
    }
    word = result;
}

// Function to check if a word is in the vector and add it to a temporary vector
void isWordInVector(const vector<WordOutput>& vec, vector<WordOutput>& temp, const string& filename) {
    
    for (const auto& element : vec) {
        if (element.documentName == filename)
            temp.push_back(element); // Word found in vector
    }
}

// Function to check if a word is found in the vector
bool isFoundWordInVector(const vector<WordOutput>& vec, const string& filename, const string& word) {
    
    for (const auto& element : vec) {
        if (element.documentName == filename && element.word == word)
            return true;
    }
    return false;
}

int main() {
    // Constants
    const string ITEM_NOT_FOUND = "not found";

    // Variables
    int num_files;
    vector<string> files_name; // List of file names
    AvlTree<string, WordItem *> myTree (ITEM_NOT_FOUND); // AVL tree to store words and their details

    // Input number of files
    cout << "Enter number of input files: ";
    cin >> num_files;

    // Input file names and process each file
    for(int i = 0; i < num_files; i++){
        
        ifstream file;
        string file_name;
        cout << "Enter " << i+1 << ". file name: ";
        cin >> file_name;
        cin.ignore(); // Ignore any remaining characters in the input buffer

        files_name.push_back(file_name); // Store file name

        // Open file
        file.open(file_name);
        string word;

        // Read each word from file
        while (file >> word) {
            
            WordItem * n_word = new WordItem;
            word = toLowercase(word); // Convert word to lowercase
            removePunctuationAndDigits(word); // Remove punctuation and digits from word
            
            if (word != ""){ // If word is not empty
                n_word->word_name = word; // Set word name in WordItem

                if(myTree.isEmpty()){ // If tree is empty
                    
                    DocumentItem document;
                    document.documentName = file_name;
                    document.count += 1;
                    n_word->documents.push_back(document); // Add document to word's document list
                    myTree.insert(word, n_word); // Insert word into AVL tree
                }
                else {
                    // if word is not in tree, insert to tree
                    if (myTree.find(word) == ITEM_NOT_FOUND){
                        
                        DocumentItem document;
                        document.documentName = file_name;
                        document.count += 1;
                        n_word->documents.push_back(document); // Add document to word's document list
                        myTree.insert(word, n_word); // Insert word into AVL tree
                    }
                    else {
                        // if word is in tree, increase count
                        int index = -1;
                        WordItem * temp = myTree.update(word)->details;
                        // if document is same, do nothing
                        if (check_document(temp->documents, file_name, index))
                            temp->documents[index].count += 1; // Increment count
                        // if document name is different, update the document item
                        else {
                            
                            DocumentItem new_document;
                            new_document.documentName = file_name;
                            new_document.count += 1;
                            temp->documents.push_back(new_document); // Add document to word's document list
                        }
                    }
                }
            }
        }
    }

    bool flag = true;
    string query;

    // Input query words until "ENDOFINPUT" is entered
    while (flag) {
        
        cout << "Enter queried words in one line: ";
        vector<string> words;
        getline(cin, query); // Read the entire line of input

        if (query == "ENDOFINPUT")
            flag = false; // Stop loop if "ENDOFINPUT" is entered
        
        else {
            
            istringstream iss(query); // Create a stringstream to tokenize the input
            string word;
            // Tokenize the input line
            while (iss >> word) {
                words.push_back(toLowercase(word)); // Store each word in the vector
            }

            bool control = true;
            vector<WordOutput> word_details;
            vector<WordOutput> found_word;

            // Process each word in the query
            for (int a = 0; a< words.size(); a++){
                
                if (words[0] == "remove"){ // Check if the command is to remove a word
                    myTree.remove(words[1]);
                    cout << words[1] << " has been REMOVED" << endl;
                    words.clear();
                    control = false;
                    break;
                }
                // Check if word exists in the AVL tree
                if (myTree.find(words[a]) != ITEM_NOT_FOUND){
                    // Get word information
                    WordItem * word_information = myTree.update(words[a])->details;
                    
                    for (int c = 0; c < word_information->documents.size(); c++){
                        
                        WordOutput temp;
                        temp.documentName = word_information->documents[c].documentName;
                        temp.count = word_information->documents[c].count;
                        temp.word = words[a];
                        word_details.push_back(temp);
                    }
                }
            }

            bool check = false;
            if (control){
                // Check if all words in query exist in any document
                for (int a = 0; a < files_name.size(); a++){
                    
                    int num = 0;
                    
                    for (int b = 0; b < words.size(); b++){
                        if(isFoundWordInVector(word_details, files_name[a], words[b]))
                            num++;
                    }
                    if (num == words.size()){
                        check = true;
                    }
                }

                if (!check)
                    cout << "No document contains the given query" << endl; // No document contains all the queried words
                else {
                    // Print occurrences of queried words in each document
                    found_word.clear();
                    for (int i = 0; i < files_name.size(); i++){
                        
                        isWordInVector(word_details, found_word, files_name[i]);
                        if (found_word.size() == words.size()){
                            
                            cout << "in Document " << files_name[i] << ", ";
                            for (int j = 0; j < words.size(); j++){
                                
                                for (int k = 0; k < found_word.size(); k++){
                                    
                                    if (j != found_word.size()-1 && words[j] == found_word[k].word)
                                        cout << words[j] << " found " << found_word[k].count << " times, ";
                                    else if (j == found_word.size()-1 && words[j] == found_word[k].word)
                                        cout << words[j] << " found " << found_word[k].count << " times." << endl;;
                                }
                            }
                        }
                        found_word.clear();
                    }
                }
            }
        }
        cout << endl;
    }
    return 0;
}
// SÜLEYMAN BERBER
