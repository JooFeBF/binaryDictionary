/*
he program aims to create a Spanish dictionary using a Binary Search Tree (BST) data structure.

Key Features:

    Data Storage: Each node in the BST will store:
    Word (Palabra): The Spanish word being defined.
    Meaning (Significado): The definition of the word.
    Grammatical Category (Categoría Gramatical): Noun, verb, adverb, adjective, or other.
    Synonyms (Sinónimos): Up to three synonyms for the word.
    User Interface: The program will be menu-driven, offering the following options:
    Add word to dictionary (Agregar palabra al diccionario)
    Modify elements of a word (Modificar elementos de una palabra)
    Show elements of a word (Mostrar elementos de una palabra)
    Delete word (Eliminar palabra)
    List of words by grammatical category (Listado de palabras por categoría gramatical)
    List of words by letter (specified by the user) (Listado de palabras por letra (indicado por el usuario))
    General list of words in alphabetical order (Listado general de palabras en orden alfabético)
    Show the first and last word of the dictionary with their components (alphabetically) (Mostrar primera y última palabra del diccionario con sus componentes (alfabéticamente))
    Show the number of words registered in the dictionary (Mostrar la cantidad de palabras registradas en el diccionario)
    Exit (Salir)

Core Concepts:

    Binary Search Tree (BST): This data structure enables efficient searching, insertion, and deletion of words. The alphabetical order of words will determine their placement within the tree.
    Data Organization: Each node encapsulates all relevant information about a word, ensuring easy access and manipulation.
    User-Friendly Menu: The menu-driven approach provides a clear and intuitive way for users to interact with the dictionary.

Potential Benefits:

    Fast word retrieval: The BST structure facilitates quick searches for specific words.
    Organized data: Categorization by grammatical type and alphabetical order enhances dictionary usability.
    Comprehensive word information: Each entry provides a well-rounded understanding of the word, including meaning, synonyms, and grammatical function.

Implementation Considerations:

    Programming Language: Choose a language suitable for data structures and user interface development.
    BST Implementation: Ensure the chosen BST implementation allows for efficient search, insertion, deletion, and traversal operations.
    User Input Validation: Implement checks to ensure the accuracy and integrity of data entered by the user.
 */

#include <iostream>
#include <strings.h>

using namespace std;

struct Node {
    string word;
    string meaning;
    string grammaticalCategory;
    string synonyms[3];
    int height;
    Node* left;
    Node* right;
};

struct Dictionary {
    Node* root;
};

Node* createNode(string word, string meaning, string grammaticalCategory, string synonyms[3]) {
    Node* newNode = new Node();
    newNode->word = word;
    newNode->meaning = meaning;
    newNode->grammaticalCategory = grammaticalCategory;
    for (int i = 0; i < 3; i++) {
        newNode->synonyms[i] = synonyms ? synonyms[i] : "";
    }
    newNode->left = nullptr;
    newNode->right = nullptr;
    newNode->height = 1;
    return newNode;
}

