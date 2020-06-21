/* Lab 4: A simple menu allowing manipulation of a file.
   by Matthew Macris & Christian Minick
   Created on 4/28/2020, last modified 5/25/2020
   Runs a menu that, based on the users input, will
   open and read a txt document, stores it into an array
   and then can Add a new entry, output its contents to the console,
   update a current entry, or parse through the array, looking for
   a user-defined search querey. Upon exiting the program, save all
   user changes to the existing txt database
*/

/************* Our Libraries and namespace *****************/
#include <iomanip>      // a library that is used to manipulate the output of C++ program. Using C++, header providing parametric manipulators as shown below
#include <iostream>     // a standard Library that implement stream-based input/output capabilities.
#include <string>       // includes support for three general types of strings: std::basic_string
#include <fstream>      // has the capabilities of both ofstream and ifstream which means it can create files, write information to files, and read information from files
#include <cstdlib>      // defines several general purpose functions, including dynamic memory management, random number generation, communication with the environment, integer arithmetics, searching, sorting and converting
using namespace std;    // you don't have to explicitly call the namespace to access them

/******* Our only globals are CONST variables of file names *******/
const string diskFileRd = "..\\vehicles_db.txt";    // Assigns the correct file to read
const int VARR_SIZE = 10;                           // Sets our array size to 10

/************* Our "Vehicle" class definition *****************/
class Vehicle                                                      // The Vehicle class
{
public:                                                            // The public functions
    void updateVin(string newVin);                                 // Mutator for Vin
    string getVin();                                               // Accessor for Vin
    void updatePrice(double newPrice);                             // Mutator for Price
    double getPrice();                                             // Accessor for Price
    void updateWeeks(int newWeek);                                 // Mutator for weeksOnLot
    int getWeeks();                                                // Accessor for weeksOnLot

    //I/O funcintions                                              // Commented out as per Assingment instructions, can be 'pruned' entirely in future versions
    //bool rdVehicle(istream& stream);                             // Bool function that reads entire doc
    //void wrtVehicle(ostream&);                                   // Has the option to output to console or text doc

    // Constructors
    Vehicle();                                                     // Default constructor
    Vehicle(string vin);                                           // Constructor with a single argument
    Vehicle(string vin, double price);                             // Constrctor with 2 arguments
    Vehicle(const Vehicle& v);                                     // Copy Constructor

    //Destructor
    ~Vehicle();                                                    //Destructor

    //Friends
    friend bool operator ==(const Vehicle&, const Vehicle&);        // Overloads the == argument to use with chosen variables
    friend void operator ++(Vehicle&);                              // Overloads the ++ argument to increment a chosen variable
    friend ostream& operator << (ostream& wrtF, const Vehicle& v);  // Overloads the << operator to write
    friend istream& operator >> (istream& rdF, Vehicle& v);         // Overloads the >> operator to read

    //Member Function Overload Operators
    void operator =(const Vehicle& compare);                        // Overloads the = operator to compare

private:
    void modelNumber();                                             // Private helper function shortens 'vin' string to 3 characters 

    string vin, model;                                              // Variables to hold vin and model
    double price = 0;                                               // Variable to hold price
    int weeksOnLot = 0;                                             // Variable to hold weeksOnLot

    int optNum = 0;                                                 // Variable to hold listSize 
    string* listPtr = NULL;                                         // Variable to hold listPtr

};

/****************  Menu function / Global function prototypes ****************/
void viewRecords(Vehicle*, int);    // User input (V:v) to view input doc in console
int addToRecords(Vehicle*, int);    // User input (A:a) to add new record to input doc
void updateRecords(Vehicle*, int);  // User input (U:u) to change current record in input doc
void search4Vin(Vehicle[], int);    // User input (S:s) to search for a vin with user input
void makeUC(string& vin);           // Changes an inputed string to all caps

