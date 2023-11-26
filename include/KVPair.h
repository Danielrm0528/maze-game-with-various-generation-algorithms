#ifndef KVPAIR_H
#define KVPAIR_H

#include <iostream>

using std::ostream;

template <typename K, typename V>
class KVPair {
public:
    K key;
    V value;

    KVPair(K key, V value) {
        this->key = key;
        this->value = value;
    }
    KVPair(K key) {
        this->key = key;
    }
    KVPair() {}
    KVPair(const KVPair<K, V> &other) {
        key = other.key;
        value = other.value;
    }
    void operator =(const KVPair<K, V> &other) {
        key = other.key;
        value = other.value;
    }
    bool operator ==(const KVPair<K, V> &other) {
        return key == other.key;
    }
    bool operator <(const KVPair<K, V> &other) {
        return key < other.key;
    }
    bool operator <=(const KVPair<K, V> &other) {
        return key <= other.key;
    }
    bool operator >(const KVPair<K, V> &other) {
        return key > other.key;
    }
    bool operator >=(const KVPair<K, V> &other) {
        return key >= other.key;
    }
    bool operator !=(const KVPair<K, V> &other) {
        return key != other.key;
    }
};

template <typename K, typename V>
ostream& operator <<(ostream &os, const KVPair<K, V> &pair) {
    os << "(" << pair.key << ", " << pair.value << ")";
    return os;
}

#endif // KVPAIR_H
