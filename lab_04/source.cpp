#include <iostream>
#include <cstring>
#include <string>

// enum has members starting from 0
//              0     1         2
// enum Genre { SF, FANTASY, ROMANCE };
//
// unless explicit values are mentioned
//               1          4             11
// enum Genre { SF = 1, FANTASY = 4, ROMANCE = 11 };

// enum class (note keyword 'class') does not have an integer association for its members
enum class Genre { SF, FANTASY, ROMANCE };

class Book {
// private: -> by default in classes
    const unsigned int id; //it needs to be initialized in a specific way
    // 1. -> where it is defined
    // 2. -> initialization lists
    char* title;
    std::string author;
    unsigned int pages = 0;
    // std::string genre; // user can input "sf", "SF", "ScienceFiction" -> bad implementation
    Genre genre;
public:
    // ctor
    // initialization lists are mandatory only for const non-static attributes
    Book() : id(0), title(nullptr) {
        std::cout << "\nConstructor with no params called\n";

        this->title = nullptr; // same thing the initialization does
        //this->id = 0; // this cannot compile
        this->pages = 0; // already is 0 from line 23
        this->genre = Genre::FANTASY;
        this->author = "";
    }

    Book(const int id, const char* title, const std::string& author,
        const int pages, const Genre genre) : id(id), genre(genre) {
        std::cout << "\nConstructor with params called\n";

        if (pages < 0) { // validation of fields is best done in SETTERS
            std::cout << "Invalid parameter - no. of pages cannot be negative" << std::endl;
            this->pages = 0;
        } else {
            this->pages = pages;
        }

        if (title == nullptr) {
            std::cout << "Invalid parameter - title cannot be null" << std::endl;
            this->title = nullptr;
        } else {
            // Q: is `delete[] this->title` needed here?
            // A: no, because the object (this) has not been created yet, thus we do not need deallocation
            this->title = new char[std::strlen(title) + 1];
            strcpy_s(this->title, std::strlen(title) + 1, title);
        }

        this->author = author;
    }

    void setTitle(const char* title) {
        if (this->title == title) { // if we pass as param the result of the title getter
            return;
        }
        if (title == nullptr) {
            std::cout << "Invalid parameter - title cannot be null" << std::endl;
            this->title = nullptr;
        } else {
            // Q: is `delete[] this->title` needed here?
            // A: yes, because the object (this) exists
            delete[] this->title;
            this->title = new char[std::strlen(title) + 1];
            strcpy_s(this->title, std::strlen(title) + 1, title);
        }
    }

    // helper job to print object
    void print() const {
        std::cout << "Book info: " << std::endl;
        std::cout << "\t" << this->id << std::endl;
        if (this->title == nullptr) {
            std::cout << "\tNULL" << std::endl;
        } else {
            std::cout << "\t" << this->title << std::endl;
        }
        std::cout << "\t"<< this->author << std::endl;
        std::cout << "\t" << this->pages << std::endl;
        // std::cout << this->genre << std::endl; -> cannot be done like this, to discuss next seminar
    }

    ~Book() {
        std::cout << "\nDestructor called\n";
        // when we have dynamically allocated memory, it is OUR job to deallocate it
        // otherwise we have MEMORY LEAK
        delete[] this->title;
    }
};

int main() {
    Book b1;
    b1.print();

    Book b2(3, "New book", "Indie author", 323, Genre::SF);
    b2.print();

    Book* b3 = new Book(7, "Pointer book", "Pointer author", 611, Genre::ROMANCE);
    b3->print();

    delete b3;
    return 0;
}
