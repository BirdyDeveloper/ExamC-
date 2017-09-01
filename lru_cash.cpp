#include <bits/stdc++.h>

struct node
{
public:
    node() : left(nullptr), right(nullptr), parent(nullptr), next(nullptr), prev(nullptr) {}
    node(node* left, node* right, node* parent, node* next, node* prev) : 
    left(left), right(right), parent(parent), next(next), prev(prev) {}

    node*       left;
    node*       right;
    node*       parent;

    node*       next;
    node*       prev;
};

template<typename T, typename U>
struct node_with_data : node
{
public:
    typedef T key_type;
    typedef U mapped_type;
    typedef std::pair<key_type, mapped_type> value_type;
    value_type val;
    node_with_data(value_type const& val) : val(val) {}
};

template<typename T, typename U>
struct lru_cache
{
private:
    node* end_; // Наибольший по значению элемент
    size_t sz, capacity;
public:
    // Вы можете определить эти тайпдефы по вашему усмотрению.
    typedef T key_type;
    typedef U mapped_type;
    typedef std::pair<key_type, mapped_type> value_type;

    // Bidirectional iterator.
    struct iterator
    {
        // Элемент на который сейчас ссылается итератор.
        // Разыменование итератора end() неопределено.
        // Разыменование невалидного итератора неопределено.
        value_type const& operator*() const {
            try {
                return static_cast<node_with_data<T, U>*>(v)->val;
            } catch(...) {
                std::cerr << "can't use operator '*' with end() iterator\n";
                throw;
            }
        }

        // Переход к элементу со следующим по величине ключом.
        // Инкремент итератора end() неопределен.
        // Инкремент невалидного итератора неопределен.
        iterator& operator++() {
            if (v->right) {
                v = v->right;
                while (v->left) { v = v->left; }
                return *this;
            }
            node* cur = v->parent;
            while (cur && v == cur->right) {
                v = cur;
                cur = cur->parent;
            }
            v = cur;
            return *this;
        }
        iterator operator++(int) {
            iterator res = *this;
            ++*this;
            return res;
        }

        // Переход к элементу со следующим по величине ключом.
        // Декремент итератора begin() неопределен.
        // Декремент невалидного итератора неопределен.
        iterator& operator--();
        iterator operator--(int);
        
        friend bool operator==(const iterator& a, const iterator& b) {
            return a.v == b.v;
        }
        friend bool operator!=(const iterator& a, const iterator& b) {
            return a.v != b.v;
        }
        
        friend lru_cache;
        
    private:
        node* v;
    };

    // Создает пустой lru_cache с указанной capacity.
    lru_cache(size_t capacity);

    // Деструктор. Вызывается при удалении объектов lru_cache.
    // Инвалидирует все итераторы ссылающиеся на элементы этого lru_cache
    // (включая итераторы ссылающиеся на элементы следующие за последними).
    ~lru_cache();

    // Поиск элемента.
    // Возвращает итератор на элемент найденный элемент, либо end().
    // Если элемент найден, он помечается как наиболее поздно использованный.
    iterator find(key_type);

    // Вставка элемента.
    // 1. Если такой ключ уже присутствует, вставка не производиться, возвращается итератор
    //    на уже присутствующий элемент и false.
    // 2. Если такого ключа ещё нет, производиться вставка, возвращается итератор на созданный
    //    элемент и true.
    // Если после вставки число элементов кеша превышает capacity, самый давно не
    // использованный элемент удаляется. Все итераторы на него инвалидируется.
    // Вставленный либо найденный с помощью этой функции элемент помечается как наиболее поздно
    // использованный.
    std::pair<iterator, bool> insert(value_type);

    // Удаление элемента.
    // Все итераторы на указанный элемент инвалидируются.
    void erase(iterator);

    // Возващает итератор на элемент с минимальный ключом.
    iterator begin() const;
    // Возващает итератор на элемент следующий за элементом с максимальным ключом.
    iterator end() const;
};

// Сравнение. Итераторы считаются эквивалентными если они ссылаются на один и тот же элемент.
// Сравнение с невалидным итератором не определено.
// Сравнение итераторов двух разных контейнеров не определено.

int main() {
    return 0;
}




