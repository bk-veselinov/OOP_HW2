#pragma once
#include "KeyValuePair.h"
#include <fstream>
#include <exception>
class Map {
	KeyValuePair* data;
	unsigned size = 0;
	unsigned capacity = 0;
	void free();
	void copyFrom(const Map& other);
	void resize();
public:
	Map();
	Map(const Map& other);
	Map(unsigned _size, unsigned _capacity);
	Map& operator=(const Map& other);
	~Map();
	int getIndexOfPair(short x)const;
	void add(short key, const short* value);
	const KeyValuePair& getKVP(short key)const;
	const short* get(short key)const;
	void changeValueOfPair(short key, short* newValue);
	bool hasKey(short key)const ;
	Map getSwapedKVPs() const;

	friend std::ostream& operator<<(std::ostream& , const Map& );
	friend std::istream& operator>>(std::istream& , Map& );


};