#include <bits/stdc++.h>

struct lru_cache
{
public:
    // Вы можете определить эти тайпдефы по вашему усмотрению.
    typedef int key_type;
    typedef int mapped_type;
    typedef std::pair<key_type, mapped_type> value_type;

    // Bidirectional iterator.
    struct iterator;

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
private:
    struct node
    {
    public:
        node() {}
        node(value_type key_mapped) :
                key_mapped(key_mapped), left(nullptr), right(nullptr),
                parent(nullptr), next(nullptr), prev(nullptr) {}
        value_type key_mapped;

        node*       left;
        node*       right;
        node*       parent;

        node*       next;
        node*       prev;
    };
    node *root_, *begin_;
    friend iterator;
};

struct lru_cache::iterator
{
private:
    node* value;
public:
    iterator() : value(nullptr) {}
    // Элемент на который сейчас ссылается итератор.
    // Разыменование итератора end() неопределено.
    // Разыменование невалидного итератора неопределено.

    value_type const& operator*() const {
        return value->key_mapped;
    }

    iterator(iterator const& other) {
        this->value = other.value;
    }

    // Переход к элементу со следующим по величине ключом.
    // Инкремент итератора end() неопределен.
    // Инкремент невалидного итератора неопределен.
    iterator& operator++() {
        if (value->right) {
            value = value->right;
            while (value->left) {
                value = value->left;
            }
            return *this;
        }
        value = value->parent;
        if (!value) return *this;
        while (value->right) {
            value = value->right;
        }
        return *this;
    }

    iterator operator++(int) {
        iterator tmp = *this;
        ++(*this);
        return tmp;
    }

    // Переход к элементу со следующим по величине ключом.
    // Декремент итератора begin() неопределен.
    // Декремент невалидного итератора неопределен.
    iterator& operator--() {
        if (value->left) {
            value = value->left;
            while (value->right) { value = value->right; }
            return *this;
        }
        node* cur = value->parent;
        while (cur && value == cur->left) {
            value = cur;
            cur = cur->parent;
        }
        value = cur;
        return *this;
    }

    iterator operator--(int) {
        iterator tmp = *this;
        --(*this);
        return *this;
    }

    friend bool operator==(iterator, iterator);
    friend bool operator!=(iterator, iterator);
};

// Сравнение. Итераторы считаются эквивалентными если они ссылаются на один и тот же элемент.
// Сравнение с невалидным итератором не определено.
// Сравнение итераторов двух разных контейнеров не определено.
bool operator==(lru_cache::iterator a, lru_cache::iterator b) {
    return a.value == b.value;
}
bool operator!=(lru_cache::iterator a, lru_cache::iterator b) {
    return !(a == b);
}

int main() {
    return 0;
}




