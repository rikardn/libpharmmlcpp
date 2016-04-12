#include "Dependencies.h"

namespace PharmML
{
    void Dependencies::addDependency(std::string symbol) {
        this->dependencies.insert(symbol);
    }

    bool Dependencies::hasDependency(std::string symbol) {
        return (this->dependencies.count(symbol) > 0);
    }
}
