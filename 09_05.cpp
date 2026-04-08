#include <iterator>
#include <memory>

template < typename T > class List
{
private:

	struct Node
	{ 
		T x = T();

		std::shared_ptr < Node > next;
		std::weak_ptr < Node > prev;
	};
	
public:

	class Iterator
	{
	public:

		using iterator_category = std::bidirectional_iterator_tag;
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using pointer = T*;
		using reference = T&;

		Iterator(std::shared_ptr < Node > node = nullptr) : m_node(node) {}

		auto operator++(int)
		{
			auto x = *this;

			m_node = m_node->next;

			return x;
		}

		auto & operator++()
		{
			m_node = m_node->next;

			return *this;
		}

		auto operator--(int)
		{
			auto x = *this;

			m_node = m_node->prev.lock();

			return x;
		}

		auto & operator--()
		{
			m_node = m_node->prev.lock();

			return *this;
		}

		auto & operator* () const { return  m_node->x; }

		auto   operator->() const { return &m_node->x; }

		friend auto operator==(Iterator const & lhs, Iterator const & rhs)
		{
			return lhs.m_node == rhs.m_node;
		}

	private:

		std::shared_ptr < Node > m_node;
	};

	auto begin() const { return Iterator(m_head); }

	auto end  () const { return Iterator(      ); }

	void push_back(T x)
	{
		auto node = std::make_shared < Node > (x, nullptr, std::weak_ptr<Node>());

		if (m_head)
		{
			auto tail = m_head;

			while (tail->next) 
			{
				tail = tail->next;
			}

			tail->next = node;
			node->prev = tail;
		}
		else
		{
			m_head = node;
		}
	}

private:

	std::shared_ptr < Node > m_head;
};

int main()
{
	List < int > list;

	list.push_back(1);
	list.push_back(2);
	list.push_back(3);

	for (auto iterator = std::begin(list); iterator != std::end(list); ++iterator);

	for ([[maybe_unused]] auto element : list);

	return 0;
}