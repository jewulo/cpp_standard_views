#pragma once

#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <numeric>
#include <ranges>
#include <string>
#include <thread>
#include <vector>

namespace ex0
{
    template <typename T>
    void print(const T& coll)
    {
        for (const auto& elem : coll) {
            std::cout << elem << ' ';
        }
        std::cout << '\n';
    }
    
    void run_print()
    {
        std::vector<int> vec0{0, 8, 15, 47, 11, 42};
        print(vec0);

        std::list<int> lst0{0, 8, 15, 47, 11, 42};
        print(lst0);

        // you can use Class Template Argument Deduction (CTAD)
        std::vector vec1{0, 8, 15, 47, 11, 42};
        print(vec1);

        std::list lst1{0, 8, 15, 47, 11, 42};
        print(lst1);

    }    
}

namespace ex1
{
    // NOTE:
    // void print(const auto& coll) {}
    //      is equivalent to
    // template <typename T>
    // void print(const T& coll) {}
    //
    void print(const auto& coll)
    {
        for (const auto& elem : coll) {
            std::cout << elem << ' ';
        }
        std::cout << '\n';
    }

    void run_print()
    {
        std::vector<int> vec0{0, 8, 15, 47, 11, 42};
        print(vec0);

        std::list<int> lst0{0, 8, 15, 47, 11, 42};
        print(lst0);

        // you can use Class Template Argument Deduction (CTAD)
        std::vector vec1{0, 8, 15, 47, 11, 42};
        print(vec1);

        std::list lst1{0, 8, 15, 47, 11, 42};
        print(lst1);
    }
}

namespace ex2
{
    // use a concept std::ranges::input_range
    void print(const std::ranges::input_range auto& coll)
    {
        for (const auto& elem : coll) {
            std::cout << elem << ' ';
        }
        std::cout << '\n';
    }

    void run_print_with_views()
    {
        std::vector<int> vec0{0, 8, 15, 47, 11, 42};
        print(vec0);

        std::list<int> lst0{0, 8, 15, 47, 11, 42};
        print(lst0);

        // you can use Class Template Argument Deduction (CTAD)
        std::vector vec1{0, 8, 15, 47, 11, 42};
        print(vec1);

        std::list lst1{0, 8, 15, 47, 11, 42};
        print(lst1);

        // you can use views to constrain the view on the collection or container
        print(std::views::take(vec0, 3));   // print first thre elements
        print(std::views::take(lst0, 3));   // print first thre elements

        // you can also use the pipe symbol | with views
        print(vec0 | std::views::take(3));   // print first thre elements
        print(lst0 | std::views::take(3));   // print first thre elements

        // complex pipeing
        print(vec0 | std::views::take(3)
                   | std::views::transform([](auto v) {
                                                return std::to_string(v) + 's';
                                            }));
    }
}

namespace ex3
{
    void filter_composers_by_year()
    {
        // https://youtu.be/qv29fo9sUjY?t=424
        std::map<std::string, int> composers {
            { "Bach", 1685 },
            { "Mozart", 1756 },
            { "Beethoven", 1770 },
            { "Tachaikovski", 1840 },
            { "Chopin", 1810 },
            { "Vivaldi", 1678 }
        };

        // iterate over the names of the first composers since 1700
        namespace vws = std::views;
        for (const auto& elem : composers
            | vws::filter([](const auto& y) {
                return y.second >= 1700;
                })
            | vws::take(3)
            | vws::keys)
        {
            std::cout << "- " << elem << '\n';
        }
    }
}

namespace ex4
{
    void zip_views()
    {
        std::vector<std::string> coll{"one", "two", "three"};

        for (auto elem : coll) {
            std::cout << elem << '\n';
        }
        
        for (auto [idx, elem] : std::views::zip(std::views::iota(1), coll)) {
            std::cout << idx << ": " << elem << '\n';
        }

        for (auto [idx, elem] : std::ranges::views::zip(std::views::iota(1), coll)) {
            std::cout << idx << ": " << elem << '\n';
        }
    }
}

