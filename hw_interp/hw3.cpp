#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <stack>
#include <algorithm>
#include <string.h>

using namespace std;

bool ifletter (char c) {
	return ((c >= 'A') && (c <= 'Z')) || ((c >= 'a') && (c <= 'z'));
}

bool ifdigit (char c) {
	return (c >= '0') && (c <= '9');
}


enum lexnum { LEX_NULL, LEX_AND, LEX_BEGIN,  LEX_STRING, LEX_DO, LEX_ELSE, LEX_IF, LEX_FALSE, LEX_INT, LEX_NOT,
//                0         1        2          3        4           5       6          7        8        9
               LEX_OR, LEX_PROGRAM, LEX_READ, LEX_THEN, LEX_REAL, LEX_TRUE, LEX_WHILE, LEX_WRITE, LEX_FOR, LEX_BREAK, LEX_FIN,
//               10         11        12        13        14        15         16         17       18        19          20
//
               LEX_SEMICOLON, LEX_LBRACE, LEX_RBRACE, LEX_COMMA, LEX_COLON, LEX_ASSIGN, LEX_LBRACKETS, LEX_RBRACKETS, LEX_EQ, LEX_LESS,
//                21             22         23         24          25          26            27           28            29       30
               LEX_GREATER, LEX_PLUS,  LEX_MINUS, LEX_STAR, LEX_SLASH, LEX_LESSEQ, LEX_NEQ, LEX_GREATEREQ,LEX_PERCENT,  LEX_ID, LEX_INT_NUM, LEX_REAL_NUM, LEX_COSTRING,
//                31         32            33         34       35       36             37       38          39            40        41         42          43
               POLIZ_LABEL,POLIZ_ADDRESS,POLIZ_GO,POLIZ_FGO,LEX_EOF
//               44          45               46        47       48
};
class Lex {
	int numberid;
	lexnum numlex;
	int ifid;
	int Ilex;
	double Rlex;
	string Slex;
public:
	Lex (lexnum num = LEX_NULL, int nid = 0 , int intv = 0, double realv = 0, string stringv = "", int x = 0) {
		numlex = num;
		Ilex = intv;
		Rlex = realv;
		Slex = stringv;
		numberid = nid;
		ifid = x;
	}
	string stringget() {
		return Slex;
	}
	lexnum numget () {
		return numlex;
	}
    int lastget() {
    	return ifid;
    }
	int Intget () {
		return Ilex;
	}
	int IDget () {
		return numberid;
	}
	double Realget() {
		return Rlex;
	}

	friend ostream& operator << (ostream &s, Lex l) {
		s << '(' << l.numlex << ',' << l.Ilex << ");";
		return s;
	}
};

class ident {
	string name;
	bool declare;
	lexnum num;
	bool assign;
	int ivalue;
	double rvalue;
	string svalue;
public:
	ident () {
		declare = false;
		assign = false;
	}
	ident (const string n) {
		name = n;
		declare = false;
		assign = false;
	}
	bool operator== (const string& s) const {
		return name == s;
	}
	void put_name (const string n) {
		name = n;
	}
	string get_name () {
		return name;
	}
	bool get_declare () {
		return declare;
	}
	void put_declare () {
		declare = true;
	}
	lexnum numget () {
		return num;
	}
	void put_num (lexnum t) {
		num = t;
	}
	void put_val () {

	}
	bool get_assign () {
		return assign;
	}
	void put_assign () {
		assign = true;
	}

	int get_ivalue () {
		return ivalue;
	}
	void put_value (int v) {
		ivalue = v;
		num = LEX_INT;
	}

	double get_rvalue () {
		return rvalue;
	}
	void put_value (double v) {
		rvalue = v;
		num = LEX_REAL;
	}

	string get_svalue () {
		return svalue;
	}
	void put_value (string v ) {
		svalue = v;
		num = LEX_STRING;
	}
};

class scanner {
	FILE *fp;
	char c;
	int look (const string& buf, string * list) {
		int i = 0;
		while (list[i] != "endArray") {
			if (buf == list [ i ])
				return i;
			i++;
		}
		return 0;
	}
	void gc () {
        c = fgetc (fp);
    }

public:
    static string TW [], TD [], TL [];
	scanner(){
		fp = fopen ("file.txt", "r");
	}
	Lex get_lex ();
    void print (int num) {
		if (num <= 20) cout << "Лексема: " << TW[num] << "     " << "тип: "<< "("<<num<<")" << TL[num]  << endl;
		else if (num <= 39) cout << "Лексема: "<< TD[num - 20] << "     " << "тип: " << "("<<num<<")" << TL[num]  << endl;
	}
};

vector <ident> TID;

int  put (const string & buf) {
	vector <ident>::iterator k;

	if ( (k = find (TID.begin(), TID.end(), buf)) != TID.end() ) {
		return (k - TID.begin());
    }
	TID.push_back(buf);
	return (TID.size() - 1);
}

Lex scanner::get_lex () {
	enum state { H, IDENT, INT, INTDOT, REAL, ALE, NEQ, IFCOM, COM, COMEND, STRING_CONST};
	state CS = H;//current state
	string buf;
	int intnum, j;
	double real_num, tmp;
	do {
		gc ();
		switch (CS) {
			case H:
				if (c == ' ' || c == '\n' || c == '\r' || c == '\t') {}
				else if (ifletter(c)) {
					buf.push_back(c);
					CS = IDENT;
				}
				else if (ifdigit(c)) {
					intnum = c - '0';
					CS = INT;
				}
				else if (c == ':' || c == '<' || c == '>') {
					buf.push_back(c);
					CS = ALE;
				}
				else if (c == '"') {
					CS = STRING_CONST;
				}
				else if (c == '!') {
					buf.push_back(c);
					CS = NEQ;
				}
				else if (c == '/' ) {
					CS = IFCOM;
				}
				else {
					buf.push_back(c);
					if ((j = look(buf, TD)))
						return Lex ((lexnum)  (j + (int) LEX_FIN), j);
					else if (c == EOF)
						return Lex (LEX_EOF);
					else
						throw "Недопустимый символ (H)";
				}
				break;
			case IDENT:
				if ((ifletter(c)) || (ifdigit(c)))
					buf.push_back(c);
				else {
					ungetc(c, fp);
					if ((j = look(buf, TW)))
						return Lex ((lexnum) j, j);
					else {
						j = put(buf);
						return Lex (LEX_ID, j);
					}
				}
				break;
			case INT:
				if (ifdigit (c)) {
					intnum = intnum * 10 + (c - '0');
				}
				else if (c == '.')
					CS = INTDOT;
 				else {
					ungetc(c, fp);
					return Lex (LEX_INT_NUM, 0, intnum);
				}
				break;
			case INTDOT:
				if (ifdigit (c)) {
					CS = REAL;
					tmp = 0.1;
					real_num = intnum;
					real_num = real_num + tmp * (c - '0');
				}
				else throw "Нет дробной части (INTDOT)";
				break;
			case STRING_CONST:
				if (c != '"' && c != EOF) {
					buf.push_back(c);
				}
				else if (c == '"') {
					CS = H;
					return Lex (LEX_COSTRING,0, 0, 0, buf);
				}
				else {
					throw '!';
				}
			break;
			case REAL:
				if (ifdigit (c)) {
					tmp = tmp / 10;
					real_num = real_num + tmp * (c - '0');
				}
				else if (c == '.')
					throw "Лишняя точка (REAL)";
 				else {
					ungetc(c, fp);
					return Lex (LEX_REAL_NUM, 0, 0, real_num );
				}
				break;
			case ALE:
				if ( c == '=' )
					buf.push_back(c);
				else
					ungetc(c, fp);
				j = look (buf, TD);
				return Lex ((lexnum) ( j + (int) LEX_FIN), j);
			case NEQ:
				if (c == '=') {
					buf.push_back(c);
					j = look ( buf, TD );
					return Lex (LEX_NEQ, j);
				}
				else
					throw "Ошибка: после = должен быть ! (NEQ)";
				break;
			case IFCOM:
				if (c != '*') {
					ungetc(c, fp);
					return Lex ((LEX_SLASH));
				}
				else
					CS = COM;
				break;
			case COM:
				if (c == '*') {
                    CS = COMEND;
                    break;
                }
                break;
			case COMEND:
				if (c == '/')
					CS = H;
                else
                	CS = COM;
                break;
        }
	}
	while (true);
}


