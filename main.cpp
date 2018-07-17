#include <iostream>
#include <cmath>
#include <chrono>
#include "geneticalgorithm.h"

using namespace std;
using namespace ga;

int main()
{
    auto t1 = chrono::high_resolution_clock::now();
    GeneticAlgorithm<bool> ga(10, 30);
    ga.setInitializationAlgorithm(make_unique<BinaryInitialization>());
    ga.setPrepopulationAlgorithm(make_unique< EliteStrategy<bool> >(2));
    ga.setSelectionAlgorithm(make_unique< TournamentSelection<bool> >(4));
    ga.setCrossoverAlgorithm(make_unique< MultiPointCrossover<bool> >(2));
    ga.setMutationAlgorithm(make_unique<BinaryMutation>());
    ga.setStoppingCriteria(make_unique< FitnessCriteria<bool> >(1));
    ga.setFitnessFunction([](Organism<bool> &org) {
        int number = 0;
        for(size_t i = 0; i < org.chromosome.size(); ++i)
        {
            if(org.chromosome[i])
            {
                number |= (1 << (org.chromosome.size() - i - 1));
            }
        }
        org.fitness = number * number + 1;
    });

    ga.setDisplayFunction(make_unique< SimpleDisplay<bool> >());

    Organism<bool> res1 = ga.optimize(5);


    GeneticAlgorithm<double> ga2(2, 100);
    ga2.setInitializationAlgorithm(
                make_unique< UniformInitialization<double> >(
                    std::vector<double>{-10, -6.5},
                    std::vector<double>{0, 0}));
    ga2.setPrepopulationAlgorithm(make_unique< EliteStrategy<double> >(2));
    ga2.setSelectionAlgorithm(make_unique< TournamentSelection<double> >(4));
    ga2.setCrossoverAlgorithm(make_unique< IntermediateCrossover<double> >(5));
    ga2.setMutationAlgorithm(make_unique< GaussianMutation<double> >(1));
    ga2.setStoppingCriteria(make_unique< FitnessCriteria<double> >(-106));
    ga2.setLinearBounds({-10, -6.5}, {0, 0});
    ga2.setDisplayFunction(make_unique< SimpleDisplay<double> >());
    ga2.setFitnessFunction([](Organism<double> &org) {
        const double &x = org.chromosome[0];
        const double &y = org.chromosome[1];
        org.fitness = sin(y) * exp(pow(1 - cos(x), 2)) + cos(x) *
                      exp(pow(1 - sin(y), 2)) + pow(x - y, 2);
    });

    Organism<double> res2 = ga2.optimize(1);


    GeneticAlgorithm<double> ga3(2, 50);
    ga3.setInitializationAlgorithm(
                make_unique< UniformInitialization<double> >(
                    std::vector<double>{-4, -4},
                    std::vector<double>{4, 4}));
    ga3.setPrepopulationAlgorithm(make_unique< EliteStrategy<double> >(2));
    ga3.setSelectionAlgorithm(make_unique< RouletteSelection<double> >());
    ga3.setCrossoverAlgorithm(make_unique< IntermediateCrossover<double> >(5));
    ga3.setMutationAlgorithm(make_unique< GaussianMutation<double> >(1));
    ga3.setStoppingCriteria(make_unique< FitnessCriteria<double> >(2.3, false));
    ga3.setLinearBounds({-4, -4}, {4, 4});
    ga3.setDisplayFunction(make_unique< SimpleDisplay<double> >());
    ga3.setFitnessFunction([](Organism<double> &org) {
        const double &x = org.chromosome[0];
        const double &y = org.chromosome[1];
        org.fitness = exp(-(x * x) - (y * y)) + sin(x + y) + 1;
    });

    Organism<double> res3 = ga3.optimize(1, false);


    GeneticAlgorithm<double> ga4(2, 50);
    ga4.setInitializationAlgorithm(
                make_unique< UniformInitialization<double> >(
                    std::vector<double>{-30, -30},
                    std::vector<double>{30, 30}));
    ga4.setPrepopulationAlgorithm(make_unique< EliteStrategy<double> >(2));
    ga4.setSelectionAlgorithm(make_unique< TournamentSelection<double> >(4));
    ga4.setCrossoverAlgorithm(make_unique< IntermediateCrossover<double> >(5));
    ga4.setMutationAlgorithm(make_unique< GaussianMutation<double> >(1));
    ga4.setStoppingCriteria(make_unique< FitnessCriteria<double> >(-0.9));
    ga4.setLinearBounds({-30, -30}, {30, 30});
    ga4.setDisplayFunction(make_unique< SimpleDisplay<double> >());
    ga4.setFitnessFunction([](Organism<double> &org) {
        const double &x = org.chromosome[0];
        const double &y = org.chromosome[1];
        org.fitness = -cos(x) * cos(y) * exp(-(pow(x - M_PI, 2) +
                                               pow(y - M_PI, 2)));
    });

    Organism<double> res4 = ga4.optimize(1);

    cout << endl << "Results:" << endl;
    cout << res1 << " " << "Fit: " << res1.fitness <<
            ", Desired: 1 (MIN)" << endl;
    cout << res2 << " " << "Fit: " << res2.fitness <<
            ", Desired: ~ -106 (MIN),\n\tBounds (-10, 0),  (-6.5, 0)" << endl;
    cout << res3 << " " << "Fit: " << res3.fitness <<
            ", Desired: ~ 2.3 (MAX),\n\tBounds (-4, 4),  (-4, 4)" << endl;
    cout << res4 << " " << "Fit: " << res4.fitness <<
            ", Desired: ~ -0.9 (MIN),\n\tBounds (-30, 30),  (-30, 30)" << endl;
    auto t2 = chrono::high_resolution_clock::now();
    cout << "Time: " <<
            chrono::duration_cast<chrono::microseconds>(t2 - t1).count() <<
            " us" << endl;

    return 0;
}