namespace ex5
{
    void map_views()
    {
        std::map<std::string, int> composers {
            { "Bach", 1685 },
            { "Mozart", 1756 },
            { "Beethoven", 1770 },
            { "Tachaikovski", 1840 },
            { "Chopin", 1810 },
            { "Vivaldi", 1678 }
        };

        namespace vws = std::views;

        for (auto& elem : composers
                            | vws::filter([](const auto& y) {   // since 1700
                                            return y.second >= 1700;
                                           })
                            | vws::take(3)                      // first 3
                            | vws::keys)                        // names only
        {
            std::cout << "- " << elem << '\n';
        }
    }
}

namespace ex6
{
    void iota_views()
    {
        auto v = std::views::iota(1,100)            // from 1 to 99
            | std::views::filter([](auto val) {     // multiples of 3 only
                                    return val % 3 == 0;
                                })
            | std::views::drop(3)                   // skip first 3
            | std::views::take(8)                   // take next 8
            | std::views::transform([](auto val) {
                                        return std::to_string(val) + "s";
                                    });

        for (const auto& elem : v) {
            std::cout << elem << '\n';
        }
    }
}

namespace ex7
{
    void print(const auto& rg)
    {
        for (auto pos = std::ranges::begin(rg); pos != std::ranges::end(rg); ++pos) {
            std::cout << *pos << ' ';
        }
        std::cout << '\n';
    }

    void run_how_views_operate()
    {
        std::vector<int> coll{42, 9, 0, 16, 7, -1, 13};
        print(coll);

        print(coll | std::views::take(3)
                   | std::views::transform(std::negate{}));

        auto v1 = std::views::take(coll, 3);
        auto v2 = std::views::transform(v1, std::negate{});

        auto pos = v2.begin();
        std::cout << "*pos: " << *pos << '\n';
        ++pos;
        std::cout << "*pos: " << *pos << '\n';
    }
}

namespace ex8
{
    void print(const auto& coll)
    {
        std::cout << coll.size() << " elems\n";
    }

    void processing_containers_and_views()
    {
        std::vector<int> vec0{0, 8, 15, 47, 11, 42};
        std::list<int> lst0{0, 8, 15, 47, 11, 42};

        print(vec0);
        print(lst0);
        print(vec0 | std::views::take(3));
        print(lst0 | std::views::take(3));
        print(vec0 | std::views::drop(3));
        print(lst0 | std::views::drop(3));
    }
}

// the behaviour of views is dependent on the container type. 
namespace ex9
{

    void print(const auto& coll)
    {
//        std::cout << coll.size() << " elems: \n";
//        // ERROR
//        std::cout << coll.front() << " ... "
//            << coll.back() << '\n';
    }

    void processing_containers_and_views()
    {
//        std::vector<int> vec0{0, 8, 15, 47, 11, 42};
//        std::list<int> lst0{0, 8, 15, 47, 11, 42};
//
//        print(vec0);
//        print(lst0);
//        print(vec0 | std::views::take(3));
//        print(lst0 | std::views::take(3));  // ERROR
// 
//        print(vec0 | std::views::drop(3));
//        print(lst0 | std::views::drop(3));  // ERROR
// 
//        // ERROR
//        print(vec0 | std::views::filter([](auto elem) {
//            return elem % 3 == 0;
//            }));
//
    }
}

