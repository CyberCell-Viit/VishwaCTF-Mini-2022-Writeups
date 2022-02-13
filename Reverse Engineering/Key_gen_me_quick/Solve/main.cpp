#include <iostream>
#include <cstdlib>
#include <ctime>

void keygen(int keyCount)
{
	for (int index = 0; index < keyCount; index++) {
		int randomNumber = (rand() % 100) + 1000;
		std::cout << randomNumber << "-0000-" << "R3KT" << std::endl;
	}
}

int main()
{
	std::srand((unsigned) time(0));
	int keyCount = { };
	
	std::cout << "How many keys do you want to generate: ";
	std::cin >> keyCount;
	
	keygen(keyCount);
}