string scanner:: TW [ ] = {"nullword","and","begin","string","do","else","if","false","int","not","or","program","read",
    "then","real","true","while","write","for","break","endArray"};
string scanner:: TD [ ] = { "nulld",";","{","}",",",":",":=","(",")","=","<",">","+","-","*","/","<=","!=",">=","%", "endArray"};

string scanner:: TL [ ] = {"LEX_NULL","LEX_AND","LEX_BEGIN","LEX_STRING","LEX_DO","LEX_ELSE","LEX_IF","LEX_FALSE","LEX_INT","LEX_NOT",
"LEX_OR","LEX_PROGRAM","LEX_READ","LEX_THEN","LEX_REAL","LEX_TRUE","LEX_WHILE","LEX_WRITE","LEX_FOR","LEX_BREAK","LEX_FIN",
"LEX_SEMICOLON","LEX_LBRACE","LEX_RBRACE","LEX_COMMA","LEX_COLON","LEX_ASSIGN","LEX_LBRACKETS","LEX_RBRACKETS","LEX_EQ","LEX_LESS"
,"LEX_GREATER","LEX_PLUS","LEX_MINUS","LEX_STAR","LEX_SLASH","LEX_LESSEQ","LEX_NEQ","LEX_GREATEREQ","LEX_PERCENT","LEX_ID","LEX_INT_NUM","LEX_REAL_NUM","LEX_COSTRING","POLIZ_LABEL","POLIZ_ADDRESS","POLIZ_GO","POLIZ_FGO","LEX_EOF"};


template <class T, int max_size> class Stack {
    T s[max_size];
    int top;
public:
    Stack() {
    	top = 0;
    }
    void reset () {
    	top = 0;
    }
    void push (T i);
    T pop();
	bool three () {
		return top >= 3;
	}
    bool is_empty () {
    	return top == 0;
    }
    bool is_full () {
    	return top == max_size;
    }
};

template <class T, int max_size>

void Stack <T, max_size>::push(T i) {
    if (!is_full()) {
        s[top] = i;
        ++top;
    }
    else
        throw "Stack_is_full";
}
template <class T, int max_size>
T Stack<T, max_size>::pop() {
    if (!is_empty()) {
        --top;
        return s[top];
    }
    else
        throw "Stack_is_empty";
}

class Poliz {
    Lex *p;
    int size;
    int free;

public:
    static string T[];
    Poliz(int max_size=300) {
        p = new Lex[size = max_size];
        free = 0;
    };
    ~Poliz() {
    	delete[] p;
    };
    void put_lex(Lex l) {
        p[free] = l;
        ++free;
    };

    void put_lex(Lex l, int place) {
    	p[place] = l;
    };
    void blank() {
    	++free;
    };
    int get_free() {
    	return free;
    };
    Lex &operator[](int index) {
        if (index > size)
            throw "POLIZ:out of array";
        else if (index > free)
            throw "POLIZ:indefinite element of array";
        else
            return p[index];
    };

    /*void print() {
        for (int i = 0; i < free; ++i)
            cout << T[(p[i]).numget()]<<endl;
    };
	void print() {
        for (int i = 0; i < free; ++i) {
			if (p[i].numget()== LEX_ID) {
				cout << TID[(p[i]).IDget()].get_name() << " ";
			}
			else if (p[i].numget() == LEX_INT_NUM) {
				cout << (p[i]).Intget() << " ";
			}
			else if (p[i].numget()== LEX_REAL_NUM) {
				cout << (p[i]).Realget() << " ";
			}
			else if (p[i].numget() == LEX_COSTRING) {
				cout << (p[i]).stringget() << " ";
			}
			else if (p[i].numget() == POLIZ_GO) {
				cout << "GO"<<" ";
			}
			else if (p[i].numget() == POLIZ_LABEL) {
				cout << ((p[i]).IDget()+1) << "! ";
			}
			else if (p[i].numget() == POLIZ_FGO) {
				cout << "FGO" << " ";
			}
			else if (p[i].numget() == POLIZ_ADDRESS) {
				cout << "&";
			}
			else
            	cout << T[(p[i]).numget()] << " ";
		};
		cout << endl;
    };*/

	void print() {
        for (int i = 0; i < free; ++i) {
			if (p[i].numget() == LEX_ID) {
				cout << i << " " << TID[(p[i]).IDget()].get_name() << endl;
			}
			else if (p[i].numget() == LEX_INT_NUM) {
				cout << i <<" "<< (p[i]).Intget() << endl;
			}
			else if (p[i].numget()== LEX_REAL_NUM){
				cout << i <<" "<< (p[i]).Realget() << endl;
			}
			else if (p[i].numget()== LEX_COSTRING){
				cout << i <<" "<< (p[i]).stringget() << endl;
			}
			else if (p[i].numget()== POLIZ_GO) {
				cout << i << " "<< "GO" << endl;
			}
			else if (p[i].numget()== POLIZ_LABEL) {
				cout << i <<" "<< ((p[i]).IDget()) << "! " << endl;
			}
			else if (p[i].numget() == POLIZ_FGO){
				cout << i << " " << "FGO" << endl;
			}
			else if (p[i].numget()== POLIZ_ADDRESS){
				cout << i <<" &" << TID[(p[i]).IDget()].get_name() << endl;
			}
			else
				cout << i << " "<< T[(p[i]).numget()] << endl;
		};
		cout << endl;

    };
};
string Poliz:: T[ ] = {"nullword", "and", "begin", "string", "do", "else", "if", "false", "int", "not", "or", "program", "read",
    "then", "real", "true", "while", "write","for","break", "endArray", ";", "{", "}", ",", ":", ":=", "(", ")", "=", "<", ">", "+", "-", "*", "/", "<=", "!=", ">=", "%", "LEX_ID", "LEX_INT_NUM", "LEX_REAL_NUM", "LEX_COSTRING", "POLIZ_LABEL", "POLIZ_ADDRESS", "POLIZ_GO", "POLIZ_FGO", "LEX_EOF"};

