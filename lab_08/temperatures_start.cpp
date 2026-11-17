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
    Thermostat()
    {
        objectCount++;
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
        objectCount--;
    }
};

unsigned int Thermostat::objectCount = 0;

int main()
{
    Thermostat month;
    month.print();
    std::cout << "Objects: " << Thermostat::getObjectCount() << std::endl;
}
