#ifndef MUTATION_H
#define MUTATION_H

#include "organism.h"
#include "geneticalgorithm.h"
#include <random>

namespace ga
{

template<typename GenType>
class Mutation
{
public:
    virtual void mutation(std::vector<GenType> &) = 0;
    virtual ~Mutation() = default;
};

template<typename GenType>
class GaussianMutation : public Mutation<GenType>
{
public:
    GaussianMutation(double deviation, double mean = 0) :
        distribution_(mean, deviation)
    {
        static_assert(!std::is_same<GenType, bool>::value,
                      "GaussianMutation doesn't work with binary encoding!");
    }
    void mutation(std::vector<GenType> &chromosome) override
    {
        for(size_t i = 0; i < chromosome.size(); ++i)
        {
            chromosome[i] += distribution_(generator_);
        }
    }

private:
    std::default_random_engine generator_;
    std::normal_distribution<GenType> distribution_;
};

template<typename GenType>
class UniformMutation : public Mutation<GenType>
{
public:
    UniformMutation(size_t quantity = 1, double max = 1, double min = 0) :
        quantity_(quantity), max_(max), min_(min)
    {
        static_assert(!std::is_same<GenType, bool>::value,
                      "UniformMutation doesn't work with binary encoding!");
    }

    void mutation(std::vector<GenType> &chromosome) override
    {
        for(size_t i = 0; i < quantity_; ++i)
        {
            chromosome[i] = min_ + static_cast<double>(rand()) /
                    (static_cast<double>(RAND_MAX / (max_ - min_)));
        }
    }

private:
    size_t quantity_;
    double max_;
    double min_;
};

// Only for binary chromosomes (bool)
class BinaryMutation : public Mutation<bool>
{
public:
    void mutation(std::vector<bool> &chromosome) override
    {
        for(size_t i = 0; i < chromosome.size(); ++i)
        {
            chromosome[i] = (rand() % 2) ? !chromosome[i] : chromosome[i];
        }
    }
};

template<typename GenType>
class ExchangeMutation : public Mutation<GenType>
{
    void mutation(std::vector<GenType> &chromosome) override
    {
        std::swap(chromosome[rand() % chromosome.size()],
                  chromosome[rand() % chromosome.size()]);
    }
};

}

#endif // MUTATION_H
