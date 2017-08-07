/* Copyright (c) 2016-2017 Eudaemon <eudaemonal@gmail.com> */

#include <iostream>
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

std::deque<double> operandStack;
std::deque<bool> dtypeStack;
std::deque<std::string> operatorStack;

std::deque<std::string> tokenStack; // tokenStack for repeat
std::deque<std::string> tokenRepStack;


std::deque<double> operandRepStack;
std::deque<bool> dtypeRepStack;
std::deque<std::string> operatorRepStack;

bool inRepeat = false;
bool runRepeat = false;
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


void calculate(){
	double x, y, z;
       	int i, n = 0;	// numbers of reverse
	std::deque<double> reverseQueue;

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
			x = operandStack.back();
			operandStack.pop_back();
			y = operandStack.back();
			operandStack.pop_back();
			z = x + y;
			std::cout <<x << " + " << y << " = "<< z <<std::endl;
			operandStack.push_back(z);
		}
		else if(s=="sub"){
			x = operandStack.back();
			operandStack.pop_back();
			y = operandStack.back();
			operandStack.pop_back();
			z = x - y;
			std::cout <<x << " - " << y << " = "<< z <<std::endl;
			operandStack.push_back(z);
		}
		else if(s=="mult"){
			x = operandStack.back();
			operandStack.pop_back();
			y = operandStack.back();
			operandStack.pop_back();
			z = x * y;
			std::cout <<x << " * " << y << " = "<< z <<std::endl;
			operandStack.push_back(z);	
		}
		else if(s=="div"){
			x = operandStack.back();
			operandStack.pop_back();
			y = operandStack.back();
			operandStack.pop_back();
			z = x / y;
			std::cout <<x << " / " << y << " = "<< z <<std::endl;
			operandStack.push_back(z);	
		}
		else if(s=="sqrt"){
			x = operandStack.back();
			operandStack.pop_back();
			z = sqrt(x) ;
			std::cout <<"sqrt " << x << " = "<< z <<std::endl;
			operandStack.push_back(z);		
		}
		else if(s=="pop"){
			operandStack.pop_back();
		}
		else if(s=="reverse"){
			n = (int)operandStack.back();
			operandStack.pop_back();
			for(i=0;i<n;++i){
				x = operandStack.back();
				operandStack.pop_back();
				reverseQueue.push_front(x);
			}
			for(i=0;i<n;++i){
				x = reverseQueue.back();
				reverseQueue.pop_back();
				operandStack.push_back(x);
			}
		}

		
		
		if(s=="repeat"){
			// Handling nested repeat
			if(inRepeat == false){
			repeatTimes = (int)operandStack.back();
			operandStack.pop_back();

			//std::cout<< "Repeat Times: "<<repeatTimes <<std::endl;//test
			//std::cout<< "Next: "<<operandStack.back() <<std::endl;//test
			
			inRepeat = true;
			}
		}
		else if(s=="endrepeat"){
			inRepeat = false;

			for(i=0;i<repeatTimes-1;++i){
				for(n=tokenRepStack.size()-1;n>=0;n--){
				tokenStack.push_back(tokenRepStack.at(n));
				//std::cout<< "Push:" << tokenRepStack.at(n) <<std::endl; //test
				}
			}
			tokenRepStack.clear();

			/*
			x = operandStack.back();
			operandStack.pop_back();
			
			for(i=0;i<repeatTimes-1;++i){
				for(n=0;n<operandRepStack.size();++n){
				operandStack.push_back(operandRepStack.at(n));
				//std::cout<< "Push:" << operandRepStack.at(n) <<std::endl; //test
				}
				for(n=0;n<operatorRepStack.size();++n){
				operatorStack.push_back(operatorRepStack.at(n));
				std::cout<< "Push:" << operatorRepStack.at(n) <<std::endl; //test
				}
			}
			operandStack.push_back(x);
			
			operandRepStack.clear();
			dtypeRepStack.clear();
			operatorRepStack.clear();
			*/
			
		}
		
		

}



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
		if(inRepeat && s!="endrepeat"){
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

