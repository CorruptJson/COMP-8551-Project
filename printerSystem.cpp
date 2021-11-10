#include "printerSystem.h"

using namespace std;
void PrinterSystem::Receive(Event event, EntityQuery* eq, list<int> indexList) {
    for (int i = 0; i < indexList.size(); i++) {
        switch (event) {
            case Event::TIMER_REACHED_200:
                cout << i << "'s timer has reached 50!" << endl;
                break;
            default:
                break;
        }
    }
    
}
