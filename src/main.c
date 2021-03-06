/* Convert a temperature from one scale to another */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>

// Integer constants representing which conversion to take place
enum { CF, FC, CK, KC, FK, KF, Invalid };

// Converts celsius to fahrenheit
double celsiusToFahrenheit(double temp) {
    temp = temp * 1.8;
    temp = temp + 32;
    temp = round(temp * 10.0)/10.0;
    return temp;
}

// Converts celsius to kelvin
double celsiusToKelvin(double temp) {
    temp = temp + 273.0;
    temp = round(temp * 10.0)/10.0;
    return temp;
}

// Converts fahrenheit to celsius
double fahrenheitToCelsius(double temp) {
    temp = temp - 32.0;
    temp = temp / 1.8;
    temp = round(temp * 10.0)/10.0;
    return temp;
}

// Converts fahrenheit to kelvin
double fahrenheitToKelvin(double temp) {
    temp = temp - 32.0;
    temp = temp / 1.8;
    //temp = fahrenheitToCelsius(temp);
    //temp = celsiusToKelvin(temp);
    temp = temp + 273.0;
    temp = round(temp * 10.0)/10.0;
    return temp;
}

// Converts kelvin to celsius
double kelvinToCelsius(double temp) {
    temp = temp - 273.0;
    temp = round(temp * 10.0)/10.0;
    return temp;
}

// Converts kelvin to fahrenheit
double kelvinToFahrenheit(double temp) {
    //temp = kelvinToCelsius(temp);
    temp = temp - 273.0;
    //temp = celsiusToFahrenheit(temp);
    temp = temp * 1.8;
    temp = temp + 32;
    temp = round(temp * 10.0)/10.0;
    return temp;
}

// Checks that the input is valid
bool valid(int n, const char current[], int m, const char new[]) {
    // Check minimum/maximum input length
    if (n < 2) return false;
    if (m > 1) return false;

    // Check that last char is made up of C/K/F
    // Then check the two letters are different
    if (current[n-1] != 'C' && current[n-1] != 'F' && current[n-1] != 'K') return false;
    if (new[0] != 'C' && new[0] != 'F' && new[0] != 'K') return false;
    if (current[n-1] == new[0]) return false;

    // Check that all other chars are digits (optional .) then more digits
    int dotCount = 0;
    int i = 0;
    while (i <= n-2) {
        if (isdigit(current[i]) == false) {
            if (current[i] == '.') dotCount++;
            else return false;
            if (dotCount == 2) return false;
        }
        i ++;
    }
    return true;
}

// Returns a integer constant specifying whcih conversion should take place
int scaleType(int n, const char current[], int m, const char new[]) {
    // Reads which scale temperature is currently in
    // Reads which scale to convert to
    if (valid(n, current, m, new) == false) return Invalid;
    else if (current[n-1] == 'C' && new[0] == 'F') return CF;
    else if (current[n-1] == 'C' && new[0] == 'K') return CK;
    else if (current[n-1] == 'F' && new[0] == 'C') return FC;
    else if (current[n-1] == 'F' && new[0] == 'K') return FK;
    else if (current[n-1] == 'K' && new[0] == 'C') return KC;
    return KF;
}

// Extracts the temperature from an array
double getTemp(int n, const char current[]) {
    char array[n-1];
    for (int i = 0; i < n-1; i++) {
        array[i] = current[i];
    }
    double temp = atof(array);
    return temp;
}

// Applies the correct conversion to the input
void convert(int n, const char current[], int m, const char new[]) {
    int conversionType;
    conversionType = scaleType(n, current, m, new);

    if (conversionType == Invalid) printf("Invalid input.\n");
    else {
        //Take the input temperature and apply conversionType
        double temp;
        temp = getTemp(n, current);
        char oldScale, newScale;
        oldScale = current[n-1];
        newScale = new[0];

        printf("%.1f%c is ", temp, oldScale);
        if (conversionType == CF) temp = celsiusToFahrenheit(temp);
        else if (conversionType == CK) temp = celsiusToKelvin(temp);
        else if (conversionType == FC) temp = fahrenheitToCelsius(temp);
        else if (conversionType == FK) temp = fahrenheitToKelvin(temp);
        else if (conversionType == KC) temp = kelvinToCelsius(temp);
        else temp = kelvinToFahrenheit(temp);
        printf("%.1f%c\n", temp, newScale);
    }
}


void testScaleType() {
    assert(scaleType(2, "0C", 1, "F") == CF);
    assert(scaleType(2, "0C", 1, "K") == CK);
    assert(scaleType(2, "0K", 1, "F") == KF);
    assert(scaleType(2, "0K", 1, "C") == KC);
    assert(scaleType(2, "0F", 1, "C") == FC);
    assert(scaleType(2, "0F", 1, "K") == FK);
    assert(scaleType(2, "0C", 1, "C") == Invalid);
}

void testTempConversions() {
    assert(celsiusToKelvin(0.0) == 273.0);
    assert(celsiusToFahrenheit(21.0) == 69.8);
    assert(fahrenheitToCelsius(46.4) == 8.0);
    assert(fahrenheitToKelvin(23.5) == 268.3);
    assert(kelvinToCelsius(305.0) == 32.0);
    assert(kelvinToFahrenheit(305.0) == 89.6);
}

// Tests the program with a variety of inputs
void test() {
    testScaleType();
    testTempConversions();
    printf("All tests pass\n");
}

// Handles input. Prints an error if there are an incorrect number of arguments
// Example input: 60C F
int main(int n, char *args[n]) {
    if (n == 2 && strcmp(args[1],"-t") ==  0) test();
    else if (n == 3) convert(strlen(args[1]), args[1], strlen(args[2]), args[2]);
    else printf("Invalid input.\n");
    return 0;
}