/*************************  Our main() ******************************/
/*Our main function that actually runs the program,
Runs a menu that, based on the users input, will
openand read a txt document, stores it into an array
and then can Add a new entry, output its contents to the console,
update a current entry, or parse through the array, looking for
a user - defined search querey.Upon exiting the program, save all
user changes to the existing txt database*/
int main()
{
    // Rd/wrt the SAME file, but need 1 of each stream objec
    ifstream rdV;	                        // Rd
    ofstream wrtV;                          // Wrt
    Vehicle VA[VARR_SIZE];		            // Our array of Vehicles
    int vArrSize = 0;			            // Variable vArrSize

    rdV.open(diskFileRd);	                // File reading done one time, before menu
    if (rdV.fail())                         // Error Checking for failing to read
    {
        cout << "ERROR!\nERROR CODE: 666\nINPUT FILE COULD NOT OPEN!\n";
        exit(1);                            // Exits the program
    }
    while (rdV >> VA[vArrSize])             //Loops until the file is done being read
    {
        if (++vArrSize >= VARR_SIZE)        // Keeps array within pre-defined bounds
        {
            cout << "Too many cars on the Lot! Do your job and sell SOMETHING!";
            break;                          // Breaks out of the loop
        }
    }
    rdV.close();                            // Done reading for input


    char menuChoice;                                                // User interface menu option
    do
    {
        cout << "\n\nMenu: (V)iew Records, (A)dd, (U)pdate, (S)earch, (R)earrange, e(X)it program: ";
        cin >> menuChoice;                                          // Takes user input and puts into var of menuChoice
        switch (menuChoice)                                         // User Menu
        {
        case 'V': case 'v':                                         // Sets options 'v' or 'V' 
            viewRecords(VA, vArrSize);                              // Runs the function viewRecords
            break;

        case 'A': case 'a':                                         // Sets options 'a' or 'A'
            if (vArrSize < VARR_SIZE)                               // Compares vArrsize and VARR_SIZE
            {
                vArrSize = addToRecords(VA, vArrSize);              // Runs the function addToRecords
            }
            else
                cout << "Stop buying cars! Your lot is FULL! Sell something first!";
            break;

        case 'U': case 'u':                                         // Sets options 'u' or 'U'
            updateRecords(VA, vArrSize);                            // Runs the function updateRecords
            break;

        case 'S': case 's':                                         // Sets options 's' or 'S'
            search4Vin(VA, (vArrSize + 1));                         // Runs the function search4Vin
            break;

        case 'R': case 'r':                                         //Sets option 'r' or 'R'
        {
            cout << "Provide array indices for 2 vehicles (2 numeric digits space seperated)";
            cout << '\n' << "between 0 & " << (vArrSize - 1) << " that will be swapped : ";
            int ln1, ln2;
            cin >> ln1 >> ln2;                                      // Fills the variables ln1 and ln2

            /*if (cin.fail())
            {
                ln1 = 0;
                ln2 = 0;
                cout << "Dude, how more specific can we be? use freaking numbers!" << endl;
                break;
            }
            else */
            if (ln1 < vArrSize && ln2 < vArrSize)                   // Compares ln1 & ln2 with vArrSize while allowing them to be equal 
            {
                Vehicle rngVehicle;                                 // Array of rngVehicle
                rngVehicle = VA[ln1];                               // Takes content of ln1 and inserts into rngVehicle
                VA[ln1] = VA[ln2];                                  // Takes content of ln2 and inserts into ln1
                VA[ln2] = rngVehicle;                               // Takes content of rngVehicle and inserts into ln2
            }
            else {
                cout << "Try again. Your inputs were out of the designated array size.";
                cout << endl;
                break;
            }
            break;
        }

        case 'X': case 'x':                                         // Sets options 'x' or 'X'
            wrtV.open(diskFileRd);                                  // Re-open file to write changes to it
            if (wrtV.fail())                                        // Error Checking for failing to read
            {
                cout << "ERROR!\nERROR CODE: 777\nOUTPUT FILE COULD NOT OPEN!\n";
                exit(1);
            }
            for (int count = 0; count < vArrSize; count++)          // Loops until the file is done writing the contents of the array
                wrtV << VA[count];                                  // Writes content from count to wrtV
            wrtV.close();

            cout << "Goodbye!\n";
            return (0);
        default:                                                    // Sets a catch-all option so incorrect input won't break the program
            cout << "Please learn to read and try again. \n";
        }
    } while (true);                                                 // Continues the loop

    return 0;
}

/***************** Friends and Operators *********************/
//Overloads the '==' operator to compare the two inputs 
bool operator == (const Vehicle& v1, const Vehicle& v2)     // Overloads the '==' operator to compare the two inputs 
{
    return (v1.model == v2.model);
}

