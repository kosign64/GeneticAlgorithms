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
    virtual bool stop(unsigned long, Population<GenType> &) = 0;
    virtual ~StoppingCriteria() = default;
};

template<typename GenType>
class IterationCriteria : public StoppingCriteria<GenType>
{
public:
    IterationCriteria(unsigned long iterations) : iterations_(iterations) {}

    virtual bool stop(unsigned long iterations, Population<GenType> &) override
    {
        return iterations >= iterations_;
    }

private:
    const unsigned long iterations_;
};

}

#endif // STOPPINGCRITERIA_H
