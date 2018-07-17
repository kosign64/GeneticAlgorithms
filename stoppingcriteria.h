#ifndef STOPPINGCRITERIA_H
#define STOPPINGCRITERIA_H

#include "organism.h"
#include "geneticalgorithm.h"

namespace ga
{

template<typename GenType>
class StoppingCriteria
{
public:
    virtual bool stop(unsigned long, const Population<GenType> &) = 0;
    virtual ~StoppingCriteria() = default;
};

template<typename GenType>
class IterationCriteria : public StoppingCriteria<GenType>
{
public:
    IterationCriteria(unsigned long iterations) : iterations_(iterations) {}

    virtual bool stop(unsigned long iterations,
                      const Population<GenType> &) override
    {
        return iterations >= iterations_;
    }

private:
    const unsigned long iterations_;
};

template<typename GenType>
class FitnessCriteria : public StoppingCriteria<GenType>
{
public:
    FitnessCriteria(double desiredFitness, bool minimize = true,
                   unsigned long maxIterations = 10000) :
        desiredFitness_(desiredFitness),
        minimize_(minimize),
        iterations_(maxIterations)
    {}
    virtual bool stop(unsigned long iterations,
                      const Population<GenType> &population) override
    {
        if(iterations >= iterations_) return true;
        double best = population.front().fitness;
        return (minimize_) ? best <= desiredFitness_ : best >= desiredFitness_;
    }

private:
    const double desiredFitness_;
    const bool minimize_;
    const unsigned long iterations_;
};

}

#endif // STOPPINGCRITERIA_H
