/* Copyright (c) 2016-2017 Eudaemon <eudaemonal@gmail.com> */

#include <iostream>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <algorithm>
#include <vector>
#include <deque>
#include <stack>
#include <cmath>

// init vector of all operators
const char* operators[] = {"add", "sub", "mult", "div", "sqrt", "pop", "reverse", "repeat", "endrepeat"};
std::vector<std::string> oplib(operators, std::end(operators));

// operator and operand stack
std::deque<double> operandStack;
std::deque<bool> dtypeStack; // digit type for int or double
std::deque<std::string> operatorStack;

 // tokenStack for processing and repeat
std::deque<std::string> tokenStack;
std::deque<std::string> tokenRepStack;


int inRepeat = 0;
int repeatTimes = 0;

// if a token is a operator
bool isoperator(std::string op){
	if(std::find(oplib.begin(), oplib.end(), op) != oplib.end())
		return true;	
	else
		return false;
}

// if a string is a digit
// invalid: x. or .x or -.x or -x.
bool isdigit(std::string s){
	std::string::const_iterator it = s.begin();
	if(*it=='-'){ // negative number
		s.erase(s.begin());
	}
	it = s.begin();
	while (it != s.end() && (std::isdigit(*it)||(*it=='.'&& it !=s.end()-1 && it !=s.begin()))) ++it;
	return !s.empty() && it == s.end();
}

// if a string is a int
bool isint(std::string s){
	std::string::const_iterator it = s.begin();
	if(*it=='-'){ // negative number
		s.erase(s.begin());
	}
	it = s.begin();
	while (it != s.end() && (std::isdigit(*it))) ++it;
	return !s.empty() && it == s.end();
}


// basic calculation display
void basicdisplay(char opt){
	bool a, b, c;
	double x, y, z=0;
	a = dtypeStack.back();
	dtypeStack.pop_back();
	b = dtypeStack.back();
	dtypeStack.pop_back();
	c = a||b;

	x = operandStack.back();
	operandStack.pop_back();
	y = operandStack.back();
	operandStack.pop_back();

	if(opt=='+')
		z = x + y;
	else if(opt=='-')
		z = x - y;
	else if(opt=='*')
		z = x * y;
	else if(opt=='/'){
		if(y==0){
			std::cerr << "Error: divisor must not equal to 0" <<std::endl;
			exit (EXIT_FAILURE);
		}else{
			z = x / y;
		}
	}

	if(!a)	std::cout << (int)x;
	else	std::cout << x;
	std::cout <<" "<<opt<<" ";
	if(!b)	std::cout << (int)y;
	else	std::cout << y;
	std::cout <<" = ";
	if(!c)	std::cout << (int)z;
	else	std::cout << z;
	std::cout << std::endl;
	
	operandStack.push_back(z);
	dtypeStack.push_back(c);

}

// calculate tokens and store result back into stack
void calculate(){
	double x, z;
       	int i, n = 0;	// numbers of reverse
	bool a;
	std::deque<double> reverseQueue;
	std::deque<bool> reverseTypeQueue;

	std::string s = operatorStack.back();
	operatorStack.pop_back();

	if(s =="add"){
		basicdisplay('+');
	}
	else if(s=="sub"){
		basicdisplay('-');
	}
	else if(s=="mult"){
		basicdisplay('*');
	}
	else if(s=="div"){
		basicdisplay('/');
	}
	else if(s=="sqrt"){
		x = operandStack.back();
		operandStack.pop_back();
		a = dtypeStack.back();
		dtypeStack.pop_back();
		if(x<0){
			std::cerr << "Error: Square root produces complex number, not supported" <<std::endl;
			exit (EXIT_FAILURE);
		}else{
			z = sqrt(x) ;
		}
		if(a)	std::cout <<"sqrt " << x << " = "<< z <<std::endl;
		else	std::cout <<"sqrt " << (int)x << " = "<< (int)z <<std::endl;

		operandStack.push_back(z);
		dtypeStack.push_back(a);		
	}
	else if(s=="pop"){
		operandStack.pop_back();
		dtypeStack.pop_back();
	}
	else if(s=="reverse"){
		n = (int)operandStack.back();
		operandStack.pop_back();
		dtypeStack.pop_back();
		for(i=0;i<n;++i){
			x = operandStack.back();
			operandStack.pop_back();
			reverseQueue.push_front(x);
			a = dtypeStack.back();
			dtypeStack.pop_back();
			reverseTypeQueue.push_front(a);
		}
		for(i=0;i<n;++i){
			x = reverseQueue.back();
			reverseQueue.pop_back();
			operandStack.push_back(x);
			a = reverseTypeQueue.back();
			reverseTypeQueue.pop_back();
			dtypeStack.push_back(a);
		}
	}
}


// Process input token
void process(std::string s){
	bool dtype = 0; // int: 0 ;double: 1

	if(isdigit(s)){
		if(isint(s))
			dtype = 0;
		else
			dtype = 1;

		operandStack.push_back(std::stod(s));
		dtypeStack.push_back(dtype);
	
	}
	else if(isoperator(s)){
		operatorStack.push_back(s);
		calculate();
	}
	else{
		std::cerr << "Invalid Token!: " <<s << std::endl;
		exit (EXIT_FAILURE);
	}
}



int main(int argc, char* argv[]) {
	// setup the print out format for the precision required.
	std::cout.setf(std::ios::fixed,std::ios::floatfield);
	std::cout.precision(3);

	// open the file for reading
	std::ifstream in;
	in.open(argv[1]);

	// string to be read into
	std::string s;

	// read the file while we have input.
	while (in >> s) {
		tokenStack.push_back(s);
	}
	// process input tokenStack and deal with nested repeat
	while(!tokenStack.empty()){
		s = tokenStack.front();
		tokenStack.pop_front();

		// Store repeat token into a seprate stack
		if(inRepeat > 0)
			tokenRepStack.push_front(s);

		if(s=="repeat"){
			if(inRepeat == 0){
				repeatTimes = (int)operandStack.back();
				operandStack.pop_back();
				dtypeStack.pop_back();
			}
			
			inRepeat++;
		}
		else if(s=="endrepeat"){
			if(inRepeat > 0)
				inRepeat --;
			if(inRepeat==0){
			for(int i=0;i<repeatTimes;++i){
				for(int n=0; n<tokenRepStack.size();++n){
				tokenStack.push_front(tokenRepStack.at(n));
				}
			}
			tokenRepStack.clear();
			}
		}

		if(inRepeat==0)
			process(s);

	}
	in.close();
}
