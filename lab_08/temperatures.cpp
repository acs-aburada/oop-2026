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

    MonthlyTemperatures(const double* values, int count,
        const std::string& location = "Unknown", Brand brand = Brand::GENERIC,
        float heatingPrice = 0.0F)
        : location(location), brand(brand), heatingPrice(heatingPrice)
    {
        if (heatingPrice < 0)
        {
            throw "Heating price cannot be negative.";
        }
        setReadings(values, count);
        objectCount++;
    }

    MonthlyTemperatures(const MonthlyTemperatures& other)
        : location(other.location), brand(other.brand), heatingPrice(other.heatingPrice)
    {
        setReadings(other.temperatures, other.dayCount);
        objectCount++;
    }

    MonthlyTemperatures(MonthlyTemperatures&& other) noexcept
        : location(std::move(other.location)), brand(other.brand),
          heatingPrice(other.heatingPrice),
          temperatures(other.temperatures), dayCount(other.dayCount)
    {
        other.location.clear();
        other.brand = Brand::GENERIC;
        other.heatingPrice = 0.0F;
        other.temperatures = nullptr;
        other.dayCount = 0;
        objectCount++;
    }

    MonthlyTemperatures& operator=(const MonthlyTemperatures& other)
    {
        if (this != &other)
        {
            std::string copiedLocation = other.location;
            setReadings(other.temperatures, other.dayCount);
            location = std::move(copiedLocation);
            brand = other.brand;
            heatingPrice = other.heatingPrice;
        }
        return *this;
    }

    MonthlyTemperatures& operator=(MonthlyTemperatures&& other) noexcept
    {
        if (this != &other)
        {
            delete[] temperatures;
            location = std::move(other.location);
            brand = other.brand;
            heatingPrice = other.heatingPrice;
            temperatures = other.temperatures;
            dayCount = other.dayCount;
            other.location.clear();
            other.brand = Brand::GENERIC;
            other.heatingPrice = 0.0F;
            other.temperatures = nullptr;
            other.dayCount = 0;
        }
        return *this;
    }

    static unsigned int getObjectCount()
    {
        return objectCount;
    }

    void setReadings(const double* values, int count)
    {
        if (count < 0 || count > MAX_DAYS || (count > 0 && values == nullptr))
        {
            throw "Provide 0 to 31 temperature readings.";
        }

        double* replacement = nullptr;
        if (count > 0)
        {
            replacement = new double[count];
            for (int i = 0; i < count; i++)
            {
                replacement[i] = values[i];
            }
        }
        delete[] temperatures;
        temperatures = replacement;
        dayCount = count;
    }

    void print() const
    {
        std::cout << location << " | brand: " << static_cast<int>(brand)
            << " | heating price: " << heatingPrice
            << " | Readings (" << dayCount << "):";
        for (int i = 0; i < dayCount; i++)
        {
            std::cout << ' ' << temperatures[i];
        }
        std::cout << std::endl;
    }

    MonthlyTemperatures& operator+=(double offset)
    {
        for (int i = 0; i < dayCount; i++)
        {
            temperatures[i] += offset;
        }
        return *this;
    }

    double operator[](int index) const
    {
        if (index < 0 || index >= dayCount)
        {
            throw "Invalid day index.";
        }
        return temperatures[index];
    }

    MonthlyTemperatures& operator++()
    {
        *this += 1;
        return *this;
    }

    MonthlyTemperatures operator++(int)
    {
        MonthlyTemperatures previous(*this);
        ++(*this);
        return previous;
    }

    friend bool operator==(const MonthlyTemperatures& left, const MonthlyTemperatures& right);
    friend std::ostream& operator<<(std::ostream& out, const MonthlyTemperatures& month);
    friend std::istream& operator>>(std::istream& in, MonthlyTemperatures& month);

    ~MonthlyTemperatures()
    {
        delete[] temperatures;
        objectCount--;
    }
};

unsigned int MonthlyTemperatures::objectCount = 0;

