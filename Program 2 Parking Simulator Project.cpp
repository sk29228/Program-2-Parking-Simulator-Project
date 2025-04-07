#include <iostream>
#include <string>
#include <ctime>
#include <cmath>
#include <vector>

class ParkedCar {
private:
    std::string make;
    std::string model;
    std::string color;
    std::string licenseNumber;
    time_t parkingStartTime;

public:
    ParkedCar(const std::string& make, const std::string& model, const std::string& color, const std::string& licenseNumber)
        : make(make), model(model), color(color), licenseNumber(licenseNumber), parkingStartTime(std::time(nullptr)) {
    }

    std::string getMake() const {
        return make;
    }

    std::string getModel() const {
        return model;
    }

    std::string getColor() const {
        return color;
    }

    std::string getLicenseNumber() const {
        return licenseNumber;
    }

    time_t getParkingStartTime() const {
        return parkingStartTime;
    }

    void setParkingStartTime(time_t startTime) {
        parkingStartTime = startTime;
    }

    double getMinutesParked() const {
        return std::difftime(std::time(nullptr), parkingStartTime) / 60.0;
    }

    void displayCarInfo() const {
        std::cout << "Make: " << make << std::endl;
        std::cout << "Model: " << model << std::endl;
        std::cout << "Color: " << color << std::endl;
        std::cout << "License Number: " << licenseNumber << std::endl;
    }
};

class ParkingMeter {
private:
    double minutesPurchased;

public:
    ParkingMeter(double minutes) : minutesPurchased(minutes) {}

    double getMinutesPurchased() const {
        return minutesPurchased;
    }
};

class ParkingTicket {
private:
    ParkedCar parkedCar;
    double fineAmount;
    std::string officerName;
    std::string officerBadgeNumber;

public:
    ParkingTicket(const ParkedCar& car, double fine, const std::string& officerName, const std::string& badge)
        : parkedCar(car), fineAmount(fine), officerName(officerName), officerBadgeNumber(badge) {
    }

    void displayTicket() const {
        std::cout << "\n----- Parking Ticket -----" << std::endl;
        parkedCar.displayCarInfo();
        std::cout << "Fine Amount: $" << fineAmount << std::endl;
        std::cout << "Officer Name: " << officerName << std::endl;
        std::cout << "Officer Badge Number: " << officerBadgeNumber << std::endl;
        std::cout << "-------------------------" << std::endl;
    }
};

class PoliceOfficer {
private:
    std::string name;
    std::string badgeNumber;

public:
    PoliceOfficer(const std::string& name, const std::string& badgeNumber)
        : name(name), badgeNumber(badgeNumber) {
    }

    std::string getName() const {
        return name;
    }

    std::string getBadgeNumber() const {
        return badgeNumber;
    }

    ParkingTicket* examineCar(const ParkedCar& car, const ParkingMeter& meter) const {
        double minutesParked = car.getMinutesParked();
        double minutesPurchased = meter.getMinutesPurchased();

        if (minutesParked > minutesPurchased) {
            double overtimeMinutes = minutesParked - minutesPurchased;
            double fine = 25.0 + std::ceil(overtimeMinutes / 60.0) * 10.0; // $10 for each additional hour or part thereof
            return new ParkingTicket(car, fine, name, badgeNumber);
        }
        return nullptr;
    }
};

int main() {
    // Simulate some parked cars and parking meters
    ParkedCar car1("Toyota", "Camry", "Silver", "CBC-123");
    ParkingMeter meter1(60.0); // Purchased 60 minutes

    ParkedCar car2("Honda", "Civic", "Blue", "DEF-456");
    ParkingMeter meter2(30.0); // Purchased 30 minutes
    car2.setParkingStartTime(std::time(nullptr) - 45 * 60); // Parked 45 minutes ago

    ParkedCar car3("Ford", "F-150", "Black", "GHI-789");
    ParkingMeter meter3(120.0); // Purchased 120 minutes
    car3.setParkingStartTime(std::time(nullptr) - 30 * 60); // Parked 30 minutes ago

    // Create a police officer
    PoliceOfficer officer("Officer Smith", "12345");

    // Officer examines the cars
    std::vector<ParkingTicket*> issuedTickets;

    ParkingTicket* ticket1 = officer.examineCar(car1, meter1);
    if (ticket1) {
        ticket1->displayTicket();
        issuedTickets.push_back(ticket1);
    }
    else {
        std::cout << "\nNo violation for car: " << car1.getLicenseNumber() << std::endl;
    }

    ParkingTicket* ticket2 = officer.examineCar(car2, meter2);
    if (ticket2) {
        ticket2->displayTicket();
        issuedTickets.push_back(ticket2);
    }
    else {
        std::cout << "\nNo violation for car: " << car2.getLicenseNumber() << std::endl;
    }

    ParkingTicket* ticket3 = officer.examineCar(car3, meter3);
    if (ticket3) {
        ticket3->displayTicket();
        issuedTickets.push_back(ticket3);
    }
    else {
        std::cout << "\nNo violation for car: " << car3.getLicenseNumber() << std::endl;
    }

    // Clean up dynamically allocated memory
    for (ParkingTicket* ticket : issuedTickets) {
        delete ticket;
    }
    issuedTickets.clear();

    return 0;
}