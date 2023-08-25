// 3-bit binary parallel adder. 
//
// The circuit is set up to take 2, 3-bit binary inputs and add them numerically.
// However, you can keep on instantiating full and half adders to create larger additions.
// It includes all parts A, B and C
// A and B: Can simulate AND, OR and XOR gates through the use of full and half adders
// C: User input for a 3 bit binary adder. This instantiates and simulates 1 half adder and 2 full adders.
// Parts A and B are encapsulated in C.

//--Includes-------------------------------------------------------------------
#include <iostream>

//--Consts, enums and lists----------------------------------------------------
const int InputsPerGate = 2;                                                // Number of inputs per nand gate
const int MaxFanout = 2;                                                    // Maximum fanout: max gate inputs that one gate output can drive
const int NumAndGates = 1;                                                  // Number of AND gates used in a single instatiation
const int NumOrGates = 1;                                                   // Number of OR gates used in a single instatiation
const int NumXorGates = 1;                                                  // Number of XOR gates used in a single instatiation
const int NumWires = 4;                                                     // Number of wires instatiation                                                
const int MaxBinaryInput = 3;                                               // Max number of inputs by the user

enum eLogicLevel                                                            // Enums used to define logic levels numerically
{
  LOGIC_UNDEFINED = -1,
  LOGIC_LOW,
  LOGIC_HIGH
};

//---Forward Declarations------------------------------------------------------
class CGate;                                                                    // Forward declaration 

//---CWire Interface-----------------------------------------------------------
// CWire is used to connect devices in this simulation
// A CWire has a single input, and may drive multiple outputs
// The global variable MaxFanout controls how many outputs each wire can have
// Each wire output drives a specific input of a specific gate
// The wire's input is controlled via the DriveLevel function
class CWire
{
    public:
        CWire();        
        // AddOutputConnection adds to the list of outputs that this wire drives
        // It accepts as parameters the nand gate whose input should be driven
        // and the index specifying which of that gate's inputs should be driven.
        void AddOutputConnection( CGate* apGateToDrive, int aGateInputToDrive );
        
        // DriveLevel drives the wire's value, so that each of its connected outputs
        // get set to the corresponding level
        void DriveLevel( eLogicLevel aNewLevel );
        
    private:
        int mNumOutputConnections;                                              // How many outputs are connected
        CGate* mpGatesToDrive[MaxFanout];                                       // List of connected gates
        int mGateInputIndices[MaxFanout];                                       // List of input to drive in each gate
};

//---CGate Interface-------------------------------------------------------
// Parent class for all child Gate classes
// This handles all connections needed for instansiated child gates and the main 
// componenets such as connecting outputs, reading the output of a gate and 
// "setting" or driving an input.
class CGate
{
    public:
        CGate();
        // ConnectOutput takes pointer OutputConnection and "connects" it to the address
        // of the output of the gate being read through a wire "holding" the logic level 
        void ConnectOutput( CWire* apOutputConnection );
    
        // Returns the output value of a logic gate
        eLogicLevel GetOutputState();

        //  Takes inputs and computes the output for that logic gate
        void DriveInput( int aInputIndex, eLogicLevel aNewLevel );              

    
    protected:
        virtual void ComputeOutput();                                           // Computes the logic output for the gate, returns mOutputValue              
        eLogicLevel mInputs[InputsPerGate];                                     // Array of values that are inputs for the gate
        eLogicLevel mOutputValue;                                               // Output value for the logic gate
        CWire* mpOutputConnection;                                              // Connects output of gate to another or is a set as an output of the logic circuit (if NULL)
};

//---Logic Gate Implementation----------------------------------------------
// Each gate when instantiated will have gate functionality, just with different logic 
// AND gate logic
class CANDGate: public CGate
{
    public:
        void ComputeOutput();
};

/// OR gate logic 
class CORGate: public CGate
{
    public:
        void ComputeOutput();
};

// XOR gate logic
class CXORGate:public CGate
{
    public:
        void ComputeOutput();
};

//---HalfAdder Interface----------------------------------------------------
// Parent class for both full adder and the 3-bit parallel adder as they are 
// made up of half adders
// Includes all the gates and wires when instantiated 
// For the case of the 3-bit adder Half adders need to return a result with sum 
// and carry which is in a struct data structure.
class CHalfAdder
{
    protected: 
        struct AdderResult                                                          // Encapsulation of the Half adders output
        {
            eLogicLevel Sum;
            eLogicLevel Carry;
        };

        CANDGate MyAndGates[NumAndGates];                                           // AND gates required for 1 half adder
        CXORGate MyXorGates[NumXorGates];                                           // XOR gates required for 1 half addder
        CWire MyWires[NumWires];                                                    // Wires required for 1 half adder

