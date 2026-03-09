#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <random>
#include <string>
#include <vector>

const std::string TARGET = "methinksitislikeaweasel";
const std::size_t TARGET_LENGTH = 23;
const double MUTATION_RATE = 0.05;
const std::size_t POPULATION_SIZE = 100;

char random_letter(std::default_random_engine& engine)
{
    std::uniform_int_distribution<int> distribution(0, 25);
    return static_cast<char>('a' + distribution(engine));
}

std::string random_string(std::default_random_engine& engine)
{
    std::string result(TARGET_LENGTH, ' ');
    for (std::size_t i = 0; i < TARGET_LENGTH; ++i)
    {
        result[i] = random_letter(engine);
    }
    return result;
}

std::string mutate_string(const std::string& source, std::default_random_engine& engine)
{
    std::uniform_real_distribution<double> mutation_distribution(0.0, 1.0);
    std::string result = source;
    
    for (std::size_t i = 0; i < TARGET_LENGTH; ++i)
    {
        if (mutation_distribution(engine) < MUTATION_RATE)
        {
            result[i] = random_letter(engine);
        }
    }
    
    return result;
}

int calculate_score(const std::string& candidate)
{
    int score = 0;
    for (std::size_t i = 0; i < TARGET_LENGTH; ++i)
    {
        if (candidate[i] == TARGET[i])
        {
            ++score;
        }
    }
    return score;
}

int main()
{
    std::random_device device;
    std::default_random_engine engine(device());
    
    std::string current = random_string(engine);
    int generation = 0;
    
    std::cout << "Target: " << TARGET << "\n\n";
    std::cout << "Generation 0: " << current << " (score: " 
              << calculate_score(current) << "/" << TARGET_LENGTH << ")\n";
    
    while (true)
    {
        std::vector<std::string> population;
        std::vector<int> scores;
        
        for (std::size_t i = 0; i < POPULATION_SIZE; ++i)
        {
            std::string mutant = mutate_string(current, engine);
            int score = calculate_score(mutant);
            
            population.push_back(mutant);
            scores.push_back(score);
            
            if (score == static_cast<int>(TARGET_LENGTH))
            {
                std::cout << "\nFound target in generation " << generation + 1 << "!\n";
                std::cout << "Final string: " << mutant << "\n";
                return 0;
            }
        }
        
        auto best_iter = std::max_element(scores.begin(), scores.end());
        std::size_t best_index = std::distance(scores.begin(), best_iter);
        current = population[best_index];
        
        ++generation;
        
        if (generation % 10 == 0 || generation == 1)
        {
            std::cout << "Generation " << generation << ": " << current 
                      << " (score: " << *best_iter << "/" << TARGET_LENGTH << ")\n";
        }
    }
    
    return 0;
}