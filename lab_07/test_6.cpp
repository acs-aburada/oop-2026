// OOP 2026 Test 1 Subject 6
//
//Maximum working time : 60 minutes
//
//- All the requirements must be tested in main to be considered for evaluation.
//- Solution is evaluated only if there are NO compiler errors and all class attributes are implemented.
//- -1pt (minus) for each shallow copy you do
//- -0.5pts (minus) for each memory leak you do

//0.25p - All variables and methods names respect the mixed Camel Case naming convention.

#include <iostream>
#include <string>
#include <cstring>

//0.5p - A car rental company manages a fleet of vehicles.
// Implement the class Rental Car with the next private attributes.
    //License plate - Dynamically allocated vehicle registration number as an array of chars
    //Car Brand - Vehicle manufacturer as a string
    //Daily Rate - Standard daily rental price as a floating-point value
    //Total mileage - Total kilometers traveled by the vehicle as an integer value
    //Car type - symbol from next enumeration ECONOMY, SUV, LUXURY
enum CarType { ECONOMY, SUV, LUXURY };
//Total Rental Cars - static attribute to count the total number of existing rental cars

class RentalCar {
    char* licensePlate = nullptr; //0.5p - attributes must have default values
    std::string carBrand = "";
    float dailyRate = 0;
    int totalMileage = 0;
    CarType carType = CarType::ECONOMY;
    static unsigned int totalRentalCars;

public:
    static const float MIN_RATE_LUXURY;
    static const int ECONOMY_SERVICE_KM = 15000;
    static const int SUV_SERVICE_KM = 10000;
    static const int LUXURY_SERVICE_KM = 8000;
    static const int ECONOMY_SERVICE_MONTHS = 12;
    static const int SUV_SERVICE_MONTHS = 9;
    static const int LUXURY_SERVICE_MONTHS = 6;
    static const int LONG_RENTAL_THRESHOLD = 7;
    static const int FREE_KILOMETERS = 100;
    static const int INSURANCE_PER_DAY = 20;

    //0.25p - Implement a getter for the license plate and a getter for the daily rate.
    char* getLicensePlate() const {
        if (this->licensePlate == nullptr) {
            return nullptr;
        }

        char* clone = new char[std::strlen(this->licensePlate) + 1];
        strcpy_s(clone, std::strlen(this->licensePlate) + 1, this->licensePlate);
        return clone;
    }

    float getDailyRate() const {
        return this->dailyRate;
    }

    static unsigned int getTotalRentalCars() {
        return RentalCar::totalRentalCars;
    }

    //0.25p - Implement a setter for the license plate that validates the received value
    // has at least 5 characters and the 2nd or the 3rd character is '-' (minus).
    void setLicensePlate(const char* licensePlate) {
        if (licensePlate == nullptr || std::strlen(licensePlate) < 5) {
            throw "ERROR: License plate must have at least 5 characters!";
        }
        if (licensePlate[1] != '-' && licensePlate[2] != '-') {
            throw "ERROR: The 2nd or the 3rd character must be '-'!";
        }

        char* clone = new char[std::strlen(licensePlate) + 1];
        strcpy_s(clone, std::strlen(licensePlate) + 1, licensePlate);
        delete[] this->licensePlate;
        this->licensePlate = clone;
    }

    RentalCar() {
        RentalCar::totalRentalCars++;
    }

    //0.25p - Implement a constructor that receives ONLY license plate, car brand and daily rate.
    // Car type is set by default to ECONOMY.
    // Car brand must have at least 3 chars and daily rate must be positive.
    RentalCar(const char* licensePlate, const std::string& carBrand,
        const float dailyRate) : carType(CarType::ECONOMY) {
        if (carBrand.size() < 3) {
            throw "ERROR: Car brand must have at least 3 characters!";
        }
        if (!(dailyRate > 0)) {
            throw "ERROR: Daily rate must be positive!";
        }

        this->carBrand = carBrand;
        this->dailyRate = dailyRate;
        this->setLicensePlate(licensePlate);
        RentalCar::totalRentalCars++;
    }

    //0.25p - Implement a function within the class that displays line by line all its attributes.
    void print() const {
        std::cout << std::endl;
        std::cout << "License plate: "
            << (this->licensePlate == nullptr ? "" : this->licensePlate) << std::endl;
        std::cout << "Car brand: " << this->carBrand << std::endl;
        std::cout << "Daily rate: " << this->dailyRate << std::endl;
        std::cout << "Total mileage: " << this->totalMileage << std::endl;
        std::cout << "Car type: ";
        switch (this->carType) {
        case CarType::ECONOMY:
            std::cout << "ECONOMY" << std::endl;
            break;
        case CarType::SUV:
            std::cout << "SUV" << std::endl;
            break;
        case CarType::LUXURY:
            std::cout << "LUXURY" << std::endl;
            break;
        }
        std::cout << "Total rental cars: " << RentalCar::totalRentalCars << std::endl;
    }

