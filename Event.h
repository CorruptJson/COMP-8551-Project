#include <string>
using namespace std;

template <typename T>
class Event
{
public:
    Event();
    Event(Ttype, const string& name = "") : typee(type), name(name) {};
    
    string name;
    T type;
    bool active = false;
    
};
