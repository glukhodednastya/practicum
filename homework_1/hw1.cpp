#include <iostream>

using namespace std;

class complex_number {
	private:
		double re, im;
	public:
		complex_number(double number_re, double number_im) {
			re = number_re;
			im = number_im;
		}
		//конструктор по умолчанию
		complex_number() {
			re = 0;
			im = 0;
		}

		complex_number operator+(complex_number number) {
			complex_number result(re + number.re, im + number.im);
			return result;
		}
		complex_number operator-(complex_number number) {
			complex_number result(re - number.re, im - number.im);
			return result;
		}
		complex_number operator*(complex_number number) {
			complex_number result(re * number.re - im * number.im,
								re * number.im + im * number.re);
			return result;
		}

		void output() {
		    if (re == 0) {
		    	cout << im;
		    }
		    else {
		    	cout << re << " + ";
		    	cout << im;
		    }
		    cout << "i\n" << endl;
		}
		void input() {
		    cout << "Real part: ";
		    cin >> re;
		    cout << "Imaginary part: ";
		    cin >> im;
		    cout << "You entered a complex number: ";
		    output();
		}
};

int main() {
	complex_number first_number, second_number;
	int operation;

	while (true) {
		cout << "Select an operation:\n";
		cout << "1 - addition\n"
				"2 - subtraction\n"
				"3 - multiplication\n"
				"4 - exit\n" << endl;
		cout << "Your choice: ";
		cin  >> operation;
		if (operation == 4) {
			cout << "Thank you for using the program!\n" << endl;
			break;
		}
		if (operation < 1 || operation > 4) {
			cout << "Try again!\n" << endl;
			continue;
		}

		cout << "Enter the first complex number\n";
		first_number.input();
		cout << "Enter the second complex number\n";
		second_number.input();

		switch(operation) {
			case 1:
				cout << "Result of addition:\n";
				(first_number + second_number).output();
				break;
			case 2:
				cout << "Result of subtraction:\n";
				(first_number - second_number).output();
				break;
			case 3:
				cout << "Result of multiplication:\n";
				(first_number * second_number).output();
				break;
		}
	}

	return 0;
}