    public:
        AdderResult HalfAdderOutput(eLogicLevel Logic1, eLogicLevel Logic2);        // Computing the logic for a hald adder and returning the result
};

//---FullAdder Interface----------------------------------------------------
// Child class of a half adder
// All logic encapsualted in FullAdderOutput returning the adder result
class CFullAdder: public CHalfAdder
{
    public:
        AdderResult FullAdderOutput(eLogicLevel FullAdderInput1, eLogicLevel FullAdderInput2, eLogicLevel FullAdderInput3);

    private:
        CORGate MyOrGates[NumOrGates];                                              // OR gates required for a full adder
};

//---ParallelAdder Interface-------------------------------------------------
// Takes inputs from the user and does binary parallel addition using 1 parallel adder
// Made up of 1 half adder and 2 full adders --> 5 half adders
class CParallelAdder: public CHalfAdder
{
    public:
        std::string FirstInputNumber;                                               // User input for the first 3 bit number
        std::string SecondInputNumber;                                              // User input for the second 3 bit number
        CHalfAdder::AdderResult HalfAdder1sOutput;                                  // Half adders output for the LSB
        CHalfAdder::AdderResult FullAdder2sOutput;                                  // Half adders output for the second MSB
        CHalfAdder::AdderResult FullAdder4sOutput;                                  // Full adders output for the third MSB and MSB

        eLogicLevel A0, A1, A2, B0, B1, B2;                                         // Each of the inputs: A is first 3-bit number, B the second, in order from LSB to MSB

    public:
        // Get user input
        int ObtainInput(eLogicLevel FirstNumber[MaxBinaryInput],eLogicLevel SecondNumber[MaxBinaryInput] );                 

        // Output of the Parallel Adder
        void ParallelAdderOutput(eLogicLevel FirstNumber[MaxBinaryInput], eLogicLevel SecondNumber[MaxBinaryInput]);
};

//---CTestParallelAdder Interface---------------------------------------------
// Test and run the parallel adder 
class CTestParallelAdder
{
    public:
         void Test();
};

//---main----------------------------------------------------------------------
int main()
{   
    CTestParallelAdder TestCase;
    TestCase.Test();
}

//---CWire Implementation------------------------------------------------------
// Initialise number of output connections
CWire::CWire()
{
    mNumOutputConnections = 0;                
}

// Connect the output of a gate to a specific gates input
void CWire::AddOutputConnection( CGate* apGateToDrive, int aGateInputToDrive )
{
    mpGatesToDrive[mNumOutputConnections] = apGateToDrive;                    
    mGateInputIndices[mNumOutputConnections] = aGateInputToDrive;             
    ++mNumOutputConnections;                                                  
}

// Drives the wires value making sure its output is the same
void CWire::DriveLevel( eLogicLevel aNewLevel )
{
    for( int i=0; i<mNumOutputConnections; ++i )
    {
        mpGatesToDrive[i]->DriveInput( mGateInputIndices[i], aNewLevel );   
    }
}  

//---CGate Implementation--------------------------------------------------
// Gate inputs set to undefined and output connections to null
// Setting all inital values
CGate::CGate()
{
    mInputs[0] = mInputs[1] = LOGIC_UNDEFINED;
    mpOutputConnection = NULL;
    ComputeOutput();
}

// Connects the corresponding output of the gate to the wire
void CGate::ConnectOutput( CWire* apOutputConnection )
{
    mpOutputConnection = apOutputConnection;
}

// Takes the inputs and computes the output for that gate
void CGate::DriveInput( int aInputIndex, eLogicLevel aNewLevel )
{
    mInputs[aInputIndex] = aNewLevel;
    ComputeOutput();
}

// Returns output state of circuit logic
eLogicLevel CGate::GetOutputState() 
{ 
    return mOutputValue;                                          
}

// Computes output for the 2 given inputs
void CGate::ComputeOutput()
{
  // Undefined logic returning undefined logic
    eLogicLevel NewVal = LOGIC_HIGH;
    if( mInputs[0] == LOGIC_UNDEFINED || mInputs[1] == LOGIC_UNDEFINED )
        NewVal = LOGIC_UNDEFINED;

    // Logic for NAND gate setting output value for that gate
    else if( mInputs[0] == LOGIC_HIGH && mInputs[1] == LOGIC_HIGH )
        NewVal = LOGIC_LOW;
    mOutputValue = NewVal;
    
    // If there is no output connection this will be the output value that we will return
    if( mpOutputConnection != NULL )
        mpOutputConnection->DriveLevel( mOutputValue );
}