    //0.5p - Implement the constructor with all parameters.
    // Respect the previous rules and validate that LUXURY cars have a daily rate of at least 300.
    // Don't use magic numbers. The static counter is managed by the class, not passed as a parameter.
    RentalCar(const char* licensePlate, const std::string& carBrand,
        const float dailyRate, const int totalMileage, const CarType carType) {
        if (carBrand.size() < 3) {
            throw "ERROR: Car brand must have at least 3 characters!";
        }
        if (!(dailyRate > 0)) {
            throw "ERROR: Daily rate must be positive!";
        }
        if (carType == CarType::LUXURY && dailyRate < RentalCar::MIN_RATE_LUXURY) {
            throw "ERROR: Daily rate for LUXURY cars must be at least 300!";
        }
        if (totalMileage < 0) {
            throw "ERROR: Total mileage cannot be negative!";
        }

        this->carBrand = carBrand;
        this->dailyRate = dailyRate;
        this->totalMileage = totalMileage;
        this->carType = carType;
        this->setLicensePlate(licensePlate);
        RentalCar::totalRentalCars++;
    }

    //0.25p - Implement the class destructor to clean objects space.
    ~RentalCar() {
        RentalCar::totalRentalCars--;
        delete[] this->licensePlate;
        this->licensePlate = nullptr;
        std::cout << std::endl << "Destructor called" << std::endl;
    }

    //1p - Determine whether a vehicle must undergo maintenance before it can be rented again.
    // ECONOMY: at least 15000 km since last service or 12 months.
    // SUV: at least 10000 km since last service or 9 months.
    // LUXURY: at least 8000 km since last service or 6 months.
    // If any received parameter is negative, return false.
    bool needsMaintenance(const int mileageAtLastService, const int monthsSinceLastService) const {
        if (mileageAtLastService < 0 || monthsSinceLastService < 0) {
            return false;
        }

        int kilometersSinceLastService = this->totalMileage - mileageAtLastService;
        switch (this->carType) {
        case CarType::ECONOMY:
            return kilometersSinceLastService >= ECONOMY_SERVICE_KM
                || monthsSinceLastService >= ECONOMY_SERVICE_MONTHS;
        case CarType::SUV:
            return kilometersSinceLastService >= SUV_SERVICE_KM
                || monthsSinceLastService >= SUV_SERVICE_MONTHS;
        case CarType::LUXURY:
            return kilometersSinceLastService >= LUXURY_SERVICE_KM
                || monthsSinceLastService >= LUXURY_SERVICE_MONTHS;
        }
        throw "ERROR: Invalid car type!";
    }

    //1p - Calculate the final rental cost based on duration, distance, category and insurance.
    // Base cost: daily rate * rental days.
    // More than 7 days: 10% discount on the base cost.
    // Each rental includes 100 free km, not 100 km per day.
    // Extra km: ECONOMY 0.5, SUV 0.8, LUXURY 1.5.
    // Insurance: 20 per rental day.
    double calculateRentalCost(const int rentalDays, const int drivenKilometers,
        const bool hasInsurance) const {
        if (rentalDays <= 0 || drivenKilometers < 0) {
            throw "ERROR: Rental days must be positive and driven kilometers non-negative!";
        }

        double total = static_cast<double>(this->dailyRate) * rentalDays;
        if (rentalDays > LONG_RENTAL_THRESHOLD) {
            total *= 0.9;
        }

        if (drivenKilometers > FREE_KILOMETERS) {
            int extraKilometers = drivenKilometers - FREE_KILOMETERS;
            switch (this->carType) {
            case CarType::ECONOMY:
                total += extraKilometers * 0.5;
                break;
            case CarType::SUV:
                total += extraKilometers * 0.8;
                break;
            case CarType::LUXURY:
                total += extraKilometers * 1.5;
                break;
            }
        }

        if (hasInsurance) {
            total += static_cast<double>(INSURANCE_PER_DAY) * rentalDays;
        }
        return total;
    }
};

unsigned int RentalCar::totalRentalCars = 0;
const float RentalCar::MIN_RATE_LUXURY = 300.0f;

