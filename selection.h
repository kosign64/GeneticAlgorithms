#ifndef SELECTION_H
#define SELECTION_H

#include "organism.h"
#include "geneticalgorithm.h"

namespace ga
{

template<typename GenType>
class Selection
{
public:
    virtual std::vector< Organism<GenType> >
    selection(const std::vector< Organism<GenType> > &) = 0;
    virtual ~Selection() = default;
};

// Works only with maximization problems and positive fitness values
template<typename GenType>
class RouletteSelection : public Selection<GenType>
{
    std::vector< Organism<GenType> >
    selection(const std::vector< Organism<GenType> > &population) override
    {
        std::vector< Organism<GenType> > parentPool;
        std::vector<double> sums(population.size());
        sums[0] = population[1].fitness;
        for(size_t i = 1; i < population.size(); ++i)
        {
            sums[i] = sums[i - 1] + population[i].fitness;
        }
        for(size_t i = 0; i < population.size(); ++i)
        {
            double val = static_cast<double>(rand()) /
                    static_cast<double>(RAND_MAX / sums.back());
            auto org = std::lower_bound(sums.begin(), sums.end(),
                     val);
            parentPool.push_back(population[org - sums.begin()]);
        }

        return parentPool;
    }
};

template<typename GenType>
class TournamentSelection : public Selection<GenType>
{
public:
    TournamentSelection(size_t size) : size_(size) {}
    std::vector< Organism<GenType> >
    selection(const std::vector< Organism<GenType> > &population) override
    {
        std::vector< Organism<GenType> > parentPool;
        for(size_t i = 0; i < population.size(); ++i)
        {
            size_t winner = population.size() - 1;
            for(size_t i = 0; i < size_; ++i)
            {
                size_t tmp = rand() % population.size();
                if(tmp < winner) winner = tmp;
            }

            parentPool.push_back(population[winner]);
        }

        return parentPool;
    }

private:
    size_t size_;
};

}

#endif // SELECTION_H