class Parser {
    int kostflag = 0;
    Lex kost_lex;
    lexnum k_type;
	int f = 0;
    int cident;
    int kident;
	Lex curr_lex;
	lexnum c_type;
	int c_id;
	int k_id;
	int ci_val;
    int ki_val;
	double cr_val;
	double kr_val;
    string cs_val;
    string ks_val;
	scanner scan;
	Stack <int, 100> st_int;
	Stack <lexnum, 100> st_lex;
	Stack <lexnum, 100> aaa;
	lexnum descr;
	int plexit = 0;
	int breakflag = 0;

	void PROG();
	void DESCRIPTIONS();
	void DESCRIPTION();
	void OPERATORS();
	void VARIABLE();
	void CONST();
	void OPERATOR();
	void IF();
	void WHILE();
	void READ();
	void WRITE();
	void BREAK();
	void FOR();
	void COMBOPERATOR();
	void EXPROPERATOR();
	void EXPRESSION();
	void A();
	void E();
	void I();
	void R();
	void T();
	void S();
	void F();
	void X();
	void dec(lexnum type);
	void check_id(int f);
	void check_op();
	void check_not();
	void eq_type();
	void eq_bool();
	void check_id_in_read();
	void gl() {
        if (kostflag != 0) {
            kostflag--;
            Lex t1 = curr_lex;
            curr_lex = kost_lex;
            kost_lex = t1;
            lexnum t2 = c_type;
            c_type = k_type;
            k_type = t2;
            int t3 = ci_val;
            ci_val = ki_val;
            ki_val = t3;
			int t6 = c_id;
            c_id = k_id;
            k_id = t6;
            double t4 = cr_val;
            cr_val = kr_val;
            kr_val = t4;
            string t5 = cs_val;
            cs_val = ks_val;
            ks_val = t5;
            int t = cident;
            cident = kident;
            kident = t;
        }
        else {
            curr_lex = scan.get_lex();
            c_type = curr_lex.numget();
            cr_val = curr_lex.Realget();
            ci_val = curr_lex.Intget();
            cs_val = curr_lex.stringget();
			c_id = curr_lex.IDget();
            cident = curr_lex.lastget();
        }
	}
    void kostin() {
        kost_lex = curr_lex;
        k_type = c_type;
        ki_val = ci_val;
        kr_val = cr_val;
        ks_val = cs_val;
		k_id = c_id;
        kident = cident;
        gl();
        kostflag = 2;
    }
    void kostout() {
        kostflag = 0;
    }

public:
	Poliz prog;
	//Parser(const char *program) : scan(), prog(1000) {}
	void analyze();
};
//////////////////////////////////////////////////////////////
/**/
void Parser::dec(lexnum type) {
	int i;
	while (!st_int.is_empty()) {
		i = st_int.pop();
		if (TID[i].get_declare())
			throw "twice dec";
		else {
			TID[i].put_declare();
			TID[i].put_num(type);
		}
	}
}

void Parser::check_id (int f = 0) {
	if (TID[c_id].get_declare()) {
        if (f == 0)
        	prog.put_lex (curr_lex);
	}
	else
		throw "not declared";
}

void Parser::check_op() {
        if (aaa.three()) {
            lexnum t1, t2, op;
            lexnum r = LEX_NULL;
            t2 = aaa.pop();
            op = aaa.pop();
            t1 = aaa.pop();

            //cout<<t1<<" "<<op<<" "<<t2<<endl;
            if ((op == LEX_PLUS || op == LEX_MINUS || op == LEX_STAR || op == LEX_SLASH|| op == LEX_PERCENT)) {
                if ((t1 == LEX_INT) && (t2 == LEX_INT))
                	r = LEX_INT;
            }
            if ((op == LEX_PLUS || op == LEX_MINUS || op == LEX_STAR || op == LEX_SLASH)) {
                if ((t1 == LEX_REAL) && (t2 == LEX_REAL))
                	r = LEX_REAL;
                if ((t1 == LEX_INT) && (t2 == LEX_REAL))
                	r = LEX_REAL;
                if ((t1 == LEX_REAL) && (t2 == LEX_INT))
                	r = LEX_REAL;
            }
            if ((op == LEX_PLUS)) {
                if ((t1 == LEX_STRING) && (t2 == LEX_STRING))
                	r = LEX_STRING;
            }
            if (op == LEX_LESS || op == LEX_LESSEQ || op == LEX_EQ || op == LEX_NEQ|| op == LEX_GREATER|| op == LEX_GREATEREQ){
                if ((t1 == LEX_INT) && (t2 == LEX_INT))
                	r = LEX_INT;
                if ((t1 == LEX_REAL) && (t2 == LEX_REAL))
                	r = LEX_INT;
                if ((t1 == LEX_REAL) && (t2 == LEX_INT))
                	r = LEX_INT;
                if ((t1 == LEX_INT) && (t2 == LEX_REAL))
                	r = LEX_INT;
            }
            if (op == LEX_LESS|| op == LEX_EQ || op == LEX_NEQ|| op == LEX_GREATER) {
                if ((t1 == LEX_STRING) && (t2 == LEX_STRING))
                	r = LEX_INT;
            }
            if (op == LEX_AND|| op == LEX_OR) {
                if ((t1 == LEX_INT )&& (t2 == LEX_INT))
                	r = LEX_INT;
            }
            if ((op == LEX_ASSIGN)) {
                if ((t1 == LEX_INT) && (t2 == LEX_INT))
                	r=LEX_INT;
                if ((t1 == LEX_REAL) && (t2 == LEX_REAL))
                	r = LEX_REAL;
                if ((t1 == LEX_REAL) && (t2 == LEX_INT))
                	r = LEX_REAL;
                if ((t1 == LEX_INT) && (t2 == LEX_REAL))
                	r = LEX_INT;
                if ((t1 == LEX_STRING) && (t2 == LEX_STRING))
                	r = LEX_STRING;
            }
            if (r == LEX_NULL)
            	throw "Wrong types are in operation";
            aaa.push(r);
        }
}


void Parser::check_not() {
	if (aaa.pop() != LEX_INT)
		throw "Wrong type is in not";
	else {
		aaa.push(LEX_INT);
	}
}

