#ifndef PHARMML_DEPENDENCIES_H_
#define PHARMML_DEPENDENCIES_H_

#include <unordered_set>

namespace PharmML
{
    class Dependencies
    {
        std::unordered_set<std::string> dependencies;

        public:
        void addDependency(std::string symbol);
        bool hasDependency(std::string symbol);
        int numDependencies();
    };
}

#endif
