//--Includes-------------------------------------------------------------------
#include <iostream>
#include <string>
#include <sstream>

//--Consts, enums and lists----------------------------------------------------

//---Forward Declarations------------------------------------------------------

class CSpeaker;

//---Interface-----------------------------------------------------------------
class CSpeaker 
{
    public:
        virtual void GreetUser() = 0;
        void GreetCount();
        void PrintCount();

    private:
        int count = 0;

    protected:
        std::string name;

};

class CEnglishSpeaker: public CSpeaker
{
    public:
        CEnglishSpeaker() 
        {
            name = "English Speaker";
        }

        void GreetUser();
    
    private:
};

class CAdvancedEnglishSpeaker: public CEnglishSpeaker
{
    public:
        void GreetUser();
};

class CItalianSpeaker: public CSpeaker
{
    public:
        CItalianSpeaker() 
        {
            name = "Italian Speaker";
        }

        void GreetUser();

    private:

};

class CFrenchSpeaker: public CSpeaker
{
    public:
        CFrenchSpeaker()
        {
            name = "French Speaker";
        }

        void GreetUser();

    private:

};

//---Main----------------------------------------------------------------------
int main(void) 
{

    const int NumberOfSpeakers = 6;
    int count = 0;
    std::string InitialArrayOfSpeakers;
    int ArrayOfSpeakers[NumberOfSpeakers];

    std::cout << "Enter the speaker order (separated by a space): " << std::endl;
    std::getline(std::cin, InitialArrayOfSpeakers);

    std::istringstream iss(InitialArrayOfSpeakers);

    for (int i = 0; i < (NumberOfSpeakers*2)-1; i++) 
    {
        if (i%2 == 0) 
        {
            ArrayOfSpeakers[count] = (InitialArrayOfSpeakers[i] - '0');
            count++;
        }
    }

    CEnglishSpeaker English;
    CFrenchSpeaker French;
    CItalianSpeaker Italian;
    CAdvancedEnglishSpeaker ProficientEnglish;

    CSpeaker** Speakers = new CSpeaker*[NumberOfSpeakers];

    for (int i = 0; i < NumberOfSpeakers; i++) 
    {
        if (ArrayOfSpeakers[i] == 0) 
        {
            Speakers[i] = new CEnglishSpeaker;
        }

        if (ArrayOfSpeakers[i] == 1)
        {
            Speakers[i] = new CFrenchSpeaker;
        }

        if (ArrayOfSpeakers[i] == 2) 
        {
            Speakers[i] = new CItalianSpeaker;
        }
    }

    for (int i = 0; i < NumberOfSpeakers; i++) 
    {
        Speakers[i]->GreetUser();
    }

    delete [] Speakers;
}

//---Implementation------------------------------------------------------------
void CEnglishSpeaker::GreetUser()
{
    std::cout << "Hello World." << std::endl;
    GreetCount();
}

void CItalianSpeaker::GreetUser()
{
    std::cout << "Ciao World." << std::endl;
    GreetCount();
}

void CFrenchSpeaker::GreetUser()
{
    std::cout << "Bonjour World." << std::endl;
    GreetCount();
}

void CAdvancedEnglishSpeaker::GreetUser()
{
    std::cout << "My name is Ari" << std::endl;
    GreetCount();
}

void CSpeaker::GreetCount() 
{
    count++;
}

void CSpeaker::PrintCount() 
{
    std::cout << "The number of greetings are: " << count << " for " << name << std::endl;
}