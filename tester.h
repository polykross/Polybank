#ifndef TESTER_H_
#define TESTER_H_
#include "digit_sequence.h"
#include "card_number.h"
#include "pin.h"
#include "validator.h"

#include <iostream>
#include <exception>
#include <cassert>

class Tester {
public:
	void run();

private:
	void digitSequenceTests();
	template <size_t size>
	bool digitSequenceTest();
	void cardNumberTests();
	bool cardNumberTest();
	void pinTests();
	bool pinTest();
	void validatorTests();
	bool validatorTest(const char*, const size_t);
};

void Tester::run()
{
	digitSequenceTests();
	cardNumberTests();
	pinTests();
	validatorTests();
}

void Tester::validatorTests()
{
	bool result = true;
	
	const char* kStr1 = "Hello World!";
	const size_t len1 = strlen(kStr1);

#ifndef NDEBUG
	result = validatorTest(kStr1, len1);
	std::cout << (result ? "[passed]" : "[failed]") << " validatorTest with " << kStr1 << std::endl;
	assert(result);
#endif

	const char* kStr2 = "0123456789012345";
	const size_t len2 = strlen(kStr2);

#ifndef NDEBUG
	result = validatorTest(kStr2, len2);
	std::cout << (result ? "[passed]" : "[failed]") << " validatorTest with " << kStr2 << std::endl;
	assert(result);
#endif

	const char* kStr3 = "0123456789012345";
	const size_t len3 = 4;

#ifndef NDEBUG
	result = validatorTest(kStr3, len3);
	std::cout << (result ? "[passed]" : "[failed]") << " validatorTest with " << kStr3 << std::endl;
	assert(result);
#endif

	const char* kStr4 = "0123";
	const size_t len4 = strlen(kStr4);

#ifndef NDEBUG
	result = validatorTest(kStr4, len4);
	std::cout << (result ? "[passed]" : "[failed]") << " validatorTest with " << kStr4 << std::endl;
	assert(result);
#endif

	const char* kStr5 = "013";
	const size_t len5 = strlen(kStr5);

#ifndef NDEBUG
	result = validatorTest(kStr5, len5);
	std::cout << (result ? "[passed]" : "[failed]") << " validatorTest with " << kStr5 << std::endl;
	assert(result);
#endif

	const char* kStr6 = "013q";
	const size_t len6 = strlen(kStr6);

#ifndef NDEBUG
	result = validatorTest(kStr6, len6);
	std::cout << (result ? "[passed]" : "[failed]") << " validatorTest with " << kStr6 << std::endl;
	assert(result);
#endif

}

bool Tester::validatorTest(const char* s, const size_t len)
{
	bool result = true;
	Validator v;
	CardNumber* ptrCn = 0;
	Pin* ptrPin = 0;
	const size_t kCardSize = 16;
	const size_t kPinSize = 4;
	if(v.validateCardNumber(s, len, ptrCn)) {
		DigitSequence<kCardSize> ds(s);
		CardNumber testCn(ds);
		result = ptrCn != 0 && testCn == *ptrCn && 
				!v.validatePin(s,len,ptrPin);
	} else {
		try {
			DigitSequence<kPinSize> ds(s); // can't catch exception here in the first test
			Pin testPin(ds);
			result = v.validatePin(s,len,ptrPin);
			result = ptrPin != 0 && testPin == *ptrPin && result;
		} catch(std::invalid_argument&) {}
	}

	return result;
}

