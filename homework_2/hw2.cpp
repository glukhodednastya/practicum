#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;

class DateEvent {
public:

    int year, month, day;
    DateEvent () {}
    DateEvent (int yy, int mm, int dd) : year(yy), month(mm), day(dd) {}

    ~DateEvent() {

    }

    bool Less (const DateEvent &firstDate, const DateEvent &secondDate) {
        if (firstDate.year < secondDate.year) {
        	return true;
        }
        if (firstDate.month < secondDate.month) {
        	return true;
        }
        if (firstDate.day < secondDate.day) {
        	return true;
        }
        return false;
    }

    int operator- (DateEvent secondDate) {
        	DateEvent firstDate(*this);
            if (Less(secondDate, firstDate)) {
            	swap(firstDate, secondDate);
            }
            int difference = 0;
            int isVariableValid = (firstDate.day != secondDate.day) || (firstDate.month != secondDate.month));
            for (int i = firstDate.year; i < secondDate.year; i++) {
            	difference += 365;
            	if ((year % 4 == 0) || (year % 100 == 0) || (year % 400 == 0)) {
                	difference++;
                }
            }
            while (isVariableValid) {
            	difference++;
            	firstDate = firstDate + 1;
            }
            return difference;
    }

    DateEvent operator+ (int days) {
    	DateEvent date (*this);
        int number = date.month;
        int amount = date.day + days;
        while (amount / 365 > 0) {
        	date.year += int (amount / 365);
        	amount = amount / 365;
        }
        switch (number) {
        	case 1:
        		while (amount / 31 > 0) {
        			number += 1;
        			amount = amount % 31;
				}
				date.day = amount;
				break;
        	case 2:
        		while (amount / 29 > 0) {
        			number += 1;
        			amount = amount % 28;
				}
				date.day = amount;
				break;
        	case 3:
        		while (amount / 31 > 0) {
        			number += 1;
        			amount = amount % 31;
				}
				date.day = amount;
				break;
        	case 4:
        		while (amount / 30 > 0) {
        			number += 1;
        			amount = amount % 30;
				}
				date.day = amount;
				break;
        	case 5:
        		while (amount / 31 > 0) {
        			number += 1;
        			amount = amount % 31;
				}
				date.day = amount;
				break;
        	case 6:
        		while (amount / 30 > 0) {
        			number += 1;
        			amount = amount % 30;
				}
				date.day = amount;
				break;
        	case 7:
        		while (amount / 31 > 0) {
        			number += 1;
        			amount = amount % 31;
				}
				date.day = amount;
				break;
        	case 8:
        		while (amount / 31 > 0) {
        			number += 1;
        			amount = amount % 31;
				}
				date.day = amount;
				break;
        	case 9:
        		while (amount / 30 > 0) {
        			number += 1;
        			amount = amount % 30;
				}
				date.day = amount;
				break;
        	case 10:
        		while (amount / 31 > 0) {
        			number += 1;
        			amount = amount % 31;
				}
				date.day = amount;
				break;
        	case 11:
        		while (amount / 30 > 0) {
        			number += 1;
        			amount = amount % 30;
				}
				date.day = amount;
				break;
        	case 12:
        		while (amount / 31 > 0) {
        			number += 1;
        			amount = amount % 31;
				}
				date.day = amount;
				if (number > 12) {
					date.year += 1;
					number = number / 12;
				}
				break;
        }
        date.month = number;
        return date;
    }
};

ostream &operator<< (ostream &out, const DateEvent &date) {
	out << date.year << '.' << date.month << '.' << date.day;
	return out;
}

istream &operator>> (istream &in, DateEvent &date) {
    char between;
    in >> date.year;
    in >> between;
    if ((between == '.') && (date.month < 13) && (date.day < 32)) {
        in >> date.month;
        in >> between;
        if (between == '.') {
        	in >> date.day;
        }
        else {
        	cout << "Wrong date format, excepted '.' after month, variable is not complete\n";
        	exit(1);
        }
    }
    else {
    	cout << "Wrong date format, excepted '.' after years, variable is not complete\n";
    	exit(1);
    }
    return in;
}

int main () {
	bool exit;
	DateEvent firstDate, secondDate;
	int count;
	do {
		cout << "Enter some date yyyy.mm.dd: ";
		cin >> firstDate;
		cout << "Enter some other date yyyy.mm.dd: ";
		cin >> secondDate;
		cout << "First date " << firstDate << ", second date " << secondDate << '\n';
		cout << firstDate - secondDate << " days between dates\n";
		cout << "Enter the number of days after which you want to know the date: ";
		cin >> count;
		cout << "First date + " << count << " days: " << firstDate + count << '\n';
		cout << "Do you want to use the program again?(1 - yes, 0 - no): ";
		cin >> exit;
	} while (exit);
	return 0;
}