void CANDGate::ComputeOutput()
{
    // Undefined logic returning undefined logic
    eLogicLevel NewVal = LOGIC_LOW;
    if( mInputs[0] == LOGIC_UNDEFINED || mInputs[1] == LOGIC_UNDEFINED )
        NewVal = LOGIC_UNDEFINED;

    // Logic for AND gate setting output value for that gate
    else if( (mInputs[0] == LOGIC_HIGH && mInputs[1] == LOGIC_HIGH) )
    {
        NewVal = LOGIC_HIGH;
    }

    mOutputValue = NewVal;
    
    // If there is no output connection this will be the output value that we will return
    if( mpOutputConnection != NULL )
        mpOutputConnection->DriveLevel( mOutputValue );
}

void CORGate::ComputeOutput()
{
     // Undefined logic returning undefined logic
    eLogicLevel NewVal = LOGIC_LOW;
    if( mInputs[0] == LOGIC_UNDEFINED || mInputs[1] == LOGIC_UNDEFINED )
        NewVal = LOGIC_UNDEFINED;

    // Logic for AND gate setting output value for that gate
    else if( (mInputs[0] == LOGIC_HIGH || mInputs[1] == LOGIC_HIGH) )
    {
        NewVal = LOGIC_HIGH;
    }

    mOutputValue = NewVal;
    
    // If there is no output connection this will be the output value that we will return
    if( mpOutputConnection != NULL )
        mpOutputConnection->DriveLevel( mOutputValue );
}

void CXORGate::ComputeOutput()
{
     // Undefined logic returning undefined logic
    eLogicLevel NewVal = LOGIC_LOW;
    if( mInputs[0] == LOGIC_UNDEFINED || mInputs[1] == LOGIC_UNDEFINED )
    {
        NewVal = LOGIC_UNDEFINED;

    }

    // Logic for AND gate setting output value for that gate
    else if( ((mInputs[0] == LOGIC_HIGH) ^ (mInputs[1] == LOGIC_HIGH)) )
    {
        NewVal = LOGIC_HIGH;
    }

    mOutputValue = NewVal;
    
    // If there is no output connection this will be the output value that we will return
    if( mpOutputConnection != NULL )
        mpOutputConnection->DriveLevel( mOutputValue );
}

//---CHalfAdder implementation-----------------------------------------
// Connecting the required wires to the required gates
// Returns a struct containing the sum and carry of the half adder output
// Takes two inputs Input A and B
CHalfAdder::AdderResult CHalfAdder::HalfAdderOutput(eLogicLevel LogicA, eLogicLevel LogicB)
{
    //XOR is sum
    MyWires[0].AddOutputConnection( &MyXorGates[0], 0 );
    MyWires[1].AddOutputConnection( &MyXorGates[0], 1 );

    //AND is carry
    MyWires[0].AddOutputConnection( &MyAndGates[0], 0 );
    MyWires[1].AddOutputConnection( &MyAndGates[0], 1 );

    //Drive required wires to the logic inputs
    MyWires[0].DriveLevel( LogicA );
    MyWires[1].DriveLevel( LogicB );            

    // Instantiate a Result struct that will be returned
    CHalfAdder::AdderResult Result;
    
    // Set Sum and carry
    Result.Sum = MyXorGates[0].GetOutputState();
    Result.Carry = MyAndGates[0].GetOutputState();
    
    // Return result
    return Result;
}

//---CFullAdder implementation-----------------------------------------
// Full Adder logic returning a single struct containing the output values
CHalfAdder::AdderResult CFullAdder::FullAdderOutput(eLogicLevel FullAdderInput1, eLogicLevel FullAdderInput2, eLogicLevel FullAdderInput3)
{
    // Instantiate two half adders and their corresponding results
    CHalfAdder HalfAdder1;                                                          
    CHalfAdder HalfAdder2;

    CHalfAdder::AdderResult HalfAdderResult1;
    CHalfAdder::AdderResult HalfAdderResult2;    

    // Instantiate a full adder
    CHalfAdder::AdderResult FullAdderOutput;    

    // Return both results 
    HalfAdderResult1 = HalfAdder1.HalfAdderOutput(FullAdderInput1, FullAdderInput2);
    HalfAdderResult2 = HalfAdder2.HalfAdderOutput(FullAdderInput3, HalfAdderResult1.Sum);

    // Desired connections for a full adder
    MyWires[2].AddOutputConnection( &MyOrGates[0], 0 );
    MyWires[3].AddOutputConnection( &MyOrGates[0], 1 );

    // Drive both wires to the output of both Half adder carry's such that they can be used in an OR gate
    MyWires[2].DriveLevel( HalfAdderResult1.Carry );
    MyWires[3].DriveLevel( HalfAdderResult2.Carry );

    // Returning the desired values of the full adder
    FullAdderOutput.Carry = MyOrGates[0].GetOutputState();
    FullAdderOutput.Sum = HalfAdderResult2.Sum;

    return FullAdderOutput;
}

