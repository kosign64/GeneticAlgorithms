#ifndef DISPLAY_H
#define DISPLAY_H

#include <vector>
#include <iostream>
#include "organism.h"
#include "geneticalgorithm.h"

namespace ga
{

template<typename GenType>
class Display
{
public:
    virtual void display(const std::vector< Organism<GenType> > &,
                         unsigned long) = 0;
    virtual ~Display() = default;
};

template<typename GenType>
class SimpleDisplay : public Display<GenType>
{
public:
    void display(const std::vector< Organism<GenType> > &population,
                    unsigned long iter) override
    {
        std::cout << "Iteration #" << iter << std::endl;
        for(const auto &org : population)
        {
            std::cout << org << " " << "Fit: " << org.fitness << std::endl;
        }

        std::cout << std::endl;
    }
};

}

#endif // DISPLAY_H
