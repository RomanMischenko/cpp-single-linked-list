#pragma once

#include <iterator>
#include <cassert>
#include <algorithm>

template <typename Type>
class SingleLinkedList {
    struct Node {
        Node() = default;
        Node(const Type& val, Node* next)
            : value(val)
            , next_node(next) {
        }
        Type value;
        Node* next_node = nullptr;
    };

    template <typename ValueType>
    class BasicIterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = Type;
        using difference_type = std::ptrdiff_t;
        using pointer = ValueType*;
        using reference = ValueType&;

        BasicIterator() = default;

        BasicIterator(const BasicIterator<Type>& other) noexcept 
        : BasicIterator(other.node_)
        {}

        BasicIterator& operator=(const BasicIterator& rhs) = default;

        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
            return node_ == rhs.node_;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
            return !(node_ == rhs.node_);
        }

        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
            if (node_ == rhs.node_) {
                return true;
            } else {
                return false;
            }
        }

        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
            if (node_ != rhs.node_) {
                return true;
            } else {
                return false;
            }
        }

        BasicIterator& operator++() noexcept {
            assert(node_!=nullptr);
            node_ = node_->next_node;
            return *this;
        }

        BasicIterator operator++(int) noexcept {
            auto old_node(*this);
            ++(*this);
            return old_node;
        }

        [[nodiscard]] reference operator*() const noexcept {
            assert(node_!=nullptr);
            return node_->value;
        }

        [[nodiscard]] pointer operator->() const noexcept {
            assert(node_!=nullptr);
            return &(node_->value);
        }
    
    private:
        friend class SingleLinkedList;
        explicit BasicIterator(Node* node)
        : node_(node) 
        {}
        Node* node_ = nullptr;
    };

public:

    SingleLinkedList()
    : size_(0) 
    {}

    SingleLinkedList(std::initializer_list<Type> values) 
    : size_(0)
    {
        for (auto it = std::rbegin(values); it != std::rend(values); ++it) {
            PushFront(*it); 
        }
    }

    SingleLinkedList(const SingleLinkedList& other) 
    : size_(0)
    {
        SingleLinkedList tmp;

        auto it_begin = tmp.before_begin();
        for (auto it = other.begin(); it != other.end(); ++it) {
            it_begin = tmp.InsertAfter(it_begin, *it);
        }

        swap(tmp);
    }

    SingleLinkedList& operator=(const SingleLinkedList& rhs) {
        if (this != &rhs) {
            SingleLinkedList tmp(rhs);
            swap(tmp);
        }
        return *this;
    }

    void swap(SingleLinkedList& other) noexcept {
        std::swap(size_, other.size_);
        std::swap(head_.next_node, other.head_.next_node);
    }

    ~SingleLinkedList() {
        Clear();
    }

    void PushFront(const Type& value) {
        Node* next_ptr = new Node(value, head_.next_node);
        head_.next_node = next_ptr;
        ++size_;
    }

    void Clear() noexcept {
        while (head_.next_node != nullptr) {
            Node* tmp_to_del = head_.next_node;
            head_.next_node = head_.next_node->next_node;
            delete tmp_to_del;
        }
        size_ = 0;
    }

    // Возвращает количество элементов в списке за время O(1)
    [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    }

    // Сообщает, пустой ли список за время O(1)
    [[nodiscard]] bool IsEmpty() const noexcept {
        if (size_ == 0) {
            return true;
        } else {
            return false;
        }
    }

    using Iterator = BasicIterator<Type>;
    using ConstIterator = BasicIterator<const Type>;

    [[nodiscard]] Iterator begin() noexcept {
        return Iterator{head_.next_node};
    }

    [[nodiscard]] Iterator end() noexcept {
        return Iterator{nullptr};
    }

    [[nodiscard]] ConstIterator begin() const noexcept {
        return ConstIterator{head_.next_node};
    }

    [[nodiscard]] ConstIterator end() const noexcept {
        return ConstIterator{nullptr};
    }

    [[nodiscard]] ConstIterator cbegin() const noexcept {
        return ConstIterator{head_.next_node};
    }
    
    [[nodiscard]] ConstIterator cend() const noexcept {
        return ConstIterator{nullptr};
    }

    // Возвращает итератор, указывающий на позицию перед первым элементом односвязного списка.
    // Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] Iterator before_begin() noexcept {
        return Iterator{&head_};
    }

    // Возвращает константный итератор, указывающий на позицию перед первым элементом односвязного списка.
    // Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
        return ConstIterator{const_cast<Node*>(&head_)};
    }

    // Возвращает константный итератор, указывающий на позицию перед первым элементом односвязного списка.
    // Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] ConstIterator before_begin() const noexcept {
        return ConstIterator{&head_};
    }

    /*
     * Вставляет элемент value после элемента, на который указывает pos.
     * Возвращает итератор на вставленный элемент
     * Если при создании элемента будет выброшено исключение, список останется в прежнем состоянии
     */
    Iterator InsertAfter(ConstIterator pos, const Type& value) {
        assert(pos.node_ != nullptr);
        Node* next_ptr = new Node{value, pos.node_->next_node};

        pos.node_->next_node = next_ptr;
        
        ++size_;
        return Iterator{next_ptr};
    }

    void PopFront() noexcept {
        if (head_.next_node == nullptr) {
            return;
        }

        Node* to_del = head_.next_node;
        head_.next_node = head_.next_node->next_node;
        delete to_del;
        --size_;
    }

    /*
     * Удаляет элемент, следующий за pos.
     * Возвращает итератор на элемент, следующий за удалённым
     */
    Iterator EraseAfter(ConstIterator pos) noexcept {
        assert(pos.node_ != nullptr);
        if (pos.node_->next_node == nullptr) {
            return Iterator{end()};
        }

        Node* to_del = pos.node_->next_node;
        pos.node_->next_node = pos.node_->next_node->next_node;
        delete to_del;
        --size_;
        
        return Iterator{pos.node_->next_node};
    }

private:
    // Фиктивный узел, используется для вставки "перед первым элементом"
    Node head_;
    size_t size_;
};

template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
    lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::equal(
        lhs.begin(), lhs.end(), rhs.begin(), rhs.end()
    );
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(std::equal(
        lhs.begin(), lhs.end(), rhs.begin(), rhs.end()
    ));
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::lexicographical_compare(
        lhs.begin(), lhs.end(), rhs.begin(), rhs.end()
    );
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return (lhs < rhs) || (lhs == rhs);
}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(std::lexicographical_compare(
        lhs.begin(), lhs.end(), rhs.begin(), rhs.end()
    ));
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return (lhs > rhs) || (lhs == rhs);
} 