//---CParallelAdder implementation-----------------------------------------
// Obtain all binary inputs from the user and convert them into a logic list
int CParallelAdder::ObtainInput(eLogicLevel FirstNumber[MaxBinaryInput], eLogicLevel SecondNumber[MaxBinaryInput])
{
    // Taking in user inputs
    std::cout << "Enter your first 3 bit number: " << std::endl;
    std::cin >> FirstInputNumber;

    std::cout << "Enter your second 3 bit number: " << std::endl;
    std::cin >> SecondInputNumber;
    std::cout << std::endl;

    // Checking User inputs
    if (FirstInputNumber.size() > 3 || SecondInputNumber.size() > 3)
    {
        std::cout << "You haven't entered a 3-bit number" << std::endl;
        
        // Exit the program
        return 0;
    }
    
    // Converting the binary inputs to a list of logic levels
    for (int i = 0; i < MaxBinaryInput; i++)
    {
        FirstNumber[i] = LOGIC_LOW;
        SecondNumber[i] = LOGIC_LOW;

        if (FirstInputNumber[i] == '1')
        {
            FirstNumber[i] = LOGIC_HIGH;
        }

        if (SecondInputNumber[i] == '1')
        {
            SecondNumber[i] = LOGIC_HIGH;
        } 

        // Checking if user input is a valid bit
        if (FirstInputNumber[i] != '1' && FirstInputNumber[i] != '0')
        {
            // Exit the program
            std::cout << "Not a binary input" << std::endl;
            return 0;
        }

        if (SecondInputNumber[i] != '1' && SecondInputNumber[i] != '0')
        {
            // Exit the program
            std::cout << "Not a binary input" << std::endl;
            return 0;
        }
    }
}

// Taking both logic lists from the user and computing the output
void CParallelAdder::ParallelAdderOutput(eLogicLevel FirstNumber[MaxBinaryInput], eLogicLevel SecondNumber[MaxBinaryInput])
{
    // Instantiate 1 half adder and 2 full adders
    CHalfAdder HalfAdder1s;
    CFullAdder FullAdder2s;
    CFullAdder FullAdder4s;

    // Both 3-bit Bianry inputs A and B
    // 0 --> 2; LSB --> MSB
    A2 = FirstNumber[0]; 
    A1 = FirstNumber[1]; 
    A0 = FirstNumber[2]; 

    B2 = SecondNumber[0]; 
    B1 = SecondNumber[1]; 
    B0 = SecondNumber[2]; 

    // Taking the half adder output from both the LSB of both 3-bit inputs
    HalfAdder1sOutput = HalfAdder1s.HalfAdderOutput( A0, B0 );

    // Taking the 2s full adder output from the HalfAdders carry and the next most significant bit of both inputs
    FullAdder2sOutput = FullAdder2s.FullAdderOutput(HalfAdder1sOutput.Carry, A1, B1);

    // Taking the 4s full adder output from the fulladder2s carry and the MSB of both inputs
    FullAdder4sOutput = FullAdder4s.FullAdderOutput(FullAdder2sOutput.Carry, A2, B2);

    // User display of the results
    std::cout << " " << FirstInputNumber[0] << FirstInputNumber[1] << FirstInputNumber[2] << std::endl;
    std::cout << " " << SecondInputNumber[0] << SecondInputNumber[1] << SecondInputNumber[2] << " +" << std::endl;

    std::cout << "------\n";

    // Printing all of the sums and the final carry bit
    std::cout << FullAdder4sOutput.Carry << FullAdder4sOutput.Sum <<  FullAdder2sOutput.Sum << HalfAdder1sOutput.Sum << std::endl;
}

// Test class to simplify main
void CTestParallelAdder::Test()
{
    // 3-bit Flag   
    int Flag = 1;

    // Stored from the user
    std::string FirstInputNumber;                                                   
    std::string SecondInputNumber;

    // Converted inputs
    eLogicLevel FirstNumber[MaxBinaryInput];
    eLogicLevel SecondNumber[MaxBinaryInput];

    // Instantiation of a parallel adder
    CParallelAdder ParallelAdder;

    // Get user inputs to be used check for a 3 bit input
    Flag = ParallelAdder.ObtainInput(FirstNumber, SecondNumber);

    if (Flag != 1) 
    {
        return;
    }

    // Compute output
    ParallelAdder.ParallelAdderOutput(FirstNumber, SecondNumber);
}