int height(Node* node) {
    if (node == nullptr) {
        return 0;
    }
    return node->height;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

Node* rightRotate(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

Node* leftRotate(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

int getBalance(Node* node) {
    if (node == nullptr) {
        return 0;
    }
    return height(node->left) - height(node->right);
}

Node* insertNode(Node* root, Node* newNode) {
    if (root == nullptr) {
        return newNode;
    }

    if (strcasecmp(newNode->word.c_str(), root->word.c_str()) < 0) {
        root->left = insertNode(root->left, newNode);
    } else if (strcasecmp(newNode->word.c_str(), root->word.c_str()) > 0) {
        root->right = insertNode(root->right, newNode);
    } else {
        return root;
    }

    root->height = 1 + max(height(root->left), height(root->right));

    int balance = getBalance(root);

    if (balance > 1 && strcasecmp(newNode->word.c_str(), root->left->word.c_str()) < 0) {
        return rightRotate(root);
    }

    if (balance < -1 && strcasecmp(newNode->word.c_str(), root->right->word.c_str()) > 0) {
        return leftRotate(root);
    }

    if (balance > 1 && strcasecmp(newNode->word.c_str(), root->left->word.c_str()) > 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    if (balance < -1 && strcasecmp(newNode->word.c_str(), root->right->word.c_str()) < 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

Node* minValueNode(Node* node) {
    Node* current = node;
    while (current->left != nullptr) {
        current = current->left;
    }
    return current;
}

Node* deleteWord(Node* root, string word) {
    if (root == nullptr) {
        return root;
    }

    if (strcasecmp(word.c_str(), root->word.c_str()) < 0) {
        root->left = deleteWord(root->left, word);
    } else if (strcasecmp(word.c_str(), root->word.c_str()) > 0) {
        root->right = deleteWord(root->right, word);
    } else {
        if ((root->left == nullptr) || (root->right == nullptr)) {
            Node* temp = root->left ? root->left : root->right;
            if (temp == nullptr) {
                temp = root;
                root = nullptr;
            } else {
                *root = *temp;
            }
            delete temp;
        } else {
            Node* temp = minValueNode(root->right);
            root->word = temp->word;
            root->meaning = temp->meaning;
            root->grammaticalCategory = temp->grammaticalCategory;
            for (int i = 0; i < 3; i++) {
                root->synonyms[i] = temp->synonyms[i];
            }
            root->right = deleteWord(root->right, temp->word);
        }
    }

    if (root == nullptr) {
        return root;
    }

    root->height = 1 + max(height(root->left), height(root->right));

    int balance = getBalance(root);

    if (balance > 1 && getBalance(root->left) >= 0) {
        return rightRotate(root);
    }

    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    if (balance < -1 && getBalance(root->right) <= 0) {
        return leftRotate(root);
    }

    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

void addWord(Dictionary* dictionary, string word, string meaning, string grammaticalCategory, string synonyms[3]) {
    Node* newNode = createNode(word, meaning, grammaticalCategory, synonyms);
    if (dictionary->root == nullptr) {
        dictionary->root = newNode;
    } else {
        dictionary->root = insertNode(dictionary->root, newNode);
    }
}

void addWordMenu(Dictionary* dictionary) {
    string word, meaning, grammaticalCategory, synonyms[3];
    cout << "Enter the word: ";
    cin >> word;
    cout << "Enter the meaning: ";
    cin >> meaning;
    cout << "Enter the grammatical category: ";
    cin >> grammaticalCategory;
    cout << "Enter up to three synonyms (separated by spaces): ";
    for (int i = 0; i < 3; i++) {
        cin >> synonyms[i];
    }
    addWord(dictionary, word, meaning, grammaticalCategory, synonyms);
    cout << "Word added successfully!\n";
}

void modifyWordMenu(Node *word) {
    cout << "Modify elements of the word \"" << word->word << "\":\n";
    cout << "1. Modify meaning\n";
    cout << "2. Modify grammatical category\n";
    cout << "3. Modify synonyms\n";
    cout << "4. Return to main menu\n";
    int choice;
    cin >> choice;

    switch (choice) {
        case 1:
            cout << "Enter the new meaning: ";
            cin >> word->meaning;
            cout << "Meaning updated successfully!\n";
            break;
        case 2:
            cout << "Enter the new grammatical category: ";
            cin >> word->grammaticalCategory;
            cout << "Grammatical category updated successfully!\n";
            break;
        case 3:
            cout << "Enter up to three new synonyms (separated by spaces): ";
            for (int i = 0; i < 3; i++) {
                cin >> word->synonyms[i];
            }
            cout << "Synonyms updated successfully!\n";
            break;
        case 4:
            return;
        default:
            cout << "Invalid choice. Please try again.\n";
    }
}

void showWord(Node* word) {
    cout << "Word: " << word->word << "\n";
    cout << "Meaning: " << word->meaning << "\n";
    cout << "Grammatical Category: " << word->grammaticalCategory << "\n";
    cout << "Synonyms: ";
    for (int i = 0; i < 3; i++) {
        cout << word->synonyms[i] << " ";
    }
    cout << "\n";
}

void printBinaryTree(string prefix, Node *root, bool isLeft) {
    if (root != nullptr) {
        cout << prefix;
        cout << (isLeft ? "├──" : "└──");
        cout << root->word.substr(0, 3) << endl;
        printBinaryTree(prefix + (isLeft ? "│   " : "    "), root->left, true);
        printBinaryTree(prefix + (isLeft ? "│   " : "    "), root->right, false);
    }
}

void visualizeTree(Node *root) {
    printBinaryTree("", root, false);
}

void listByCategory(Node* root, string category) {
    if (root == nullptr) {
        return;
    }
    listByCategory(root->left, category);
    if (root->grammaticalCategory == category) {
        showWord(root);
    }
    listByCategory(root->right, category);
}

void listByLetter(Node* root, char letter) {
    if (root == nullptr) {
        return;
    }
    listByLetter(root->left, letter);
    if (root->word[0] == letter) {
        showWord(root);
    }
    listByLetter(root->right, letter);
}

void listAllWords (Node* root) {
    if (root == nullptr) {
        return;
    }
    listAllWords(root->left);
    showWord(root);
    listAllWords(root->right);
}

void showFirstAndLast(Node* root) {
    if (root == nullptr) {
        cout << "Dictionary is empty.\n";
        return;
    }
    Node* first = root;
    while (first->left != nullptr) {
        first = first->left;
    }
    Node* last = root;
    while (last->right != nullptr) {
        last = last->right;
    }
    cout << "First word: " << first->word << "\n";
    showWord(first);
    cout << "Last word: " << last->word << "\n";
    showWord(last);
}

int countWords(Node* root) {
    if (root == nullptr) {
        return 0;
    }
    return 1 + countWords(root->left) + countWords(root->right);
}

Node *searchWord(Node *root, string word) {
    if (root == nullptr || root->word == word) {
        return root;
    }
    if (strcasecmp(word.c_str(), root->word.c_str()) < 0) {
        return searchWord(root->left, word);
    }
    return searchWord(root->right, word);
}

void displayMenu() {
    cout << "Menu:\n";
    cout << "1. Add word to dictionary\n";
    cout << "2. Modify elements of a word\n";
    cout << "3. Show elements of a word\n";
    cout << "4. Delete word\n";
    cout << "5. List of words by grammatical category\n";
    cout << "6. List of words by letter\n";
    cout << "7. General list of words in alphabetical order\n";
    cout << "8. Show the first and last word of the dictionary with their components\n";
    cout << "9. Show the number of words registered in the dictionary\n";
    cout << "10. Visualize the dictionary tree\n";
    cout << "11. Exit\n";
}

int main() {
    Dictionary dictionary;
    dictionary.root = nullptr;
    int choice;

    do {
        displayMenu();
        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice) {
            case 1:
                addWordMenu(&dictionary);
                break;
            case 2: {
                string word;
                cout << "Enter the word to modify: ";
                cin >> word;
                Node *found = searchWord(dictionary.root, word);
                if (found) {
                    modifyWordMenu(found);
                } else {
                    cout << "Word not found.\n";
                }
                break;
            }
            case 3: {
                string word;
                cout << "Enter the word to show: ";
                cin >> word;
                Node *found = searchWord(dictionary.root, word);
                if (found) {
                    showWord(found);
                } else {
                    cout << "Word not found.\n";
                }
                break;
            }
            case 4: {
                string word;
                cout << "Enter the word to delete: ";
                cin >> word;
                dictionary.root = deleteWord(dictionary.root, word);
                break;
            }
            case 5: {
                string category;
                cout << "Enter the grammatical category: ";
                cin >> category;
                listByCategory(dictionary.root, category);
                break;
            }
            case 6: {
                char letter;
                cout << "Enter the letter: ";
                cin >> letter;
                listByLetter(dictionary.root, letter);
                break;
            }
            case 7:
                listAllWords(dictionary.root);
                break;
            case 8:
                showFirstAndLast(dictionary.root);
                break;
            case 9:
                cout << "Number of words in the dictionary: " << countWords(dictionary.root) << "\n";
                break;
            case 10:
                visualizeTree(dictionary.root);
                break;
            case 11:
                cout << "Exiting program...\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 11);
    return 0;
}
