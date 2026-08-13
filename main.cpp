#include <iostream>
#include <vector>
#include <boost/iterator/iterator_facade.hpp>

/**
 * Iterator for traversing a vector of vectors (std::vector<std::vector<T>>).
 * Uses Boost.Iterator's iterator_facade to simplify implementation.
 */
template<typename T>
class VectorOfVectorsIterator 
    : public boost::iterators::iterator_facade<
        VectorOfVectorsIterator<T>,
        T,
        boost::iterators::single_pass_traversal_tag,
        const T&
      >
{
public:
    using OuterIterator = typename std::vector<std::vector<T>>::iterator;
    using InnerIterator = typename std::vector<T>::iterator;

    VectorOfVectorsIterator() : outer_(), inner_(), endInner_() {}

    VectorOfVectorsIterator(OuterIterator begin, OuterIterator end)
        : outer_(begin), outerEnd_(end)
    {
        if (outer_ != outerEnd_) {
            inner_ = outer_->begin();
            endInner_ = outer_->end();
            skipEmpty();
        }
    }

private:
    friend class boost::iterators::iterator_core_access;

    const T& dereference() const {
        return *inner_;
    }

    bool equal(const VectorOfVectorsIterator& other) const {
        // Both at end?
        if (outer_ == outerEnd_ && other.outer_ == other.outerEnd_)
            return true;
        // Only one at end?
        if (outer_ == outerEnd_ || other.outer_ == other.outerEnd_)
            return false;
        // Compare positions
        return outer_ == other.outer_ && inner_ == other.inner_;
    }

    void increment() {
        ++inner_;
        if (inner_ == endInner_) {
            ++outer_;
            if (outer_ != outerEnd_) {
                inner_ = outer_->begin();
                endInner_ = outer_->end();
                skipEmpty();
            }
        }
    }

    void skipEmpty() {
        while (outer_ != outerEnd_ && outer_->empty()) {
            ++outer_;
        }
        if (outer_ != outerEnd_) {
            inner_ = outer_->begin();
            endInner_ = outer_->end();
        }
    }

    OuterIterator outer_;
    OuterIterator outerEnd_;
    InnerIterator inner_;
    InnerIterator endInner_;
};

int main() {
    // Create a vector of vectors with some test data
    std::vector<std::vector<int>> data = {
        {1, 2, 3},
        {},           // Empty vector (should be skipped)
        {4, 5},
        {6},
        {},           // Another empty vector
        {7, 8, 9, 10}
    };

    std::cout << "Vector of vectors contents:" << std::endl;
    for (const auto& vec : data) {
        std::cout << "  [";
        for (size_t i = 0; i < vec.size(); ++i) {
            std::cout << vec[i];
            if (i < vec.size() - 1) std::cout << ", ";
        }
        std::cout << "]" << std::endl;
    }

    std::cout << "\nIterating with custom iterator:" << std::endl;
    
    VectorOfVectorsIterator<int> begin(data.begin(), data.end());
    VectorOfVectorsIterator<int> end; // default constructed = end iterator

    int count = 0;
    for (auto it = begin; it != end; ++it) {
        std::cout << "Element " << count++ << ": " << *it << std::endl;
    }

    std::cout << "\nTotal elements iterated: " << count << std::endl;
    std::cout << "Expected total: 10 (skipping 2 empty vectors)" << std::endl;

    return 0;
}