// Overloads the '++' operator to count up a specific variable 
void operator ++ (Vehicle& v)                               // Overloads the '++' operator to count up a specific variable 
{
    v.weeksOnLot = v.weeksOnLot + 1;
}

/************ Member Function Overload Operators ****************/
//Overloads the '=' operator to copy the inputs  
void Vehicle::operator =(const Vehicle& compare)                //Overloads the '=' operator to copy the inputs
{
    if (listPtr != NULL)                                        // If listPtr is not NULL...
        delete[] listPtr;	                                    //     ...gets rid of the existing dynamic array
    vin = compare.vin;                                          // Copy the vin
    modelNumber();                                              // Runs the modelNumber function
    price = compare.price;                                      // Copy the price
    weeksOnLot = compare.weeksOnLot;                            // Copy the weeksOnLot
    optNum = compare.optNum;                                    // Copy the listSize
    if (optNum != 0)					                        // If list is present...
    {
        listPtr = new string[optNum];                           // Create new dynamic array
        for (int count = 0; count < optNum; count++)            // Compares the count and listSize
            *(listPtr + count) = *(compare.listPtr + count);    // Copy all our strings
    }
    else listPtr = NULL;                                        // Fills listPtr with NULL
}

/**************** MUTATORS and ACCESSORS ********************/
// MUTATOR function for private 'vin'
void Vehicle::updateVin(string s)     // MUTATOR function for private 'vin'
{
    vin = s;                          // Modifies 'vin' with the inputed string
    modelNumber();                    // Calls the 'modelNumber' function to modify the private 'model'
}

// ACCESSOR function for private 'vin'
string Vehicle::getVin()              // ACCESSOR function for private 'vin'
{
    return(vin);                      // Returns existing value for private 'vin'
}

// MUTATOR function for private 'price'
void Vehicle::updatePrice(double d)   // MUTATOR function for private 'price'
{
    price = d;                        // Modifies 'price' with the inputed double
}

// ACCESSOR function for private 'price'
double Vehicle::getPrice()            // ACCESSOR function for private 'price'
{
    return(price);                    // Returns existing value for private 'price'
}

// MUTATOR function for private 'weeksOnLot'
void Vehicle::updateWeeks(int i)      // MUTATOR function for private 'weeksOnLot'
{
    weeksOnLot = i;                   // Modifies 'weeksOnLot' with the inputed int
}

// ACCESSOR function for private 'weeksOnLot'
int Vehicle::getWeeks()               // ACCESSOR function for private 'weeksOnLot'
{
    return(weeksOnLot);               // Returns existing value for private 'weeksOnLot'
}

/********** Constructors ********************************************/
// Default constructor for Vehicle
Vehicle::Vehicle() : vin("NULL"), model("N/A"), price(0.0), weeksOnLot(0)   // Default constructor has NO args
{
    optNum = 0;                                                             // Sets listSize to 0
    listPtr = NULL;                                                         // Sets listPtr to NULL
}

// Single argument constructor for Vehicle
Vehicle::Vehicle(string s)  // This constructor handles single arg
{
    vin = s;                // Inputs variable s into variable vin
    modelNumber();          // Runs the function modelNumber
    price = 0.0;            // Defaults the price 0.0
    weeksOnLot = 0;         // Defaults the weeksOnLot to 0
    optNum = 0;             // Defaults the listSize to 0
    listPtr = NULL;         // Defaults the listPtr to NULL
}

// Double argument constructor for Vehicle
Vehicle::Vehicle(string s, double newPrice)     // This one handles 2 args
{
    vin = s;                                    // Inputs variable s into variable vin
    modelNumber();                              // Runs the function modelNumber
    price = newPrice;                           // Inputs variable newPrice into variable price
    weeksOnLot = 0;                             // Defaults the weeksOnLot variable to 0
    optNum = 0;                                 // Defaults the listSize to 0
    listPtr = NULL;                             // Defaults the listPtr to NULL
}

// Copy constructor for Vehicle
Vehicle::Vehicle(const Vehicle& original)                           // Copy constructor
{
    vin = original.vin;                                             // Copies the vin into vin
    modelNumber();                                                  // Runs the function modelNumber
    price = original.price;                                         // Copies the price into price
    weeksOnLot = original.weeksOnLot;                               // Copies the weeksOnLot into weeksOnLot
    optNum = original.optNum;                                       // Copies the listSize into listSize
    if (optNum != 0)                                                // If listSize doesn't equal 0
    {
        listPtr = new string[optNum];                               // Create new dynamic array
        for (int count = 0; count < optNum; count++)                // Compare count and listSize
            *(listPtr + count) = *(original.listPtr + count);       // Copy all our strings
    }
    else listPtr = NULL;                                            // NO dynamic storage, so just make NULL ptr
    cout << "+";                                                    // Shows with this has been run, which seems to be never
}

