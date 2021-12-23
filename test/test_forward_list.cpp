#include "tinystl_algo.h"
#include "tinystl_forward_list.h"
#include <iostream>
#include <ostream>
#include <string>
#include <vector>
#include <forward_list>
#include <numeric>

// template init
template class tinystd::forward_list<int>;

void __test_std_forward_list();
void __test_normal_use();
void __test_constructor();
void __test_operator_eq();
void __test_assign();
void __test_front();
void __test_begin();
void __test_empty();
void __test_maxsize();
void __test_clear();
void __test_insert_after();
void __test_erase_after();

int main() {
    std::vector<std::pair<std::string, void (*)()>> __test_cases{
        { "test STL's forward_list", __test_std_forward_list },
        { "test normal use", __test_normal_use },
        { "test constructor", __test_constructor },
        { "test operator equal. ", __test_operator_eq },
        { "test assign. ", __test_assign },
        { "test front. ", __test_front },
        { "test begin. ", __test_begin },
        { "test empty. ", __test_empty },
        { "test max_szie. ", __test_maxsize },
        { "test clear. ", __test_clear },
        { "test insert_after. ", __test_insert_after },
        { "test erase_after. ", __test_erase_after },
    };

    for (const std::pair<std::string, void (*)()> &__p : __test_cases) {
        std::cout << "\n>> " << __p.first << " ========================" << std::endl;  
        __p.second();
    }
}

template <typename T>
std::ostream &operator<<(std::ostream &s, const std::forward_list<T> &v) {
    s.put('[');
    char comma[3] = {'\0', ' ', '\0'};
    for (const auto &e : v) {
        s << comma << e;
        comma[0] = ',';
    }
    return s << ']';
}


template <typename T>
std::ostream &operator<<(std::ostream &s, const tinystd::forward_list<T> &v) {
    s.put('[');
    char comma[3] = {'\0', ' ', '\0'};
    for (const auto &e : v) {
        s << comma << e;
        comma[0] = ',';
    }
    return s << ']';
}

struct __obj {
    __obj() : _M_pi(new int) {}
    __obj(int i) : _M_pi(new int(i)) {}
    __obj(const __obj &__o) : _M_pi(new int(*__o._M_pi)) {}
    ~__obj() { delete _M_pi; }
    __obj &operator=(const __obj &__o) {
        if (&__o == this) return *this;
        delete _M_pi;
        _M_pi = new int(*__o._M_pi);
        return *this;
    }

    int *_M_pi;
};

void __test_std_forward_list() {
    std::cout << "test1: Normal use" << std::endl;
    {
        std::forward_list<int> mylist = {34, 77, 16, 2};
        std::cout << "mylist contains:";
        for (auto it = mylist.begin(); it != mylist.end(); ++it)
            std::cout << ' ' << *it;
        std::cout << '\n';
    }
    std::cout << "test2: constructor: " << std::endl;
    {
        // c++11 initializer list syntax:
        std::forward_list<std::string> words1{"the", "frogurt", "is", "also", "cursed"};
        std::cout << "words1: " << words1 << '\n';

        // words2 == words1
        std::forward_list<std::string> words2(words1.begin(), words1.end());
        std::cout << "words2: " << words2 << '\n';

        // words3 == words1
        std::forward_list<std::string> words3(words1);
        std::cout << "words3: " << words3 << '\n';

        // words4 is {"Mo", "Mo", "Mo", "Mo", "Mo"}
        std::forward_list<std::string> words4(5, "Mo");
        std::cout << "words4: " << words4 << '\n';
    }
}
void __test_normal_use() {
    std::cout << "test1: Normal use" << std::endl;
    {
        tinystd::forward_list<int> mylist = {34, 77, 16, 2};
        std::cout << "mylist contains:";
        for (auto it = mylist.begin(); it != mylist.end(); ++it)
            std::cout << ' ' << *it;
        std::cout << '\n';
    }
}