namespace ex10
{
    void member_functions_of_views()
    {
        const auto n = 5;
        const auto idx = 4;

        std::vector vec{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        auto vVec = vec | std::views::drop(n);
        vVec.begin();       // fast: vec.begin() + n
        vVec.empty();       // fast: vec.size() <= n
        vVec.size();        // fast: n >= vec.size() ? 0 : vec.size() - n
        vVec[idx];          // fast: vec[idx + n]

        std::list lst{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        auto vLst = lst | std::views::drop(n);
        vLst.begin();       // slow: lst.begin() and n times ++
        vLst.empty();       // fast: lst.size() <= n
        vLst.size();        // fast: n >= vec.size() ? 0 : vec.size() - n
        //vLst[idx];        // slow: lst.begin() and n + idx times ++ : IILEGAL operation

        // for any collection coll that is filtered
        std::vector coll{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        auto pred = [](auto x) { return x % 2 == 0; };
        auto vFlt = coll | std::views::filter(pred);
        vFlt.begin();       // slow: pred for all elements until first true
        vFlt.empty();       // slow: pred for all elements until first true
        //vFlt.size();      // slow: pred for all elements: IILEGAL operation
        //vFlt[idx];        // slow: pred for all elements until idx times true : IILEGAL operation
    }
}

// class declaration of a range
namespace ex11
{
    template<typename D>
    requires std::is_class_v<D> && std::same_as<D, std::remove_cv_t<D>>
    class view_interface {
    public:

        // (not) empty?
        constexpr bool empty() requires std::ranges::forward_range<D>;
        constexpr explicit operator bool() requires
            requires {
                std::ranges::empty(static_cast<D&>(*this));
        };

        // number of elements:
        constexpr auto size() requires std::ranges::forward_range<D>&&
            std::sized_sentinel_for<std::ranges::sentinel_t<D>, std::ranges::iterator_t<D>>;

        // element access:
        constexpr decltype(auto) front() requires std::ranges::forward_range<D>;
        constexpr decltype(auto) back() requires std::ranges::bidirectional_range<D> && std::ranges::common_range<D>;

        template <std::ranges::random_access_range R = D>
        constexpr decltype(auto) operator[](std::ranges::range_difference_t<R> n);

        constexpr auto data() requires std::contiguous_iterator<std::ranges::iterator_t<D>> {
            return std::to_address(std::ranges::begin(std::ranges::empty(static_cast<D&>(*this))));
        }
    };
}

// processing containers and views: A
namespace ex11
{
    void print(const auto& coll)
    {
        for (const auto& elem : coll) {
            std::cout << elem << ' ';
        }
        std::cout << '\n';
    }

    void run_print()
    {
        std::vector<int> vec0{0, 8, 15, 47, 11, 42};
        std::list<int> lst0{0, 8, 15, 47, 11, 42};

        print(vec0);
        print(lst0);
        print(vec0 | std::views::take(3));  // print first three elements
        print(lst0 | std::views::take(3));  // print first three elements
        print(vec0 | std::views::drop(3));  // print fourth to last elements

        //print(lst0 | std::views::drop(3));  // Compile-Time ERROR
        // ....but it works in a loop
        for (int v : lst0 | std::views::drop(3)) {  // OK: print fourth to last element 
            std::cout << v << ' ';
        }
        // ... or better still
        auto v = lst0 | std::views::drop(3);
        print(std::ranges::subrange{v.begin(), v.end()});
    }
}

// processing containers and views: B
namespace ex12
{
    // use a universal or forwarding reference
    // - can universally refer to every expression
    //   (even temporaries/rvalues) without making it const 
    void print(auto&& coll)
    {
        for (const auto& elem : coll) {
            std::cout << elem << ' ';
        }
        std::cout << '\n';
    }

    void run_print()
    {
        std::vector<int> vec0{0, 8, 15, 47, 11, 42};
        std::list<int> lst0{0, 8, 15, 47, 11, 42};

        print(vec0);
        print(lst0);
        print(vec0 | std::views::take(3));  // print first three elements
        print(lst0 | std::views::take(3));  // print first three elements
        print(vec0 | std::views::drop(3));  // print fourth to last elements
        print(lst0 | std::views::drop(3));  // // OK: print fourth to last element 
        for (int v : lst0 | std::views::drop(3)) {  // OK: print fourth to last element 
            std::cout << v << ' ';
        }
    }
}

// concurrent iterations over views:
namespace ex13
{
    auto printAndSum(auto&& rg)
    {
        // one thread prints the elements
        std::jthread printThread{[&] () {
            for (const auto& elem : rg) {
                std::cout << elem << ' ';
            }
            std::cout << '\n';
        }};

        // this thread computes the sum of the element values
        return std::accumulate(rg.begin(), rg.end(), 0L);
    }

    void run_print()
    {
        std::vector<int> vec0{0, 8, 15, 47, 11, 42};
        std::list<int> lst0{0, 8, 15, 47, 11, 42};

        auto sum1 = printAndSum(vec0);      // OK
        auto sum2 = printAndSum(lst0);      // OK

        auto sum3 = printAndSum(vec0 | std::views::drop(2));      // OK
        auto sum4 = printAndSum(lst0 | std::views::drop(2));      // Runtime ERROR
    }
}

// passing containers and views by value
namespace ex14
{
    void print(auto coll)
    {
        for (const auto& elem : coll) {
            std::cout << elem << ' ';
        }
        std::cout << '\n';
    }

    void run_print()
    {
        std::vector<int> vec0{0, 8, 15, 47, 11, 42};
        std::list<int> lst0{0, 8, 15, 47, 11, 42};

        print(vec0);                                // expensive
        print(lst0);                                // expensive
        print(vec0 | std::views::take(3));          // print first three elements
        print(lst0 | std::views::take(3));          // print first three elements
        print(vec0 | std::views::drop(3));          // print fourth to last elements
        print(lst0 | std::views::drop(3));          // OK: print fourth to last element 
        for (int v : lst0 | std::views::drop(3)) {  // OK: print fourth to last element 
            std::cout << v << ' ';
        }
    }
}

// passing containers and views by value with constraint
namespace ex15
{
    void print(std::ranges::view auto coll)
    {
        for (const auto& elem : coll) {
            std::cout << elem << ' ';
        }
        std::cout << '\n';
    }

    void run_print()
    {
        std::vector<int> vec0{0, 8, 15, 47, 11, 42};
        std::list<int> lst0{0, 8, 15, 47, 11, 42};

        //print(vec0);                                // ERROR: a container is not a view
        //print(lst0);                                // ERROR: a container is not a view
        print(std::views::all(vec0));               // cheap
        print(std::views::all(lst0));               // cheap
        print(vec0 | std::views::take(3));          // print first three elements
        print(lst0 | std::views::take(3));          // print first three elements
        print(vec0 | std::views::drop(3));          // print fourth to last elements
        print(lst0 | std::views::drop(3));          // OK: print fourth to last element 
        for (int v : lst0 | std::views::drop(3)) {  // OK: print fourth to last element 
            std::cout << v << ' ';
        }
    }
}

// overloading for containers and views
namespace ex16
{
    // for views only
    void print(std::ranges::view auto coll)
    {
        for (const auto& elem : coll) {
            std::cout << elem << ' ';
        }
        std::cout << '\n';
    }

    // not for views
    template <typename T>
    void print(const T& coll)
    {
        print(std::views::all(coll));
    }

    auto getColl()
    {
        std::vector<int> v{0, 8, 15, 47, 11, 42};
        return v;
    }

    void run_print()
    {
        std::vector<int> vec{1, 2, 3, 4, 5, 6, 7, 8, 9};

        print(vec);                                 // OK: call 2nd print
        print(vec | std::views::take(3));           // OK: call 1st print

        print(getColl());                          // OK: call 2nd print
        print(getColl() | std::views::take(3));    // OK: call 1st print
    }
}

// overloading for containers and views
// but with explicit requires specialisation 
namespace ex17
{
    // for views only
    void print(std::ranges::view auto coll)
    {
        for (const auto& elem : coll) {
            std::cout << elem << ' ';
        }
        std::cout << '\n';
    }

    // not for views
    template <typename T>
    void print(const T& coll)
    requires (!std::ranges::view<T>)    // this is necessary to avoid ambiguities
    {
        print(std::views::all(coll));
    }

    auto getColl()
    {
        std::vector<int> v{0, 8, 15, 47, 11, 42};

        return v;
    }

    void run_print()
    {
        std::vector<int> vec{1, 2, 3, 4, 5, 6, 7, 8, 9};

        print(vec);                                 // OK: call 2nd print
        print(vec | std::views::take(3));           // OK: call 1st print

        print(getColl());                          // OK: call 2nd print
        print(getColl() | std::views::take(3));    // OK: call 1st print
    }
}

// continue https://youtu.be/qv29fo9sUjY?t=2939
// modifying view elements
namespace ex18
{ }