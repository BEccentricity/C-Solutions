#include <algorithm>
#include <boost/graph/adjacency_matrix.hpp>
#include <iostream>
#include <limits>
#include <random>
#include <vector>

const int kVertexCount = 10;
const int kMinWeight = 1;
const int kMaxWeight = 10;

using Graph = boost::adjacency_matrix<boost::undirectedS, boost::no_property,
                                       boost::property<boost::edge_weight_t, int>>;

void initialize_graph(Graph& graph)
{
    std::random_device device;
    std::default_random_engine engine(device());
    std::uniform_int_distribution<int> distribution(kMinWeight, kMaxWeight);

    auto vertices = boost::vertices(graph);
    for (auto vi = vertices.first; vi != vertices.second; ++vi)
    {
        for (auto vj = vi + 1; vj != vertices.second; ++vj)
        {
            int weight = distribution(engine);
            boost::add_edge(*vi, *vj, weight, graph);
        }
    }
}

void print_adjacency_matrix(const Graph& graph)
{
    std::cout << "Adjacency matrix:\n";
    auto vertices = boost::vertices(graph);
    for (auto vi = vertices.first; vi != vertices.second; ++vi)
    {
        for (auto vj = vertices.first; vj != vertices.second; ++vj)
        {
            if (*vi == *vj)
            {
                std::cout << "0\t";
            }
            else
            {
                auto edge = boost::edge(*vi, *vj, graph);
                if (edge.second)
                {
                    int weight = boost::get(boost::edge_weight, graph, edge.first);
                    std::cout << weight << '\t';
                }
                else
                {
                    std::cout << "-\t";
                }
            }
        }
        std::cout << '\n';
    }
}

int calculate_path_cost(const std::vector<int>& path, const Graph& graph)
{
    int total_cost = 0;
    for (std::size_t i = 0; i < path.size() - 1; ++i)
    {
        auto edge = boost::edge(path[i], path[i + 1], graph);
        if (edge.second)
        {
            total_cost += boost::get(boost::edge_weight, graph, edge.first);
        }
    }

    auto edge = boost::edge(path.back(), path.front(), graph);
    if (edge.second)
    {
        total_cost += boost::get(boost::edge_weight, graph, edge.first);
    }

    return total_cost;
}

void solve_tsp(const Graph& graph)
{
    std::vector<int> vertices(kVertexCount);
    for (int i = 0; i < kVertexCount; ++i)
    {
        vertices[i] = i;
    }

    std::vector<int> best_path;
    int best_cost = std::numeric_limits<int>::max();

    do
    {
        int cost = calculate_path_cost(vertices, graph);
        if (cost < best_cost)
        {
            best_cost = cost;
            best_path = vertices;
        }
    } while (std::next_permutation(vertices.begin() + 1, vertices.end()));

    std::cout << "Optimal path: ";
    for (int vertex : best_path)
    {
        std::cout << vertex << ' ';
    }
    std::cout << best_path.front() << '\n';

    std::cout << "Total cost: " << best_cost << '\n';
}

int main()
{
    Graph graph(kVertexCount);

    initialize_graph(graph);

    print_adjacency_matrix(graph);

    solve_tsp(graph);

    return 0;
}