void Parser::eq_type() {
	if (st_lex.pop() != st_lex.pop())
		throw "Wrong types are in :=";
}

void Parser::eq_bool() {
	if (aaa.pop() != LEX_INT)
		throw "Expression is not boolean";
}

void Parser::check_id_in_read() {
	if (!TID[c_id].get_declare())
		throw "Not declared";
}

void Parser::analyze() {
	gl();
	PROG();
	cout << endl
		 << "Yes!!!" << endl;
    prog.print();
}
void Parser::PROG() {
	if (c_type == LEX_PROGRAM)
		gl();
	else {
		printf("PROG1");
		throw curr_lex.numget();
	}
	if (c_type == LEX_LBRACE)
		gl();
	else {
		printf("PROG2");
		throw curr_lex.numget();
	}
	DESCRIPTIONS();
	OPERATORS();
	if (c_type == LEX_RBRACE)
		gl();
	else {
		printf("PROG3");
		throw curr_lex.numget();
	}
	if (c_type != LEX_EOF) {
		printf("PROG4");
		throw curr_lex.numget();
	}
}
void Parser::DESCRIPTIONS() {
	while ((c_type == LEX_REAL)||(c_type == LEX_INT)||(c_type == LEX_STRING)) {
		descr = c_type;
		st_int.reset();
		gl();
		DESCRIPTION();
		//dec(descr);
		if (c_type == LEX_SEMICOLON)
			gl();
		else {
			printf("DESCRIPTIONS");
			throw curr_lex.numget();
		}
	}
}
void Parser::DESCRIPTION() {
	if (c_type != LEX_ID) {
		printf("DESCRIPTION1 ");
		throw curr_lex.numget();
	}
	else {
		while(1) {
			VARIABLE();
			if (c_type == LEX_COMMA)
				gl();
			else
				break;
			if (c_type != LEX_ID) {
				printf("DESCRIPTION2 ");
				throw curr_lex.numget();
			}
		}
	}
}
void Parser::VARIABLE() {
	st_int.push (c_id);
	dec(descr);
	aaa.push((TID[c_id]).numget());
	kostin();
	//check_id();
	if (c_type == LEX_ASSIGN) {
		gl();//id
		prog.put_lex (Lex(POLIZ_ADDRESS,c_id));
		check_id(1);
		gl();//assign
		aaa.push(c_type);
		gl();
		if ((c_type == LEX_COSTRING) || (c_type == LEX_PLUS) || (c_type == LEX_MINUS) || (c_type == LEX_INT_NUM) || (c_type == LEX_REAL_NUM)) {
			CONST();
			check_op();
			prog.put_lex(LEX_ASSIGN);
			gl();
		}
		else {
			printf("VARIABLE");
			throw curr_lex.numget();
		}
	}
	kostout();
	aaa.reset();
}

void Parser::CONST() {

//	prog.put_lex(curr_lex);
	//	aaa.push(LEX_INT);

	if ((c_type == LEX_MINUS)) {
		st_lex.push (c_type);
		gl();
		if (c_type ==LEX_INT_NUM) {
			aaa.push(LEX_INT);
			prog.put_lex(curr_lex);
		}
		else if (c_type ==LEX_REAL_NUM) {
			aaa.push(LEX_REAL);
			prog.put_lex(curr_lex);
		}
		else {
			printf("CONST");
			throw curr_lex.numget();
		}
	}
	if (c_type == LEX_INT_NUM) {
		aaa.push(LEX_INT);
		prog.put_lex(curr_lex);
	}
	else if (c_type == LEX_REAL_NUM) {
		aaa.push(LEX_REAL);
		prog.put_lex(curr_lex);
	}
	else if (c_type == LEX_COSTRING) {
		aaa.push(LEX_STRING);
		prog.put_lex(curr_lex);
	}
	else {
		printf("CONST");
		throw curr_lex.numget();
	}
}

void Parser::OPERATORS() {
	while (1){
		if (c_type == LEX_RBRACE)
			break;
		OPERATOR();
		if (c_type == LEX_RBRACE)
			break;
	}
}