/****** Public member functions that read and write record entries *********/
/* A member function that populates a Vehicle object with values
   fetched from a records file.  If an object is fetched from file,
   returns TRUE, otherwise, when nothing is left to fetch, returns FALSE*/
   /*bool Vehicle::rdVehicle(istream& rdF)
   {
       if (rdF >> vin)                     // Checks new line for text, and stores it in a 'vin' variable. Skips if no new text.
       {
           rdF >> price >> weeksOnLot >> listSize;     // Parses the next two /t seperated text chunks and passes them into their respective variable
           modelNumber();                  // Calls the 'modelNumber' function to populate that variable

           rdF.get();
           if (listSize > 0)
           {
               listPtr = new string[listSize];
               string* sPtr = listPtr;
               for (int count = 0; count < listSize; count++)
               {
                   getline(rdF, *sPtr);
                   sPtr++;
               }
           }
           return(true);                   // Returns a 'true' boolean variable
       }
       return (false);                     // Returns a 'false' boolean variable
   }*/

   // Overloads the '>>' operator to count up a specific variable 
istream& operator >> (istream& rdF, Vehicle& v)                 // Overloads the '>>' operator to count up a specific variable 
{
    rdF >> v.vin >> v.price >> v.weeksOnLot >> v.optNum;        // Runs through the file 
    rdF.get();

    v.modelNumber();                                            // Calls the 'modelNumber' function to populate that variable
    if (v.optNum > 0)                                           // If optNum is greater then 0
    {
        v.listPtr = new string[v.optNum];
        string* sPtr = v.listPtr;
        for (int count = 0; count < v.optNum; count++)
        {
            getline(rdF, *sPtr);
            sPtr++;
        }
    }
    return (rdF);                                               // Returns the file
}

/* A member function that writes the fields of a Vehicle object
   out to a records file, tab separated, one Vehicle per line.  Order
   of variables match order in above rdVehicle() function,
   as well as order in original record. */
   /*void Vehicle::wrtVehicle(ostream& wrtV)   // Allows input of either 'cout' or a user defined location.
   {
       // Formating to orgainize the output spacing and add decimals to the price (double) variable.
       wrtV.setf(ios::fixed);
       wrtV.setf(ios::showpoint);
       wrtV.precision(2);

       wrtV << vin << "\t" << setw(12) << price << "\t" << setw(11) << weeksOnLot << '\t' << listSize << endl;

       if (listSize > 0)
       {
           string* sPtr = listPtr;
           for (int count = 0; count < listSize; count++)
           {
               wrtV << *sPtr << endl;
               sPtr++;
           }
       }
   }*/

   // Overloads the '<<' operator to count up a specific variable 
ostream& operator << (ostream& wrtF, const Vehicle& v)          // Overloads the '<<' operator to count up a specific variable 
{
    // Formating to orgainize the output spacing and add decimals to the price (double) variable.
    wrtF.setf(ios::fixed);
    wrtF.setf(ios::showpoint);
    wrtF.precision(2);

    // Allows for better formating for easier reading
    wrtF << v.vin << "\t" << setw(12) << v.price << "\t" << setw(11) << v.weeksOnLot << '\t' << setw(11) << v.optNum << endl;

    if (v.optNum > 0)                                         // Checks if listSize is greater than 0
    {
        string* sPtr = v.listPtr;
        for (int count = 0; count < v.optNum; count++)
        {
            wrtF << *sPtr << endl;
            sPtr++;
        }
    }
    return(wrtF);                                              // Returns the file
}

