#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <unordered_set>
#include <vector>

/*
Обоснование формы зависимости количества коллизий от числа хешируемых строк:
 
  При малом количестве строк вероятность коллизий мала для любой достаточно
  хорошей хеш-функции. С ростом числа строк количество коллизий возрастает
  нелинейно, примерно как N^2 / (2 * M), где M - размер выходного пространства
  хеш-функции (для 32-битных хешей M = 2^32). Худшие функции демонстрируют
  более быстрый рост из-за неравномерного распределения или лавинного эффекта.
 
  Лучшие функции: DJB, SDBM, BKDR (равномерное распределение, мало коллизий).
  Худшие: RS (из-за переполнения множителя a), JS (слабая диффузия).
*/

unsigned int RSHash(const char* str, unsigned int length)
{
    unsigned int b    = 378551;
    unsigned int a    = 63689;
    unsigned int hash = 0;
    for (unsigned int i = 0; i < length; ++i, ++str)
    {
        hash = hash * a + static_cast<unsigned int>(*str);
        a    = a * b;
    }
    return hash;
}

unsigned int JSHash(const char* str, unsigned int length)
{
    unsigned int hash = 1315423911;
    for (unsigned int i = 0; i < length; ++i, ++str)
    {
        hash ^= ((hash << 5) + static_cast<unsigned int>(*str) + (hash >> 2));
    }
    return hash;
}

unsigned int PJWHash(const char* str, unsigned int length)
{
    const unsigned int BitsInUnsignedInt = sizeof(unsigned int) * 8;
    const unsigned int ThreeQuarters     = (BitsInUnsignedInt * 3) / 4;
    const unsigned int OneEighth         = BitsInUnsignedInt / 8;
    const unsigned int HighBits          = 0xFFFFFFFFu << (BitsInUnsignedInt - OneEighth);
    unsigned int hash = 0;
    unsigned int test = 0;
    for (unsigned int i = 0; i < length; ++i, ++str)
    {
        hash = (hash << OneEighth) + static_cast<unsigned int>(*str);
        if ((test = hash & HighBits) != 0)
        {
            hash = ((hash ^ (test >> ThreeQuarters)) & (~HighBits));
        }
    }
    return hash;
}

unsigned int ELFHash(const char* str, unsigned int length)
{
    unsigned int hash = 0;
    unsigned int x    = 0;
    for (unsigned int i = 0; i < length; ++i, ++str)
    {
        hash = (hash << 4) + static_cast<unsigned int>(*str);
        if ((x = hash & 0xF0000000u) != 0)
        {
            hash ^= (x >> 24);
        }
        hash &= ~x;
    }
    return hash;
}

unsigned int BKDRHash(const char* str, unsigned int length)
{
    unsigned int seed = 131;
    unsigned int hash = 0;
    for (unsigned int i = 0; i < length; ++i, ++str)
    {
        hash = (hash * seed) + static_cast<unsigned int>(*str);
    }
    return hash;
}

unsigned int SDBMHash(const char* str, unsigned int length)
{
    unsigned int hash = 0;
    for (unsigned int i = 0; i < length; ++i, ++str)
    {
        hash = static_cast<unsigned int>(*str) + (hash << 6) + (hash << 16) - hash;
    }
    return hash;
}

unsigned int DJBHash(const char* str, unsigned int length)
{
    unsigned int hash = 5381;
    for (unsigned int i = 0; i < length; ++i, ++str)
    {
        hash = ((hash << 5) + hash) + static_cast<unsigned int>(*str);
    }
    return hash;
}

unsigned int DEKHash(const char* str, unsigned int length)
{
    unsigned int hash = length;
    for (unsigned int i = 0; i < length; ++i, ++str)
    {
        hash = ((hash << 5) ^ (hash >> 27)) ^ static_cast<unsigned int>(*str);
    }
    return hash;
}

