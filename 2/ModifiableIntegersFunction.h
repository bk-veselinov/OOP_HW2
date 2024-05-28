#pragma once
#include "Map.h"
#include "BitSet.h"
#include <fstream>
#include <iostream>
constexpr short MIN_SHORT_VAL = -32768;
constexpr short MAX_SHORT_VAL = 32767;
constexpr int BOUND_LEN = 20;

class ModifiableIntegersFunction {
	short(*func)(short) = nullptr;
	Map modifiedPoints;
	bool* isInjective = nullptr;
	bool* isSurjective = nullptr;
public:
	ModifiableIntegersFunction() = default;

	ModifiableIntegersFunction(short(*_func)(short));

	ModifiableIntegersFunction(short(*_func)(short), const Map& _points);

	ModifiableIntegersFunction(short(*_func)(short), const Map& _points, bool* _isInjective, bool* _isSurjective);

	ModifiableIntegersFunction(const Map& _modifiedPoints);

	void setModifiedPoints(const Map& _modifiedPoints);

	bool isFuncInjective()const;

	bool isFuncSurjective() const;

	bool isBijective() const;

	void setPoint(short key, const short value);

	ModifiableIntegersFunction operator()(const ModifiableIntegersFunction& other)const;

	ModifiableIntegersFunction getReverseFunc()const;

	void removePoint(short key);

	const short* getValue(short x) const;

	friend bool areParalel(const ModifiableIntegersFunction& lhs, const ModifiableIntegersFunction& rhs);

	void calculateAllValues();

	void calculateBound(short x, short y)const;

	friend std::ostream& operator<<(std::ostream& out, const ModifiableIntegersFunction& lhs);
	friend std::istream& operator>>(std::istream& in, ModifiableIntegersFunction& lhs);

	friend const short* calculateKthPow(const ModifiableIntegersFunction& func, short x, unsigned k);

};
ModifiableIntegersFunction operator^(const ModifiableIntegersFunction& lhs, unsigned k);

ModifiableIntegersFunction operator+(const ModifiableIntegersFunction& lhs, const ModifiableIntegersFunction& rhs);

ModifiableIntegersFunction operator-(const ModifiableIntegersFunction& lhs, const ModifiableIntegersFunction& rhs);

bool operator<(const ModifiableIntegersFunction& lhs, const ModifiableIntegersFunction& rhs);

bool operator>(const ModifiableIntegersFunction& lhs, const ModifiableIntegersFunction& rhs);

bool operator==(const ModifiableIntegersFunction& lhs, const ModifiableIntegersFunction& rhs);
bool operator>=(const ModifiableIntegersFunction& lhs, const ModifiableIntegersFunction& rhs);

bool operator<=(const ModifiableIntegersFunction& lhs, const ModifiableIntegersFunction& rhs);
