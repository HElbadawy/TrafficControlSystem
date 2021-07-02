#include <Windows.h>
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

//Struct definitions
struct Date
{
    int day;
    int month;
    int year;

    Date() {
        day = 0, month = 0, year = 0;
    }

};

struct Fine
{
    float value;
    string streetNames;     //stores street names where fines were issued if more than one
    Date lastDate;          //day of last fine recorded
    bool isPaid;

    Fine() {
        value = 0;
        isPaid = true;
    }
};

struct Car
{
    string plateNumber;
    string model;
    Fine recordedFines;
    int productionYear;
};

struct Driver
{
    string licenseNumber;
    string name;
    Date birthdate;
    Car ownedCars[3];
    int numOfCars;
    int index;

    Driver() { numOfCars = 0; }

};


//function declaration
void displayDt(Date d);
void displayF(Fine f);
void displayC(Car c);
void addD(Driver* dPtr);
void displayDr(Driver d);
void addDriver(Driver list[], int& count);
void addCarToDriver(Driver list[], int count);
void recordFine(Driver list[], int count);
void payFine(Driver list[], int count);
Driver searchDriverByLN(string ln, Driver list[], int count);   //LN stand for License Number
Car* searchCarByPN(string PN, Driver list[], int count);        //PN stands for Plate Number

int main()
{
    //system("color F0");
    Driver allDrivers[50]; //50 is the maximum capacity of drivers in the system
    int driverCount = 0;
    bool continueLoop = true;

    cout << "Welcome!";
    do
    {
        cout << "\nEnter number to choose the service you want:\n";
        cout << "\t1. Add New Driver\n\t2. Register Car\n\t3. Record Violation\n\t4. Display Drivers info\n\t5. Exit\n";
        char choice;
        cin >> choice;
        switch (choice)
        {
        case '1':
            addDriver(allDrivers, driverCount);
            break;
        case '2':
            addCarToDriver(allDrivers, driverCount);
            break;
        case '3':
            recordFine(allDrivers, driverCount);
            break;
        case '4':
            for (int i = 0; i < driverCount; i++)
                displayDr(allDrivers[i]);
            break;
        case '5':
            continueLoop = false;
            break;
        default:
            cout << "Invalid.\n";
            break;
        }
    } while (continueLoop);


    return 0;
}


//function definitions
void payFine(Driver list[], int count) {        //Searches for Specific PN and points to its Car struct. validates that theres a fine. changes the isPaid Fine of the Car from 
                                                //false to true if the user confirms the payment.
    string PN;
    Car* temp;
    cout << "Enter Plane Number: ";
    cin >> PN;

    temp = searchCarByPN(PN, list, count);
    if (temp == nullptr) {
        cout << "Error: Car Not Found.\n";
        return;
    }

    if (temp->recordedFines.isPaid)
    {
        cout << "No unpaid fines.\n";
    }
    else {

        cout << "You need to pay " << temp->recordedFines.value << endl;
        cout << "Enter 'C' to confirm payment\n";
        char confirm;   cin >> confirm;
        if (confirm == 'c' || confirm == 'C')
        {
            cout << "Fine Paid.\n";
            temp->recordedFines.isPaid = true;
            temp->recordedFines.value = 0;

        }
        else {
            cout << "Canceled.\n";
        }
    }


}

void recordFine(Driver list[], int count) {                                 //takes a list of drivers and the amount of drivers. asks user for plate number and uses searchByPN to find specific Car. user inputs info about Fine which is then attached to the car.
    string PN, newStreet;
    int speed, maxSpeed;
    Date newDate;
    Car* temp;

    cout << "Enter plate number: "; 
    cin >> PN;
    temp = searchCarByPN(PN, list, count);
    if (temp == nullptr) {
        cout << "Error: Car Not Found.\n";
        return;
    }

    cout << "Enter recorded speed: ";
    cin >> speed;
    cout << "Enter maximum road speed: ";
    cin >> maxSpeed;
    if (speed <= maxSpeed)
    {
        cout << "Error: No Violation.\n";
        return;
    }

    cout << "Enter Street Name: ";
    getline(cin, newStreet);
    getline(cin, newStreet);
    
    cout << "Enter Violation's Date:\n";
    cout << "day: ";
    cin >> newDate.day;
    cout << "month: ";
    cin >> newDate.month;
    cout << "year: ";
    cin >> newDate.year;

    cout << "This Plate Number belongs to a " << temp->model << " (" << temp->productionYear << ")" << endl;

    int finalFineVal = (speed - maxSpeed) * 2;  //calculated fine value

    //adding value of fine to existing unapid fines
    temp->recordedFines.value += finalFineVal;


    //updating rest of cars fine info
    if (!temp->recordedFines.isPaid)
        temp->recordedFines.streetNames += (", " + newStreet);
    else
        temp->recordedFines.streetNames += newStreet;

    temp->recordedFines.lastDate = newDate;
    temp->recordedFines.isPaid = false;
    cout << "Fine: \n";
    displayC(*temp);
    cout << "Added.\n";
}