void Tester::digitSequenceTests()
{
	bool result = true;
#ifndef NDEBUG
	result = digitSequenceTest<0>();
	std::cout << (result ? "[passed]" : "[failed]") << " digitSequenceTest<0>" << std::endl;
	assert(result);
#endif
#ifndef NDEBUG
	result = digitSequenceTest<1>();
	std::cout << (result ? "[passed]" : "[failed]") << " digitSequenceTest<1>" << std::endl;
	assert(result);
#endif
#ifndef NDEBUG
	result = digitSequenceTest<2>();
	std::cout << (result ? "[passed]" : "[failed]") << " digitSequenceTest<2>" << std::endl;
	assert(result);
#endif
#ifndef NDEBUG
	result = digitSequenceTest<10>();
	std::cout << (result ? "[passed]" : "[failed]") << " digitSequenceTest<10>" << std::endl;
	assert(result);
#endif
#ifndef NDEBUG
	result = digitSequenceTest<11>();
	std::cout << (result ? "[passed]" : "[failed]") << " digitSequenceTest<11>" << std::endl;
	assert(result);
#endif
#ifndef NDEBUG
	result = digitSequenceTest<15>();
	std::cout << (result ? "[passed]" : "[failed]") << " digitSequenceTest<15>" << std::endl;
	assert(result);
#endif
#ifndef NDEBUG
	result = digitSequenceTest<16>();
	std::cout << (result ? "[passed]" : "[failed]") << " digitSequenceTest<16>" << std::endl;
	assert(result);
#endif
#ifndef NDEBUG
	result = digitSequenceTest<100>();
	std::cout << (result ? "[passed]" : "[failed]") << " digitSequenceTest<100>" << std::endl;
	assert(result);
#endif
}

template <size_t size>
bool Tester::digitSequenceTest()
{
	if(size == 0) { // Must throw logic_error if digitSequence size == 0
		try {
			DigitSequence<size> ds;
			return false;
		} catch (std::logic_error&) {
			return true;
		}
	}

	try {
		const unsigned short* undefinedPtr = 0;
		DigitSequence<size> ds0(undefinedPtr);
		return false;
	} catch (std::invalid_argument&) {}

	try {
		const char* undefinedPtr = 0;
		DigitSequence<size> ds0(undefinedPtr);
		return false;
	} catch (std::invalid_argument&) {}

	try {
		char* invalidCstr = new char[size];
		for(int i = 0; i < size; ++i) {
			invalidCstr[i] = '?';
		}

		DigitSequence<size> ds0(invalidCstr);
		return false;
	} catch (std::invalid_argument&) {}

	char* validCstr = new char[size];
	for(int i = 0; i < size; ++i) {
		validCstr[i] = '1';
	}

	try {
		DigitSequence<size> ds0(validCstr); // constructor by char* test case 1
	} catch (std::invalid_argument e) {
		return false;
	}
	
	DigitSequence<size> ds1(validCstr); // constructor by char* test case 2
	DigitSequence<size> ds2;
	ds1=ds2;

	for(int i = 0; i < size; i++) {
		ds1.get(i % 10); // access to each digit without exceptions
		ds2.set(i, i % 10); // the same
	}

	try {
		ds1.get(size + 1);
		return false;
	} catch(std::out_of_range&) {}

	try {
		ds1.get(-1);
		return false;
	} catch(std::out_of_range&) {}

	try {
		ds1.set(size + 1, -1);
		return false;
	} catch(std::out_of_range&) {}

	try {
		ds1.set(-1,1);
		return false;
	} catch(std::out_of_range&) {}

	try {
		ds1.set(0,12);
		return false;
	} catch(std::invalid_argument&) {}

	try {
		ds1.set(0,-1);
		return false;
	} catch(std::invalid_argument&) {}

	// equality operator test

	DigitSequence<size> ds3;
	for(int i = 0; i < size; ++i) {
		if(ds1.get(i) != ds3.get(i)) return false;
	}

	if(ds1._numbers == ds3._numbers) return false;
	if(ds1 != ds3) return false;
	if(size > 1 && ds3 == ds2) return false;
	if(ds1 != ds1) return false;

	// assignment operator test

	ds1 = ds1;

	if(ds1._numbers != ds1._numbers) return false;

	if(ds1 != ds1) return false;

	ds1 = ds2;

	if(ds1._numbers == ds2._numbers) return false;
	if (ds1 != ds2) return false;

	// Copy constructor test

	DigitSequence<size> ds4(ds1);

	if(ds1._numbers == ds4._numbers) return false;

	if(ds1 != ds4) return false;

	// And if all the stars came together
	return true;
}

