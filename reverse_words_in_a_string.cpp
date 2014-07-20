class Solution {
public:
    void reverseWords(string &s) {
        if (s.empty())      //如果s为空，直接返回
	        return;
        int i = s.size() - 1;
        int j = s.size() - 1;
        string resultString;
        while (i >= 0) {
	        while (i >= 0 && s[i] == ' ')      //找到不是空格的最后一个字符
	        	--i;
	        if (i < 0)    //说明都是空格，跳出循环
	            break;
            j = i;
        	while (j >= 0 && s[j] != ' ')  //j从i开始向前找到第一个空格，那么(j + 1, i)是个单词
	        	--j;
        	resultString = resultString + s.substr(j + 1, i - j) + " ";
        	i = j; //i = j，再次循环去找下一个单词
        }
        if (!resultString.empty())  //如果不为空，就要删除最后一个空格
            resultString.pop_back();
        s = resultString;
    }
};