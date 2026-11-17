// OOP 2026 Test 1 Subject 5
//
//Maximum working time : 60 minutes
//
//- All the requirements must be tested in main to be considered for evaluation (if the method is not tested in main it will not be evaluated).
//- Solution is evaluated only if there are NO compiler errors and all class attributes are implemented.
//- -1pt (minus) for each shallow copy you do
//- -0.5pts (minus) for each memory leak you do

//0.25p - All variables and methods names respect the mixed Camel Case naming convention.

#include <iostream>
#include <string>

//0.5p
//An electricity supplier manages customer accounts and monitors their monthly electricity consumption. Implement the class Electricity Account with the next private attributes.
    //Customer name - Name of the electricity facility as string
    //Monthly Consumption - Dynamically allocated array of double values representing monthly electricity consumption values, in kWh
    //Number of months - Number of recorded months
    //Price per KWh - Standard electricity price per kWh as a floating - point value
    //Customer type - symbol from next enumeration RESIDENTIAL, BUSINESS, INDUSTRIAL
enum CustomerType { RESIDENTIAL, BUSINESS, INDUSTRIAL };
    //Total Accounts - static attribute to count the total number of existing accounts

class ElectricityAccount {
    std::string customerName = ""; //0.5p - attributes must have default values
    double* monthlyConsumption = nullptr;
    unsigned int numberOfMonths = 0; // NOTE: unsigned int makes most sense since the values shouldn't be negative, but int works too -> validation is needed regardless
    float pricePerKWh = 0;
    CustomerType customerType = CustomerType::RESIDENTIAL;
    static unsigned int totalAccounts;

public:
    const static int MIN_PRICE = 0.5;
    const static int MIN_PRICE_INDUSTRIAL = 1.2;
    const static int RESIDENTIAL_DISCOUNT_THRESHOLD = 150;

//0.25p - Implement a getter for the monthly consumption array and a getter for the customer name.
    std::string getCustomerName() const {
        return this->customerName;
    }

    double* getMonthlyConsumption() const {
        if (this->numberOfMonths == 0) {
            return nullptr;
        }

        double* clone = new double[this->numberOfMonths];

        for (unsigned int i = 0; i < this->numberOfMonths; i++) {
            clone[i] = this->monthlyConsumption[i];
        }

        return clone;
    }

    unsigned int getNumberOfMonths() const {
        return this->numberOfMonths;
    }

//0.25p - Implement a setter for the monthly consumption that validates the received array exists and all values are non-negative
    void setMonthlyConsumption(const double const* arr, const unsigned int size) {
        if (size <= 0 || arr == nullptr) {
            throw std::exception("ERROR: Monthly Consumption array cannot be empty!");
        }
        delete[] this->monthlyConsumption;
        this->monthlyConsumption = new double[size];
        for (unsigned int i = 0; i < size; i++) {
            if (arr[i] < 0) {
                throw std::exception("ERROR: Monthly Consumption cannot be negative!");
            }
            this->monthlyConsumption[i] = arr[i];
        }
        this->numberOfMonths = size;
    }