Car* searchCarByPN(string PN, Driver list[], int count) {                   //takes an array of all driver structs, the amount of drivers, and a target plate number. does a linear search to compare each plate number of each car for every driver with the target. returns a reference of the car if found. return nullptr if none found.
    for (int i = 0; i < count; i++)
    {
        for (int j = 0; j < list[i].numOfCars; j++)
        {
            if (list[i].ownedCars[j].plateNumber == PN)
                return &list[i].ownedCars[j];
        }
    }

    return nullptr;
}

void displayDt(Date d) {                                                    //takes a date struct and outputs its member variables
    if (d.day != 0)
    {
        cout << "(DD/MM/YYYY): ";
        cout << "(" << d.day << "/" << d.month << "/" << d.year << ")";
    }
    else {
        cout << "No Date.";
    }
}

void displayF(Fine f) {                                                     //takes a Fine struct and outputs its member variables. uses displayDt.
    cout << "Paid: ";
    cout << (f.isPaid ? "True" : "False");
    cout << "\nTotal Value of Unpaid Fine: " << f.value;
    cout << "\nRecorded in street: " << f.streetNames;
    cout << ".\nlast recorded at ";
    displayDt(f.lastDate);
    cout << endl; 
}

void displayC(Car c) {                                                      //takes a Car struct and outputs its member variables. uses displayF.
    cout << "Plate Number: " << c.plateNumber;
    cout << "\nModel: " << c.model;
    cout << "\nProduction Year: " << c.productionYear << endl;
    displayF(c.recordedFines); 
}

void displayDr(Driver d) {                                                  //takes a Driver struct and outputs its member variables. uses displayC & displayDt.
    cout << "Name: " << d.name << endl;
    cout << "License Number:" << d.licenseNumber << endl;
    cout << "Birth Date: "; displayDt(d.birthdate);
    cout << endl;

    for (int i = 0; i < d.numOfCars; i++) {
        cout << "Car " << i + 1 << ": \n";
        displayC(d.ownedCars[i]);
        cout << "\n---------------------------\n";
    }
}

void addDriver(Driver list[], int& count)                                   //Takes a list of Driver structs and the amount of current drivers by reference, and creates a temporary Driver which is filled with the new Driver info by the user using addD function. the temp Driver is then assigned to the next spot in the array and increments the counter.
{
    Driver temp;
    addD(&temp);
    temp.index = count;
    list[count++] = temp;
}

void addD(Driver* dPtr) {                                                   //takes a Driver struct by reference and fills it with user input.
    cout << "Enter Your Name\n";
    getline(cin, dPtr->name);
    getline(cin, dPtr->name);
    cout << "Enter License Number\n";
    getline(cin, dPtr->licenseNumber);
    cout << "Enter Your Date of Birth\n";
    cout << "day: ";
    cin >> dPtr->birthdate.day;
    cout << "month: ";
    cin >> dPtr->birthdate.month;
    cout << "year: ";
    cin >> dPtr->birthdate.year;
}

void addCarToDriver(Driver list[], int count) {                             //Takes array of driver structs and amount of drivers to pass for the search function. uses the searchByLN to find the specific Driver by license number and assigns it to the temporary Driver after validation. fills information of the ownedCars
    Driver tmp;
    string LN; 
    do
    {
        cout << "Enter your license number\n";
        cin >> LN;
        tmp = searchDriverByLN(LN, list, count);
        if (tmp.index == -1)
        { 
            cout << "Enter 0 to cancel\nor ";
        }
        else if (LN == "0")
        {
            return;
        }
    } while (tmp.index == -1);

    int num;
    bool validNum = false;
    do {
        cout << "How many cars do you you want to register for " << tmp.name << endl;
        cin >> num;
        //tmp.numOfCars += num;
        if (tmp.numOfCars + num > 3)
            cout << "Error: Exceeded Maximum Number of Cars Owned (3)\n";
        else if (num < 0)
            cout << "Error: Number Cant be Less Than 0\n";
        else
            validNum = true;
    } while (!validNum);

    tmp.numOfCars += num;

    for (int i = 0; i < tmp.numOfCars; i++) {
        cout << "***************************\n";
        cout << "Registration of car " << i + 1 <<":\n";
        cout << "plateNumber: ";      
        cin >> tmp.ownedCars[i].plateNumber;
        cout << "\nmodel: ";         
        cin >> tmp.ownedCars[i].model;
        cout << "\nproductionYear: ";  
        cin >> tmp.ownedCars[i].productionYear;
    }

    //list[searchDriverByLN(LN, list, count).index] = tmp;
    list[tmp.index] = tmp;
    cout << "Done. Added " << num << " car(s) to " << tmp.name << " (" << tmp.licenseNumber << ")\n";
    cout << "***************************\n";
}

Driver searchDriverByLN(string ln, Driver list[], int count) {              //takes an array of all driver structs, the amount of drivers, and a target license number. does a linear search to compare each license number of every driver with the target. Returns the Driver if found. return empty Driver with index -1 if none found.
    for (int i = 0; i < count; i++)
    {
        if (list[i].licenseNumber == ln) {
            return list[i];
        }
    }
    cout << "Driver Not Found.\n";
    Driver notFound;
    notFound.index = -1;    //indicates search coudnt find target
    return notFound;
}