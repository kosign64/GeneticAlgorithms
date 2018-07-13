#ifndef INITIALIZATION_H
#define INITIALIZATION_H

#include "organism.h"
#include "geneticalgorithm.h"
#include <random>

namespace ga
{

template<typename GenType>
class Initialization
{
public:
    virtual void initialize(Population<GenType> &) = 0;
    virtual ~Initialization() = default;
};

template<typename GenType>
class UniformInitialization : public Initialization<GenType>
{
public:
    UniformInitialization(std::vector<double> &&mins,
                          std::vector<double> &&maxs) :
        mins_(mins), maxs_(maxs), gen_(std::random_device()()) {}
    void initialize(Population<GenType> &population) override
    {
        for(auto &org : population)
        {
            for(size_t i = 0; i < org.chromosome.size(); ++i)
            {
                distribution_ =
                        std::uniform_real_distribution<>(mins_[i], maxs_[i]);
                org.chromosome[i] = distribution_(gen_);
            }
        }
    }

private:
    std::vector<double> mins_;
    std::vector<double> maxs_;
    std::mt19937_64 gen_;
    std::uniform_real_distribution<> distribution_;
};

class BinaryInitialization : public Initialization<bool>
{
public:
    void initialize(std::vector< Organism<bool> > &population) override
    {
        for(auto &org : population)
        {
            for(size_t i = 0; i < org.chromosome.size(); ++i)
            {
                org.chromosome[i] = rand() % 2;
            }
        }
    }
};

template<typename GenType>
class GaussianInitialization : public Initialization<GenType>
{
public:
    GaussianInitialization(double deviation, double mean = 0) :
        distribution_(mean, deviation)
    {
        static_assert(!std::is_same<GenType, bool>::value,
                 "GaussianInitialization doesn't work with binary encoding!");
    }
    void initialize(Population<GenType> &population) override
    {
        for(auto &org : population)
        {
            for(size_t i = 0; i < org.chromosome.size(); ++i)
            {
                org.chromosome[i] = distribution_(generator_);
            }
        }
    }

private:
    std::default_random_engine generator_;
    std::normal_distribution<GenType> distribution_;
};

}

#endif // INITIALIZATION_H
