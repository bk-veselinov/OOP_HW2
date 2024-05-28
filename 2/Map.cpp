#include "Map.h"

void Map::free()
{
    delete[] data;
    size = 0;
    capacity = 0;
}

void Map::copyFrom(const Map& other)
{
    size = other.size;
    capacity = other.capacity;
    data = new KeyValuePair[other.capacity];
    for (int i = 0; i < size; ++i) {
        data[i] = other.data[i];
    }
}

void Map::resize()
{
    int newCapacity = (capacity+1) * 2;
    KeyValuePair* newKvpArray = new KeyValuePair[newCapacity];

    for (int i = 0; i < size; ++i) {
        newKvpArray[i] = data[i];
    }

    delete[] data;
    data = newKvpArray;
    capacity = newCapacity;
}
Map::Map() :size(0), capacity(4) {
    data = new KeyValuePair[capacity];
}
Map::Map(const Map& other)
{
    copyFrom(other);
}

Map& Map::operator=(const Map& other)
{
    if (this != &other) {
        free();
        copyFrom(other);
    }
    return *this;
}

Map::~Map()
{
    free();
}

Map::Map(unsigned _size, unsigned _capacity):size(_size), capacity(_capacity)
{
    data = new KeyValuePair[capacity];
}



void Map::add(short key, const short* value)
{
    if (hasKey(key))
    {
        throw std::exception("the key already exists");
    }
    if (size>=capacity)
    {
        resize();
    }
    data[size++] = KeyValuePair(key, value);

    for (int i = 0; i < size - 1; ++i) {
        int minIndex = i;
        for (int j = i + 1; j < size; ++j) {
            if (data[j].getKey() < data[minIndex].getKey()) {
                minIndex = j;
            }
        }
        if (minIndex != i) {
            std::swap(data[i], data[minIndex]);
        }
    }
    
}

const KeyValuePair& Map::getKVP(short key) const
{
    int indexOfPair = getIndexOfPair(key);
    if (indexOfPair == -1)
    {
        throw std::exception("the key does not exists");
    }
    return data[indexOfPair];
}

const short* Map::get(short key) const
{
    return getKVP(key).getValue();
}

void Map::changeValueOfPair(short key, short* newValue)
{
    int indexOfPair = getIndexOfPair(key);
    if (indexOfPair==-1)
    {
        throw std::exception("the key does not exists");
    }
    data[indexOfPair].setValue(newValue);
}
int Map::getIndexOfPair(short key)const {
    int left = 0;
    int right = size - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (data[mid].getKey() == key) {
            return mid;
        }
        else if (data[mid].getKey() < key) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }

    return -1;
}

bool Map::hasKey(short key) const
{
    int left = 0;
    int right = size - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (data[mid].getKey() == key) {
            return true;
        }
        else if (data[mid].getKey() < key) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }

    return false;
}
Map Map::getSwapedKVPs() const
{
    Map res(size, capacity);
    for (short i = 0; i < size; i++)
    {
        KeyValuePair kvp = data[i];
        if (!kvp.getValue())
        {
            continue;
        }
        res.data[i] = kvp.getSwapedKVP();
    }
    return res;
}

std::ostream& operator<<(std::ostream& out, const Map& map)
{
    out.write((const char*)(&map.size), sizeof(map.size));
    for (size_t i = 0; i < map.size; i++)
    {
        out << map.data[i];
    }
    return out;
}

std::istream& operator>>(std::istream& out, Map& map)
{
    out.read((char*)(&map.size), sizeof(unsigned));
    delete[] map.data;
    map.capacity = (map.size+1)*2;
    map.data = new KeyValuePair[map.capacity];
    for (size_t i = 0; i < map.size; i++)
    {
        out >> map.data[i];
    }
    return out;
}
