#ifndef PREPOPULATION_H
#define PREPOPULATION_H

#include "organism.h"
#include "geneticalgorithm.h"

namespace ga
{

template<typename GenType>
class Prepopulation
{
public:
    virtual void prepopulate(const std::vector< Organism<GenType> > &,
                             std::vector< Organism<GenType> > &) = 0;
    virtual ~Prepopulation() = default;
};

template<typename GenType>
class EliteStrategy : public Prepopulation<GenType>
{
public:
    EliteStrategy(int size) : size_(size) {}
    void prepopulate(const std::vector< Organism<GenType> > &currentPopulation,
                     std::vector< Organism<GenType> > &nextPopulation) override
    {
        for(size_t i = 0; i < size_; ++i)
        {
            nextPopulation.push_back(currentPopulation[i]);
        }
    }

private:
    size_t size_;
};

}

#endif // PREPOPULATION_H