unsigned int APHash(const char* str, unsigned int length)
{
    unsigned int hash = 0xAAAAAAAA;
    for (unsigned int i = 0; i < length; ++i, ++str)
    {
        hash ^= ((i & 1) == 0) ? ((hash <<  7) ^ (static_cast<unsigned int>(*str)) * (hash >> 3))
                               : (~((hash << 11) + ((static_cast<unsigned int>(*str)) ^ (hash >> 5))));
    }
    return hash;
}

std::vector<std::string> generate_random_strings(std::size_t count, std::size_t length)
{
    std::vector<std::string> strings;
    strings.reserve(count);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<char> dist('a', 'z');
    for (std::size_t i = 0; i < count; ++i)
    {
        std::string str(length, '\0');
        for (std::size_t j = 0; j < length; ++j)
        {
            str[j] = dist(gen);
        }
        strings.push_back(str);
    }
    return strings;
}

std::size_t count_collisions(const std::vector<std::string>& strings,
                             unsigned int (*hash_func)(const char*, unsigned int))
{
    std::unordered_set<unsigned int> seen_hashes;
    std::size_t collisions = 0;
    for (const auto& str : strings)
    {
        unsigned int h = hash_func(str.c_str(), static_cast<unsigned int>(str.size()));
        if (seen_hashes.contains(h))
        {
            ++collisions;
        }
        else
        {
            seen_hashes.insert(h);
        }
    }
    return collisions;
}

struct HashFunction
{
    const char* name;
    unsigned int (*func)(const char*, unsigned int);
};

int main()
{
    const std::size_t string_length = 16;
    const std::size_t step = 5000;
    const std::size_t max_strings = 100000;

    auto all_strings = generate_random_strings(max_strings, string_length);

    HashFunction hash_functions[] = {
        {"RS",   RSHash},
        {"JS",   JSHash},
        {"PJW",  PJWHash},
        {"ELF",  ELFHash},
        {"BKDR", BKDRHash},
        {"SDBM", SDBMHash},
        {"DJB",  DJBHash},
        {"DEK",  DEKHash},
        {"AP",   APHash}
    };

    std::ofstream data_file("collision_data.csv");
    if (!data_file)
    {
        std::cerr << "Failed to create collision_data.csv\n";
        return 1;
    }

    data_file << "count";
    for (const auto& hf : hash_functions)
    {
        data_file << "," << hf.name;
    }
    data_file << "\n";

    for (std::size_t count = step; count <= max_strings; count += step)
    {
        std::vector<std::string> current_strings(all_strings.begin(), all_strings.begin() + count);
        data_file << count;
        for (const auto& hf : hash_functions)
        {
            std::size_t collisions = count_collisions(current_strings, hf.func);
            data_file << "," << collisions;
        }
        data_file << "\n";
        
        if (count % 10000 == 0)
        {
            std::cout << "Processed " << count << " strings...\n";
        }
    }
    data_file.close();

    std::cout << "Data written to collision_data.csv\n\n";

    std::size_t test_count = 100000;
    std::vector<std::string> test_strings(all_strings.begin(), all_strings.begin() + test_count);

    std::cout << "Collisions for " << test_count << " strings:\n";
    std::cout << "----------------------------------------\n";
    
    HashFunction best = hash_functions[0];
    HashFunction worst = hash_functions[0];
    std::size_t best_collisions = count_collisions(test_strings, best.func);
    std::size_t worst_collisions = best_collisions;

    for (const auto& hf : hash_functions)
    {
        std::size_t collisions = count_collisions(test_strings, hf.func);
        std::cout << hf.name << ": " << collisions << " collisions\n";
        
        if (collisions < best_collisions)
        {
            best = hf;
            best_collisions = collisions;
        }
        if (collisions > worst_collisions)
        {
            worst = hf;
            worst_collisions = collisions;
        }
    }

    std::cout << "\n----------------------------------------\n";
    std::cout << "Best hash function: " << best.name << " (" << best_collisions << " collisions)\n";
    std::cout << "Worst hash function: " << worst.name << " (" << worst_collisions << " collisions)\n";

    return 0;
}