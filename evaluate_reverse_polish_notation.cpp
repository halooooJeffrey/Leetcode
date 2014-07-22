class Solution {
public:
	enum Operator{ ADD, SUB, MUL, DIV, L_P, R_P, EOE };
    
    //操作符优先级表
    const char priTable[7][7] = {
    	// +    -    *    /    (    )    \0
/* + */ { '>', '>', '<', '<', '<', '>', '>' },
/* - */ { '>', '>', '<', '<', '<', '>', '>' },
/* * */ { '>', '>', '>', '>', '<', '>', '>' },
/* / */ { '>', '>', '>', '>', '<', '>', '>' },
/* ( */ { '<', '<', '<', '<', '<', '=', '>' },
/* ) */ { ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
/* \0*/ { '<', '<', '<', '<', '<', '<', '=' }
    };
    
	int evalRPN(vector<string> &tokens) {
		stack<int> s;
		for (vector<string>::const_iterator it = tokens.begin(); it != tokens.end(); ++it) {
			//如果是数字
			if (isNumber(*it))
				s.push(stoi(*it)); //将数字压入栈中
			//如果是运算符
			else if (*it == "+" || *it == "-" || *it == "*" || *it == "/") {
				if (!s.empty()) {	//如果栈中有数字
					int opnd2 = s.top();	//获得表达式中的右边的操作数
					s.pop();	//弹出
					int opnd1 = s.top();	//获得左边的操作数
					s.pop();	//弹出
					s.push(calculate(opnd1, it->c_str(), opnd2));	//计算表达式，将结果再压入栈中
				}
			}
			//如果是表达式
			else {
				vector<string> anotherTokens;
				s.push(evalRPN(anotherTokens = convertToRPN(*it)));	//如果是表达式那么转化成逆波兰式，在调用evalRPN，所得结果压入栈中
			}
		}
		return s.top();	//返回最终结果
	}
    
    //判断是不是数字，注意还有负数
	int isNumber(string s) {
		if (s[0] == '-' && s.size() == 1)	//如果第一个是'-',但是大小为1，肯定不是负数而是减号
			return 0;
		else if (s[0] == '-' || isdigit(s[0])){		//如果第一个是'-'或者是数字，那么依次判断后面几个，只要一个不是数字就不是数字
			for (string::size_type sz = 1; sz != s.size(); ++sz) {
				if (!isdigit(s[sz]))
					return 0;
			}
			return 1;
		}
		else
			return 0;
	}
    
    //计算表达式
	int calculate(int opnd1, const char *c, int opnd2) {
		switch (*c) {
            case '+': return opnd1 + opnd2; break;
            case '-': return opnd1 - opnd2; break;
            case '*': return opnd1 * opnd2; break;
            case '/': return opnd1 / opnd2; break;
            default: exit(-1);
		}
	}
    
    //找到这个操作符在优先级表中的位置
	Operator findPosition(char op) {
		switch (op) {
            case '+':
                return ADD; break;
            case '-':
                return SUB; break;
            case '*':
                return MUL; break;
            case '/':
                return DIV; break;
            case '(':
                return L_P; break;
            case ')':
                return R_P; break;
            case '\0':
                return EOE; break;
            default:
                exit(-1);
		}
	}
    
    //比较两个操作符的优先级，返回>,<,=
	char comparePriority(char op1, char op2) {
		return priTable[findPosition(op1)][findPosition(op2)];
	}
    
    //将表达式转化成逆波兰式
	vector<string> convertToRPN(const string& s) {
		vector<string> retVec;	//要返回的vector，里面的元素构成逆波兰式
		stack<char> operatorStack;	//存放操作符的栈
		operatorStack.push('\0');	//首先将\0入栈，否则如果表达式第一个就是操作符，那么就无法与栈中的元素比较
		string::size_type sz = 0;
		while (sz != s.size()) {
			if (isdigit(s[sz])) {	//如果是数字，那么直接放到逆波兰式中
				retVec.push_back(string(&s[sz], 1));
				++sz; //看接下来的元素
			}
			else {	//如果是操作符，那么与栈中的操作符比较
				char op = comparePriority(operatorStack.top(), s[sz]);
				switch (op) {
                    case '<': {		//如果栈中的操作符大，那么元素入栈，看接下来的元素
                        operatorStack.push(s[sz]);
                        ++sz;
                        break;
                    }
                    case '=': {		//如果相等，这说明栈中是左括号，元素是右括号，它们都不用放入逆波兰式
                        operatorStack.pop();
                        ++sz;
                        break;
                    }
                    case '>': {		//如果栈中的操作符大，说明要先计算栈中的操作符，所以将它弹出放到逆波兰式中，注意，这里sz不增加，因为还要
                    				//比较当前元素与新栈顶的元素优先级
                        string op = string(&operatorStack.top(), 1);
                        retVec.push_back(op);
                        operatorStack.pop();
                        break;
                    }
                    default:
                        exit(-1);
				}
			}
		}	
		while (!operatorStack.empty()) {	//现在我们已经都将所有元素比较好了，如果栈中还有元素，就要依次的放入逆波兰式中
			retVec.push_back(string(&operatorStack.top(), 1));	
			operatorStack.pop();
		}
		retVec.erase(retVec.begin() + retVec.size() - 1);	//注意，由于开始放入了\0，最后要讲它从逆波兰式中去除
		return retVec;
	}
};
