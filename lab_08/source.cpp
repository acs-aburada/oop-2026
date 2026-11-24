#include <iostream>
#include <string>

enum class Brand { GENERIC, OMRON, BEURER };

class Thermostat
{
    std::string location = "Unknown";
    Brand brand = Brand::GENERIC;
    float heatingPrice = 0.0F;
    double* temperatures = nullptr;
    int entriesTemp = 0;
    static unsigned int objectCount;

public:
    static const float MIN_TEMPERATURE;
    static const float MAX_TEMPERATURE;

    // Default constructor
    Thermostat()
    {
        objectCount++;
    }

    // Parameterized constructor
    Thermostat(const std::string& location, Brand brand,
        float heatingPrice, const double* temperatures, int entriesTemp)
    {
        this->location = location;
        this->brand = brand;
        this->heatingPrice = heatingPrice;

        setTemperatures(temperatures, entriesTemp);

        objectCount++;
    }

    // Copy constructor
    // gets called by default when we pass by value an object of type Thermostat to a function
    // the default behaviour is to copy (SHALLOW-COPY) all fields of the object to the clone
    Thermostat(const Thermostat& obj) : location(obj.location), brand(obj.brand),
        heatingPrice(obj.heatingPrice) {
        std::cout << std::endl << "Copy constructor" << std::endl;

        setTemperatures(obj.temperatures, obj.entriesTemp); // inside a class member function
        // we can directly access the fields of the object

        objectCount++;
    }

    void setTemperatures(const double* temperatures, int entriesTemp)
    {
        if (entriesTemp < 0)
        {
            throw "Number of entries cannot be negative.";
        }

        if (entriesTemp > 0 && temperatures == nullptr)
        {
            throw "Temperature array cannot be nullptr.";
        }

        for (int i = 0; i < entriesTemp; i++)
        {
            if (temperatures[i] < MIN_TEMPERATURE ||
                temperatures[i] > MAX_TEMPERATURE)
            {
                throw "Temperature is outside the allowed range.";
            }
        }

        double* newTemperatures = nullptr;

        if (entriesTemp > 0)
        {
            newTemperatures = new double[entriesTemp];

            for (int i = 0; i < entriesTemp; i++)
            {
                newTemperatures[i] = temperatures[i];
            }
        }

        delete[] this->temperatures;
        this->temperatures = newTemperatures;
        this->entriesTemp = entriesTemp;
    }

    const double* getTemperatures() const
    {
        return temperatures;
    }

    int getEntriesTemp() const
    {
        return entriesTemp;
    }

    static unsigned int getObjectCount()
    {
        return objectCount;
    }

    void print() const
    {
        std::string brandName;

        switch (brand)
        {
        case Brand::GENERIC:
            brandName = "GENERIC";
            break;
        case Brand::OMRON:
            brandName = "OMRON";
            break;
        case Brand::BEURER:
            brandName = "BEURER";
            break;
        default:
            brandName = "UNKNOWN";
        }
        // -> after break;
        std::cout << location << " | brand: " << brandName
            << " | heating price: " << heatingPrice
            << " | Readings (" << entriesTemp << "):";

        for (int i = 0; i < entriesTemp; i++)
        {
            std::cout << ' ' << temperatures[i];
        }

        std::cout << std::endl;
    }

    // assignment operator
    Thermostat& operator=(const Thermostat& other) {
        //Thermostat example;
        //return example; // -> the object returned is actually a copy of example
        //// the LOCAL (from function stack) example is DESTROYED at the end of the function

        // First check if we are trying to assign the object to itself
        if (this != &other) {
            this->location = other.location;
            this->heatingPrice = other.heatingPrice;
            this->brand = other.brand;
            setTemperatures(other.temperatures, other.entriesTemp);
            // ! DO NOT INCREMENT THE OBJECT COUNTER, BECAUSE this ALREADY EXISTS
        }

        return *this;
    }

    //Thermostat& badExample(const Thermostat& other) {
    //    Thermostat example;
    //    return example; // -> the object returned is actually a reference to example
    //    // !!! BUT, example gets destroyed at the end of the function -> dangling pointer
    //}

    // operator+ -> returns a new object
    Thermostat operator+(double value) {
        Thermostat result = *this; // called copy ctor to create the baseline of my result
        // one way of implementing the addition logic:
        for (unsigned int i = 0; i < result.entriesTemp; i++) {
            result.temperatures[i] += value; // NOTE: should validate with thresholds
        }
        //// another way of implementing the addition logic:
        //result.heatingPrice += value;
        return result;
    }

    Thermostat& operator+=(double value) {
        // TODO
    }

    ~Thermostat()
    {
        delete[] temperatures;
        temperatures = nullptr;
        objectCount--;
    }
};

const float Thermostat::MIN_TEMPERATURE = -50.0F;
const float Thermostat::MAX_TEMPERATURE = 60.0F;

void aFunction(Thermostat a);

unsigned int Thermostat::objectCount = 0;

int main()
{
    double readings[] = { 20.5, 21.0, 19.8, 22.1 };

    Thermostat t1;
    Thermostat t2("Living Room", Brand::OMRON, 0.25F, readings, 4);

    t1.print();
    t2.print();

    std::cout << "Objects: " << Thermostat::getObjectCount() << std::endl;

    Thermostat t3(t2);
    t3.print();

    Thermostat t4 = t2; // copy initialization -> copy ctor gets called ONLY if declaration and initialization is on the same line
    t4.print();

    Thermostat t5;
    t5 = t2; // operator= - assignment operator
    // t5.operator=(t2) -> this operator actually has two params: this (current obj), t2
    t5.print();

    t2 + 1; // t2.operator+(1); -> it returns an object as a result where the addition logic was applied
    t2.print();

    t2 = t2 + 1;
    t2.print();

    t2 += 1; // t2.operator+=(1)
    t2.print();
}
