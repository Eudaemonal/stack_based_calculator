/* Copyright (c) 2016-2017 Eudaemon <eudaemonal@gmail.com> */

#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <vector>
#include <deque>
#include <stack>
#include <cmath>



/*
 * Test: 
 * Boundary: div 0, sqrt [-Inf, 0]
 * Nested Repeat
 *
 */

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
// invalid: x. or .x
bool isdigit(std::string s){
    std::string::const_iterator it = s.begin();
    while (it != s.end() && (std::isdigit(*it)||(*it=='.'&& it !=s.end()-1 && it !=s.begin()))) ++it;
    return !s.empty() && it == s.end();
}

// if a string is a int
bool isint(std::string s){
    std::string::const_iterator it = s.begin();
    while (it != s.end() && (std::isdigit(*it))) ++it;
    return !s.empty() && it == s.end();

}


// basic display
void basicdisplay(char opt){
	bool a, b, c;
	double x, y, z;
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
	else if(opt=='/')
		z = x / y;

	if(!a)
		std::cout << (int)x;
	else
		std::cout << x;
	std::cout <<" "<<opt<<" ";

	if(!b)
		std::cout << (int)y;
	else
		std::cout << y;

	std::cout <<" = ";

	if(!c)
		std::cout << (int)z;
	else
		std::cout << z;
	std::cout << std::endl;
	
	operandStack.push_back(z);
	dtypeStack.push_back(c);

}


void calculate(){
	double x, z;
       	int i, n = 0;	// numbers of reverse
	bool a;
	std::deque<double> reverseQueue;
	std::deque<bool> reverseTypeQueue;

	std::string s = operatorStack.back();
	operatorStack.pop_back();

	/*
	std::cout<< "-------------OP Stack-----------" <<std::endl;
	std::cout<< "size: "<< operatorStack.size()<<std::endl;
	for(i=0;i<tokenStack.size();i++){
	std::cout<< " " << tokenStack.at(i);
	}
	std::cout << std::endl;
	std::cout<< "=============OP Stack===========" <<std::endl;
	*/
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

			z = sqrt(x) ;
			std::cout <<"sqrt " << x << " = "<< z <<std::endl;
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
		else if(s=="repeat"){
			repeatTimes = (int)operandStack.back();
			operandStack.pop_back();
			dtypeStack.pop_back();
			inRepeat++;
		}
		else if(s=="endrepeat"){
			if(inRepeat > 0)
				inRepeat --;
			if(inRepeat==0){
			for(i=0;i<repeatTimes-1;++i){
				for(n=tokenRepStack.size()-1;n>=0;n--){
				tokenStack.push_back(tokenRepStack.at(n));							       }
			}
			tokenRepStack.clear();
			}
		}

}



void process(std::string s){
	bool dtype = 0; // int: 0 ;double: 1


	if(isdigit(s)){
		if(isint(s))
			dtype = 0;
		else
			dtype = 1;

	/*
	std::cout<< "=============OP Stack===========" <<std::endl;	
	std::cout<< "size: "<< operandStack.size()<< "  size: "<< dtypeStack.size()<<std::endl;
	for(int i=0;i<operandStack.size();i++){
	std::cout<< " " << operandStack.at(i);
	}
	std::cout << std::endl;
	for(int i=0;i<dtypeStack.size();i++){
	std::cout<< " " << dtypeStack.at(i);
	}
	std::cout << std::endl;
	std::cout<< "-------------OP Stack-----------" <<std::endl;
	*/

		operandStack.push_back(std::stod(s));
		dtypeStack.push_back(dtype);


	}
	else if(isoperator(s)){
		operatorStack.push_back(s);
		calculate();

	}
	else{
		std::cout << "Invalid Token!:" <<s << std::endl;
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
		tokenStack.push_front(s);
		if(inRepeat > 0 && (s!="endrepeat" && inRepeat ==1)){
			tokenRepStack.push_front(s);
		}
		
		while(tokenStack.size()!=0){
		s = tokenStack.front();
		tokenStack.pop_front();

		process(s);

		}
	}
	in.close();
	
}

