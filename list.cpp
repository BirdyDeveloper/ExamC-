#include <bits/stdc++.h>

template<typename T>
struct list {
public:
    struct const_iterator;
    struct iterator;
private:
    struct node_base {
        std::vector<const_iterator*> cits;
        std::vector<iterator*> its;
        node_base *l, *r;
        node_base () : l(nullptr), r(nullptr) {}
        node_base (node_base *l, node_base *r) : l(l), r(r) {}
        virtual ~node_base() {
            for (size_t i = 0; i < its.size(); ++i) {
                its[i]->invalid();
            }
            for (size_t i = 0; i < cits.size(); ++i) {
                cits[i]->invalid();
            }
        }
    };

    struct node : node_base {
        T val;
        node(T const& val) : val(val) {}
    };
public:
    struct iterator {
    public:
        iterator& operator++() {
            assert(valid);
            assert(valid && dynamic_cast<node*>(v) != nullptr);
            v = v->r;
            return *this;
        }
        iterator& operator--() {
            assert(valid && dynamic_cast<node*>(v->l) != nullptr);
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
            assert(valid && dynamic_cast<node*>(v) != nullptr);
            node* cur = dynamic_cast<node*>(v);
            return cur->val;
        }
        friend bool operator==(const iterator& a, const iterator& b) {
            assert(a.valid && b.valid);
            return a.v == b.v;
        }
        friend bool operator!=(const iterator& a, const iterator& b) {
            assert(a.valid && b.valid);
            return a.v != b.v;
        }

        typedef std::ptrdiff_t difference_type;
        typedef T value_type;
        typedef T const * pointer;
        typedef T const & reference;
        typedef std::bidirectional_iterator_tag iterator_category;
        ~iterator() {
            if (valid) {
                std::vector<iterator*> new_its;
                for (size_t i = 0; i < v->its.size(); ++i)
                    if (v->its[i] != this)
                        new_its.push_back(v->its[i]);
                v->its = new_its;
            }
        }
    private:
        node_base* v;
        bool valid;
        list<T>* parent;
        explicit iterator(node_base* v, list<T>* parent) : v(v), valid(true), parent(parent) {
            v->its.push_back(this);
        }

        void invalid() {
            valid = false;
            v = nullptr;
            parent = nullptr;
        }

        friend list;
        friend node_base;
        friend const_iterator;
    };

    struct const_iterator {
    public:
        const_iterator& operator++() {
            assert(valid && dynamic_cast<node*>(v) != nullptr);
            v = v->r;
            return *this;
        }
        const_iterator& operator--() {
            assert(valid && dynamic_cast<node*>(v->l) != nullptr);
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
            assert(valid && dynamic_cast<node*>(v) != nullptr);
            node* cur = dynamic_cast<node*>(v);
            return cur->val;
        }
        friend bool operator==(const const_iterator &a, const const_iterator &b) {
            assert(a.valid && b.valid);
            return a.v == b.v;
        }
        friend bool operator!=(const const_iterator &a, const const_iterator &b) {
            assert(a.valid && b.valid);
            return a.v != b.v;
        }

        typedef std::ptrdiff_t difference_type;
        typedef T value_type;
        typedef T const * pointer;
        typedef T const & reference;
        typedef std::bidirectional_iterator_tag iterator_category;

        const_iterator(iterator it) : v(it.v), valid(it.valid), parent(it.parent) {}
        ~const_iterator() {
            if (valid) {
                std::vector<const_iterator*> new_cits;
                for (size_t i = 0; i < v->cits.size(); ++i)
                    if (v->cits[i] != this)
                        new_cits.push_back(v->cits[i]);
                v->cits = new_cits;
            }
        }
    private:
        node_base * v;
        bool valid;
        list<T>* parent;
        explicit const_iterator(node_base * v) : v(v), valid(true), parent(parent) {
            v->cits.push_back(this);
        }
        void invalid() {
            valid = false;
            v = nullptr;
            parent = nullptr;
        }
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
        while(!empty()) {
            erase(begin());
        }
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
        iterator it(end_, this);
        ++it;
        return *it;
    }
    T const& front() const {
        const_iterator it(end_, this);
        ++it;
        return *it;
    }

    //iterator
    iterator begin() {
        return iterator(end_->r, this);
    }
    iterator end() {
        return iterator(end_, this);
    }
    const_iterator begin() const {
        return const_iterator(end_->r);
    }
    const_iterator end() const {
        return const_iterator(end_);
    }
    reverse_iterator rbegin() {
        return reverse_iterator(end(), this);
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
        assert(it.parent == this);
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
        return iterator(newNode, this);
    }
    iterator erase(const_iterator it) {
        assert(it.parent == this && it != end());
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
        return iterator(prev->r, this);
    }
    void splice(const_iterator pos, list<T>& other, const_iterator first, const_iterator last) {
        assert(pos.parent == this);
        for (const_iterator it = first; it != last; ++it) {
            assert(it.parent == &other && it != pos);
            for (size_t i = 0; i < it.v->its.size(); ++i)
                it.v->its[i]->parent = &other;
            for (size_t i = 0; i < it.v->cits.size(); ++i)
                it.v->cits[i]->parent = &other;
        }
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
    for (int i = 1; i < 11; ++i)
        x.push_back(i);
    for (int i = 11; i < 21; ++i)
        y.push_back(i);
    print(x);
    print(y);
    x.splice(++++x.begin(), y, ++y.begin(), --y.end());
    print(x);
    print(y);
    return 0;
}
