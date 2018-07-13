#ifndef FITNESSCALING_H
#define FITNESSCALING_H

#include "organism.h"
#include "geneticalgorithm.h"

namespace ga
{

template<typename GenType>
class FitnessScaling
{
public:
    virtual void scale(std::vector< Organism<GenType> > &, bool) = 0;
    virtual ~FitnessScaling() = default;
};

template<typename GenType>
class RankScaling : public FitnessScaling<GenType>
{
public:
    void scale(std::vector< Organism<GenType> > &population,
               bool minimize) override
    {
        for(size_t i = 0; i < population.size(); ++i)
        {
            Organism<GenType> &org = population[i];
            if(minimize)
            {
                org.fitness = i;
            }
            else
            {
                org.fitness = population.size() - i - 1;
            }
        }
    }
};

}

#endif // FITNESSCALING_H
