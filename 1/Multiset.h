#pragma once
#include <exception>
#include <stdexcept>
#include <iostream>
#include <fstream>
#pragma warning (disable: 4996)

double custom_modulo(double x, double y);


const char* intToBinary(int n);

class Multiset {
	
	int maxNum;
	int k;
	unsigned char* data;

	int getBucket(int n)const;
	int getPosition(int n) const;

	int getSelectMask(int bitsInBicket, int posInChar)const;
	int getBucketsCount()const;

	void free();

	void copyFrom(const Multiset& other);

public:
	Multiset();
	Multiset(int _maxNum, int _k);

	Multiset(int _maxNum, int _k, unsigned char* _data);

	Multiset(const Multiset& other);
	Multiset& operator=(const Multiset& other);

	int getNumCount(int num) const;
	void add(int num);
	void addNTimes(int num, int times);
	void remove(int num);

	void printAllNumbers() const;

	void printBinaryRepresentation()const;

	friend Multiset intersection(const Multiset& lhs, const Multiset& rhs);
	friend std::istream& operator>>(std::istream in, Multiset& rhs);

	friend std::ostream& operator<<(std::ostream out, const Multiset& rhs);


	friend Multiset difference(const Multiset& lhs, const Multiset& rhs);

	friend Multiset opposite(const Multiset& set);
	~Multiset();

};