void Parser::OPERATOR() {
	switch (c_type) {
		case LEX_IF:
			IF();
			break;
		case LEX_WHILE:
			WHILE();
			break;
		case LEX_READ:
			READ();
			break;
		case LEX_WRITE:
			WRITE();
			break;
		case LEX_BREAK:
			BREAK();
			break;
		case LEX_FOR:
			FOR();
			break;
		case LEX_LBRACE:
			COMBOPERATOR();
			break;
		default:
			EXPRESSION();
			aaa.reset();
			if (c_type == LEX_SEMICOLON) {
				prog.put_lex(Lex (LEX_SEMICOLON));
				gl();
			}
			else {
				printf("OPERATOR");
				throw curr_lex.numget();
			}
			break;
	}
}
void Parser::IF() {
	int  pl2, pl3;
	gl();
	if (c_type == LEX_LBRACKETS)
		gl();
	else {
		printf("IF");
		throw curr_lex.numget();
	}

	EXPRESSION();
    eq_bool();
    aaa.reset();
	pl2 = prog.get_free();
	prog.blank();
	prog.put_lex(Lex(POLIZ_FGO));

	if (c_type == LEX_RBRACKETS) gl();
	else {
		printf("IF");
		throw curr_lex.numget();
	}

	OPERATOR();

	if (c_type == LEX_ELSE) {
		pl3 = prog.get_free();
		prog.blank();
		prog.put_lex(Lex(POLIZ_GO));
		prog.put_lex(Lex(POLIZ_LABEL, prog.get_free()), pl2);
		gl();
		OPERATOR();
		prog.put_lex(Lex(POLIZ_LABEL, prog.get_free()), pl3);
	}
	else
		prog.put_lex(Lex(POLIZ_LABEL, prog.get_free()), pl2);
}
void Parser::WHILE() {
    f++;
	breakflag = 0;
	int  pl0, pl1;
	pl0 = prog.get_free();
	gl();
	if (c_type == LEX_LBRACKETS)
		gl();
	else {
		printf("WHILE");
		throw curr_lex.numget();
	}
	EXPRESSION();
    eq_bool();
    aaa.reset();
	pl1 = prog.get_free();
	prog.blank();
	prog.put_lex(Lex(POLIZ_FGO));

	if (c_type == LEX_RBRACKETS)
		gl();
	else {
		printf("WHILE");
		throw curr_lex.numget();
	}

	OPERATOR();
	if (breakflag) {
		breakflag--;
		prog.put_lex(Lex(POLIZ_LABEL, prog.get_free()+2), plexit);
	}
	prog.put_lex(Lex(POLIZ_LABEL, pl0));
	prog.put_lex(Lex(POLIZ_GO));
	prog.put_lex(Lex(POLIZ_LABEL, prog.get_free()), pl1);
	plexit = prog.get_free();
	f--;
}
void Parser::READ() {
	st_lex.push (c_type);
	gl();
	if (c_type == LEX_LBRACKETS)
		gl();
	else {
		printf("READ");
		throw curr_lex.numget();
	}

	if (c_type == LEX_ID) {
		prog.put_lex (Lex (POLIZ_ADDRESS,c_id));
		check_id_in_read();
		gl();
	}
	else {
		printf("READ");
		throw curr_lex.numget();
	}

	if (c_type == LEX_RBRACKETS) {
		gl();
		while ((st_lex.is_empty()) == false){
			prog.put_lex(Lex (st_lex.pop()));
		}
	}
	else {
		printf("READ");
		throw curr_lex.numget();
	}
	if (c_type == LEX_SEMICOLON) {
		prog.put_lex(Lex (LEX_SEMICOLON));
		gl();
	}
	else {
		printf("READ");
		throw curr_lex.numget();
	}
}
void Parser::WRITE() {
	st_lex.push (c_type);
	gl();
	if (c_type == LEX_LBRACKETS)
		gl();
	else {
		printf("WRITE1");
		throw curr_lex.numget();
	}
	while(1) {
		EXPRESSION();
        aaa.reset();
		if (c_type != LEX_COMMA)
			break;
		gl();
	}
	if (c_type == LEX_RBRACKETS) {
		gl();
		while ((st_lex.is_empty()) == false) {
			prog.put_lex(Lex (st_lex.pop()));
		}
	}
	else {
		printf("WRITE2 ");
		throw curr_lex.numget();
	}
	if (c_type == LEX_SEMICOLON) {
		prog.put_lex(Lex(LEX_SEMICOLON));
		gl();
	}
	else {
		printf("WRITE3 ");
		throw curr_lex.numget();
	}
}
void Parser::BREAK() {
    if (f<1) throw "break вне цикла";
	breakflag++;
	plexit = prog.get_free();
	prog.blank();
	prog.put_lex(Lex(POLIZ_GO));
	gl();
	if (c_type == LEX_SEMICOLON) {
		gl();
	}
	else {
		printf("BREAK");
		throw curr_lex.numget();
	}
}
void Parser::FOR() {
    f++;
	breakflag = 0;
	int flag1 = 0, flag2 = 0;
	int pl0 = 0, pl1 = 0, pl2 = 0, pl3 = 0;
	gl();
	if (c_type == LEX_LBRACKETS)
		gl();
	else {
		printf("FOR");
		throw curr_lex.numget();
	}

	if (c_type != LEX_SEMICOLON) {
		EXPRESSION();
        prog.put_lex(Lex (LEX_SEMICOLON));
        eq_bool();
        aaa.reset();
	}
	if (c_type == LEX_SEMICOLON)
		gl();
    else {
    	throw "no semicolon";
    }

	if (c_type != LEX_SEMICOLON) {
		flag1 = 1;
		pl0 = prog.get_free();//1

		EXPRESSION();
        //prog.put_lex(Lex (LEX_SEMICOLON));
        eq_bool();
		aaa.reset();
		pl1 = prog.get_free();
		prog.blank();
		prog.put_lex(Lex(POLIZ_FGO));
	}
	else plexit = prog.get_free();
	if (c_type == LEX_SEMICOLON)
		gl();
    else {
    	throw "no semicolon";
    }

	if (c_type != LEX_RBRACKETS) {
		flag2 = 1;
		pl3 = prog.get_free();
		prog.blank();
		prog.put_lex(Lex(POLIZ_GO));
		pl2 = prog.get_free();

		EXPRESSION();
        prog.put_lex(Lex (LEX_SEMICOLON));
        eq_bool();
		aaa.reset();
	}
	if (c_type == LEX_RBRACKETS) {
		if (flag1&&flag2){
			gl();
			prog.put_lex(Lex(POLIZ_LABEL, pl0));
			prog.put_lex(Lex(POLIZ_GO));
			prog.put_lex(Lex(POLIZ_LABEL, prog.get_free()), pl3);
			OPERATOR();
			if (breakflag) {
				breakflag--;
				prog.put_lex(Lex(POLIZ_LABEL,prog.get_free()+2), plexit);
			}
			prog.put_lex(Lex(POLIZ_LABEL, pl2));
			prog.put_lex(Lex(POLIZ_GO));
			prog.put_lex(Lex(POLIZ_LABEL, prog.get_free()), pl1);
		}
		else if (flag1||flag2) {
			gl();

			if (flag2)
				prog.put_lex(Lex(POLIZ_LABEL, prog.get_free()), pl3);

			OPERATOR();
			if (breakflag) {
				breakflag--;
				prog.put_lex(Lex(POLIZ_LABEL,prog.get_free()+2), plexit);
			}
			if (flag2) {
				prog.put_lex(Lex(POLIZ_LABEL, pl3+2));
				prog.put_lex(Lex(POLIZ_GO));
			}
			if (flag1) {
				prog.put_lex(Lex(POLIZ_LABEL, pl0));
				prog.put_lex(Lex(POLIZ_GO));
			}
			if (flag1)
				prog.put_lex(Lex(POLIZ_LABEL, prog.get_free()), pl1);
		}
		else{
			gl();
			pl0 = prog.get_free();

			OPERATOR();
			if (breakflag) {
				breakflag--;
				prog.put_lex(Lex(POLIZ_LABEL, prog.get_free()+2), plexit);
			}
			prog.put_lex(Lex(POLIZ_LABEL, pl0));
			prog.put_lex(Lex(POLIZ_GO));
		}
	}
	else {
		printf("FOR");
		throw curr_lex.numget();
	}
    f--;
}


void Parser::COMBOPERATOR() {
	if (c_type == LEX_LBRACE)
		gl();
	else {
		printf("COMBOPERATOR");
		throw curr_lex.numget();
	}
	OPERATORS();
	if (c_type == LEX_RBRACE)
		gl();
	else {
		printf("COMBOPERATOR");
		throw curr_lex.numget();
	}
}

void Parser::EXPRESSION() {
	lexnum a;
    if (c_type == LEX_ID) {
        kostin();
        if (c_type == LEX_ASSIGN) {
			gl();// ID
			aaa.push((TID[c_id]).numget());
			prog.put_lex (Lex(POLIZ_ADDRESS, c_id));
			check_id(1);
			gl();// ASSIGN
			aaa.push(c_type);
			st_lex.push (c_type);
			gl();// NEXT

			EXPRESSION();

			while ((st_lex.is_empty()) == false) {
				a = st_lex.pop();
				if (a != LEX_ASSIGN) {
					st_lex.push(a);
					break;
				}
				prog.put_lex(Lex (a));
			}
            check_op();
        }
        else {
			gl();//ID
			I();
            check_op();
		}
    }
    else {
		//const
		I();
        check_op();
	}
}

