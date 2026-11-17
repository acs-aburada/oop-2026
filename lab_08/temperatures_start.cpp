#include <iostream>
#include <string>
#include <utility>

enum class Brand { GENERIC, OMRON, BEURER };

class MonthlyTemperatures
{
    std::string location = "Unknown";
    Brand brand = Brand::GENERIC;
    float heatingPrice = 0.0F;
    double* temperatures = nullptr;
    int dayCount = 0;
    static unsigned int objectCount;

public:
    static const int MAX_DAYS = 31;

    MonthlyTemperatures()
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
            << " | Readings (" << dayCount << "):";

        for (int i = 0; i < dayCount; i++)
        {
            std::cout << ' ' << temperatures[i];
        }

        std::cout << std::endl;
    }

    ~MonthlyTemperatures()
    {
        delete[] temperatures;
        objectCount--;
    }
};

unsigned int MonthlyTemperatures::objectCount = 0;

int main()
{
    MonthlyTemperatures month;
    month.print();
    std::cout << "Objects: " << MonthlyTemperatures::getObjectCount() << std::endl;
}