/**************  Menu-choice handler functions ************/
// Choice to (S)earch. 
// By entering a S or s the user can then search for a vehicle
// by using the first 3 of the vin
void search4Vin(Vehicle vArray[], int vArrSize)	               // Choice to (S)earch 
{
    string srchModel;                                          // Determine what we're searching for
    bool notFound = true;                                      // Assume it's NOT there...

    cout << "What are first 3 characters of the Vin?: ";       // Prompts the user for first 3 char of vin
    cin >> srchModel;                                          // Stores the user input into the variable 'srchModel'
    makeUC(srchModel);                                         // Makes user input all uppercase
    Vehicle srchVehicle(srchModel);                            // Construct a local object with only the vin & model

    for (int count = 0; count < vArrSize; count++)             // Compares count and vArrSize
    {
        if (vArray[count] == srchVehicle)                      // Uses overloaded ==
        {
            cout << "\nFound! >>\n";
            cout << vArray[count];
            notFound = false;                                  // Skips failure message below
        }
    }
    if (notFound)                                              // If no matching vin is found will run the error message
        cout << "Vehicle starting with Vin \"" << srchModel << "\" not in records.";
}

// The (V)iew menu choice.  
// By pressing V or v the user can view the current list of vehicles
void viewRecords(Vehicle* pts2V, int vArrSize)          // The (V)iew menu choice
{
    //
    cout << "\n" << "Vin" << "\t" << setw(19) << "Price" << "\t" << setw(16) << "Weeks On Lot" << endl;

    Vehicle* topVehiclePtr = pts2V + vArrSize - 1;       // Adjusts the position of the pointer
    for (; pts2V <= topVehiclePtr; pts2V++)
        cout << (*pts2V);                               // Outputs to consule the contents of the array
}

// The (A)dd menu choice. 
// By pressing A or a the user can then add a vehicle
// the user will be asked to enter the Vin and a price if the price is known
int addToRecords(Vehicle vArray[], int vArrSize)    // The (A)dd menu choice
{
    string vin;                                     // Variable for vin
    double price;                                   // Variable for price
    char y_n;                                       // Variable for y_n

    cout << "\nVin of car?: ";
    cin >> vin;
    makeUC(vin);                                    // Makes user input all uppercase by calling the makeUC function
    cout << "Known Price? (Y/N)?: ";
    cin >> y_n;

    if ((y_n != 'y') && (y_n != 'Y')) {             // User entry of y or n
        vArray[vArrSize] = Vehicle(vin);            // Uses the overloaded '='
        return(vArrSize + 1);                       // Adjusts the current Array size (NOT MAX) to fill another entry
    }
    else {
        cout << "\nEnter the price of the vehicle: ";
        cin >> price;
        vArray[vArrSize] = Vehicle(vin, price);     // Constructor with 2 args
        return(vArrSize + 1);                       // Adjusts the current Array size (NOT MAX) to fill another entry
    }
}

// The (U)pdate menu choice.  
// By pressign U or u the user can cycles record entries through a local Vehicle
// object, updating weeksOnLot if necessary
void updateRecords(Vehicle* pts2V, int vArrSize)        // The (U)pdate menu choice.
{
    cout << "\nWay to go!  You wasted another week of your life at this place.";

    Vehicle* topVehiclePtr = pts2V + vArrSize - 1;      // Inputs 
    for (; pts2V <= topVehiclePtr; pts2V++)             // As long as pts2V is less than topVehicle, then increases pts2V
        ++(*pts2V);                                     // Increases the position of pts2V
}

/******************  Global Functions ************************/

// Function makeUC 
// makes the inputed string all uppercase
void makeUC(string& vin)                    // Makes an inputed string all uppercase
{
    int count = (int)vin.length();          // Checks string length and stores it as an int variable

    for (; count-- > 0;)                    // Loops through whole string
    {
        vin[count] = toupper(vin[count]);   // stores the now uppercase string back into the Array
    }
}

/******************  Private Helper Functions ************************/
// Function modelNumber 
// takes the 'vin' variable and stores the first three char
// into a seperate 'model' variable
void Vehicle::modelNumber()     // Takes the 'vin' variable and stores the first three char into a seperate 'model' variable
{
    model = vin.substr(0, 3);   // Takes the 'vin' variable and stores the first three char into a seperate 'model' variable
}

/******************  Destructor ************************/
// Destructor for Vehicle
Vehicle::~Vehicle()         // Destructor for Vehicle
{
    if (listPtr != NULL)	// If listPtr is not NUll
        delete[] listPtr;	// then deletes what's there
    listPtr = NULL;			// Sets the listPtr to NULL
    optNum = 0;             // Sets the listSize to 0
    cout << "-";            // Out puts to consule when runs
}