void Tester::cardNumberTests()
{
	bool result = true;
	result = cardNumberTest();
#ifndef NDEBUG
	std::cout << (result ? "[passed]" : "[failed]") << " cardNumberTest" << std::endl;
	assert(result);
#endif
}

bool Tester::cardNumberTest()
{
	const size_t cardNumbersSize = 16;
	// Constructor and destructor tests
	{ 
		CardNumber cn; 
	}
	
	DigitSequence<cardNumbersSize> ds0;
	CardNumber cn0(ds0);

	for(size_t i = 0; i < cardNumbersSize; ++i) {
		if(cn0.get(i) != ds0.get(i)) return false;
	}
	
	DigitSequence<16> ds1;
	for(size_t i = 0; i < cardNumbersSize; ++i) {
		ds1.set(i,(cardNumbersSize-i)%10);
	}
	CardNumber cn1(ds1);

	for(size_t i = 0; i < cardNumbersSize; ++i) {
		if(cn1.get(i)!=ds1.get(i)) return false;
	}
	
	try {
		cn1.get(cardNumbersSize+1);
		return false;
	} catch(std::out_of_range&) {}

	try {
		cn1.get(-1);
		return false;
	} catch(std::out_of_range&) {}

	try {
		cn1.set(cardNumbersSize + 1, -1);
		return false;
	} catch(std::out_of_range&) {}

	try {
		cn1.set(-1,1);
		return false;
	} catch(std::out_of_range&) {}

	try {
		cn1.set(0,12);
		return false;
	} catch(std::invalid_argument&) {}

	try {
		cn1.set(0,-1);
		return false;
	} catch(std::invalid_argument&) {}


	// Copy constructor and assignment operator test
	CardNumber cn3(cn1);
	CardNumber cn4 = cn1;

	for(size_t i = 0; i < cardNumbersSize; ++i) {
		if(cn3.get(i) != cn4.get(i) && cn1.get(i) != cn3.get(i)) return false;
	}

	return (cn3 == cn4 || cn1 == cn3 || cn0 != cn1); // Equality operator test
}

void Tester::pinTests()
{
	bool result = true;
	result = pinTest();
#ifndef NDEBUG
	std::cout << (result ? "[passed]" : "[failed]") << " pinTest" << std::endl;
	assert(result);
#endif
}

bool Tester::pinTest()
{
	const size_t pinsSize = 4;
	// Constructor and destructor tests
	{ 
		Pin pin; 
	}
	
	DigitSequence<pinsSize> ds0;
	Pin pin0(ds0);

	for(size_t i = 0; i < pinsSize; ++i) {
		if(pin0.get(i) != ds0.get(i)) return false;
	}
	
	DigitSequence<4> ds1;
	for(size_t i = 0; i < pinsSize; ++i) {
		ds1.set(i,(pinsSize-i)%10);
	}
	Pin pin1(ds1);

	for(size_t i = 0; i < pinsSize; ++i) {
		if(pin1.get(i)!=ds1.get(i)) return false;
	}
	
	try {
		pin1.get(pinsSize+1);
		return false;
	} catch(std::out_of_range&) {}

	try {
		pin1.get(-1);
		return false;
	} catch(std::out_of_range&) {}

	try {
		pin1.set(pinsSize + 1, -1);
		return false;
	} catch(std::out_of_range&) {}

	try {
		pin1.set(-1,1);
		return false;
	} catch(std::out_of_range&) {}

	try {
		pin1.set(0,12);
		return false;
	} catch(std::invalid_argument&) {}

	try {
		pin1.set(0,-1);
		return false;
	} catch(std::invalid_argument&) {}


	// Copying constructor and assignment operator test
	Pin pin3(pin1);
	Pin pin4 = pin1;

	for(size_t i = 0; i < pinsSize; ++i) {
		if(pin3.get(i) != pin4.get(i) && pin1.get(i) != pin3.get(i)) return false;
	}

	return (pin3 == pin4 || pin1 == pin3 || pin0 != pin1); // Equality operator test
}

#endif // TESTER_H_