// Implementation file for Country Class
// Written by: Remy Dinh
// Primarily exists to define overloaded operators
// outside of the class specification file.

#include <iostream>
#include <iomanip>
#include "Country.h" // This header already calls <string>

Country::Country(std::string nameIn, std::string langIn, unsigned popIn, std::string religionIn,
                 double gdpIn, double areaIn, std::string capitalIn)
{
    name          = nameIn;
    language      = langIn;
    population    = popIn;
    majorReligion = religionIn;
    GDP           = gdpIn;
    surfaceArea   = areaIn;
    capitalCity   = capitalIn;
}

std::ostream& operator<<(std::ostream &out, const Country& rhs)
{
    out << "###########################################" << std::endl;
    out << "            Country: " << rhs.name           << std::endl;
    out << "       Capital City: " << rhs.capitalCity    << std::endl;
    out << "Recognized Language: " << rhs.language       << std::endl;
    out << "         Population: " << rhs.population     << std::endl;
    out << "     Major Religion: " << rhs.majorReligion  << std::endl;
    out << std::fixed << std::setprecision(2);
    out << "          GDP (USD): $"<< rhs.GDP            << std::endl;
    out << "       Surface Area: " << rhs.surfaceArea << " km^2" << std::endl;
    out << "###########################################" << std::endl;
    return out;
}

bool Country::operator==(const Country &rhs) const
{
    return name == rhs.getName();
}

bool Country::operator< (const Country &rhs) const
{
    return name < rhs.getName();
}

bool Country::operator> (const Country &rhs) const
{
    return !(name < rhs.getName());
}

bool Country::operator==(const std::string &rhs) const
{
    return name == rhs;
}

bool Country::operator< (const std::string &rhs) const
{
    return name < rhs;
}

bool Country::operator> (const std::string &rhs) const
{
    return !(name < rhs);
}

