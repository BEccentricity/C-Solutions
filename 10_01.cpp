#include <cassert>
#include <cstddef>
#include <deque>
#include <iostream>
#include <vector>

void investigate_vector_growth()
{
    std::vector<int> vector;
    std::size_t previous_capacity = vector.capacity();
    std::cout << "std::vector capacity growth:\n";
    std::cout << "size=" << vector.size() << " capacity=" << previous_capacity << "\n";

    constexpr std::size_t insertions = 1024;
    for (std::size_t i = 0; i < insertions; ++i)
    {
        vector.push_back(static_cast<int>(i));
        if (vector.capacity() != previous_capacity)
        {
            std::cout << "size=" << vector.size() 
                      << " capacity=" << vector.capacity();
            if (previous_capacity > 0)
            {
                double factor = static_cast<double>(vector.capacity()) / static_cast<double>(previous_capacity);
                std::cout << " growth factor=" << factor;
            }
            std::cout << "\n";
            previous_capacity = vector.capacity();
        }
    }
    std::cout << "\n";
}

void investigate_deque_block_size()
{
    std::deque<int> deq;
    constexpr std::size_t insertions = 2048;
    int* previous_address = nullptr;
    std::size_t elements_in_current_block = 0;
    std::size_t block_size = 0;
    bool block_size_determined = false;

    std::cout << "std::deque block size investigation:\n";

    for (std::size_t i = 0; i < insertions; ++i)
    {
        deq.push_back(static_cast<int>(i));
        int* current_address = &deq.back();

        if (previous_address != nullptr)
        {
            std::ptrdiff_t diff = current_address - previous_address;
            if (diff == 1)
            {
                ++elements_in_current_block;
            }
            else
            {
                if (!block_size_determined && elements_in_current_block > 0)
                {
                    block_size = elements_in_current_block;
                    block_size_determined = true;
                    std::cout << "Detected block size (elements per contiguous chunk): " << block_size << "\n";
                }
                elements_in_current_block = 1;
            }
        }
        else
        {
            elements_in_current_block = 1;
        }

        previous_address = current_address;
    }

    if (!block_size_determined && elements_in_current_block > 0)
    {
        block_size = elements_in_current_block;
        std::cout << "Detected block size (elements per contiguous chunk): " << block_size << "\n";
    }

    std::cout << "Addresses of first elements of each block (push_back order):\n";
    deq.clear();
    previous_address = nullptr;
    for (std::size_t i = 0; i < insertions; ++i)
    {
        deq.push_back(static_cast<int>(i));
        int* current_address = &deq.back();
        if (previous_address == nullptr || (current_address - previous_address) != 1)
        {
            std::cout << "Block start at index " << i << " address=" << static_cast<void*>(current_address) << "\n";
        }
        previous_address = current_address;
    }
}

int main()
{
    investigate_vector_growth();
    investigate_deque_block_size();
}