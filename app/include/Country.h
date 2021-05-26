#ifndef COUNTRY_H
#define COUNTRY_H

// Originally put into code by: Shun Furuya
//
//        Style adjustments by: Remy Dinh

#include <string>

using std::string;

class Country
{
private:
    string   name;          //   Primary Key
    string   language;      // Secondary Key
    unsigned population;
    string   stateReligion;
    double   GDP;
    double   surfaceArea;
    
public:
    Country() {name = ""; language = ""; population = -1; stateReligion = ""; GDP = -1; surfaceArea = -1;}
    Country(string   n;
            string   lang;
            unsigned pNum;
            string   religion;
            double   g;
            double   area;) {name = n; language = lang; population = pNum; stateReligion = religion; GDP = g; surfaceArea = area;}
    
    // Setters and getters
    void setName          (string   n)        {name = n;}
    void setLanguage      (string   lang)     {language = lang;}
    void setPopulation    (unsigned pNum)     {population = pNum;}
    void setStateReligion (string   religion) {stateReligion = religion;}
    void setGDP           (double   g)        {GDP = g;}
    void setSurfaceArea   (double   area)     {surfaceArea = area;}
    
    string   getName          () const {return name;}
    string   getLanguage      () const {return language;}
    unsigned getPopulation    () const {return population;}
    string   getStateReligion () const {return stateReligion;}
    double   getGDP           () const {return GDP;}
    double   getSurfaceArea   () const {return surfaceArea;}
    
    // Overloaded operators
    // TBD
};

#endif // Country.h