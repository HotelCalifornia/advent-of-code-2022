#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <new>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

class Item {
    std::string::value_type i;
    Item(const std::string::value_type& c) : i(c) {};
public:
    int get_priority() const {
        if ('a' <= i && i <= 'z') {
            return (i - 'a') + 1;
        } else if ('A' <= i && i <= 'Z') {
            return (i - 'A') + 27;
        } else {
            throw std::range_error(
                std::string("item with character ") + i + " is invalid"
            );
        }
    };
    static Item from_c(const std::string::value_type& c) {
        return Item(c);
    };

    auto operator<=>(const Item&) const = default;
};

class Rucksack {
    std::vector<Item> items;
    std::vector<Item> compartment_1;
    std::vector<Item> compartment_2;

    Rucksack(std::vector<Item>&& i) : items(std::move(i)) {};

    Rucksack(std::vector<Item>&& c1, std::vector<Item>&& c2) : 
        compartment_1(std::move(c1)),
        compartment_2(std::move(c2)) 
    {};
public:
    auto get_items() const -> std::vector<Item>&& {
        std::vector<Item> items;
        items.reserve(compartment_1.size() + compartment_2.size());
        items.insert(items.end(), compartment_1.begin(), compartment_1.end());
        items.insert(items.end(), compartment_2.begin(), compartment_2.end());

        return std::move(items);
    };

    auto get_shared_item() const -> std::optional<Item> {
        std::vector<Item>::size_type midpoint = items.size() / 2;
        std::vector<Item> c1(items.begin(), items.begin() + midpoint);
        for (auto item : c1) {
            if (auto f = std::find(
							items.begin() + midpoint,
							items.end(),
							item
				         );
                     f != items.end())
            {
                return item;
            }
        }
        return std::nullopt;
    };

	auto get_shared_item(
            const Rucksack& member_2,
            const Rucksack& member_3
    ) const -> std::optional<Item> {
        
        std::cout << "Debug:\tget_shared_item" << std::endl;
        std::vector<Item> m1_items, m2_items, m3_items;
        std::copy(items.begin(), items.end(), std::back_inserter(m1_items));
        std::copy(member_2.items.begin(), member_2.items.end(), std::back_inserter(m2_items));
        std::copy(member_3.items.begin(), member_3.items.end(), std::back_inserter(m3_items));

        std::cout << "Debug:\t\tcopied items" << std::endl;
        
        std::sort(m1_items.begin(), m1_items.end());
        std::sort(m2_items.begin(), m2_items.end());
        std::sort(m3_items.begin(), m3_items.end());

        std::cout << "Debug:\t\tsorted items" << std::endl;

        std::vector<Item> m1x2;
        std::set_intersection(
            m1_items.begin(), m1_items.end(),
            m2_items.begin(), m2_items.end(),
            std::back_inserter(m1x2)
        );

        std::cout << "Debug:\t\tcomputed m1 ^ m2" << std::endl;

        std::vector<Item> m1x3;
        std::set_intersection(
            m1_items.begin(), m1_items.end(),
            m3_items.begin(), m3_items.end(),
            std::back_inserter(m1x3)
        );

        std::cout << "Debug:\t\tcomputed m1 ^ m3" << std::endl;

        std::vector<Item> intersection;
        std::set_intersection(
            m1x2.begin(), m1x2.end(),
            m1x3.begin(), m1x3.end(),
            std::back_inserter(intersection)
        );

        std::cout << "Debug:\t\tcomputed (m1 ^ m2) ^ (m1 ^ m3)" << std::endl;

        if (not intersection.empty()) {
            std::cout << "Debug:\t\tfound shared item " << intersection.front().get_priority() << std::endl;
            return intersection.front();
        }

		return std::nullopt;
	};

    static auto make_rucksack(std::string descriptor) -> Rucksack {
        std::vector<Item> is;
        std::transform(descriptor.begin(), descriptor.end(), std::back_inserter(is), Item::from_c);

        return Rucksack(std::move(is));
    };
};

struct line {
    std::string data;

    operator std::string() const { return data; };
};

std::istream& operator>>(std::istream& str, line& data) {
    std::getline(str, data.data);
    return str;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " input_file" << std::endl;
        return -1;
    }

    std::ifstream input(argv[1]);
    if (not input) {
        std::cerr << "Error: could not open file " << argv[1] << std::endl;
        return -1;
    }

    std::vector<Rucksack> rucksacks;
    std::transform(
        std::istream_iterator<line>(input),
        std::istream_iterator<line>(),
        std::back_inserter(rucksacks),
        Rucksack::make_rucksack
    );

    int total_priorities = 0;
    for (auto r : rucksacks) {
        if (auto i = r.get_shared_item(); i) {
            total_priorities += i->get_priority();
        }
    }

    std::cout << "The total priority value of mispacked items is "
              << total_priorities
              << std::endl;

    total_priorities = 0;
    for (auto it = rucksacks.begin(); it + 2 < rucksacks.end(); it += 3) {
        std::cout << "Debug: considering group at " << std::distance(rucksacks.begin(), it) << std::endl;
        if (auto badge = it->get_shared_item(*(it + 1), *(it + 2)); badge) {
            std::cout << "Debug:\tfound badge " << badge->get_priority() << std::endl;
            total_priorities += badge->get_priority();
        }
    }

    std::cout << "The total priority value of group badges is "
              << total_priorities
              << std::endl;
}