void __test_constructor() {
    // c++11 initializer list syntax:
    tinystd::forward_list<std::string> words1{"the", "frogurt", "is", "also", "cursed"};
    std::cout << "words1: " << words1 << '\n';

    // words2 == words1
    tinystd::forward_list<std::string> words2(words1.begin(), words1.end());
    std::cout << "words2: " << words2 << '\n';

    // words3 == words1
    tinystd::forward_list<std::string> words3(words1);
    std::cout << "words3: " << words3 << '\n';

    // words4 is {"Mo", "Mo", "Mo", "Mo", "Mo"}
    tinystd::forward_list<std::string> words4(5, "Mo");
    std::cout << "words4: " << words4 << '\n';
}

void display_sizes(const tinystd::forward_list<int> &nums1,
                   const tinystd::forward_list<int> &nums2,
                   const tinystd::forward_list<int> &nums3) {
    std::cout << "nums1: " << tinystd::distance(nums1.begin(), nums1.end())
              << " nums2: " << tinystd::distance(nums2.begin(), nums2.end())
              << " nums3: " << tinystd::distance(nums3.begin(), nums3.end()) << '\n';
}

/**
 * @brief 
 * Initially:
 * nums1: 6 nums2: 0 nums3: 0
 * After assigment:
 * nums1: 6 nums2: 6 nums3: 0
 * After move assigment:
 * nums1: 0 nums2: 6 nums3: 6
 * 
 */
void __test_operator_eq() {
    tinystd::forward_list<int> nums1{3, 1, 4, 6, 5, 9};
    tinystd::forward_list<int> nums2;
    tinystd::forward_list<int> nums3;

    std::cout << "Initially:\n";
    display_sizes(nums1, nums2, nums3);

    // copy assignment copies data from nums1 to nums2
    nums2 = nums1;

    std::cout << "After assigment:\n";
    display_sizes(nums1, nums2, nums3);

    // move assignment moves data from nums1 to nums3,
    // modifying both nums1 and nums3
    nums3 = std::move(nums1);

    std::cout << "After move assigment:\n";
    display_sizes(nums1, nums2, nums3);
}

/**
 * @brief 
 * a a a a a 
 * b b b b b b 
 * C + + 1 1
 * 
 */
void __test_assign() {
    tinystd::forward_list<char> characters;
 
    auto print_forward_list = [&](){
        for (char c : characters)
            std::cout << c << ' ';
        std::cout << '\n';  
    };
 
    characters.assign(5, 'a');
    print_forward_list();
 
    const std::string extra(6, 'b');
    characters.assign(extra.begin(), extra.end());
    print_forward_list();
 
    characters.assign({'C', '+', '+', '1', '1'});
    print_forward_list();
}

/**
 * @brief 
 * The first character is 'o'.
 */
void __test_front() {
    tinystd::forward_list<char> letters {'o', 'm', 'g', 'w', 't', 'f'};
 
    if (!letters.empty()) {
        std::cout << "The first character is '" << letters.front() << "'.\n";
    }  
}

/**
 * @brief 
 * 1 2 4 8 16
 * Sum of nums: 31
 * First fruit: orange
 * forward_list 'empty' is indeed empty.
 */
void __test_begin() {
    tinystd::forward_list<int> nums {1, 2, 4, 8, 16};
    tinystd::forward_list<std::string> fruits {"orange", "apple", "raspberry"};
    tinystd::forward_list<char> empty;
 
    // Print forward_list.
    tinystd::for_each(nums.begin(), nums.end(), [](const int n) { std::cout << n << ' '; });
    std::cout << '\n';
 
    // Sums all integers in the forward_list nums (if any), printing only the result.
    std::cout << "Sum of nums: "
              << std::accumulate(nums.begin(), nums.end(), 0) << '\n';

    // Prints the first fruit in the forward_list fruits, checking if there is any.
    if (!fruits.empty())
        std::cout << "First fruit: " << *fruits.begin() << '\n';
 
    if (empty.begin() == empty.end())
        std::cout << "forward_list 'empty' is indeed empty.\n";
}

