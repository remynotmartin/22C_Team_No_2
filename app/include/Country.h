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
    string   majorReligion;
    double   GDP;
    double   surfaceArea;
    string   capitalCity;
    
public:
    Country(): name{""}, language{""}, population{0}, majorReligion{""}, GDP{-1}, surfaceArea{-1}, capitalCity{""} {}
    Country(string name, string lang, unsigned population, string religion, double gdp, double area, string capital);
    ~Country() {}
    
    // Setters and getters
    void setName          (string   n)        {name = n;}
    void setLanguage      (string   lang)     {language = lang;}
    void setPopulation    (unsigned pNum)     {population = pNum;}
    void setMajorReligion (string   religion) {majorReligion = religion;}
    void setGDP           (double   g)        {GDP = g;}
    void setSurfaceArea   (double   area)     {surfaceArea = area;}
    void setCapitalCity   (string   cityName) {capitalCity = cityName;}
    
    string   getName          () const {return name;}
    string   getLanguage      () const {return language;}
    unsigned getPopulation    () const {return population;}
    string   getMajorReligion () const {return majorReligion;}
    double   getGDP           () const {return GDP;}
    double   getSurfaceArea   () const {return surfaceArea;}
    string   getCapitalCity   () const {return capitalCity;}
    
    // Overloaded operators
    friend std::ostream &operator<<(std::ostream &out, const Country &rhs);

    bool operator==(const Country     &rhs) const;
    bool operator< (const Country     &rhs) const; 
    bool operator> (const Country     &rhs) const; 

    bool operator==(const std::string &rhs) const;
    bool operator< (const std::string &rhs) const;
    bool operator> (const std::string &rhs) const;

};

#endif // Country.h