void Parser::I() {
    int pl0, pl1, pl2, pl3;
	R();
    while (c_type == LEX_OR) {
		aaa.push(c_type);
        pl0 = prog.get_free();
        prog.blank();
        prog.put_lex(Lex(POLIZ_FGO));
        prog.put_lex(Lex(LEX_INT_NUM, 0, 1));
        pl1= prog.get_free();
        prog.blank();
        prog.put_lex(Lex(POLIZ_GO));
        prog.put_lex(Lex(POLIZ_LABEL, prog.get_free()), pl0);
        gl();
        R();
		check_op();

        pl2 = prog.get_free();
        prog.blank();
        prog.put_lex(Lex(POLIZ_FGO));
        prog.put_lex(Lex(LEX_INT_NUM, 0, 1));
        pl3 = prog.get_free();
        prog.blank();
        prog.put_lex(Lex(POLIZ_GO));
        prog.put_lex(Lex(POLIZ_LABEL, prog.get_free()), pl2);
        prog.put_lex(Lex(LEX_INT_NUM));
        prog.put_lex(Lex(POLIZ_LABEL,prog.get_free()), pl3);
        prog.put_lex(Lex(POLIZ_LABEL,prog.get_free()), pl1);
    }
}

void Parser::R() {
    int pl0, pl1, pl2;
	T();
    while (c_type == LEX_AND) {
		aaa.push(c_type);

        pl0 = prog.get_free();
        prog.blank();
        prog.put_lex(Lex(POLIZ_FGO));
        gl();
        T();
        check_op();
        pl1 = prog.get_free();
        prog.blank();
        prog.put_lex(Lex(POLIZ_FGO));
        prog.put_lex(Lex(LEX_INT_NUM, 0, 1));
        pl2 = prog.get_free();
        prog.blank();
        prog.put_lex(Lex(POLIZ_GO));

        prog.put_lex(Lex(POLIZ_LABEL, prog.get_free()), pl1);
        prog.put_lex(Lex(POLIZ_LABEL, prog.get_free()), pl0);
        prog.put_lex(Lex(LEX_INT_NUM));
		prog.put_lex(Lex(POLIZ_LABEL, prog.get_free()), pl2);

    }
}
void Parser::T()
{
    S();
    while ((c_type == LEX_LESS) || (c_type == LEX_GREATER) || (c_type == LEX_LESSEQ) || (c_type == LEX_GREATEREQ) || (c_type == LEX_EQ) || (c_type == LEX_NEQ)) {
        st_lex.push (c_type);
		aaa.push(c_type);
        gl();
        S();
		check_op();
        if ((st_lex.is_empty())) {}
        else
        	prog.put_lex(Lex (st_lex.pop()));
    }
}
void Parser::S() {
    F();
    while ((c_type == LEX_PLUS)||(c_type == LEX_MINUS)) {
        st_lex.push (c_type);
		aaa.push(c_type);
        gl();
        F();
		check_op();
        if ((st_lex.is_empty())) {}
        else
        	prog.put_lex(Lex (st_lex.pop()));
    }
}
void Parser::F() {
    X();
    while ((c_type == LEX_STAR) || (c_type == LEX_SLASH) || (c_type == LEX_PERCENT)) {
        st_lex.push (c_type);
		aaa.push (c_type);
        gl();
        X();
		check_op();
        if ((st_lex.is_empty())) {}
        else
        	prog.put_lex(Lex (st_lex.pop()));
    }

}
void Parser::X() {
	if (c_type == LEX_INT_NUM) {
		prog.put_lex(curr_lex);
		aaa.push(LEX_INT);
		gl();
	}
	else if (c_type == LEX_REAL_NUM) {
		prog.put_lex (curr_lex);
		aaa.push (LEX_REAL);
		gl();
	}
	else if (c_type == LEX_COSTRING) {
		prog.put_lex (curr_lex);
		aaa.push (LEX_STRING);
		gl();
	}
	else if (c_type == LEX_LBRACKETS) {
		gl();
		I();
		if (c_type == LEX_RBRACKETS) ;
		else {
			printf("X1  ");
			throw curr_lex.numget();
		}
		gl();
	}
	else if ((c_type == LEX_NOT) || (c_type == LEX_MINUS)) {
		st_lex.push (c_type);
		gl();
		X();
        check_not();
		if ((st_lex.is_empty())) {}
        else
        	prog.put_lex(Lex (st_lex.pop()));
	}
    else if (c_type == LEX_ID) {
		check_id();
		aaa.push((TID[c_id]).numget());
		gl();
    }
	else if (c_type != LEX_ID) {
		printf("X2  ");
		throw curr_lex.numget(); gl();
	}
}


class Executer {
    Lex pc_el;
public:
    void execute(Poliz &prog);
};

