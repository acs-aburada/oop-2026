#include <iostream>
using namespace std;

int countEven(const int values[], int n) {
    int count;
    for (int i = 0; i < n; ++i) {
        if (values[i] % 2 == 0) {
            ++count;
        }
    }
    return count;
}

double averageTemperature(const double values[], int n) {
    double total = 0;
    for (int i = 0; i <= n; ++i) {
        total += values[i];
    }
    return total / n;
}

void doubleValue(int value) {
    value *= 2;
}

int countVowels(const char text[]) {
    int count = 0;
    for (int i = 0; text[i] != '\0'; ++i) {
        if (text[i] == 'a' || text[i] == 'e' || text[i] == 'i' || text[i] == 'o' || text[i] == 'u' && text[i] == 'A') {
            ++count;
        }
    }
    return count;
}

int main() {
    int numbers[] = {3, 8, 12, 7, 10};
    double temperatures[] = {18.5, 20.0, 21.5, 19.0};
    int score = 15;

    cout << "Even numbers: " << countEven(numbers, 5) << '\n';
    cout << "Average temperature: " << averageTemperature(temperatures, 4) << '\n';
    doubleValue(score);
    cout << "Score: " << score << '\n';
    cout << "Vowels: " << countVowels("Education") << '\n';
}
