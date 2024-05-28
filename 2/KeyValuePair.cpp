#include "KeyValuePair.h"


KeyValuePair::KeyValuePair(short _key, const short* _value) :key(_key) {
	if (!_value)
	{
		value = nullptr;
	}
	else {
		*value = *_value;
	}
}

short KeyValuePair::getKey()const {
	return key;
}

const short* KeyValuePair::getValue() const {
	return value;
}

void KeyValuePair::setKey(short _key) {
	key = _key;
}
void KeyValuePair::setValue(const short* _value) {
	if (!value)
	{
		value = nullptr;
	}
	else {
		*value = *_value;
	}
}
KeyValuePair KeyValuePair::getSwapedKVP()const {
	return KeyValuePair(*value, &key);
}

std::ostream& operator<<(std::ostream& out, const KeyValuePair& kvp) {
	out.write((const char*)(&kvp.key), sizeof(kvp.key));
	int tempVal = 0;
	if (!kvp.value)
	{
		tempVal = MAX_SHORT_VAL + 10;
		out.write((const char*)(&tempVal), sizeof(tempVal));

	}
	else {
		tempVal = *kvp.value;
		out.write((const char*)(&tempVal), sizeof(tempVal));
	}
	return out;
}
std::istream& operator>>(std::istream& in, KeyValuePair& kvp) {
	in.read((char*)(&kvp.key), sizeof(kvp.key));
	int tempVal = 0;
	in.read((char*)(&tempVal), sizeof(tempVal));
	if (tempVal == MAX_SHORT_VAL + 10)
	{
		kvp.setValue(nullptr);
	}
	else {
		kvp.setValue((const short*)&tempVal);
	}
	return in;
}