    ElectricityAccount() { ElectricityAccount::totalAccounts++; }

//0.25p - Implement a constructor that receives ONLY customer name, monthly consumption, number of months and price per KWh. 
// Customer type is set by default to RESIDENTIAL. 
// Customer name must have at least 3 chars and price per KWh must be at least 0.5
    ElectricityAccount(const std::string& customerName, const double const* monthlyConsumption, 
        const unsigned int noMonths, const float pricePerKWh) : customerType(CustomerType::RESIDENTIAL){
        ElectricityAccount::totalAccounts++;

        if (customerName.size() <= 3) {
            throw std::exception("ERROR: Customer name must have at least 3 chars!");
        }
        this->customerName = customerName;

        if (pricePerKWh <= 0.5) {
            throw std::exception("ERROR: Price per KWh must be at least 0.5!");
        }
        this->pricePerKWh = pricePerKWh;

        this->setMonthlyConsumption(monthlyConsumption, noMonths);
    }

//0.25p - Implement a function within the class that will display line by line all its attributes
    void print() const {
        std::cout << std::endl;

        std::cout << "Customer name: " << this->customerName << std::endl;
        std::cout << "Price per KWh: " << this->pricePerKWh << std::endl;
        std::cout << "No months: " << this->numberOfMonths << std::endl;
        if (this->numberOfMonths > 0) {
            for (unsigned int i = 0; i < this->numberOfMonths; i++) {
                std::cout << this->monthlyConsumption[i] << "; ";
            }
        }
        std::cout << std::endl;

        std::cout << "Customer type: ";
        switch (this->customerType) {
            case CustomerType::RESIDENTIAL:
                std::cout << "RESIDENTIAL" << std::endl;
                break;
            case CustomerType::BUSINESS:
                std::cout << "BUSINESS" << std::endl;
                break;
            case CustomerType::INDUSTRIAL:
                std::cout << "INDUSTRIAL" << std::endl;
                break;
            default:
                std::cout << "UNKNOWN" << std::endl;
        }
    }

//0.5p - Implement the constructor with all parameters. Respect the following validation(and the previous rules) validation rules : INDUSTRIAL clients must have a price per KWh of minimum 1.2.
// Don't use magic numbers.
    ElectricityAccount(const std::string& customerName, const double const* monthlyConsumption,
        const unsigned int noMonths, const float pricePerKWh, const CustomerType customerType) {
        ElectricityAccount::totalAccounts++;

        if (customerName.size() <= 3) {
            throw std::exception("ERROR: Customer name must have at least 3 chars!");
        }
        this->customerName = customerName;

        if (pricePerKWh <= ElectricityAccount::MIN_PRICE) {
            throw std::exception("ERROR: Price per KWh must be at least 0.5!");
        }
        else if (customerType == CustomerType::INDUSTRIAL && pricePerKWh <= ElectricityAccount::MIN_PRICE_INDUSTRIAL) {
            throw std::exception("ERROR: Price per KWh for INDUSTRIAL customers must be at least 1.2!");
        }
        this->pricePerKWh = pricePerKWh;

        this->setMonthlyConsumption(monthlyConsumption, noMonths);
        this->customerType = customerType;
    }

//0.25p - Implement the class destructor to clean objects space
    ~ElectricityAccount() {
        ElectricityAccount::totalAccounts--;
        delete[] this->monthlyConsumption;
        std::cout << std::endl << "Destructor called" << std::endl;
    }


//1p - Implement a class method that calculates the electricity bill for the entire period of months.
// The method receives a parameter indicating the client has or has not a green energy contract. Consider next rules :
//
//    • RESIDENTIAL customers receive a 10 % discount on the energy charge if consumption is at most 150 kWh.
//    • BUSINESS customers pay an additional fixed service fee of 25.
//    • All customers with a green energy contract receive an additional 5 % discount on the energy charge after the category discount.
    double calculateElectricityBill(const bool hasGreenContract) {
        if (this->numberOfMonths == 0) {
            return 0;
        }
        double total = 0;
        double currentMonth;
        for (unsigned int i = 0; i < this->numberOfMonths; i++) {
            currentMonth = this->pricePerKWh * this->monthlyConsumption[i];
            
            if (this->customerType == CustomerType::RESIDENTIAL && this->monthlyConsumption[i] <= RESIDENTIAL_DISCOUNT_THRESHOLD) {
                currentMonth *= 0.9;
            }

            if (hasGreenContract) {
                currentMonth *= 0.95;
            }

            total += currentMonth;
        }

        if (this->customerType == CustomerType::BUSINESS) {
            total += 25;
        }

        return total;
    }


//1p - Implement a method that analyzes the monthly electricity consumption history and counts how many months experienced an abnormal increase compared to the previous month, based on a given threshold percent (like 30 %).
//
//    Business rules :
//    • Compare each month with the immediately preceding month.
//    • Count all months satisfying the condition(value bigger than the given threshold percent).
//    • If fewer than two months are recorded, return 0.
    int abnormalGrowth(float threshold) {
        if (this->numberOfMonths < 2) {
            return 0;
        }
        int count = 0;
        for (unsigned int i = 0; i < this->numberOfMonths - 1; i++) {
            // weird edge case
            if (this->monthlyConsumption[i] == 0) {
                if (this->monthlyConsumption[i + 1] > 0) {
                    count++;
                }
            }
            else if ((this->monthlyConsumption[i + 1] - this->monthlyConsumption[i]) / this->monthlyConsumption[i] > threshold) {
                count++;
            }
        }
        return count;
    }
};

unsigned int ElectricityAccount::totalAccounts = 0;

