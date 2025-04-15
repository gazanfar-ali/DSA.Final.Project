#include <iostream>
#include <string>
#include <queue>
#include <vector>

using namespace std;

struct Car {
    string licensePlate;
    int floor;
    int position;
};

class ParkingLot {
    int maxCarsPerFloor;
    int totalFloors;
    
    struct Floor {
        vector<Car> cars;   
    } *floors;

    queue<Car> entryQueue;
    queue<Car> overflowWaitlist;
    vector<Car> parkedCars;  
    
public:
    ParkingLot(int maxCarsPerFloor, int totalFloors)
        : maxCarsPerFloor(maxCarsPerFloor), totalFloors(totalFloors) {
        floors = new Floor[totalFloors];
        for (int i = 0; i < totalFloors; ++i) {
            floors[i].cars.reserve(maxCarsPerFloor);
        }
    }

    ~ParkingLot() {
        delete[] floors;
    }

    void addCar(string licensePlate) {
        Car newCar = {licensePlate, -1, -1};

        if (allFloorsFull()) {
            overflowWaitlist.push(newCar);
            cout << "Parking lot is full. Car added to overflow waitlist: " << licensePlate << endl;
        } else {
            parkCar(newCar);
        }
    }

    void removeCar(string licensePlate) {
        for (int i = 0; i < totalFloors; ++i) {
            for (size_t j = 0; j < floors[i].cars.size(); ++j) {
                if (floors[i].cars[j].licensePlate == licensePlate) {
                    cout << "\nCar " << licensePlate << " removed from Floor " << i + 1 << endl;
                    floors[i].cars.erase(floors[i].cars.begin() + j);
                    processEntryQueue();
                    return;
                }
            }
        }
    }

    void displayStatus() {
        cout << "\nCurrent Parking Lot Status:" << endl;
        for (int i = 0; i < totalFloors; ++i) {
            cout << "Floor " << i + 1 << ": ";
            for (auto& car : floors[i].cars) {
                cout << car.licensePlate << " ";
            }
            cout << endl;
        }
        
        cout << "\nCars in Entry Queue: ";
        queue<Car> tempQueue = entryQueue;
        while (!tempQueue.empty()) {
            cout << tempQueue.front().licensePlate << " ";
            tempQueue.pop();
        }
        cout << endl;
        
        cout << "Cars in Overflow Waitlist: ";
        queue<Car> tempWaitlist = overflowWaitlist;
        while (!tempWaitlist.empty()) {
            cout << tempWaitlist.front().licensePlate << " ";
            tempWaitlist.pop();
        }
        cout << endl;
    }

    void expandParkingLot(int additionalFloors) {
        Floor* newFloors = new Floor[totalFloors + additionalFloors];

        for (int i = 0; i < totalFloors; ++i) {
            newFloors[i] = floors[i];
        }
        for (int i = totalFloors; i < totalFloors + additionalFloors; ++i) {
            newFloors[i].cars.reserve(maxCarsPerFloor);
        }
        
        delete[] floors;
        floors = newFloors;
        totalFloors += additionalFloors;
        
        cout << "\nParking lot expanded by " << additionalFloors << " floors." << endl;
        processOverflowWaitlist();
    }

private:
    bool allFloorsFull() {
        for (int i = 0; i < totalFloors; ++i) {
            if (floors[i].cars.size() < maxCarsPerFloor) {
                return false;
            }
        }
        return true;
    }

    void parkCar(Car newCar) {
        for (int i = 0; i < totalFloors; ++i) {
            if (floors[i].cars.size() < maxCarsPerFloor) {
                newCar.floor = i;
                newCar.position = floors[i].cars.size();
                floors[i].cars.push_back(newCar);
                parkedCars.push_back(newCar);
                cout << "Car " << newCar.licensePlate << " parked at Floor " << i + 1 << ", Position " << newCar.position << endl;
                return;
            }
        }
    }

    void processEntryQueue() {
        if (!entryQueue.empty() && !allFloorsFull()) {
            Car car = entryQueue.front();
            entryQueue.pop();
            parkCar(car);
        }
    }

    void processOverflowWaitlist() {
        while (!overflowWaitlist.empty() && !allFloorsFull()) {
            Car car = overflowWaitlist.front();
            overflowWaitlist.pop();
            parkCar(car);
        }
    }
};

int main() {
    ParkingLot parkingLot(3, 3;  

    parkingLot.addCar("ABC123");
    parkingLot.addCar("XYZ456");
    parkingLot.addCar("LMN789");
    parkingLot.addCar("PQR012");
    parkingLot.addCar("DEF345");

    parkingLot.displayStatus();

    parkingLot.removeCar("XYZ456");
    parkingLot.displayStatus();

    parkingLot.expandParkingLot(2);
    
    cout<<endl;
    parkingLot.addCar("GHI678");
    parkingLot.displayStatus();
    
    cout<<endl;
    parkingLot.addCar("EF3009");
    parkingLot.displayStatus();
    
    cout<<endl;
    parkingLot.addCar("KK9087");
    parkingLot.addCar("SS5210");
    parkingLot.addCar("ZF2130");
    parkingLot.addCar("ABCD10");
    parkingLot.addCar("GK1826");
    parkingLot.addCar("GK1010");
    parkingLot.addCar("MU1212");

    parkingLot.displayStatus();

    return 0;
}