/**
 * @brief 
 * Initially, numbers.empty(): true
 * After adding elements, numbers.empty(): false
 */
void __test_empty() {
    tinystd::forward_list<int> numbers;
    std::cout << std::boolalpha;
    std::cout << "Initially, numbers.empty(): " << numbers.empty() << '\n';
 
    numbers.push_front(42);
    numbers.push_front(13317); 
    std::cout << "After adding elements, numbers.empty(): " << numbers.empty() << '\n';
}


const char *separate(unsigned long long n) {
    static char buf[64];

    int i{sizeof(buf) - 1}, j{};
    buf[i] = '\0';

    do {
        buf[--i] = '0' + (n % 10);
        if (j++ % 3 == 2)
            buf[--i] = '\'';
    } while (n /= 10);

    return buf + i + (buf[i] == '\'' ? 1 : 0);
}

/**
 * @brief 
 * Maximum size of a 'forward_list' is 1'152'921'504'606'846'975
 */
void __test_maxsize() {
    tinystd::forward_list<char> s;
    std::cout << "Maximum size of a 'forward_list' is " << separate(s.max_size()) << "\n";
}

/**
 * @brief 
 * Before clear: 1 2 3
 * Clear
 * After clear:
 */
void __test_clear() {
    tinystd::forward_list<int> container{1, 2, 3};

    auto print = [](const int &n) { std::cout << " " << n; };

    std::cout << "Before clear:";
    tinystd::for_each(container.begin(), container.end(), print);
    std::cout << '\n';

    std::cout << "Clear\n";
    container.clear();

    std::cout << "After clear:";
    tinystd::for_each(container.begin(), container.end(), print);
    std::cout << '\n';
}

/**
 * @brief 
 * words: [the, frogurt, is, also, cursed]
 * words: [the, strawberry, frogurt, is, also, cursed]
 * words: [the, strawberry, strawberry, strawberry, frogurt, is, also, cursed]
 * words: [the, strawberry, strawberry, strawberry, apple, banana, cherry, frogurt, is, also, cursed]
 * words: [the, strawberry, strawberry, strawberry, apple, banana, cherry, jackfruit, kiwifruit, lime, mango, frogurt, is, also, cursed]
 */
void __test_insert_after() {
    tinystd::forward_list<std::string> words {"the", "frogurt", "is", "also", "cursed"};
    std::cout << "words: " << words << '\n';                                    
 
    // insert_after (2)                                                         
    auto beginIt = words.begin();                                               
    words.insert_after(beginIt, "strawberry");                                  
    std::cout << "words: " << words << '\n';                                    
 
    // insert_after (3)                                                         
    auto anotherIt = beginIt;                                                   
    ++anotherIt;                                                                
    anotherIt = words.insert_after(anotherIt, 2, "strawberry");                 
    std::cout << "words: " << words << '\n';                                    
 
    // insert_after (4)
    std::vector<std::string> V = { "apple", "banana", "cherry"};                
    anotherIt = words.insert_after(anotherIt, V.begin(), V.end());              
    std::cout << "words: " << words << '\n';                                    
 
    // insert_after (5)                                                         
    words.insert_after(anotherIt, {"jackfruit", "kiwifruit", "lime", "mango"});
    std::cout << "words: " << words << '\n';   
}

/**
 * @brief 
 * 2 3 4 5 6 7 8 9
 * 2 3 6 7 8 9
 */
void __test_erase_after() {
    tinystd::forward_list<int> l = {1, 2, 3, 4, 5, 6, 7, 8, 9};

    //    l.erase( l.begin() ); // ERROR: No function erase

    l.erase_after(l.before_begin()); // Removes first element

    for (auto n : l) std::cout << n << " ";
    std::cout << '\n';

    auto fi = std::next(l.begin());
    auto la = std::next(fi, 3);

    l.erase_after(fi, la);

    for (auto n : l) std::cout << n << " ";
    std::cout << '\n';
}