int main()
{
//0.25p - Test the getter for the customer name on an object created with default constructor
    ElectricityAccount acc1;
    std::cout << "Customer Name of acc1: \"" << acc1.getCustomerName() << "\"" << std::endl;

//0.25p - Test the setter for the monthly consumption on previous object and then show the values after change(with the getter)
    double consumption1[4] = { 0.1, 0.3, 1.4, 3.2 };
    acc1.setMonthlyConsumption(consumption1, 4);

    double* consumptionAcc1 = acc1.getMonthlyConsumption();
    std::cout << "Consumption array of acc1: ";
    for (unsigned int i = 0; i < acc1.getNumberOfMonths(); i++) {
        std::cout << consumptionAcc1[i] << "; ";
    }
    std::cout << std::endl;

    double consumption2[3] = { 3.1, -2.9, 1.4 };
    //acc1.setMonthlyConsumption(consumption2, 3); // <- this crashed the program - as intended

//0.5p - Test the previous constructor in main() and create a new object.
    double consumption3[4] = { 4.2, 0.25, 1.7, 5.1 };
    ElectricityAccount acc3("John Doe", consumption3, 4, 2.6);
    double* consumptionAcc3 = acc3.getMonthlyConsumption();
    std::cout << "Consumption array of acc3: ";
    for (unsigned int i = 0; i < acc3.getNumberOfMonths(); i++) {
        std::cout << consumptionAcc3[i] << "; ";
    }
    std::cout << std::endl;

//0.25p - Test the display function in main to show the values for the 2 previous created objects
    acc1.print();
    acc3.print();

//0.5p - Test the previous constructor in main with values that show the previous validation works. 
//    Display the values of the new object with the previous method.
    ElectricityAccount acc4("John Doe", consumption1, 4, 1.6, CustomerType::BUSINESS);
    acc4.print();

    //ElectricityAccount acc5("John Doe", consumption3, 4, 0.6, CustomerType::INDUSTRIAL); // <- this crashed the program - as intended
    //acc5.print();

//0.25p - Test the destructor in main by explicitly forcing the destruction of an object
    ElectricityAccount* acc5 = new ElectricityAccount();
    std::cout << "Testing destructor" << std::endl;
    delete acc5;
    std::cout << "Testing done" << std::endl;

//1p - Test the previous methods in main by testing different scenarios with different objects and printing the booking price or proper messages (0.5p for each method test)
    double residentialConsumption[] = { 100, 200, 120 };
    double businessConsumption[] = { 100, 200, 300 };
    double industrialConsumption[] = { 100, 200, 300 };

    ElectricityAccount residential("Alice", residentialConsumption, 3, 1.0);
    ElectricityAccount business("Bobby", businessConsumption, 3, 1.0, CustomerType::BUSINESS);
    ElectricityAccount industrial("Charlie", industrialConsumption, 3, 1.2, CustomerType::INDUSTRIAL);

    std::cout << "\nTesting electricity bills:\n";

    std::cout << "Residential, no green contract: "
        << residential.calculateElectricityBill(false) << std::endl;

    std::cout << "Residential, green contract: "
        << residential.calculateElectricityBill(true) << std::endl;

    std::cout << "Business, no green contract: "
        << business.calculateElectricityBill(false) << std::endl;

    std::cout << "Business, green contract: "
        << business.calculateElectricityBill(true) << std::endl;

    std::cout << "Industrial, no green contract: "
        << industrial.calculateElectricityBill(false) << std::endl;

    std::cout << "Industrial, green contract: "
        << industrial.calculateElectricityBill(true) << std::endl;

    double consumptionA[] = { 100, 120, 180, 200, 300, 390 };
    double consumptionB[] = { 100, 130, 169, 250 };
    double consumptionC[] = { 0, 100, 110, 200 };
    double consumptionD[] = { 100 };

    ElectricityAccount accA("Test A", consumptionA, 6, 1.0);
    ElectricityAccount accB("Test B", consumptionB, 4, 1.0);
    ElectricityAccount accC("Test C", consumptionC, 4, 1.0);
    ElectricityAccount accD("Test D", consumptionD, 1, 1.0);

    std::cout << "\nTesting abnormal growth (threshold = 0.30):\n";

    std::cout << "Account A: " << accA.abnormalGrowth(0.30) << std::endl;
    std::cout << "Account B: " << accB.abnormalGrowth(0.30) << std::endl;
    std::cout << "Account C: " << accC.abnormalGrowth(0.30) << std::endl;
    std::cout << "Account D: " << accD.abnormalGrowth(0.30) << std::endl;
}
