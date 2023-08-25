//--Includes-------------------------------------------------------------------
#include <iostream>

//--Consts, enums and lists----------------------------------------------------

//---Forward Declarations------------------------------------------------------
class CCountry;

//---Interface-----------------------------------------------------------------

class CPlanet 
{
    public:
        CPlanet();
        ~CPlanet();

    // Sets the planets name
    void SetPlanetName(std::string PlanetName);

    //Lists the countries on the planets equator to std out
    void ReportEquitorials();

    private:
        std::string m_PlanetName;
        CCountry* pCountry;
        int NumberOfCountries;
        bool IsOnEquator;
};

class CCountry
{
    public:
        CCountry();
        ~CCountry();

        std::string NameOfCountry;
        bool EquatorCheck;
    private:
};


//---Main----------------------------------------------------------------------
int main(void) 
{
    CPlanet Earth;
    Earth.ReportEquitorials();
}

//---Implementation------------------------------------------------------------

CPlanet::CPlanet()
{

    std::cout << "Enter the name of the Planet: ";
    std::cin >> m_PlanetName;

    std::cout << "Enter the amount of countries for that planet: " << std::endl;
    std::cin >> NumberOfCountries;
    
    // Instantiating the number of countries in the planet
    pCountry = new CCountry[NumberOfCountries];
}

CPlanet::~CPlanet()
{
    std::cout << m_PlanetName << " Dtor." << std::endl;
    delete [] pCountry;
}

void CPlanet::ReportEquitorials() 
{   
    std::cout << "The Planets that are on the equator are: " << std::endl;

    for(int i = 0; i < NumberOfCountries; i++) {
        if (pCountry[i].EquatorCheck == 1) {
            std::cout << pCountry[i].NameOfCountry << " is on the equator" << std::endl;
        }
    }
}

CCountry::CCountry()
{
    std::cout << "What is the name of the country: ";
    std::cin >> NameOfCountry;

    std::cout << "Enter a 1 or a 0 if the country lies on the equator: ";
    std::cin >> EquatorCheck;

    std::cout << "Country " << NameOfCountry << " created." << std::endl;
}

CCountry::~CCountry()
{
    std::cout << NameOfCountry << " Dtor." << std::endl; 
}









