#include <iostream>
#include <string>

void purchase(int& quantity, int howMany) {
    std::cout << "I buy " << howMany << " items" << std::endl;
    if (howMany > quantity) {
        std::cout << "Trying to buy more than it is allowed!" << std::endl;
    }
    else {
        quantity -= howMany;
    }
}

// it's like a container of all objects of similar structure
// template
// private, protected, public
// by default, all attributes are private
// private = accessible only within the class
class Product {
    std::string name;
    int stock;
    float price;
    static int noProducts; // counter - keep track of how many Product objects I have
    // 1. when I make a new object I increment it -> in ctor
    // 2. when an object gets destroyed I decrement it -> in destructor
    int* p;

public:
    static const int MAX_STOCK = 10000;
    // constructor
    // default constructor - given by default
    // once we define one custom ctor -> we lose the default one
    Product() {
        Product::noProducts++;

        this->name = "Unknown Product " + std::to_string(Product::noProducts);
        this->stock = 0;
        this->price = 0.0;
        this->p = new int(0);
    }

    Product(const std::string name, const int stock, const float price) {
        Product::noProducts++;

        this->name = name;
        // instead of
        // this->stock = stock; // .. -> validations
        // do this
        setStock(stock);
        setPrice(price);
        this->p = new int(0);
    }

    // getter and setter
    // getter -> a method which allows an user to access an attribute in a controlled way
    int getStock() const {
        //this->stock = 0; // this is not allowed because keyword const turn the getter into a const member function -> this becomes constant
        return this->stock;
    }

    static int getNoProducts() {
        return Product::noProducts;
    }

    // setter -> a method which allows an user to CHANGE an attribute in a controlled way
    // these methods have parameters (most of the time)
    void setStock(const int stock) {
        // Q: do i want stock to be negative?
        if (stock < 0) {
            std::cout << "Invalid stock value - cannot be negative" << std::endl;
            return;
        }
        else if (stock > MAX_STOCK) {
            std::cout << "Invalid stock value - exceeds limit" << std::endl;
            return;
        }
        this->stock = stock;
    }

    void setPrice(const float price) {
        if (price < 0) {
            std::cout << "Invalid price value - cannot be negative" << std::endl;
            return;
        }
        else if (price > 1000) {
            std::cout << "Invalid price value - insane price" << std::endl;
            return;
        }
        this->price = price;
    }

    void purchase(const int quantity) {
        if (quantity > stock) {
            std::cout << "Invalid action, cannot purchase " << quantity << " units." << std::endl;
        }
        else {
            this->stock -= quantity;
        }
    }

    void print() const {
        std::cout << "name: " << this->name << "; stock: " << this->stock
            << "; price: " << this->price << std::endl;
    }

    // the destructor cleans up the memory space of an object
    // once the object becomes out of scope
    ~Product() {
        std::cout << "This is the destructor - one call" << std::endl;
        delete this->p; // we must deallocate dynamically allocated memory in the DESTRUCTOR
        Product::noProducts--;
    }
};

int Product::noProducts = 0;

int main()
{
    //// std::string - wrapper over char arrays
    //std::string product1 = "bag of chips";
    //int product1Quantity = 5;
    //std::string product1BarCode = "18391034";

    //std::string product2 = "orange juice";
    //int product2Quantity = 10;
    //std::string product2BarCode = "182433134";

    Product p1;
    p1.print();
    //p1.name = "idk";
    p1.purchase(2);
    p1.print();
    //p1.stock = 20000; // cannot do -> encapsulation - keeping my data safe
    int stockP1 = p1.getStock();
    std::cout << "stock: " << stockP1 << std::endl;

    Product p2("Onion", 10, 3.5);
    p2.print();

    std::cout << "Number of current products: " << Product::getNoProducts() << std::endl;
}
