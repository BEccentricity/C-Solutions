#include <boost/multi_array.hpp>
#include <chrono>
#include <cstddef>
#include <iostream>
#include <thread>

class GameOfLife
{
public:
    static constexpr std::size_t ROWS = 10;
    static constexpr std::size_t COLS = 10;

    using Grid = boost::multi_array<bool, 2>;

    GameOfLife()
        : m_current(Grid(boost::extents[ROWS][COLS]))
        , m_next(Grid(boost::extents[ROWS][COLS]))
    {
        initialize_pattern();
    }

    void run(std::size_t iterations, std::chrono::milliseconds delay)
    {
        for (std::size_t iter = 0; iter < iterations; ++iter)
        {
            std::cout << "Iteration " << iter << ":\n";
            print();
            update();
            std::this_thread::sleep_for(delay);
            std::cout << "\n";
        }
    }

private:
    Grid m_current;
    Grid m_next;

    void initialize_pattern()
    {
        for (std::size_t i = 0; i < ROWS; ++i)
        {
            for (std::size_t j = 0; j < COLS; ++j)
            {
                m_current[i][j] = false;
            }
        }

        m_current[4][3] = true;
        m_current[4][4] = true;
        m_current[4][5] = true;
        m_current[5][4] = true;
        m_current[5][5] = true;
        m_current[5][6] = true;
    }

    int count_neighbors(std::size_t row, std::size_t col) const
    {
        int count = 0;
        for (std::size_t i = (row > 0 ? row - 1 : 0);
             i <= (row < ROWS - 1 ? row + 1 : ROWS - 1); ++i)
        {
            for (std::size_t j = (col > 0 ? col - 1 : 0);
                 j <= (col < COLS - 1 ? col + 1 : COLS - 1); ++j)
            {
                if (i == row && j == col) continue;
                if (m_current[i][j]) ++count;
            }
        }
        return count;
    }

    void update()
    {
        for (std::size_t i = 0; i < ROWS; ++i)
        {
            for (std::size_t j = 0; j < COLS; ++j)
            {
                int neighbors = count_neighbors(i, j);
                if (m_current[i][j])
                {
                    m_next[i][j] = (neighbors == 2 || neighbors == 3);
                }
                else
                {
                    m_next[i][j] = (neighbors == 3);
                }
            }
        }
        m_current = m_next;
    }

    void print() const
    {
        for (std::size_t i = 0; i < ROWS; ++i)
        {
            for (std::size_t j = 0; j < COLS; ++j)
            {
                std::cout << (m_current[i][j] ? '#' : '.') << ' ';
            }
            std::cout << '\n';
        }
        std::cout << std::flush;
    }
};

int main()
{
    GameOfLife game;
    game.run(20, std::chrono::milliseconds(200));
}