//--Includes-------------------------------------------------------------------
#include <iostream>
#include <cstdlib>

//--Consts, enums and lists----------------------------------------------------

//---Forward Declarations------------------------------------------------------
class CDoor;
static CDoor* pDoor;

//---Interface-----------------------------------------------------------------

class CDoor 
{
    public:
        CDoor();
        bool CarStatus;
        bool PickedStatus;
        void EditWin();

    private:
        int WinStatus;


};

class CHost
{
    public:
        int PlayerInvite();
        void OpenGoat(int NumberOfDoors);
        void ChooseDoor();
        int ChooseAgain();

    private:
        int DoorNumber;
        int SecondChoiceNumber;
};

class CPlayer
{
    public:
        void PickDoor(int DoorNumber, CDoor* pDoor);

    private:

};

class CGame
{
    public:
        CGame();
        ~CGame();
        int Run();
        void DoorCheck(int DoorNumber);
        void SecondDoorCheck(int SecondChoiceNumber);

    private:
        int NumberOfGames;
        int NumberOfDoors;
        int WinStatus;


};


//---Main----------------------------------------------------------------------
int main(void) 
{
    CGame Game;
    CHost Host;
    CPlayer Player;
    

    int NumberOfDoors = Game.Run();
        
    // Ask Player to pick a door
    int DoorNumber = Host.PlayerInvite();

    // Update the opened status
    Player.PickDoor(DoorNumber, pDoor);

    // Check if the door has a car or a goat
    Game.DoorCheck(DoorNumber);

    // If not a car get the host to show a goat

    Host.OpenGoat(NumberOfDoors);
    // Let player either choose again 

    int SecondChoice = Host.ChooseAgain();

    // Picks a car --> win
    // Picks a goat --> loose 
    // Onto next game if possible 

}

//---Implementation------------------------------------------------------------
CGame::CGame()
{
    WinStatus = 0;
}

CGame::~CGame()
{
    delete [] pDoor;
}

CDoor::CDoor()
{
    PickedStatus = 0;
}

int CGame::Run() 
{   
    std::cout << "Enter the number of games: " << std::endl;
    std::cin >> NumberOfGames;

    std::cout << "Enter the number of doors: " << std::endl;
    std::cin >> NumberOfDoors;

    for (int i = 0; i < NumberOfDoors; i++)
    {
        pDoor = new CDoor[NumberOfDoors];
    }

    // Generate a random number in range of number of doors to define the car
    srand((unsigned) time(NULL));
		
	// Retrieve a random number between 0 and NumberOfDoors
	int random = (rand() % (NumberOfDoors));

    //Set the index and assign the selected door to have the car

    for (int i=0; i<NumberOfDoors; i++)
    {
        if (i == random) 
        {
            pDoor[i].CarStatus = 1;
        }

        else 
            pDoor[i].CarStatus = 0;
        
        std::cout << pDoor[i].CarStatus << std::endl;
    } 
    return NumberOfDoors;
}

int CHost::PlayerInvite()
{
    std::cout << "Pick a door: " << std::endl;
    std::cin >> DoorNumber;
    
    return DoorNumber;
}

void CHost::OpenGoat(int NumberOfDoors)
{
    // Generate a random number in range of number of doors to define the car
    srand((unsigned) time(NULL));
		
	// Retrieve a random number between 0 and NumberOfDoors
	int random = (rand() % (NumberOfDoors));

    if (pDoor[random].CarStatus == 1 || pDoor[random].PickedStatus == 1)
    {
        std::cout << "Host opened a car with a goat or is already picked." << std::endl;
        OpenGoat(NumberOfDoors);
    }    

    else 
        std::cout << "Host opened a door with a goat." << std::endl;
        pDoor[random].PickedStatus = 1;
}

void CPlayer::PickDoor(int DoorNumber, CDoor* pDoor)
{
    // Change the value of the door to picked
    pDoor[DoorNumber].PickedStatus = 1;

    std::cout << "The picked status of the doors are: " << std::endl;

    for (int i=0; i<3; i++)
    {   
        std::cout << pDoor[i].PickedStatus << std::endl;
    } 
}

void CGame::DoorCheck(int DoorNumber)
{
    if (pDoor[DoorNumber].CarStatus == 1) 
    {
        WinStatus = 1;
    }
    std::cout << "The win status of the game is:" << std::endl << WinStatus << std::endl;
}


void CGame::SecondDoorCheck(int SecondChoiceNumber)
{
    if (SecondChoiceNumber == 1) 
    {

    }

    if (SecondChoiceNumber == 0 && WinStatus == 1)
    {
        std::cout << "You have won." << std::endl;
    } 

    if (SecondChoiceNumber == 0 && WinStatus == 0)
    {
        std::cout << "You have lost." << std::endl;
    }



}

int CHost::ChooseAgain()
{
    std::cout << "Enter a 1 if you would like to swap, or a 0 if you would like to stay with your choice:" << std::endl;
    std::cin >> SecondChoiceNumber;

    return SecondChoiceNumber;
}









