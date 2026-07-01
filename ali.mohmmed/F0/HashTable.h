#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <cstddef>
#include <functional>
#include <stdexcept>
#include <utility>
#include <vector>

template <class Key, class Value, class Hash = std::hash<Key>>
class HashTable {
public:
    using KeyValue = std::pair<Key, Value>;

private:
    enum class State {
        empty,
        occupied,
        deleted
    };

    struct Entry {
        Key key;
        Value value;
        State state;

        Entry() : key(), value(), state(State::empty) {}
    };

    std::vector<Entry> table_;
    std::size_t size_;
    Hash hash_;

    static const std::size_t npos = static_cast<std::size_t>(-1);

    static bool isPrime(std::size_t number) {
        if (number < 2) return false;
        if (number == 2 || number == 3) return true;
        if (number % 2 == 0 || number % 3 == 0) return false;

        for (std::size_t i = 5; i * i <= number; i += 6) {
            if (number % i == 0 || number % (i + 2) == 0) return false;
        }
        return true;
    }

    static std::size_t nextPrime(std::size_t number) {
        while (!isPrime(number)) ++number;
        return number;
    }

    std::size_t firstHash(const Key& key, std::size_t capacity) const {
        return hash_(key) % capacity;
    }

    std::size_t secondHash(const Key& key, std::size_t capacity) const {
        return 1 + (hash_(key) % (capacity - 1));
    }

    double loadFactor() const {
        return static_cast<double>(size_) / static_cast<double>(table_.size());
    }

    void insertInto(std::vector<Entry>& target, std::size_t& targetSize, const Key& key, const Value& value) const {
        const std::size_t capacity = target.size();
        const std::size_t h1 = firstHash(key, capacity);
        const std::size_t h2 = secondHash(key, capacity);
        std::size_t firstDeleted = npos;

        for (std::size_t i = 0; i < capacity; ++i) {
            const std::size_t index = (h1 + i * h2) % capacity;

            if (target[index].state == State::occupied) {
                if (target[index].key == key) {
                    target[index].value = value;
                    return;
                }
            } else if (target[index].state == State::deleted) {
                if (firstDeleted == npos) firstDeleted = index;
            } else {
                const std::size_t place = (firstDeleted == npos) ? index : firstDeleted;
                target[place].key = key;
                target[place].value = value;
                target[place].state = State::occupied;
                ++targetSize;
                return;
            }
        }

        if (firstDeleted != npos) {
            target[firstDeleted].key = key;
            target[firstDeleted].value = value;
            target[firstDeleted].state = State::occupied;
            ++targetSize;
            return;
        }

        throw std::runtime_error("hash table is full");
    }

    void rehash() {
        std::vector<Entry> newTable(nextPrime(table_.size() * 2));
        std::size_t newSize = 0;

        for (const Entry& entry : table_) {
            if (entry.state == State::occupied) {
                insertInto(newTable, newSize, entry.key, entry.value);
            }
        }

        table_.swap(newTable);
        size_ = newSize;
    }

public:
    explicit HashTable(std::size_t capacity = 1009) :
        table_(nextPrime(capacity)),
        size_(0),
        hash_()
    {}

    bool insert(const Key& key, const Value& value) {
        if (loadFactor() >= 0.7) rehash();

        const std::size_t capacity = table_.size();
        const std::size_t h1 = firstHash(key, capacity);
        const std::size_t h2 = secondHash(key, capacity);
        std::size_t firstDeleted = npos;

        for (std::size_t i = 0; i < capacity; ++i) {
            const std::size_t index = (h1 + i * h2) % capacity;

            if (table_[index].state == State::occupied) {
                if (table_[index].key == key) {
                    table_[index].value = value;
                    return false;
                }
            } else if (table_[index].state == State::deleted) {
                if (firstDeleted == npos) firstDeleted = index;
            } else {
                const std::size_t place = (firstDeleted == npos) ? index : firstDeleted;
                table_[place].key = key;
                table_[place].value = value;
                table_[place].state = State::occupied;
                ++size_;
                return true;
            }
        }

        if (firstDeleted != npos) {
            table_[firstDeleted].key = key;
            table_[firstDeleted].value = value;
            table_[firstDeleted].state = State::occupied;
            ++size_;
            return true;
        }

        return false;
    }

    Value* find(const Key& key) {
        const std::size_t capacity = table_.size();
        const std::size_t h1 = firstHash(key, capacity);
        const std::size_t h2 = secondHash(key, capacity);

        for (std::size_t i = 0; i < capacity; ++i) {
            const std::size_t index = (h1 + i * h2) % capacity;

            if (table_[index].state == State::empty) return 0;
            if (table_[index].state == State::occupied && table_[index].key == key) return &table_[index].value;
        }

        return 0;
    }

    const Value* find(const Key& key) const {
        const std::size_t capacity = table_.size();
        const std::size_t h1 = firstHash(key, capacity);
        const std::size_t h2 = secondHash(key, capacity);

        for (std::size_t i = 0; i < capacity; ++i) {
            const std::size_t index = (h1 + i * h2) % capacity;

            if (table_[index].state == State::empty) return 0;
            if (table_[index].state == State::occupied && table_[index].key == key) return &table_[index].value;
        }

        return 0;
    }

    bool erase(const Key& key) {
        const std::size_t capacity = table_.size();
        const std::size_t h1 = firstHash(key, capacity);
        const std::size_t h2 = secondHash(key, capacity);

        for (std::size_t i = 0; i < capacity; ++i) {
            const std::size_t index = (h1 + i * h2) % capacity;

            if (table_[index].state == State::empty) return false;
            if (table_[index].state == State::occupied && table_[index].key == key) {
                table_[index].state = State::deleted;
                table_[index].value = Value();
                --size_;
                return true;
            }
        }

        return false;
    }

    bool contains(const Key& key) const {
        return find(key) != 0;
    }

    void clear() {
        table_.assign(table_.size(), Entry());
        size_ = 0;
    }

    std::size_t size() const {
        return size_;
    }

    bool empty() const {
        return size_ == 0;
    }

    template <class Function>
    void forEach(Function function) const {
        for (const Entry& entry : table_) {
            if (entry.state == State::occupied) {
                function(entry.key, entry.value);
            }
        }
    }
};

#endif