void Executer::execute(Poliz &prog) {
    Lex a,b;
    int iv = 0, f = 0;
    double rv = 0;
    string sv = "";
    Stack < Lex, 100 > args;
    int i, index = 0, size = prog.get_free();
    while (index < size) {
        pc_el = prog[index];
        switch (pc_el.numget()) {
        case LEX_SEMICOLON:
            args.reset();
            break;
        case LEX_COSTRING:
        case LEX_REAL_NUM:
        case LEX_INT_NUM:
        case POLIZ_ADDRESS:
        case POLIZ_LABEL:
            args.push(pc_el);
            break;
        case LEX_ID:
            //printf("ID");
            i = prog[index].IDget();
            if (TID[i].get_assign()) {
                if (TID[i].numget() == LEX_INT) {
                    args.push(Lex(LEX_INT_NUM, i, TID[i].get_ivalue(), 0, "", 1));
                }
                if (TID[i].numget() == LEX_REAL) {
                    args.push(Lex(LEX_REAL_NUM, i, 0, TID[i].get_rvalue(), "", 1));
                }
                if (TID[i].numget() == LEX_STRING) {
                    args.push(Lex(LEX_COSTRING, i, 0, 0, TID[i].get_svalue(), 1));
                }
                //args.push(TID[i].get_value());
                break;
            }
            else
                throw "POLIZ: indefinite identifier1";
            break;
        case LEX_NOT:
            f = 0;
            if (!args.pop().Intget())
            	f=1;
            args.push(Lex(LEX_INT_NUM,0,f));
            break;
        /*case LEX_OR:
            f = 0;
            andflag = 0;
            if (orflag){
                args.pop();
                args.pop();
                f = 1;
            }
            else {
                i = args.pop().Intget();
                iv = args.pop().Intget();
                if (iv || i)
                	f = 1;
                if (f)
                	orflag = 3;
            }
            args.push(Lex(LEX_INT_NUM, 0, f));
            break;
        case LEX_AND:
            f = 0;
            orflag = 0;
            if (andflag) {
                args.pop();
                args.pop();
                f = 0;
            }
            else{
                i = args.pop().Intget();
                iv = args.pop().Intget();
                if (iv && i)
                	f = 1;
                if (!f)
                	andflag = 3;
            }
            args.push(Lex(LEX_INT_NUM,0,f));
            break;*/
        case POLIZ_GO:
            index = args.pop().IDget() - 1;
            break;

        case POLIZ_FGO:
            i = args.pop().IDget();
            if (!args.pop().Intget())
                index = i - 1;
            break;
        case LEX_WRITE:
            a = args.pop();
            if (a.lastget() == 0) {
                if (a.numget() == LEX_INT_NUM) cout << ">" << a.Intget() << endl;
                if (a.numget() == LEX_REAL_NUM) cout << ">" << a.Realget() << endl;
                if (a.numget() == LEX_COSTRING) {/*if (a.stringget() == "\n") cout << endl; else*/cout << ">" << a.stringget() << endl;}
            }
            else {
                if (a.numget() == LEX_INT_NUM)
                	cout << ">" << TID[a.IDget()].get_ivalue() << endl; //a.Intget() << endl;
                if (a.numget() == LEX_REAL_NUM)
                	cout << ">" << TID[a.IDget()].get_rvalue() << endl;
                if (a.numget()  ==LEX_COSTRING)
                	cout << ">" << TID[a.IDget()].get_svalue() << endl;
            }
            break;
        case LEX_READ:
            //TID[a.IDget()].put_value(b.Realget());
            i = args.pop().IDget();
            if (TID[i].numget() == LEX_INT) {
                cout << "Input int value for ";
                cout << TID[i].get_name() << endl;
                cin >> iv;
                TID[i].put_value(iv);
            }
            else if (TID[i].numget() == LEX_REAL) {
                cout << "Input real value for ";
                cout << TID[i].get_name() << endl;
                cin >> rv;
                TID[i].put_value(rv);
            }
            else if (TID[i].numget() == LEX_STRING) {
                cout << "Input string value for ";
                cout << TID[i].get_name() << endl;
                cin >> sv;
                TID[i].put_value(sv);
            }
            else
            	throw "РѕС€РёР±РєР° РІ Read";
            TID[i].put_assign();
            break;
        case LEX_PLUS:
            b = args.pop();
            a = args.pop();
            if (a.numget() == LEX_COSTRING)
                args.push(Lex(LEX_COSTRING, 0, 0, 0, a.stringget() + b.stringget()));
            if ((a.numget() == LEX_INT_NUM) && (b.numget() == LEX_INT_NUM))
                args.push(Lex(LEX_INT_NUM, 0, a.Intget() + b.Intget()));

            if ((a.numget() == LEX_INT_NUM) && (b.numget() == LEX_REAL_NUM))
                args.push(Lex(LEX_REAL_NUM, 0, 0, a.Intget() + b.Realget()));

            if ((a.numget() == LEX_REAL_NUM) && (b.numget() == LEX_INT_NUM))
                args.push(Lex(LEX_REAL_NUM, 0, 0, a.Realget() + b.Intget()));

            if ((a.numget() == LEX_REAL_NUM) && (b.numget() == LEX_REAL_NUM))
                args.push(Lex(LEX_REAL_NUM, 0, 0, a.Realget() + b.Realget()));
            break;
        case LEX_STAR:
            b = args.pop();
            a = args.pop();
            if ((a.numget() == LEX_INT_NUM) && (b.numget() == LEX_INT_NUM))
                args.push(Lex(LEX_INT_NUM, 0, a.Intget()*b.Intget()));

            if ((a.numget() == LEX_INT_NUM) && (b.numget() == LEX_REAL_NUM))
                args.push(Lex(LEX_REAL_NUM, 0, 0, a.Intget()*b.Realget()));

            if ((a.numget() == LEX_REAL_NUM) && (b.numget() == LEX_INT_NUM))
                args.push(Lex(LEX_REAL_NUM, 0, 0, a.Realget()*b.Intget()));

            if ((a.numget() == LEX_REAL_NUM) && (b.numget() == LEX_REAL_NUM))
                args.push(Lex(LEX_REAL_NUM, 0, 0, a.Realget()*b.Realget()));
            break;
        case LEX_MINUS:
            b = args.pop();
            a = args.pop();
            if ((a.numget() != LEX_INT_NUM) && (a.numget() != LEX_REAL_NUM) && (a.numget() != LEX_COSTRING)) {
                args.push(a);
                args.push(Lex(LEX_INT_NUM, 0, -b.Intget()));
            }
            else{
                //cout<<a<<endl;
                if ((a.numget() ==LEX_INT_NUM)&&(b.numget() == LEX_INT_NUM))
                    args.push(Lex(LEX_INT_NUM, 0, a.Intget()-b.Intget()));

                if ((a.numget() == LEX_INT_NUM)&&(b.numget()==LEX_REAL_NUM))
                    args.push(Lex(LEX_REAL_NUM, 0, 0, a.Intget()-b.Realget()));

                if ((a.numget() == LEX_REAL_NUM)&&(b.numget()==LEX_INT_NUM))
                    args.push(Lex(LEX_REAL_NUM, 0, 0, a.Realget()-b.Intget()));

                if ((a.numget() == LEX_REAL_NUM) && (b.numget() == LEX_REAL_NUM))
                    args.push(Lex(LEX_REAL_NUM, 0, 0, a.Realget()-b.Realget()));
            }
            break;
        case LEX_SLASH:
            b = args.pop();
            a = args.pop();
            if (((b.Intget() == 0) && (b.numget() == LEX_INT_NUM)) || ((b.Realget() == 0.0) && (b.numget() == LEX_REAL_NUM)))
            	throw "POLIZ:divide by zero";
            else {
                if ((a.numget() == LEX_INT_NUM) && (b.numget() == LEX_INT_NUM))
                    args.push(Lex(LEX_INT_NUM, 0, a.Intget()/b.Intget()));

                if ((a.numget() == LEX_INT_NUM) && (b.numget() == LEX_REAL_NUM))
                    args.push(Lex(LEX_REAL_NUM, 0, 0, a.Intget()/b.Realget()));

                if ((a.numget() == LEX_REAL_NUM) && (b.numget() == LEX_INT_NUM))
                    args.push(Lex(LEX_REAL_NUM, 0, 0, a.Realget()/b.Intget()));

                if ((a.numget() == LEX_REAL_NUM) && (b.numget() == LEX_REAL_NUM))
                    args.push(Lex(LEX_REAL_NUM, 0, 0, a.Realget()/b.Realget()));
            break;
            }

        case LEX_PERCENT:
            b = args.pop();
            a = args.pop();
            if (b.Intget() == 0)
            	throw "POLIZ:divide by zero";
            else {
                    args.push(Lex(LEX_INT_NUM, 0, a.Intget()%b.Intget()));
            break;
            }
        case LEX_EQ:
            f = 0;
            a = args.pop();
            b = args.pop();
            if (a.numget() == LEX_COSTRING)
                if(a.stringget() == b.stringget())
                	f = 1;
            if ((a.numget() == LEX_INT_NUM) && (b.numget() == LEX_INT_NUM))
                if (a.Intget() == b.Intget())
                	f = 1;
            if ((a.numget() == LEX_INT_NUM) && (b.numget() == LEX_REAL_NUM))
                if (a.Intget() == b.Realget())
                	f = 1;
            if ((a.numget() == LEX_REAL_NUM) && (b.numget() == LEX_INT_NUM))
                if (a.Realget() == b.Intget())
                	f = 1;
            if ((a.numget() == LEX_REAL_NUM) && (b.numget() == LEX_REAL_NUM))
                if (a.Realget() == b.Realget())
                	f = 1;

            args.push(Lex(LEX_INT_NUM, 0, f));
            break;
        case LEX_GREATER:
            f = 0;
            a = args.pop();
            b = args.pop();
            if (a.numget() == LEX_COSTRING)
                if(a.stringget() < b.stringget())
                	f = 1;
            if ((a.numget() == LEX_INT_NUM) && (b.numget() == LEX_INT_NUM))
                if (a.Intget() < b.Intget())
                	f = 1;
            if ((a.numget() == LEX_INT_NUM) && (b.numget() == LEX_REAL_NUM))
                if (a.Intget() < b.Realget())
                	f = 1;
            if ((a.numget() == LEX_REAL_NUM) && (b.numget() == LEX_INT_NUM))
                if (a.Realget() < b.Intget())
                	f = 1;
            if ((a.numget() == LEX_REAL_NUM) && (b.numget() == LEX_REAL_NUM))
                if (a.Realget() < b.Realget())
                	f = 1;

            args.push(Lex(LEX_INT_NUM, 0, f));
            break;
        case LEX_LESS:
            f = 0;
            a = args.pop();
            b = args.pop();
            if (a.numget() == LEX_COSTRING)
                if(a.stringget() > b.stringget())
                	f = 1;
            if ((a.numget() == LEX_INT_NUM) && (b.numget() == LEX_INT_NUM))
                if (a.Intget() > b.Intget())
                	f = 1;
            if ((a.numget() ==  LEX_INT_NUM) && (b.numget() == LEX_REAL_NUM))
                if (a.Intget() > b.Realget())
                	f = 1;
            if ((a.numget() == LEX_REAL_NUM) && (b.numget() == LEX_INT_NUM))
                if (a.Realget() > b.Intget())
                	f = 1;
            if ((a.numget() == LEX_REAL_NUM) && (b.numget() == LEX_REAL_NUM))
                if (a.Realget() > b.Realget())
                	f = 1;

            args.push(Lex(LEX_INT_NUM,0,f));
            break;
        case LEX_GREATEREQ:
            f = 0;
            a = args.pop();
            b = args.pop();
            if ((a.numget() == LEX_INT_NUM) && (b.numget() == LEX_INT_NUM))
                if (a.Intget() <= b.Intget()) f=1;
            if ((a.numget() == LEX_INT_NUM) && (b.numget() == LEX_REAL_NUM))
                if (a.Intget() <= b.Realget())
                	f = 1;
            if ((a.numget() == LEX_REAL_NUM) && (b.numget() == LEX_INT_NUM))
                if (a.Realget() <= b.Intget())
                	f = 1;
            if ((a.numget() == LEX_REAL_NUM) && (b.numget() == LEX_REAL_NUM))
                if (a.Realget() <= b.Realget())
                	f = 1;

            args.push(Lex(LEX_INT_NUM, 0, f));
            break;
        case LEX_LESSEQ:
            f = 0;
            a = args.pop();
            b = args.pop();
            if ((a.numget() == LEX_INT_NUM) && (b.numget() == LEX_INT_NUM))
                if (a.Intget() >= b.Intget())
                	f = 1;
            if ((a.numget() == LEX_INT_NUM) && (b.numget() == LEX_REAL_NUM))
                if (a.Intget() >= b.Realget())
                	f = 1;
            if ((a.numget() == LEX_REAL_NUM) && (b.numget() == LEX_INT_NUM))
                if (a.Realget() >= b.Intget())
                	f = 1;
            if ((a.numget() == LEX_REAL_NUM) && (b.numget() == LEX_REAL_NUM))
                if (a.Realget() >= b.Realget())
                	f = 1;

            args.push(Lex(LEX_INT_NUM, 0, f));
            break;
        case LEX_NEQ:
            f = 0;
            a = args.pop();
            b = args.pop();
            if (a.numget() == LEX_COSTRING)
                if(a.stringget() != b.stringget())
                	f = 1;
            if ((a.numget() == LEX_INT_NUM) && (b.numget() == LEX_INT_NUM))
                if (a.Intget() != b.Intget())
                	f = 1;
            if ((a.numget() == LEX_INT_NUM) && (b.numget() == LEX_REAL_NUM))
                if (a.Intget() != b.Realget())
                	f = 1;
            if ((a.numget() == LEX_REAL_NUM) && (b.numget() == LEX_INT_NUM))
                if (a.Realget() != b.Intget())
                	f = 1;
            if ((a.numget() == LEX_REAL_NUM) && (b.numget() == LEX_REAL_NUM))
                if (a.Realget() != b.Realget())
                	f = 1;

            args.push(Lex(LEX_INT_NUM, 0, f));
            break;
        case LEX_ASSIGN:
            b = args.pop();
            a = args.pop();
            //cout << (TID[a.IDget()].numget()) << endl;
            if (TID[a.IDget()].numget() == LEX_STRING)
                TID[a.IDget()].put_value(b.stringget());

            if ((TID[a.IDget()].numget() == LEX_INT) && (b.numget() == LEX_INT_NUM)) {

                TID[a.IDget()].put_value(b.Intget());
            }
            if ((TID[a.IDget()].numget() == LEX_INT) && (b.numget() == LEX_REAL_NUM)) {
                iv = b.Realget();
                TID[a.IDget()].put_value(iv);
            }
            if ((TID[a.IDget()].numget() == LEX_REAL) && (b.numget() == LEX_INT_NUM)) {
                rv = b.Intget();
                TID[a.IDget()].put_value(rv);
            }
            if ((TID[a.IDget()].numget()==LEX_REAL)&&(b.numget()==LEX_REAL_NUM)) {
                TID[a.IDget()].put_value(b.Realget());
            }

            TID[a.IDget()].put_assign();
            break;
        default:
            throw "POLIZ: unexpected elem";
        } // end of switch
        ++index;
        //cout << index << endl;
    }; //end of while
    cout << "Finish of executing!!!" << endl;
}


int main () {
	Parser aaa;
	try {
		aaa.analyze();
	}
	catch (lexnum const bbb) {
		cout << "Ошибка в лексеме: " << bbb << endl;
		return 0;
	}
	catch (char const * str) {
		cout << str << endl;
		return 0;
	}
	catch (char const  str) {
		cout << str << endl;
		return 0;
	}

	return 0;
}
