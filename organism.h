#ifndef ORGANISM_H
#define ORGANISM_H

#include <iostream>
#include <iomanip>
#include <vector>
#include <limits>

namespace ga
{

template<typename GenType>
struct Organism
{
    Organism() {}
    Organism(size_t size) : chromosome(std::vector<GenType>(size))
    {
        size_t max = std::numeric_limits<GenType>::max() + 1;
        if(max > 10000 || max == 0) max = 10000;
        for(size_t i = 0; i < chromosome.size(); ++i)
        {
            chromosome[i] = rand() % max;
        }
        fitness = std::numeric_limits<double>::max();
    }

    friend bool operator<(const Organism<GenType> &lhs,
                          const Organism<GenType> &rhs)
    {
        return lhs.fitness < rhs.fitness;
    }

    friend bool operator>(const Organism<GenType> &lhs,
                          const Organism<GenType> &rhs)
    {
        return rhs < lhs;
    }

    friend std::ostream &operator<<(std::ostream &o,
                                    const Organism<GenType> &org)
    {
        for(size_t gen = 0; gen < org.chromosome.size() - 1; ++gen)
        {
            o << org.chromosome[gen] << " ";
        }
        o << org.chromosome.back();

        return o;
    }

    std::vector<GenType> chromosome;
    double fitness;
};

template<typename T>
using Population = std::vector<Organism<T>>;

}

#endif // ORGANISM_H
