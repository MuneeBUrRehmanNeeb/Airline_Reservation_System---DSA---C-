#include <iostream>
#include <string>
#include <queue>
#include <stack>
#include <map>
#include <vector>
#include <algorithm>
using namespace std;



// Passenger class — passenger ki basic info store karne ke liye
class Passenger {
public:
    string name;
    string phone;
    string passportNo;
    
    Passenger() {}
    // Constructor — passenger object initialize karta hai
    Passenger(string n, string ph, string pass) {
        name = n;
        phone = ph;
        passportNo = pass;
    }
    // Passenger ki details display karne ke liye
    void display() {
        cout << "Name: " << name << endl;
        cout << "Phone: " << phone << endl;
        cout << "Passport: " << passportNo << endl;
    }


};








// Flight class — flight ki sari details store hoti hain
class Flight {
public:
    string flightNo;
    string from;
    string to;
    string date;
    string time;
    int totalSeats;
    int availableSeats;
    double price;
    
    Flight() {}
    // Constructor — flight data initialize karta hai
    Flight(string fNo, string f, string t, string d, string tm, int seats, double pr) {
        flightNo = fNo;
        from = f;
        to = t;
        date = d;
        time = tm;
        totalSeats = seats;
        availableSeats = seats;
        price = pr;
    }
    

    // Flight ki complete info show karne ke liye
    void display() {
        cout << "\n---------------------------------" << endl;
        cout << "Flight: " << flightNo << endl;
        cout << "Route: " << from << " -> " << to << endl;
        cout << "Date: " << date << " | Time: " << time << endl;
        cout << "Available: " << availableSeats << "/" << totalSeats << endl;
        cout << "Price: $" << price << endl;
        cout << "---------------------------------" << endl;
    }
};








// Booking class — passenger aur flight ke darmiyan booking record
class Booking {
public:
    int bookingId;
    string flightNo;
    Passenger passenger;
    string seatNo;
    string status;
    


    Booking() {}
    // Constructor — new booking create karta hai
    Booking(int id, string fNo, Passenger p, string seat) {
        bookingId = id;
        flightNo = fNo;
        passenger = p;
        seatNo = seat;
        status = "Confirmed";
    }
    


    // Booking ticket display karne ke liye
    void display() {
        cout << "\n========== BOOKING TICKET ==========" << endl;
        cout << "Booking ID: " << bookingId << endl;
        cout << "Flight: " << flightNo << endl;
        cout << "Seat: " << seatNo << endl;
        cout << "Status: " << status << endl;
        cout << "--- Passenger Info ---" << endl;
        passenger.display();
        cout << "===================================" << endl;
    }
};



// Waiting list ke liye structure — priority ke sath
struct WaitlistEntry {
    int priority;
    string passengerName;
    string flightNo;
    
    // Priority queue ke liye comparison
    bool operator<(const WaitlistEntry& other) const {
        return priority < other.priority; // zyada priority pehle
    }


};


// Data structures use ho rahe hain (DSA concepts)
map<string, Flight> flightMap;                // Flights store karne ke liye
priority_queue<WaitlistEntry> waitingList;    // Waiting list (priority queue)
stack<Booking> bookingHistory;                // Recent bookings (stack)
vector<Booking> allBookings;                  // All bookings record
map<string, vector<int>> passengerBookings;   // Passport ke against booking IDs

int bookingIdCounter = 1000; // Unique booking ID counter



// Default flights initialize karne ke liye
void initializeFlights() {
    flightMap["AA101"] = Flight("AA101", "New York", "London", "2025-01-15", "10:00 AM", 5, 450.0);
    flightMap["BA202"] = Flight("BA202", "London", "Paris", "2025-01-16", "02:00 PM", 4, 250.0);
    flightMap["EK303"] = Flight("EK303", "Dubai", "Singapore", "2025-01-17", "11:30 PM", 6, 680.0);
    flightMap["UA404"] = Flight("UA404", "Los Angeles", "Tokyo", "2025-01-18", "09:00 AM", 5, 720.0);
    flightMap["LH505"] = Flight("LH505", "Frankfurt", "New York", "2025-01-19", "05:00 PM", 4, 520.0);
    
    cout << flightMap.size() << " flights loaded successfully!" << endl;
}



