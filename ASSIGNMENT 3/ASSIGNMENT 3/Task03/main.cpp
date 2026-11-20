#include <iostream>
#include <stdexcept>
#include <string>

class Event {
    int rows, seats, booked;
    bool** map;
public:
    Event(int r, int s) : rows(r < 0 ? 0 : r), seats(s < 0 ? 0 : s), booked(0), map(nullptr) {
        if (rows == 0 || seats == 0) { rows = (rows < 0 ? 0 : rows); seats = (seats < 0 ? 0 : seats); }
        if (rows > 0 && seats > 0) {
            map = new bool*[rows]{};
            try {
                for (int i = 0; i < rows; ++i) map[i] = new bool[seats]{};
            } catch (...) {
                for (int i = 0; i < rows; ++i) delete[] map[i];
                delete[] map; map = nullptr; throw;
            }
        }
        std::cout << "Opened event " << rows << "x" << seats << '\n';
    }
    bool book(int r, int s) {
        if (!map || r < 0 || r >= rows || s < 0 || s >= seats || map[r][s]) return false;
        map[r][s] = true; ++booked; return true;
    }
    bool cancel(int r, int s) {
        if (!map || r < 0 || r >= rows || s < 0 || s >= seats || !map[r][s]) return false;
        map[r][s] = false; --booked; return true;
    }
    ~Event() {
        if (map) { for (int i = 0; i < rows; ++i) delete[] map[i]; delete[] map; }
        std::cout << "Closed event; bookings remaining: " << booked << '\n';
    }
};
int main() {
    Event stackEvent(2, 3);
    stackEvent.book(0, 1); stackEvent.book(1, 2); stackEvent.cancel(0, 1);
    {
        Event blockEvent(1, 2); blockEvent.book(0, 0);
    }
    Event* heapEvent = new Event(2, 2);
    heapEvent->book(1, 1);
    delete heapEvent;
    Event zeroEvent(0, 5);
}