MonthlyTemperatures operator+(const MonthlyTemperatures& month, double offset)
{
    MonthlyTemperatures result(month);
    result += offset;
    return result;
}

bool operator==(const MonthlyTemperatures& left, const MonthlyTemperatures& right)
{
    if (left.location != right.location || left.brand != right.brand
        || left.heatingPrice != right.heatingPrice || left.dayCount != right.dayCount)
    {
        return false;
    }
    for (int i = 0; i < left.dayCount; i++)
    {
        if (left.temperatures[i] != right.temperatures[i])
        {
            return false;
        }
    }
    return true;
}

std::ostream& operator<<(std::ostream& out, const MonthlyTemperatures& month)
{
    out << month.location << " | brand: " << static_cast<int>(month.brand)
        << " | heating price: " << month.heatingPrice
        << " | Readings (" << month.dayCount << "):";
    for (int i = 0; i < month.dayCount; i++)
    {
        out << ' ' << month.temperatures[i];
    }
    return out;
}

std::istream& operator>>(std::istream& in, MonthlyTemperatures& month)
{
    MonthlyTemperatures temporary;
    int brandValue = 0;
    if (!(in >> temporary.location >> brandValue
        >> temporary.heatingPrice >> temporary.dayCount))
    {
        return in;
    }
    if (brandValue < 0 || brandValue > 2 || temporary.heatingPrice < 0
        || temporary.dayCount < 0 || temporary.dayCount > MonthlyTemperatures::MAX_DAYS)
    {
        in.setstate(std::ios::failbit);
        return in;
    }
    temporary.brand = static_cast<Brand>(brandValue);
    if (temporary.dayCount > 0)
    {
        temporary.temperatures = new double[temporary.dayCount];
        for (int i = 0; i < temporary.dayCount; i++)
        {
            if (!(in >> temporary.temperatures[i]))
            {
                return in;
            }
        }
    }
    month = std::move(temporary);
    return in;
}

int main()
{
    std::cout << "Objects before block: " << MonthlyTemperatures::getObjectCount() << std::endl;
    try
    {
        double readings[] = {18, 20, 19};
        MonthlyTemperatures original(readings, 3, "Bucharest", Brand::OMRON, 2.5F);
        MonthlyTemperatures copy(original);
        copy += 2;
        std::cout << "Original: " << original << std::endl;
        std::cout << "Copy + 2: " << copy << std::endl;

        MonthlyTemperatures assigned;
        assigned = original;
        std::cout << std::boolalpha << "Equal: " << (assigned == original) << std::endl;

        MonthlyTemperatures moved(std::move(copy));
        assigned = std::move(moved);
        std::cout << "Move-assigned: " << assigned << std::endl;
        std::cout << "Moved-from objects: " << copy << "; " << moved << std::endl;

        MonthlyTemperatures shifted = original + 5;
        std::cout << "Original + 5: " << shifted << std::endl;
        std::cout << "First reading: " << original[0] << std::endl;

        MonthlyTemperatures before = original++;
        std::cout << "Postfix result: " << before << std::endl;
        std::cout << "After postfix: " << original << std::endl;
        MonthlyTemperatures after = ++original;
        std::cout << "Prefix result: " << after << std::endl;
        std::cout << "After prefix: " << original << std::endl;

        std::cout << "Enter a one-word location, brand (0=GENERIC, 1=OMRON, 2=BEURER),"
            << std::endl << "heating price, day count (0-31), then temperatures:" << std::endl;
        if (!(std::cin >> assigned))
        {
            std::cerr << "Invalid input. Unchanged: " << assigned << std::endl;
            return 1;
        }
        std::cout << assigned << std::endl;
        std::cout << "Objects in block: " << MonthlyTemperatures::getObjectCount() << std::endl;
    }
    catch (const char* message)
    {
        std::cerr << message << std::endl;
        return 1;
    }
    std::cout << "Objects after block: " << MonthlyTemperatures::getObjectCount() << std::endl;
}
