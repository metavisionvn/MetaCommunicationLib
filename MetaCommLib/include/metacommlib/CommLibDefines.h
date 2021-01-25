#ifndef COMMLIBDEFINES_H
#define COMMLIBDEFINES_H

#include <stdexcept>
using namespace std;

namespace mtcl {

class NotImplemented : public std::logic_error
{
public:
    NotImplemented() : std::logic_error("Function not yet implemented") { };
};
}

#endif // COMMLIBDEFINES_H