int main()
{
//0.25p - Test the license plate getter on an object created with the default constructor.
    RentalCar car1;
    char* plateCar1 = car1.getLicensePlate();
    std::cout << "License plate of car1: \""
        << (plateCar1 == nullptr ? "" : plateCar1) << "\"" << std::endl;
    delete[] plateCar1;

    //0.25p - Test the setter on the previous object, then show the values with the getter.
    car1.setLicensePlate("B-123ABC");
    plateCar1 = car1.getLicensePlate();
    std::cout << "License plate after change: " << plateCar1 << std::endl;
    delete[] plateCar1;

    //car1.setLicensePlate(nullptr); // <- stops the program - as intended
    //car1.setLicensePlate("B-12"); // <- stops the program - as intended
    //car1.setLicensePlate("B123ABC"); // <- stops the program - as intended

//0.5p - Test the previous constructor in main() and create a new object.
    RentalCar car2("CJ-123ABC", "Dacia", 100.0f);
    std::cout << "Daily rate of car2: " << car2.getDailyRate() << std::endl;
    //RentalCar invalidBrand("B-123ABC", "VW", 100.0f); // <- stops the program - as intended
    //RentalCar invalidRate("B-123ABC", "Dacia", 0.0f); // <- stops the program - as intended

//0.25p - Test the display function on the two previous objects.
    car1.print();
    car2.print();

    //0.5p - Test the full constructor and show that validation works.
    RentalCar car3("BV-456ABC", "Toyota", 200.0f, 25000, CarType::SUV);
    RentalCar car4("B-789ABC", "Mercedes", RentalCar::MIN_RATE_LUXURY, 18000, CarType::LUXURY);
    car3.print();
    car4.print();
    //RentalCar invalidLuxury("B-123ABC", "Mercedes", 299.0f, 18000, CarType::LUXURY);
    //invalidLuxury.print(); // <- constructor stops the program - as intended

//0.25p - Explicitly force destruction of a dynamically allocated object.
    RentalCar* car5 = new RentalCar();
    std::cout << "Testing destructor, total cars: " << RentalCar::getTotalRentalCars() << std::endl;
    delete car5;
    std::cout << "Testing done, total cars: " << RentalCar::getTotalRentalCars() << std::endl;

    //1p - Test both business methods with different scenarios (0.5p for each method test).
    RentalCar economy("IF-123ABC", "Dacia", 100.0f, 30000, CarType::ECONOMY);
    std::cout << std::boolalpha << "\nTesting maintenance:\n";
    std::cout << "Economy, exactly 15000 km: " << economy.needsMaintenance(15000, 1) << std::endl;
    std::cout << "Economy, exactly 12 months: " << economy.needsMaintenance(30000, 12) << std::endl;
    std::cout << "Economy, below both limits: " << economy.needsMaintenance(15001, 11) << std::endl;
    std::cout << "SUV, exactly 10000 km: " << car3.needsMaintenance(15000, 1) << std::endl;
    std::cout << "SUV, exactly 9 months: " << car3.needsMaintenance(25000, 9) << std::endl;
    std::cout << "SUV, below both limits: " << car3.needsMaintenance(15001, 8) << std::endl;
    std::cout << "Luxury, exactly 8000 km: " << car4.needsMaintenance(10000, 1) << std::endl;
    std::cout << "Luxury, exactly 6 months: " << car4.needsMaintenance(18000, 6) << std::endl;
    std::cout << "Luxury, below both limits: " << car4.needsMaintenance(10001, 5) << std::endl;
    std::cout << "Negative last service mileage: " << economy.needsMaintenance(-1, 12) << std::endl;
    std::cout << "Negative months: " << economy.needsMaintenance(0, -1) << std::endl;

    std::cout << "\nTesting rental costs:\n";
    std::cout << "Economy, 7 days, 100 km, no insurance: "
        << economy.calculateRentalCost(7, 100, false) << std::endl; //700
    std::cout << "Economy, 8 days, 150 km, insurance: "
        << economy.calculateRentalCost(8, 150, true) << std::endl; //905
    std::cout << "SUV, 3 days, 200 km, no insurance: "
        << car3.calculateRentalCost(3, 200, false) << std::endl; //680
    std::cout << "SUV, 8 days, 200 km, insurance: "
        << car3.calculateRentalCost(8, 200, true) << std::endl; //1680
    std::cout << "Luxury, 2 days, 200 km, insurance: "
        << car4.calculateRentalCost(2, 200, true) << std::endl; //790
    std::cout << "Luxury, 8 days, 100 km, no insurance: "
        << car4.calculateRentalCost(8, 100, false) << std::endl; //2160
    //economy.calculateRentalCost(0, 100, false); // <- stops the program - as intended

    return 0;
}
