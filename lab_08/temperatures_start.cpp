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
        }

        std::cout << location << " | brand: " << brandName
            << " | heating price: " << heatingPrice
            << " | Readings (" << entriesTemp << "):";

        for (int i = 0; i < entriesTemp; i++)
        {
            std::cout << ' ' << temperatures[i];
        }

        std::cout << std::endl;
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

unsigned int Thermostat::objectCount = 0;

int main()
{
    double readings[] = { 20.5, 21.0, 19.8, 22.1 };

    Thermostat t1;
    Thermostat t2("Living Room", Brand::OMRON, 0.25F, readings, 4);

    t1.print();
    t2.print();

    std::cout << "Objects: " << Thermostat::getObjectCount() << std::endl;
}
