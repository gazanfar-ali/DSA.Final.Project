#include <iostream>
#include <string>

using namespace std;

struct Book {
    string ISBN;
    string title;
    string author;
    Book* left;
    Book* right;
    int height;

    Book(string isbn, string t, string a)
        : ISBN(isbn), title(t), author(a), left(nullptr), right(nullptr), height(1) {}
};

class AVLTree {
private:
    Book* root;

    // Get the height of a node
    int height(Book* node) {
        return (node == nullptr) ? 0 : node->height;
    }

    // Get the balance factor of a node
    int balanceFactor(Book* node) {
        return (node == nullptr) ? 0 : height(node->left) - height(node->right);
    }

    // Right rotate the subtree rooted at node
    Book* rightRotate(Book* y) {
        Book* x = y->left;
        Book* T2 = x->right;

        // Perform rotation
        x->right = y;
        y->left = T2;

        // Update heights
        y->height = max(height(y->left), height(y->right)) + 1;
        x->height = max(height(x->left), height(x->right)) + 1;

        // Return new root
        return x;
    }

    // Left rotate the subtree rooted at node
    Book* leftRotate(Book* x) {
        Book* y = x->right;
        Book* T2 = y->left;

        // Perform rotation
        y->left = x;
        x->right = T2;

        // Update heights
        x->height = max(height(x->left), height(x->right)) + 1;
        y->height = max(height(y->left), height(y->right)) + 1;

        // Return new root
        return y;
    }

    // Balance the node
    Book* balance(Book* node) {
        int balance = balanceFactor(node);

        // Left heavy case
        if (balance > 1) {
            if (balanceFactor(node->left) < 0) { // Left-right case
                node->left = leftRotate(node->left);
            }
            return rightRotate(node); // Left-left case
        }

        // Right heavy case
        if (balance < -1) {
            if (balanceFactor(node->right) > 0) { // Right-left case
                node->right = rightRotate(node->right);
            }
            return leftRotate(node); // Right-right case
        }

        // Already balanced
        return node;
    }

    // Insert a book into the AVL tree
    Book* insert(Book* node, const string& ISBN, const string& title, const string& author) {
        if (node == nullptr) {
            return new Book(ISBN, title, author);
        }

        if (ISBN < node->ISBN) {
            node->left = insert(node->left, ISBN, title, author);
        } else if (ISBN > node->ISBN) {
            node->right = insert(node->right, ISBN, title, author);
        } else {
            return node; // Duplicate ISBNs are not allowed
        }

        // Update height of this node
        node->height = max(height(node->left), height(node->right)) + 1;

        // Balance the node
        return balance(node);
    }

    // Find the node with the minimum ISBN
    Book* minValueNode(Book* node) {
        Book* current = node;
        while (current->left != nullptr) {
            current = current->left;
        }
        return current;
    }

    // Remove a book by ISBN
    Book* remove(Book* root, const string& ISBN) {
        if (root == nullptr) {
            return root;
        }

        if (ISBN < root->ISBN) {
            root->left = remove(root->left, ISBN);
        } else if (ISBN > root->ISBN) {
            root->right = remove(root->right, ISBN);
        } else {
            // Node to be deleted found

            // Node with only one child or no child
            if (root->left == nullptr) {
                Book* temp = root->right;
                delete root;
                return temp;
            } else if (root->right == nullptr) {
                Book* temp = root->left;
                delete root;
                return temp;
            }

            // Node with two children: Get the inorder successor (smallest in the right subtree)
            Book* temp = minValueNode(root->right);

            // Copy the inorder successor's content to this node
            root->ISBN = temp->ISBN;
            root->title = temp->title;
            root->author = temp->author;

            // Delete the inorder successor
            root->right = remove(root->right, temp->ISBN);
        }

        // Update height of the current node
        root->height = max(height(root->left), height(root->right)) + 1;

        // Balance the node
        return balance(root);
    }

    // Search for a book by ISBN
    Book* search(Book* node, const string& ISBN) {
        if (node == nullptr || node->ISBN == ISBN) {
            return node;
        }

        if (ISBN < node->ISBN) {
            return search(node->left, ISBN);
        } else {
            return search(node->right, ISBN);
        }
    }

    // Inorder traversal to display all books in sorted order
    void inorder(Book* node) {
        if (node != nullptr) {
            inorder(node->left);
            cout << "ISBN: " << node->ISBN << ", Title: " << node->title << ", Author: " << node->author << endl;
            inorder(node->right);
        }
    }

    // Range traversal to display books within the ISBN range
    void rangeQuery(Book* node, const string& startISBN, const string& endISBN) {
        if (node == nullptr) {
            return;
        }

        if (startISBN < node->ISBN) {
            rangeQuery(node->left, startISBN, endISBN);
        }

        if (startISBN <= node->ISBN && endISBN >= node->ISBN) {
            cout << "ISBN: " << node->ISBN << ", Title: " << node->title << ", Author: " << node->author << endl;
        }

        if (endISBN > node->ISBN) {
            rangeQuery(node->right, startISBN, endISBN);
        }
    }

public:
    AVLTree() : root(nullptr) {}

    void addBook(const string& ISBN, const string& title, const string& author) {
        root = insert(root, ISBN, title, author);
    }

    void removeBook(const string& ISBN) {
        root = remove(root, ISBN);
    }

    void findBook(const string& ISBN) {
        Book* book = search(root, ISBN);
        if (book != nullptr) {
            cout << "Book found: ISBN: " << book->ISBN << ", Title: " << book->title << ", Author: " << book->author << endl;
        } else {
            cout << "Book not found." << endl;
        }
    }

    void displayAllBooks() {
        inorder(root);
    }

    void displayBooksInRange(const string& startISBN, const string& endISBN) {
        rangeQuery(root, startISBN, endISBN);
    }
};

int main() {
    AVLTree bookstore;

    // Adding books
    bookstore.addBook("978-3-16-148410-0", "The C++ Programming Language", "Bjarne Stroustrup");
    bookstore.addBook("978-0-201-63361-0", "Effective C++", "Scott Meyers");
    bookstore.addBook("978-1-59327-599-1", "Clean Code", "Robert C. Martin");

    // Display all books
    cout << "All Books in Sorted Order:" << endl;
    bookstore.displayAllBooks();

    // Search for a book
    cout << "\nSearching for book with ISBN 978-3-16-148410-0:" << endl;
    bookstore.findBook("978-3-16-148410-0");

    // Remove a book
    cout << "\nRemoving book with ISBN 978-0-201-63361-0:" << endl;
    bookstore.removeBook("978-0-201-63361-0");

    // Display all books after removal
    cout << "\nAll Books in Sorted Order after removal:" << endl;
    bookstore.displayAllBooks();

    // Display books in a given ISBN range
    cout << "\nBooks between ISBNs 978-0-201-63300-0 and 978-3-16-148410-0:" << endl;
    bookstore.displayBooksInRange("978-0-201-63300-0", "978-3-16-148410-0");

    return 0;
}

