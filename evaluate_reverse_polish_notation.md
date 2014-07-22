# Evaluate Reverse Polish Notation

Ecaluate the value of an arithmetic expression in **Reverse Polish Notation**.

Valid operators are `+`, `-`, `*`, `/`. Each operand may be an integer or another expression.

Some examples:

```
["2", "1", "+", "3", "*"] -> ((2 + 1) * 3) -> 9
["4", "13", "5", "/", "+"] -> (4 + (13 / 5)) -> 6
```

## 先来回答几个问题

## 什么是逆波兰式？

逆波兰式是数学表达式的一种，其语法规则可概括为：操作符紧邻于对应的（最后一个）操作数之后。

例如 `(a + b) * c - (a + b) / e` 的逆波兰式就是：`ab+c*ab+e/-`

## 为什么要使用逆波兰式？

因为在日常生活中，对于人类来说理解中序表达式是相当容易的，但是对于计算机来说很困难，因为表达式不是按顺序计算的，涉及到操作符的优先级，所以我们要讲表达式转化成计算机容易理解的，对于计算机结构来说容易实现的。而逆波兰式的计算都是按顺序进行的，如上面的第一个表达式，先取到1，2，接下来取到"+"，计算1+2=3，那么计算机获得了3，接着再取得3，最后取得"*"，计算3乘3得9。总之取得一个操作符，就相应的获得操作数进行计算，保存结果。

## 怎么把中序表达式转化成后序表达式呢？（逆波兰式）

如果碰到操作数，那么将它放到逆波兰式中，如果碰到操作符就将它与操作符栈的栈顶元素比较，如果栈顶元素小，说明它优先执行，放入栈顶，看看后一个操作数如何。如果栈顶元素大，说明栈顶元素要先执行，就把它放到逆波兰式中，接着将它与新的栈顶元素比较，方法同上。还有一个问题是括号的问题，准则是这样的，碰到左括号一律入栈，如果栈顶是左括号，那么新的元素一律入栈，如果碰到右括号，那么栈顶弹出操作符，直到碰到左括号，左括号和右括号一律舍弃。从中我们可以看到大小关系，右括号是最小的，因为碰到它，栈顶元素一律弹出，而左括号没入栈时是最大的，一入栈就变最小了。如果能制作一张二维的优先级表，那么实现起来就会很简单（见代码）。


--------

## 代码



```
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
		retVec.erase(retVec.begin() + retVec.size() - 1);	//注意，由于开始放入了\0，最后要将它从逆波兰式中去除
		return retVec;
	}
};

```

## 编代码时遇到的问题

1. 如果str是string类，那么str中的每一个元素都是char类型而不是string类型。
2. string类构造函数中的元素是char *类型的，所以无法将char转化成string类，可以用另一种转化，见代码



