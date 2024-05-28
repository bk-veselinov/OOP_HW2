#include "Multiset.h"


double custom_modulo(double x, double y) {
	double quotient = (int)(x / y);
	double remainder = x - quotient * y;
	return remainder;
}

const char* intToBinary(int n) {
	char* binary = new char[9] {'\0'};

	for (int i = 7; i >= 0; --i) {
		binary[i] = (n & 1) ? '1' : '0';
		n >>= 1;
	}

	return binary;
}

int Multiset::getBucket(int n)const {
	return (n / 8.0) * k;
}
int Multiset::getPosition(int n) const {
	return custom_modulo(n, 8.0 / k) * k;
}

int Multiset::getSelectMask(int bitsInBicket, int posInChar)const {
	int mask = 1;
	for (size_t i = 1; i < bitsInBicket; i++)
	{
		mask = mask << 1;
		mask += 1;
	}

	return mask << (8 - (posInChar + bitsInBicket));
}
int Multiset::getBucketsCount()const {
	return std::ceil((maxNum / 8.0) * k + 1);
}

void Multiset::free() {
	delete[] data;
	k = 0;
	maxNum = 0;
	data = nullptr;
}

void Multiset::copyFrom(const Multiset& other) {
	maxNum = other.maxNum;
	k = other.k;
	data = new unsigned char[other.getBucketsCount()] {0};
	for (size_t i = 0; i < other.getBucketsCount(); i++)
	{
		data[i] = other.data[i];
	}
}

Multiset::Multiset() :k(0), maxNum(0), data(nullptr) {

}
Multiset::Multiset(int _maxNum, int _k) : maxNum(_maxNum), k(_k) {
	data = new unsigned char[getBucketsCount()] {0};
};

Multiset::Multiset(int _maxNum, int _k, unsigned char* _data) : maxNum(_maxNum), k(_k), data(_data) {}

Multiset::Multiset(const Multiset& other) {
	copyFrom(other);
}
Multiset& Multiset::operator=(const Multiset& other) {
	if (this != &other)
	{
		free();
		copyFrom(other);
	}
	return *this;
}
Multiset::~Multiset() {
	free();
}

int Multiset::getNumCount(int num) const {
	int bucket = getBucket(num);
	int posInChar = getPosition(num);

	if (posInChar + k > 8)
	{

		int remainingBits = (posInChar + k) - 8;
		int bitsInCurrBicket = k - remainingBits;

		int maskInCurrBucket = getSelectMask(bitsInCurrBicket, posInChar);
		int maskInNextBucket = getSelectMask(remainingBits, 0);

		return ((data[bucket] & (maskInCurrBucket)) >> (8 - (posInChar + k))) + 
			   ((data[bucket + 1] & maskInNextBucket) >> (8 - remainingBits));
	}
	else {
		int maskInCurrBucket = getSelectMask(k, posInChar);
		return (data[bucket] & (maskInCurrBucket)) >> ((8 - (posInChar + k)));
	}

}
void Multiset::add(int num) {
	addNTimes(num, 1);
}
void Multiset::addNTimes(int num, int times) {

	int bucket = getBucket(num);
	int posInChar = getPosition(num);
	int count = getNumCount(num) + times;
	if (count >= ((1 << k)) - 1)
	{
		throw std::out_of_range("The max count for the number is reached!");
	}

	if (posInChar + k > 8)
	{
		int remainingBits = (posInChar + k) - 8;

		int bitsInCurrBicket = k - remainingBits;

		int maskInCurrBucket = getSelectMask(bitsInCurrBicket, posInChar);
		int newMaskInCurrBucket = count & (maskInCurrBucket << remainingBits);

		int maskInNextBucket = getSelectMask(remainingBits, 0) >> (8 - remainingBits);
		int newMaskInNextBucket = count & maskInNextBucket;

		int mask = 255;
		mask &= data[bucket];
		mask &= ~maskInCurrBucket;
		data[bucket] &= mask;
		data[bucket] |= (newMaskInCurrBucket) >> remainingBits;

		int mask2 = 255;
		mask2 &= data[bucket];
		mask2 &= ~maskInNextBucket;
		data[bucket + 1] &= mask2;

		data[bucket + 1] |= (newMaskInNextBucket) << (8 - remainingBits);

	}
	else {
		int maskInCurrBucket = getSelectMask(k, posInChar);
		int finalMask = maskInCurrBucket & (count << (8 - (posInChar + k)));

		int mask = 255;
		mask &= data[bucket];
		mask &= ~maskInCurrBucket;
		data[bucket] = -data[bucket];
		data[bucket] &= mask;
		data[bucket] |= finalMask;
	}

}
void Multiset::remove(int num) {
	int bucket = getBucket(num);
	int posInChar = getPosition(num);
	int count = getNumCount(num) - 1;

	if (count < 0)
	{
		throw std::logic_error("The number appears 0 thimes in the multiset!");
	}

	if (posInChar + k > 8)
	{
		int remainingBits = (posInChar + k) - 8;

		int bitsInCurrBicket = k - remainingBits;

		int maskInCurrBucket = getSelectMask(bitsInCurrBicket, posInChar);
		int newMaskInCurrBucket = count & (maskInCurrBucket << remainingBits);

		int maskInNextBucket = getSelectMask(remainingBits, 0);
		int newMaskInNextBucket = count & (maskInNextBucket >> (8 - remainingBits));

		int mask = 255;
		mask &= data[bucket];
		mask &= ~maskInCurrBucket;
		data[bucket] &= mask;
		data[bucket] |= newMaskInCurrBucket;
		

		int mask2 = 255;
		mask2 &= data[bucket];
		mask2 &= ~maskInNextBucket;
		data[bucket + 1] &= mask2;

		data[bucket + 1] |= (newMaskInNextBucket) << (8 - remainingBits);
	}
	else {
		int maskInCurrBucket = getSelectMask(k, posInChar);

		int finalMask = maskInCurrBucket & (count << (8 - (posInChar + k)));
		data[bucket] |= maskInCurrBucket;
		data[bucket] &= finalMask;
	}
}