// Sari available flights show karne ke liye
void displayAllFlights() {
    cout << "\n========== AVAILABLE FLIGHTS ==========" << endl;
    for (auto& pair : flightMap) {
        pair.second.display();
    }
}


// Flight number se search karne ke liye
Flight* searchFlight(string flightNo) {
    if (flightMap.find(flightNo) != flightMap.end()) {
        return &flightMap[flightNo];
    }
    return nullptr;
}




// Route ke basis pe search (from -> to)
void searchByRoute() {
    string from, to;
    cout << "\nEnter Origin: ";
    cin >> from;
    cout << "Enter Destination: ";
    cin >> to;
    
    bool found = false;
    for (auto& pair : flightMap) {
        if (pair.second.from == from && pair.second.to == to) {
            pair.second.display();
            found = true;
        }
    }
    
    if (!found) {
        cout << "No flights found for this route." << endl;
    }


}

// Seat number generate karne ke liye
string generateSeatNo(int seatNum) {
    int row = (seatNum / 6) + 1;
    char col = 'A' + (seatNum % 6);
    return to_string(row) + col;
}



// Flight booking ka main function
void bookFlight() {
    displayAllFlights();
    string flightNo;
    cout << "\nEnter Flight Number: ";
    cin >> flightNo;
    Flight* flight = searchFlight(flightNo);
    if (flight == nullptr) {
        cout << "Flight not found!" << endl;
        return;
    }
    // Agar seats available nahi hain
    if (flight->availableSeats <= 0) {
        cout << "\nFlight is FULL!" << endl;
        
        char addToWaitlist;
        cout << "Add to waiting list? (y/n): ";
        cin >> addToWaitlist;
        
        if (addToWaitlist == 'y' || addToWaitlist == 'Y') {
            string name;
            int priority;
            cout << "Enter your name: ";
            cin.ignore();
            getline(cin, name);
            cout << "Enter priority (1-10): ";
            cin >> priority;
            
            waitingList.push({priority, name, flightNo});
            cout << "Added to waiting list!" << endl;
        }
        return;


    }
    

    // Passenger ki details lena
    string name, phone, passport;
    cout << "\n--- Passenger Details ---" << endl;
    cout << "Name: ";
    cin.ignore();
    getline(cin, name);
    cout << "Phone: ";
    cin >> phone;
    cout << "Passport Number: ";
    cin >> passport;
    




    Passenger passenger(name, phone, passport);
    int seatIndex = flight->totalSeats - flight->availableSeats;
    string seatNo = generateSeatNo(seatIndex);
    Booking booking(bookingIdCounter++, flightNo, passenger, seatNo);
    bookingHistory.push(booking);               // Stack
    allBookings.push_back(booking);             // Vector
    passengerBookings[passport].push_back(booking.bookingId); // Map
    
    flight->availableSeats--;
    
    cout << "\nBooking Successful!" << endl;
    booking.display();
}

// Recent booking dekhne ke liye (stack)
void viewRecentBooking() {
    if (bookingHistory.empty()) {
        cout << "\nNo recent bookings!" << endl;
        return;
    }
    bookingHistory.top().display();


}









//-----------------====DSA Project=====--------------------//
// Sir Kashif sittar
// Main function — menu driven program
int main() {
    cout << "\nAIRLINE RESERVATION SYSTEM (DSA)\n" << endl;
    initializeFlights();
    
    int choice;
    do {
        cout << "\n========== MAIN MENU ==========" << endl;
        cout << "1. View All Flights\n2. Search Flight by Number\n3. Search Flights by Route\n";
        cout << "4. Book a Flight\n5. View Recent Booking\n6. View All Bookings\n";
        cout << "7. Search Booking by Passport\n8. Cancel Last Booking\n";
        cout << "9. View Waiting List\n10. Sort Bookings by ID\n11. Exit\n";
        cout << "Choice: ";
        cin >> choice;

        //----------------------------------//
        
        switch (choice) {
            case 1: displayAllFlights(); break;
            case 4: bookFlight(); break;
            case 11: cout << "\nThank you for using our system!" << endl; break;
            default: cout << "\nInvalid choice!" << endl;
        }
    } while (choice != 11);
    
    return 0;
}

