#ifndef CROSSOVER_H
#define CROSSOVER_H

#include <iostream>
#include <set>
#include "organism.h"
#include "geneticalgorithm.h"
#include <random>

namespace ga
{

template<typename GenType>
class Crossover
{
public:
    virtual std::vector< Organism<GenType> >
    crossover(const Organism<GenType> &,
              const Organism<GenType> &) = 0;
    virtual ~Crossover() = default;
};

template<typename GenType>
class DiscreteCrossover : public Crossover<GenType>
{
public:
    std::vector< Organism<GenType> >
    crossover(const Organism<GenType> &lhs,
              const Organism<GenType> &rhs) override
    {
        std::vector<Organism <GenType> > offspring;
        offspring.emplace_back(lhs.chromosome.size());
        offspring.emplace_back(lhs.chromosome.size());
        for(size_t i = 0; i < lhs.chromosome.size(); ++i)
        {
            if(rand() % 2)
            {
                offspring[0].chromosome[i] = lhs.chromosome[i];
                offspring[1].chromosome[i] = rhs.chromosome[i];
            }
            else
            {
                offspring[1].chromosome[i] = lhs.chromosome[i];
                offspring[0].chromosome[i] = rhs.chromosome[i];
            }
        }

        return offspring;
    }
};

template<typename GenType>
class IntermediateCrossover : public Crossover<GenType>
{
public:
    IntermediateCrossover(double deviation, double mean = 0) :
        distribution_(mean, deviation)
    {
        static_assert(!std::is_same<GenType, bool>::value,
                "IntermediateCrossover doesn't work with binary encoding!");
    }
    std::vector< Organism<GenType> >
    crossover(const Organism<GenType> &lhs,
              const Organism<GenType> &rhs) override
    {
        std::vector<Organism <GenType> > offspring;
        offspring.emplace_back(lhs.chromosome.size());
        for(size_t i = 0; i < lhs.chromosome.size(); ++i)
        {
            double alpha = distribution_(generator_);
            offspring[0].chromosome[i] = lhs.chromosome[i] +
                    alpha * (rhs.chromosome[i] - lhs.chromosome[i]);
        }


        return offspring;
    }

private:
    std::default_random_engine generator_;
    std::normal_distribution<GenType> distribution_;
};

template<typename GenType>
class LinearCrossover : public Crossover<GenType>
{
public:
    LinearCrossover(double deviation, double mean = 0) :
        distribution_(mean, deviation)
    {
        static_assert(!std::is_same<GenType, bool>::value,
                      "LinearCrossover doesn't work with binary encoding!");
    }
    std::vector< Organism<GenType> >
    crossover(const Organism<GenType> &lhs,
              const Organism<GenType> &rhs) override
    {
        std::vector<Organism <GenType> > offspring;
        offspring.emplace_back(lhs.chromosome.size());
        double alpha = distribution_(generator_);
        for(size_t i = 0; i < lhs.chromosome.size(); ++i)
        {
            offspring[0].chromosome[i] = lhs.chromosome[i] +
                    alpha * (rhs.chromosome[i] - lhs.chromosome[i]);
        }

        return offspring;
    }

private:
    std::default_random_engine generator_;
    std::normal_distribution<GenType> distribution_;
};

template<typename GenType>
class SinglePointCrossover : public Crossover<GenType>
{
public:
    std::vector< Organism<GenType> >
    crossover(const Organism<GenType> &lhs,
              const Organism<GenType> &rhs) override
    {
        std::vector<Organism <GenType> > offspring;
        offspring.emplace_back(lhs.chromosome.size());
        offspring.emplace_back(lhs.chromosome.size());
        size_t mutationPoint = rand() % lhs.chromosome.size();
        for(size_t i = 0; i < lhs.chromosome.size(); ++i)
        {
            if(i < mutationPoint)
            {
                offspring[0].chromosome[i] = lhs.chromosome[i];
                offspring[1].chromosome[i] = rhs.chromosome[i];
            }
            else
            {
                offspring[1].chromosome[i] = lhs.chromosome[i];
                offspring[0].chromosome[i] = rhs.chromosome[i];
            }
        }

        return offspring;
    }
};

template<typename GenType>
class MultiPointCrossover : public Crossover<GenType>
{
public:
    MultiPointCrossover(size_t size = 2) : size_(size) {}
    std::vector< Organism<GenType> >
    crossover(const Organism<GenType> &lhs,
              const Organism<GenType> &rhs) override
    {
        std::vector<Organism <GenType> > offspring;
        offspring.emplace_back(lhs.chromosome.size());
        offspring.emplace_back(lhs.chromosome.size());
        std::vector<size_t> points(size_);
        std::set<size_t> pointSet;
        while(pointSet.size() < size_)
        {
            pointSet.insert(rand() % lhs.chromosome.size());
        }
        std::copy(pointSet.begin(), pointSet.end(), points.begin());
        bool reverse = false;
        size_t pointIndex = 0;
        for(size_t i = 0; i < lhs.chromosome.size(); ++i)
        {
            if(i >= points[pointIndex] && i <= points.back())
            {
                reverse = !reverse;
                pointIndex++;
            }
            if(reverse)
            {
                offspring[0].chromosome[i] = lhs.chromosome[i];
                offspring[1].chromosome[i] = rhs.chromosome[i];
            }
            else
            {
                offspring[1].chromosome[i] = lhs.chromosome[i];
                offspring[0].chromosome[i] = rhs.chromosome[i];
            }
        }

        return offspring;
    }

private:
    const size_t size_;
};

}

#endif // CROSSOVER_H
