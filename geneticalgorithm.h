#ifndef GENETICALGORITHM_H
#define GENETICALGORITHM_H

#include <algorithm>
#include <functional>
#include <memory>
#include <ctime>
#include <thread>

#include "organism.h"
#include "initialization.h"
#include "fitnesscaling.h"
#include "prepopulation.h"
#include "selection.h"
#include "crossover.h"
#include "mutation.h"
#include "stoppingcriteria.h"
#include "display.h"

namespace ga
{

template<typename T>
using InitializationPtr = std::unique_ptr<Initialization<T>>;

template<typename T>
using FitnessScalingPtr = std::unique_ptr<FitnessScaling<T>>;

template<typename T>
using PrepopulationPtr = std::unique_ptr<Prepopulation<T>>;

template<typename T>
using SelectionPtr = std::unique_ptr<Selection<T>>;

template<typename T>
using CrossoverPtr = std::unique_ptr<Crossover<T>>;

template<typename T>
using MutationPtr = std::unique_ptr<Mutation<T>>;

template<typename T>
using StoppingPtr = std::unique_ptr<StoppingCriteria<T>>;

template<typename T>
using DisplayPtr = std::unique_ptr<Display<T>>;

template<typename GenType>
class GeneticAlgorithm
{
public:
    GeneticAlgorithm(size_t chromosomeSize, size_t populationSize = 50) :
        initialization_(nullptr),
        scale_(nullptr),
        prepopulation_(nullptr),
        selection_(nullptr),
        crossover_(nullptr),
        mutation_(nullptr),
        stopping_(nullptr),
        display_(nullptr),
        gen_(std::random_device()()),
        distribution_(0, 100),
        numberOfThreads_(1)
    {
        srand(time(nullptr));
        for(size_t i = 0; i < populationSize; ++i)
        {
            population_.emplace_back(chromosomeSize);
        }
    }
    void setInitializationAlgorithm(InitializationPtr<GenType> initialization)
    {
        initialization_ = std::move(initialization);
    }

    void setFitnessScaleAlgorithm(FitnessScalingPtr<GenType> scale)
    {
        scale_ = std::move(scale);
    }

    void setPrepopulationAlgorithm(PrepopulationPtr<GenType> prepopulation)
    {
        prepopulation_ = std::move(prepopulation);
    }

    void setSelectionAlgorithm(SelectionPtr<GenType> selection)
    {
        selection_ = std::move(selection);
    }
    void setCrossoverAlgorithm(CrossoverPtr<GenType> crossover)
    {
        crossover_ = std::move(crossover);
    }
    void setMutationAlgorithm(MutationPtr<GenType> mutation)
    {
        mutation_ = std::move(mutation);
    }
    void setStoppingCriteria(StoppingPtr<GenType> stopping)
    {
        stopping_ = std::move(stopping);
    }
    void setFitnessFunction(std::function<void(Organism<GenType> &)> fitness)
    {
        fitnessFunction_ = fitness;
    }
    void setDisplayFunction(DisplayPtr<GenType> display)
    {
        display_ = std::move(display);
    }

    void setLinearBounds(std::vector<GenType> &&lower,
                         std::vector<GenType> &&upper)
    {
        lowerBounds_ = lower;
        upperBounds_ = upper;
    }

    Organism<GenType> optimize(double mutationProbability = 0.1,
                  bool minimize = true,
                  unsigned int numberOfThreads = 1)
    {
        numberOfThreads_ = numberOfThreads;
        initialization_->initialize(population_);
        calcFitnessForPopulation();
        for(unsigned long i = 0; !stopping_->stop(i, population_); ++i)
        {
            calcFitnessForPopulation();
            if(minimize)
            {
                std::sort(population_.begin(), population_.end());
            }
            else
            {
                std::sort(population_.begin(), population_.end(),
                          std::greater<Organism <GenType> >());
            }
            if(scale_) scale_->scale(population_, minimize);
            std::vector< Organism<GenType> > nextPopulation;
            if(prepopulation_)
            {
                prepopulation_->prepopulate(population_, nextPopulation);
            }
            const auto parentPool = selection_->selection(population_);
            while(nextPopulation.size() < population_.size())
            {
                size_t first = rand() % parentPool.size();
                size_t second = rand() % parentPool.size();
                const auto offspring =
                        crossover_->crossover(parentPool[first],
                                              parentPool[second]);
                nextPopulation.insert(nextPopulation.end(),
                                      offspring.begin(),
                                      offspring.end());
            }
            while(nextPopulation.size() > population_.size())
            {
                nextPopulation.pop_back();
            }
            if(mutation_)
            {
                for(auto &organism : nextPopulation)
                {
                    if(distribution_(gen_) <= mutationProbability)
                    {
                        mutation_->mutation(organism.chromosome);
                    }
                }
            }

            if(display_) display_->display(population_, i);

            population_ = std::move(nextPopulation);
        }

        calcFitnessForPopulation();
        if(minimize)
        {
            std::sort(population_.begin(), population_.end());
        }
        else
        {
            std::sort(population_.begin(), population_.end(),
                      std::greater<Organism <GenType> >());
        }

        return population_.front();
    }

private:
    Population<GenType> population_;
    InitializationPtr<GenType> initialization_;
    FitnessScalingPtr<GenType> scale_;
    PrepopulationPtr<GenType>  prepopulation_;
    SelectionPtr<GenType> selection_;
    CrossoverPtr<GenType> crossover_;
    MutationPtr<GenType> mutation_;
    StoppingPtr<GenType> stopping_;
    DisplayPtr<GenType> display_;

    std::vector<GenType> lowerBounds_;
    std::vector<GenType> upperBounds_;

    std::function<void(Organism<GenType> &)> fitnessFunction_;

    std::mt19937_64 gen_;
    std::uniform_real_distribution<> distribution_;

    unsigned int numberOfThreads_;

    void calcFitnessForPopulation()
    {
        if(numberOfThreads_ <= 1)
        {
            calcFitnessForPopulationPart(0, population_.size());
        }
        else
        {
            size_t step = population_.size() / numberOfThreads_;
            size_t currentPosition = 0;
            std::thread threads[numberOfThreads_];
            for(unsigned int i = 0; i < (numberOfThreads_ - 1); ++i)
            {
                threads[i] = std::thread(&GeneticAlgorithm<GenType>::
                                         calcFitnessForPopulationPart,
                              this, currentPosition, currentPosition + step);
                currentPosition += step;
            }
            threads[numberOfThreads_ - 1] =
                    std::thread(&GeneticAlgorithm<GenType>::
                                calcFitnessForPopulationPart,
                          this, currentPosition, population_.size());
            for(size_t i = 0; i < numberOfThreads_; ++i)
            {
                threads[i].join();
            }
        }
    }

    void calcFitnessForPopulationPart(size_t start, size_t end)
    {
        for(size_t i = start; i < end; ++i)
        {
            auto &organism = population_[i];
            for(size_t i = 0; i < lowerBounds_.size(); ++i)
            {
                if(organism.chromosome[i] < lowerBounds_[i])
                {
                    organism.chromosome[i] = lowerBounds_[i];
                }
            }
            for(size_t i = 0; i < upperBounds_.size(); ++i)
            {
                if(organism.chromosome[i] > upperBounds_[i])
                {
                    organism.chromosome[i] = upperBounds_[i];
                }
            }
            fitnessFunction_(organism);
        }
    }
};

}


#endif // GENETICALGORITHM_H
