#include "ModifiableIntegersFunction.h"

ModifiableIntegersFunction::ModifiableIntegersFunction(short(*_func)(short)) : func(_func) {
	calculateAllValues();
}

ModifiableIntegersFunction::ModifiableIntegersFunction(short(*_func)(short), const Map& _points) : func(_func), modifiedPoints(_points) {
	calculateAllValues();
}

ModifiableIntegersFunction::ModifiableIntegersFunction(short(*_func)(short), const Map& _points, bool* _isInjective, bool* _isSurjective)
	: func(_func), modifiedPoints(_points), isInjective(_isInjective), isSurjective(_isSurjective) {
	calculateAllValues();
}

ModifiableIntegersFunction::ModifiableIntegersFunction(const Map& _modifiedPoints) {
	setModifiedPoints(_modifiedPoints);
}

void ModifiableIntegersFunction::setModifiedPoints(const Map& _modifiedPoints) {
	modifiedPoints = _modifiedPoints;
}

bool ModifiableIntegersFunction::isFuncInjective()const {
	if (isInjective != nullptr)
	{
		return *isInjective;
	}
	Bitset positive(32767);
	Bitset negative(32768);

	for (int i = MIN_SHORT_VAL; i < MAX_SHORT_VAL; i++)
	{
		const short* val = getValue(i);
		if (!val)
		{
			continue;
		}
		if (*val >= 0)
		{
			if (positive.hasNumber(*val))
			{
				*isInjective = false;
				return *isInjective;
			}
			positive.addNumber(*val);
		}
		else
		{
			if (negative.hasNumber(-*val))
			{
				*isInjective = false;
				return *isInjective;
			}
			negative.addNumber(-*val);

		}
	}
	*isInjective = true;
	return *isInjective;
}

bool ModifiableIntegersFunction::isFuncSurjective() const {
	if (isSurjective != nullptr)
	{
		return *isSurjective;
	}
	Bitset positive(32767);
	int counter = 0;
	Bitset negative(32768);

	for (int i = MIN_SHORT_VAL; i < MAX_SHORT_VAL; i++)
	{
		const short* val = getValue(i);
		if (!val)
		{
			continue;
		}
		if (*val >= 0)
		{
			if (!positive.hasNumber(*val))
			{
				positive.addNumber(*val);
				counter++;
			}
		}
		else
		{
			if (!negative.hasNumber(-*val))
			{
				negative.addNumber(-*val);
				counter++;
			}
		}
	}
	*isSurjective = counter - (1 << 16) == 0;
	return *isSurjective;
}

bool ModifiableIntegersFunction::isBijective() const {
	if (!isInjective)
	{
		isFuncInjective();
	}
	if (!isSurjective)
	{
		isFuncSurjective();
	}
	return *isInjective && *isSurjective;
}

void ModifiableIntegersFunction::setPoint(short key, const short value) {
	modifiedPoints.add(key, &value);
}

ModifiableIntegersFunction ModifiableIntegersFunction::operator()(const ModifiableIntegersFunction& other)const {
	Map points;
	for (int i = MIN_SHORT_VAL; i <= MAX_SHORT_VAL; i++)
	{
		const short* otherVal = other.getValue(i);
		if (!otherVal)
		{
			points.add(i, nullptr);
		}
		else
		{
			points.add(i, getValue(*other.getValue(i)));
		}
	}
	return ModifiableIntegersFunction(points);
}

ModifiableIntegersFunction ModifiableIntegersFunction::getReverseFunc()const {
	if (!isInjective)
	{
		isFuncInjective();
	}
	if (!*isInjective)
	{
		throw std::logic_error("The function is not injective");
	}

	return ModifiableIntegersFunction(modifiedPoints.getSwapedKVPs());
}

void ModifiableIntegersFunction::removePoint(short key) {
	modifiedPoints.add(key, nullptr);
}

const short* ModifiableIntegersFunction::getValue(short x) const {
	if (modifiedPoints.hasKey(x))
	{
		return modifiedPoints.get(x);
	}
	if (!func)
	{
		return nullptr;
	}
	short res = func(x);
	return &res;
}

bool areParalel(const ModifiableIntegersFunction& lhs, const ModifiableIntegersFunction& rhs) {
	int counter = 0;
	while (!lhs.getValue(MIN_SHORT_VAL + counter) && !rhs.getValue(MIN_SHORT_VAL + counter))
	{
		counter++;
	}
	if (!lhs.getValue(MIN_SHORT_VAL + counter) || !rhs.getValue(MIN_SHORT_VAL + counter))
	{
		return false;
	}

	int diff = lhs.getValue(MIN_SHORT_VAL + counter) - rhs.getValue(MIN_SHORT_VAL + counter);

	for (size_t i = MIN_SHORT_VAL + counter; i <= MAX_SHORT_VAL; i++)
	{
		if (!lhs.getValue(i) && !rhs.getValue(i))
		{
			continue;
		}
		if (!lhs.getValue(i) || !rhs.getValue(i) || diff != (*lhs.getValue(i) - *rhs.getValue(i)))
		{
			return false;
		}
	}
	return true;
}

void ModifiableIntegersFunction::calculateAllValues() {
	for (int i = MIN_SHORT_VAL; i < MAX_SHORT_VAL; i++)
	{
		if (getValue(i) != nullptr) {
			setPoint(i, *getValue(i));
		}
		else
		{
			removePoint(i);
		}
	}
}

