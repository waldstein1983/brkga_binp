#include <iostream>
#include <unistd.h>
#include <time.h>

#include "BinPackingDecoder2N.h"
#include "ORLibraryInstanceReader.h"
#include "Instance.h"
#include "MTRand.h"
#include "BRKGA.h"

void printUsage() {
	std::cout << "Usage:" << std::endl
		 << "\t-i\tInput file" << std::endl
		 << "\t-p\tSize of population (default: 1000)" << std::endl
		 << "\t-e\tFraction of population to be elite (default: 0.2)" << std::endl
		 << "\t-m\tFraction of population to be replaced by mutants (default: 0.1)" << std::endl
		 << "\t-n\tProbability that offspring inherit an allele from elite parent (default: 0.7)" << std::endl
		 << "\t-k\tNumber of independent populations (default: 1)" << std::endl
		 << "\t-s\tRandom seed (default: 0)" << std::endl
		 << "\t-g\tNumber of generations (default: 100)" << std::endl
		 << "\t-t\tNumber of threads (default: 1)" << std::endl;
}

struct Config {
	Config() : population(1000), frac_elite(0.2), frac_mutants(0.1), prob_elite(0.7), independent_pop(1), seed(0), generations(100), threads(1) {}
	std::string filename;		// instance file
	unsigned population;		// size of population
	float frac_elite;			// fraction of population to be the elite-set
	float frac_mutants;			// fraction of population to be replaced by mutants
	float prob_elite;			// probability that offspring inherit an allele from elite parent
	unsigned independent_pop;	// number of independent populations
	unsigned seed;				// seed for random number generator
	unsigned generations;		// number of generations
	unsigned threads;			// number of threads (for parallel processing)
};

Config parseCommandLine(int argc, char** argv) {
	if ( argc == 1 ) {
		printUsage();
		exit(0);
	}

	Config config;

	int opt;
	while ((opt = getopt(argc, argv, "i:p:e:m:n:k:s:g:t:")) != -1)
		switch(opt) {
			case 'i':
				config.filename = optarg;
				break;
			case 'p':
				config.population = atoi(optarg);
				break;
			case 'e':
				config.frac_elite = atof(optarg);
				break;
			case 'm':
				config.frac_mutants = atof(optarg);
				break;
			case 'n':
				config.prob_elite = atof(optarg);
				break;
			case 'k':
				config.independent_pop = atoi(optarg);
				break;
			case 's':
				config.seed = atoi(optarg);
				break;
			case 'g':
				config.generations = atoi(optarg);
				break;
			case 't':
				config.threads = atoi(optarg);
				break;
			case '?':
				std::cout << "Syntax error" << std::endl;
				exit(1);
		}

	return config;
}


int main(int argc, char* argv[]) {
	Config config = parseCommandLine(argc, argv);

	BinPackingDecoder2N decoder(FitnessCalculator::FALKENAUER_FITNESS);
	MTRand rng(config.seed);
	Instance instance = ORLibraryInstanceReader::readInstance(config.filename);

	BRKGA< BinPackingDecoder2N, MTRand > algorithm(instance.getNumberOfObjects() * 2, config.population, config.frac_elite,
			config.frac_mutants, config.prob_elite, decoder, rng, config.independent_pop, config.threads);
	
	unsigned generation = 0;		// current generation
  
	clock_t start = clock();

	do {
		algorithm.evolve();	// evolve the population for one generation
		generation++;
	} while (generation < config.generations);
	
	clock_t end = clock();
	float seconds = (float)(end - start) / CLOCKS_PER_SEC;

	std::cout << decoder.boxesUsed(algorithm.getBestChromosome()) << " " << seconds << std::endl;
	//std::cout << algorithm.getBestFitness() << std::endl;
	return 0;
}
