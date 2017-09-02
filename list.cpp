#include <bits/stdc++.h>

template<typename T>
struct list {

private:
    struct node_base {
        node_base *l, *r;
        node_base () : l(nullptr), r(nullptr) {}
        node_base (node_base *l, node_base *r) : l(l), r(r) {}
        virtual ~node_base() {}
    };

    struct node : node_base {
        T val;
        node(T const& val) : val(val) {}
    };
public:

    struct const_iterator;
    struct iterator;

    struct iterator {
    public:
        iterator& operator++() {
            assert(v);
            assert(dynamic_cast<node*>(v) != nullptr);
            v = v->r;
            return *this;
        }
        iterator& operator--() {
            assert(v);
            assert(dynamic_cast<node*>(v->l) != nullptr);
            v = v->l;
            return *this;
        }
        iterator operator++(int) {
            iterator res = *this;
            ++*this;
            return res;
        }
        iterator operator--(int) {
            iterator res = *this;
            --*this;
            return res;
        }

        T & operator*() const {
            assert(v);
            node* cur = dynamic_cast<node*>(v);
            assert(cur);
            return cur->val;
        }
        friend bool operator==(const iterator& a, const iterator& b) {
            return a.v == b.v;
        }
        friend bool operator!=(const iterator& a, const iterator& b) {
            return a.v != b.v;
        }

        typedef std::ptrdiff_t difference_type;
        typedef T value_type;
        typedef T const * pointer;
        typedef T const & reference;
        typedef std::bidirectional_iterator_tag iterator_category;
    private:
        node_base* v;
        explicit iterator(node_base* v) : v(v) {}

        friend list;
        friend const_iterator;
    };

    struct const_iterator {
    public:
        const_iterator& operator++() {
            assert(v);  
            assert(dynamic_cast<node*>(v) != nullptr);
            v = v->r;
            return *this;
        }
        const_iterator& operator--() {
            assert(v);
            assert(dynamic_cast<node*>(v->l) != nullptr);
            v = v->l;
            return *this;
        }
        const_iterator operator++(int) {
            const_iterator res = *this;
            ++*this;
            return res;
        }
        const_iterator operator--(int) {
            const_iterator res = *this;
            --*this;
            return res;
        }

        T const& operator*() const {
            assert(v);
            node* cur = dynamic_cast<node*>(v);
            assert(cur);
            return cur->val;
        }
        friend bool operator==(const const_iterator &a, const const_iterator &b) {
            return a.v == b.v;
        }
        friend bool operator!=(const const_iterator &a, const const_iterator &b) {
            return a.v != b.v;
        }

        typedef std::ptrdiff_t difference_type;
        typedef T value_type;
        typedef T const * pointer;
        typedef T const & reference;
        typedef std::bidirectional_iterator_tag iterator_category;

        const_iterator(iterator it) : v(it.v) {}
    private:
        node_base * v;
        explicit const_iterator(node_base * v) : v(v) {}

        friend list;
    };

    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    list() {
        end_ = new node_base;
        end_->l = end_;
        end_->r = end_;
    }
    ~list() {
        clear();
        delete end_;
    }
    list(list<T> const& other) : list() {
        for (const_iterator it = other.begin(); it != other.end(); ++it)
            push_back(*it);
    }
    list<T>& operator=(list const& other) {
        list<T> cur(other);
        swap(*this, cur);
        return *this;
    }
    bool empty() const {
        return begin() == end();
    }
    void clear() {
        if (!empty())
            erase(begin(), end());
    }

    void push_back(T const& val) {
        insert(end(), val);
    }
    void pop_back() {
        erase(--end());
    }
    T& back() {
        iterator it(end_);
        --it;
        return *it;
    }
    T const& back() const {
        const_iterator it(end_);
        --it;
        return *it;
    }

    // front
    void push_front(T const& val) {
        if (empty()) {
            node_base* newNode = new node(val);
            newNode->r = newNode->l = end_;
            end_->l = end_->r = newNode;
        } else {
            node_base* nextNode = end_->r;
            node_base* newNode = new node(val);
            nextNode->l = end_->r = newNode;
            newNode->r = nextNode;
            newNode->l = end_;
        }
    }
    void pop_front() {
        erase(begin());
    }
    T& front() {
        iterator it(end_);
        ++it;
        return *it;
    }
    T const& front() const {
        const_iterator it(end_);
        ++it;
        return *it;
    }

    //iterator
    iterator begin() {
        return iterator(end_->r);
    }
    iterator end() {
        return iterator(end_);
    }
    const_iterator begin() const {
        return const_iterator(end_->r);
    }
    const_iterator end() const {
        return const_iterator(end_);
    }
    reverse_iterator rbegin() {
        return reverse_iterator(end());
    }
    reverse_iterator rend() {
        return reverse_iterator(begin());
    }
    const_reverse_iterator rbegin() const {
        return const_reverse_iterator(end());
    }
    const_reverse_iterator rend() const {
        return const_reverse_iterator(begin());
    }

    iterator insert(const_iterator it, T const& val) {
        if (empty()) {
            node_base* newNode = new node(val);
            newNode->r = end_;
            newNode->l = end_;
            end_->r = newNode;
            end_->l = newNode;
            return begin();
        }
        node_base* prev = it.v->l, *next = it.v;
        node_base* newNode = new node(val);
        newNode->r = next;
        newNode->l = prev;
        prev->r = newNode;
        next->l = newNode;
        return iterator(newNode);
    }
    iterator erase(const_iterator it) {
        assert(it != end());
        node_base* prev = it.v->l, *next = it.v;
        if (next->r == end_ && next->l == end_) {
            end_->r = end_;
            end_->l = end_;
            delete next;
        } else {
            next->l->r = next->r;
            next->r->l = next->l;
            delete next;
        }
        return iterator(prev->r);
    }
    void splice(const_iterator pos, list<T>& other, const_iterator first, const_iterator last) {
        node_base *prev1 = first.v->l;
        node_base *next1 = last.v;
        node_base *prev2 = pos.v->l;
        node_base *next2 = pos.v;

        prev2->r = first.v;
        first.v->l = prev2;
        last.v->l->r = next2;
        next2->l = last.v->l;

        prev1->r = next1;
        next1->l = prev1;
    }

    template<typename U>
    friend void swap(list<U>& a, list<U>& b);

private:
    iterator erase(const_iterator first, const_iterator last) {
        while (first != last) {
            first = erase(first);
        }
        return iterator(first.v);
    }
    node_base* end_;
};


template<typename T>
void swap(list<T>& a, list<T>& b) {
    typename list<T>::node_base* cur = a.end_;
    a.end_ = b.end_;
    b.end_ = cur;
}

template<typename T>
void print(list<T>& l) {
    for (auto it = l.begin(); it != l.end(); ++it) {
        std::cout << *it << ' ';
    }
    std::cout << '\n';
}

int main() {
    list<int> x, y;
    return 0;
}

// лучше сделать зацикленным с фэйковым node
