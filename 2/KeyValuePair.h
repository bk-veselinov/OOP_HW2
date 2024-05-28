#pragma once
#include<fstream>
constexpr short MAX_SHORT_VAL = 32767;
class KeyValuePair {
	short key=0;
	short* value=nullptr;
public:
	KeyValuePair() = default;
	KeyValuePair(short _key, const short* _value);

	short getKey()const;

	const short* getValue() const;

	void setKey(short _key);
	void setValue(const short* _value);
	KeyValuePair getSwapedKVP()const;

	friend std::ostream& operator<<(std::ostream& os, const KeyValuePair& kvp);
	friend std::istream& operator>>(std::istream& is, KeyValuePair& kvp);
};