void Multiset::printAllNumbers() const {
	for (size_t i = 0; i <= maxNum; i++)
	{
		int numCount = getNumCount(i);
		if (numCount > 0)
		{
			std::cout << i << ':' << numCount << '\n';
		}
	}
}

void Multiset::printBinaryRepresentation()const {
	int bucketsCount = getBucketsCount();
	for (size_t i = 0; i < bucketsCount; i++)
	{
		const char* curBit = intToBinary(data[i]);

		std::cout << curBit << ' ';
		delete[] curBit;
	}
	std::cout << '\n';
}

std::istream& operator>>(std::istream in, Multiset& rhs) {
	in.read((char*)&rhs.maxNum, sizeof(rhs.maxNum));
	in.read((char*)&rhs.k, sizeof(rhs.k));
	rhs.data = new unsigned char[rhs.getBucketsCount()] {0};
	in.read((char*)rhs.data, sizeof(char) * rhs.getBucketsCount());
	return in;
}

std::ostream& operator<<(std::ostream out, const Multiset& rhs) {
	out.write((const char*)&rhs.maxNum, sizeof(rhs.maxNum));
	out.write((const char*)&rhs.k, sizeof(rhs.k));
	out.write((const char*)&rhs.data, sizeof(char) * rhs.getBucketsCount());
	return out;
}

Multiset intersection(const Multiset& lhs, const Multiset& rhs) {
	if (lhs.k != rhs.k || lhs.maxNum != rhs.maxNum)
	{
		throw std::logic_error("Not compatable sets");
	}
	Multiset result(lhs.maxNum, lhs.k);
	for (size_t i = 0; i < lhs.getBucketsCount(); i++)
	{
		result.data[i] = lhs.data[i] & rhs.data[i];
	}
	return result;
}



Multiset difference(const Multiset& lhs, const Multiset& rhs) {
	if (lhs.k != rhs.k || lhs.maxNum != rhs.maxNum)
	{
		throw std::logic_error("Not compatable sets");
	}
	Multiset result(lhs.maxNum, lhs.k);
	for (size_t i = 0; i < result.maxNum; i++)
	{
		int resNumCount = lhs.getNumCount(i) - rhs.getNumCount(i);
		if (resNumCount)
		{
			resNumCount = 0;
		}
		result.addNTimes(i, resNumCount);
	}
	return result;
}

Multiset opposite(const Multiset& set) {
	if (!set.data)
	{
		throw std::logic_error("the set's data is not initialised");
	}
	Multiset result(set.maxNum, set.k);
	for (size_t i = 0; i < result.getBucketsCount(); i++)
	{
		result.data[i] = set.data[i] ^ 255;
	}
	return result;
}


