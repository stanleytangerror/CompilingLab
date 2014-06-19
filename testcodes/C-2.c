
int mod(int number2, int m2)
{
	return number2 - number2 / m2 * m2;
}

int printHexDigit(int digit6)
{
	if(digit6 < 10)
		write(digit6);
	else
		write(-digit6);
	return 0;
}

int printHex(int number5)
{
	int a5[4];
	int j5 = 0;
	while(j5<4) {
		a5[j5] = mod(number5, 16);
		number5 = number5 / 16;
		j5=j5+1;
	}
	j5 = 3;
	while(j5>=0) {
		write(a5[j5]);
		j5=j5-1;
	}
	return 0;
}


int main() {
	printHex(9450);
	return 0;
}