void ModifiableIntegersFunction::calculateBound(short x, short y)const {
	char matrix[BOUND_LEN + 1][BOUND_LEN + 1];
	for (size_t i = x; i <= x + BOUND_LEN; i++)
	{
		for (size_t j = y; j <= y + BOUND_LEN; j++)
		{
			matrix[(x + BOUND_LEN) - i][(y + BOUND_LEN) - j] = '.';
		}
		const short* curVal = getValue(i);
		if (!curVal || *curVal > y + BOUND_LEN || *curVal < y)
		{
			continue;
		}

		matrix[(x + BOUND_LEN) - i][(y + BOUND_LEN) - *curVal] = '*';
	}
	for (size_t i = 0; i < BOUND_LEN; i++)
	{
		for (size_t i = 0; i < BOUND_LEN; i++)
		{
			std::cout << matrix[i];
		}
		std::cout << '\n';
	}
}

std::ostream& operator<<(std::ostream& out, const ModifiableIntegersFunction& lhs) {
	out << lhs.modifiedPoints;
}
std::istream& operator>>(std::istream& in, ModifiableIntegersFunction& lhs) {
	in >> lhs.modifiedPoints;
}

const short* calculateKthPow(const ModifiableIntegersFunction& func, short x, unsigned k) {
	if (!func.getValue(x))
	{
		return nullptr;
	}
	if (k == 0)
	{
		short res = 1;
		return &res;
	}
	if (k == 1)
	{
		return func.getValue(x);
	}
	return calculateKthPow(func, *func.getValue(x), k - 1);
}

ModifiableIntegersFunction operator^(const ModifiableIntegersFunction& lhs, unsigned k) {
	Map points;
	for (short i = MIN_SHORT_VAL; i < MAX_SHORT_VAL; i++)
	{
		points.add(i, calculateKthPow(lhs, i, k));
	}
	return ModifiableIntegersFunction(points);
}

ModifiableIntegersFunction operator+(const ModifiableIntegersFunction& lhs, const ModifiableIntegersFunction& rhs) {
	Map points;
	for (short i = MIN_SHORT_VAL; i <= MAX_SHORT_VAL; i++)
	{
		const short* leftVal = lhs.getValue(i);
		const short* rightVal = rhs.getValue(i);
		if (!leftVal || !rightVal)
		{
			points.add(i, nullptr);
		}
		else
		{
			int res = *leftVal + *rightVal;
			if (res > MAX_SHORT_VAL || res < MIN_SHORT_VAL)
			{
				throw std::logic_error("the operation is invalid. The value overflows");
			}
			short final = res;
			points.add(i, &final);
		}
	}
	return ModifiableIntegersFunction(points);
}

ModifiableIntegersFunction operator-(const ModifiableIntegersFunction& lhs, const ModifiableIntegersFunction& rhs) {
	Map points;
	for (short i = MIN_SHORT_VAL; i <= MAX_SHORT_VAL; i++)
	{
		const short* leftVal = lhs.getValue(i);
		const short* rightVal = rhs.getValue(i);
		if (!leftVal || !rightVal)
		{
			points.add(i, nullptr);
		}
		else
		{
			int res = *leftVal - *rightVal;
			if (res > MAX_SHORT_VAL || res < MIN_SHORT_VAL)
			{
				throw std::logic_error("the operation is invalid. The value overflows");
			}
			short final = res;
			points.add(i, &final);
		}
	}
	return ModifiableIntegersFunction(points);
}

bool operator<(const ModifiableIntegersFunction& lhs, const ModifiableIntegersFunction& rhs) {
	for (size_t i = MIN_SHORT_VAL; i < MAX_SHORT_VAL; i++)
	{
		const short* lVal = lhs.getValue(i);
		const short* rVal = rhs.getValue(i);

		if (!lVal && !rVal)
		{
			continue;
		}
		if (!rVal)
		{
			return false;
		}
		if (!lVal)
		{
			continue;
		}
		if (*lVal > *rVal)
		{
			return false;
		}
	}
	return true;
}

bool operator>(const ModifiableIntegersFunction& lhs, const ModifiableIntegersFunction& rhs) {
	for (size_t i = MIN_SHORT_VAL; i < MAX_SHORT_VAL; i++)
	{
		const short* lVal = lhs.getValue(i);
		const short* rVal = rhs.getValue(i);

		if (!lVal && !rVal)
		{
			continue;
		}
		if (!lVal)
		{
			return false;
		}
		if (!rVal)
		{
			continue;
		}
		if (*lVal < *rVal)
		{
			return false;
		}
	}
	return true;
}

bool operator==(const ModifiableIntegersFunction& lhs, const ModifiableIntegersFunction& rhs) {
	for (size_t i = MIN_SHORT_VAL; i < MAX_SHORT_VAL; i++)
	{
		const short* lVal = lhs.getValue(i);
		const short* rVal = rhs.getValue(i);

		if (!lVal && !rVal)
		{
			continue;
		}
		if (!lVal || !rVal)
		{
			return false;
		}

		if (*lVal != *rVal)
		{
			return false;
		}
	}
	return true;
}

bool operator>=(const ModifiableIntegersFunction& lhs, const ModifiableIntegersFunction& rhs) {

	return lhs > rhs && lhs == rhs;
}

bool operator<=(const ModifiableIntegersFunction& lhs, const ModifiableIntegersFunction& rhs) {

	return lhs < rhs && lhs == rhs;
}
