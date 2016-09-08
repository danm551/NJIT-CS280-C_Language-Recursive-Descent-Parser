//============================================================================
// CS 280-102, SPRING 2015
// Ernesto Martinez
// Program #4
//============================================================================

#include <iostream>
#include <fstream>
#include "p2lex.h"
#include <typeinfo>
#include <vector>
#include <map>;
#include <string>;
#include <stdlib.h>
using namespace std;

map<string, string> setStr;
map<string, int> setInt;
int Str2Int(string a){
	char b[100];
	string newstring;

	if(!isdigit(a[0])){
		newstring = a;
		newstring.erase(0,1);

		for(int i = 0; i < newstring.size(); i++){
			if(isdigit(newstring[i]))
			{
				b[i] = newstring[i];
			}
		}
	}
	else{
		for(int i = 0; i < a.size(); i++){
			if(isdigit(a[i])){
				b[i] = a[i];
			}
		}
	}
	int c = atoi(b);

	if(!isdigit(a[0]))
		return c = c*(-1);

	return c;
}
class PTree{
public:
	PTree *left;
	PTree *right;
	Token data;
	PTree(){
		left = NULL;
		right = NULL;
	}
	PTree(PTree *left){
		this->left = left;
		right = NULL;
	}
	PTree(PTree *left,PTree *right){
		this->left = left;
		this->right = right;
	}
	void Insert(PTree *insert){
		if(left == NULL)
			left = insert;
		else
			right = insert;
	}
	void SetData(Token &token){
		data = token;
	}
	void GetData(){
		cout << "Data: " << data << endl;
	}
	int traverse(){
		PTree *tree = new PTree;
		int cnt = 0;

		if( left == 0 && right == 0 ) return 1;

		if( left ){
			string c = left->GetType();

			if(c.compare("Set\n") == 0){
				tree = left;
				SetMap(tree);
			}
			else if(c.compare("Print\n") == 0){
				if(left->left)
					c = left->left->GetType();
				if(c.compare("String\n") == 0 && left->right == 0){
					string a = left->left->GetStr();
					a.insert(0, "\"");
					a.insert(a.size(), "\"");
					cout << a << endl;
				}
				else if(c.compare("ID\n") == 0 && left->right == 0){
					if(setStr.count(left->left->GetID())){
						string a = setStr[left->left->GetID()];
						a.insert(0, "\"");
						a.insert(a.size(), "\"");
						cout << a << endl;
					}
				}
				else if(c.compare("Plus\n") == 0){
					tree = left->left;
					int sumint;
					string sumstr;
					string a, b, l, r;

					l = tree->left->GetType();
					r = tree->right->GetType();

					if(l.compare("ID\n") == 0){
						a = tree->left->GetID();

						if(setInt[a]){
							sumint = SumINT(tree);
						}
						else
							sumstr = SumSTR(tree);
					}
					else if(r.compare("ID\n") == 0){
						b = tree->right->GetID();
						if(setInt[b])
						{
							sumint = SumINT(tree);
						}
						else
							sumstr = SumSTR(tree);
					}
					else if(l.compare("Integer\n") == 0 || r.compare("Integer\n") == 0)
						sumint = SumINT(tree);
					else if(l.compare("String\n") == 0 || r.compare("String\n") == 0)
						sumstr = SumSTR(tree);

					if(!sumstr.empty()){
						sumstr.insert(0, "\"");
						sumstr.insert(sumstr.size(), "\"");
						cout << sumstr << endl;
					}
					if(sumint != 0)
						cout << sumint << endl;
				}
				else if(c.compare("Minus\n") == 0){
					tree = left->left;
					string diffStr, a, b, l, r;
					int diffint;

					l = tree->left->GetType();

					if(l.compare("Integer\n") == 0)
					{
						diffint = Difference(tree);
						cout << diffint << endl;
					}
					else
					{
						diffStr = DifferenceSTR(tree);
						diffStr.insert(0, "\"");
						diffStr.insert(diffStr.size(), "\"");
						cout << diffStr << endl;
					}
				}
				else if(c.compare("Star\n") == 0){
					tree = left->left;
					int prodint;
					string prodstr;
					string a, b, l, r;

					l = tree->left->GetType();
					r = tree->right->GetType();

					if(l.compare("ID\n") == 0){
						a = tree->left->GetID();

						if(setInt[a]){
							prodint = Multiply(tree);
						}
						else
							prodstr = MultiplySTR(tree);

						prodstr.insert(0, "\"");
						prodstr.insert(prodstr.size(), "\"");
						cout << prodstr << endl;
					}
					else if(l.compare("Integer\n") == 0){
						prodint = Multiply(tree);
						cout << prodint << endl;
					}
				}
				else if(c.compare("Slash\n") == 0){
					tree = left->left;
					int qint;
					string qstr;
					string a, b, l, r;

					l = tree->left->GetType();
					r = tree->right->GetType();

					if(l.compare("ID\n") == 0 || r.compare("ID\n") == 0){
						qstr = DivideSTR(tree);
						qstr.insert(0, "\"");
						qstr.insert(qstr.size(), "\"");
						cout << qstr << endl;
					}
					else if(l.compare("String\n") == 0){
						qstr = DivideSTR(tree);
						qstr.insert(0, "\"");
						qstr.insert(qstr.size(), "\"");
						cout << qstr << endl;
					}
					else if(l.compare("Integer\n") == 0){
						qint = Divide(tree);
						cout << qint << endl;
					}
				}
				else{
					tree = left->left;
					string diffStr;
					diffStr = DifferenceSTR(tree);
					cout << diffStr << endl;
				}
			}
			left->traverse();
		}
		else
			cout << "not a valid statement";
		if( right ){
			string c = right->GetType();
			if(c.compare("Statement List\n")){
				if(right->left);
			}
			right->traverse();
		}

		return cnt;
	}
	string SumSTR(PTree *plus){
		string a;
		PTree *currRight = new PTree;
		string currType, leftType, rightType, leftID, rightID, l, r;
		currType = plus->GetType();
		currRight = plus->right;

		leftType = plus->left->GetType();
		rightType = plus->right->GetType();

		if(leftType.compare("ID\n") == 0)
			l =  plus->left->GetID();
		else if(leftType.compare("Star\n") == 0){
			string a;
			if(rightType.compare("ID\n") == 0){
				a = plus->right->GetID();
				//return Multiply(plus->left) + setStr[a];
			}
		}
		else if(leftType.compare("String\n") == 0){
			string a, b;
			a = plus->left->GetStr();
			if(rightType.compare("ID\n") == 0){
				b = plus->right->GetID();
				return a + setStr[b];
			}
		}

		if(rightType.compare("ID\n") == 0){
			r = plus->right->GetID();

			if(currType.compare("Plus\n") == 0){
				if(setInt.count(l) && setInt.count(r));
					return setStr[l] + setStr[r];
			}
			else if(currType.compare("Star\n") == 0){
				if(setInt.count(l) && setInt.count(r));
			}
		}
		else if(currType.compare("Plus\n") == 0){
			if(setStr.count(l));
				return setStr[l] + SumSTR(currRight);
		}
		else{
			return a;
		}
	}
	int SumINT(PTree *plus){
		PTree *currRight = new PTree;
		int leftint, rightint;
		string currType, leftType, rightType, leftID, rightID, l, r;
		currType = plus->GetType();
		currRight = plus->right;

		leftType = plus->left->GetType();
		rightType = plus->right->GetType();

		//If adding IDs
		if(leftType.compare("ID\n") == 0)
			l =  plus->left->GetID();
		else if(leftType.compare("Integer\n") == 0){
			l = plus->left->GetInt();
			leftint = Str2Int(l);
		}
		else if(leftType.compare("Star\n") == 0){
			string a;
			int integer, integer2;
			if(rightType.compare("ID\n") == 0){
				a = plus->right->GetID();
				return Multiply(plus->left) + setInt[a];
			}
			else if(rightType.compare("Integer\n") == 0){
				a = plus->right->GetInt();
				integer = Str2Int(a);
				return Multiply(plus->left) + integer;
			}
		}

		if(rightType.compare("ID\n") == 0){
			r = plus->right->GetID();

			if(currType.compare("Plus\n") == 0){
				if(setInt.count(l) && setInt.count(r))
					return setInt[l] + setInt[r];
			}
			else if(currType.compare("Star\n") == 0){
				if(setInt.count(l) && setInt.count(r))
					return setInt[l] * setInt[r];
			}
		}
		if(rightType.compare("Integer\n") == 0){
			r = plus->right->GetInt();
			rightint = Str2Int(r);

			return leftint + rightint;
		}
		else if(currType.compare("Plus\n") == 0){
			if(setInt.count(l))
				return setInt[l] + SumINT(currRight);
			else if(leftint)
				return leftint + SumINT(currRight);
		}
		else if(currType.compare("Star\n") == 0){
			if(setInt.count(l))
				return setInt[l] * SumINT(currRight);
		}
		else{
			return 0;
		}

		return 0;
	}
	string DifferenceSTR(PTree *minus){
		string a;
		PTree *currRight = new PTree;
		string currType, leftType, rightType, leftID, rightID, l, r;
		currType = minus->GetType();
		currRight = minus->right;

		leftType = minus->left->GetType();
		rightType = minus->right->GetType();

		//If subtracting IDs
		if(leftType.compare("ID\n"))
			l = minus->left->GetID();

		if(!currRight){
			return setStr[l];
		}
		else if(rightType.compare("ID\n") == 0){
			r = minus->right->GetID();

			if(currType.compare("Minus\n") == 0){
				int bsize, asize;
				string substring;

				leftID = minus->left->GetID();
				rightID = minus->right->GetID();

				string b = setStr[rightID];
				string a = setStr[leftID];

				bsize = b.size();
				asize = a.size();

				int loc = a.find(b);

				if(loc >= 0){
					a.erase(loc, bsize);
					return a;
				}
				else
					return a;
			}
			else if(currType.compare("Plus\n") == 0){
				if(setStr.count(l) && setStr.count(r))
					return setStr[l] + setStr[r];
			}
		}
		else if(rightType.compare("Minus\n") == 0){
			PTree *diff = new PTree;
			string test;
			test = DifferenceSTR(currRight);

			int bsize, asize;
			string substring;

			leftID = minus->left->GetID();;

			string a = setStr[leftID];

			bsize = test.size();
			asize = a.size();

			int loc = a.find(test);
			if(loc >= 0){
				a.erase(loc, bsize);
				return a;
			}
			else
				return a;
		}
		//If subtracting strings
		else if(leftType.compare("String\n") == 0 && rightType.compare("String\n") == 0){
			int bsize, asize;
			string substring;

			string b = minus->right->GetStr();
			string a = minus->left->GetStr();

			bsize = b.size();
			asize = a.size();

			int loc = a.find(b);
			if(loc >= 0){
				a.erase(loc, loc);
				cout << a << endl;
			}
			else
				cout << a << endl;
		}
		else
			cout << "Cannot add integer to string.\n";
		return a;
	}
	int Difference(PTree *minus){
		PTree *currRight = new PTree;
		int leftint, rightint;
		string currType, leftType, rightType, leftID, rightID, l, r;
		currType = minus->GetType();
		currRight = minus->right;

		leftType = minus->left->GetType();
		rightType = minus->right->GetType();

		//If adding IDs
		if(leftType.compare("ID\n") == 0)
			l =  minus->left->GetID();
		else if(leftType.compare("Integer\n") == 0){
			l = minus->left->GetInt();
			leftint = Str2Int(l);
		}
		else if(leftType.compare("Star\n") == 0){
			string a;
			int integer, integer2;
			if(rightType.compare("ID\n") == 0){
				a = minus->right->GetID();
				return Multiply(minus->left) - setInt[a];
			}
			else if(rightType.compare("Integer\n") == 0){
				a = minus->right->GetInt();
				integer = Str2Int(a);
				return Multiply(minus->left) - integer;
			}
		}

		if(rightType.compare("ID\n") == 0){
			r = minus->right->GetID();

			if(currType.compare("Plus\n") == 0){
				if(setInt.count(l) && setInt.count(r))
					return setInt[l] - setInt[r];
			}
			else if(currType.compare("Star\n") == 0){
				if(setInt.count(l) && setInt.count(r))
					return setInt[l] * setInt[r];
			}
		}
		if(rightType.compare("Integer\n") == 0){
			r = minus->right->GetInt();
			rightint = Str2Int(r);

			return leftint - rightint;
		}
		else if(currType.compare("Plus\n") == 0){
			if(setInt.count(l))
				return setInt[l] + SumINT(currRight);
			else if(leftint)
				return leftint + SumINT(currRight);
		}
		else if(currType.compare("Minus\n") == 0){
			if(setInt.count(l))
				return setInt[l] - SumINT(currRight);
			else if(leftint)
				return leftint - SumINT(currRight);
		}
		else if(currType.compare("Star\n") == 0){
			if(setInt.count(l))
				return setInt[l] * SumINT(currRight);
		}
		else{
			return 0;
		}

		return 0;
	}
	string MultiplySTR(PTree *multi)
	{
		PTree *currRight = new PTree;
		string currType, leftType, rightType, leftID, rightID, l, r;
		currType = multi->GetType();
		currRight = multi->right;

		leftType = multi->left->GetType();
		rightType = multi->right->GetType();

		//If multiplying IDs
		if(leftType.compare("ID\n") == 0)
			l =  multi->left->GetID();

		if(rightType.compare("Integer\n") == 0){
			string convert = currRight->GetInt();
			int size = Str2Int(convert);
			string original = setStr[l];
			string newstring;

			if(setStr.count(l)){
				for(int i = 0; i < size; i++){
					newstring.append(original);
				}
				return newstring;
			}
		}

		return 0;
	}
	int Multiply(PTree *multi)
	{
		PTree *currRight = new PTree;
		int leftint, rightint;
		string currType, leftType, rightType, leftID, rightID, l, r;
		currType = multi->GetType();
		currRight = multi->right;

		leftType = multi->left->GetType();
		rightType = multi->right->GetType();

		//If multiplying IDs
		if(leftType.compare("ID\n") == 0)
			l =  multi->left->GetID();
		else if(leftType.compare("Integer\n") == 0){
			l = multi->left->GetInt();
			leftint = Str2Int(l);
		}

		if(rightType.compare("ID\n") == 0){
			r = multi->right->GetID();

			if(setInt.count(l) && setInt.count(r))
					return setInt[l] * setInt[r];
		}
		else if(rightType.compare("Slash\n") == 0){
			return Divide(multi->right) * leftint;
		}
		else if(rightType.compare("Integer\n") == 0){
			r = multi->right->GetInt();
			rightint = Str2Int(r);
			return leftint * rightint;
		}
		else if(currType.compare("Star\n") == 0){
			if(setInt.count(l))
				return setInt[l] * Multiply(currRight);
			else if(leftint)
				return leftint * Multiply(currRight);
		}

		return 0;
	}
	string DivideSTR(PTree *div){
		PTree *currRight = new PTree;
		string currType, leftType, rightType, leftID, rightID, l, r, a;
		currType = div->GetType();
		currRight = div->right;

		leftType = div->left->GetType();
		rightType = div->right->GetType();

		//If multiplying IDs
		if(leftType.compare("ID\n") == 0 && rightType.compare("Integer\n") == 0){
			l =  div->left->GetID();
			string newstring;

			if(setStr.count(l)){
				string convert = div->right->GetInt();
				int divisor = Str2Int(convert);
				string original = setStr[div->left->GetID()];
				string substring;
				if(divisor == 0){
					return newstring;
				}
				else{
					substring = original.substr(0, divisor);
					return substring;
				}
			}
		}
		else if(leftType.compare("Integer\n") == 0 && rightType.compare("ID\n") == 0){
			r =  div->right->GetID();
			string newstring;

			if(setStr.count(r)){
				string convert = div->left->GetInt();
				int divisor = Str2Int(convert);
				string original = setStr[currRight->GetID()];
				string substring;
				if(divisor == 0){
					return newstring;
				}
				else{
					substring = original.substr(original.size()-divisor, original.size());
					return substring;
				}
			}
		}
		else if(leftType.compare("Integer\n") == 0 && rightType.compare("String\n") == 0){
			string newstring;

			string convert = div->left->GetInt();
			int divisor = Str2Int(convert);
			string original = div->right->GetStr();;
			string substring;
			if(divisor == 0){
				return newstring;
			}
			else{
				substring = original.substr(original.size()-divisor, original.size());
				return substring;
			}
		}
		if(leftType.compare("String\n") == 0 && rightType.compare("Integer\n") == 0){
			string newstring;
			string convert = div->right->GetInt();
			int divisor = Str2Int(convert);
			string original = div->left->GetStr();
			string substring;
			if(divisor == 0){
				return newstring;
			}
			else if(divisor < 0)
				cout << "Expected an expression\n";
			else{
				substring = original.substr(0, divisor);
				return substring;
			}
		}

		return a;
	}
	int Divide(PTree *div){
		PTree *currRight = new PTree;
		int leftint, rightint;
		string currType, leftType, rightType, leftID, rightID, l, r;
		currType = div->GetType();
		currRight = div->right;

		leftType = div->left->GetType();
		rightType = div->right->GetType();

		//If multiplying IDs
		if(leftType.compare("ID\n") == 0)
			l =  div->left->GetID();
		else if(leftType.compare("Integer\n") == 0){
			l = div->left->GetInt();
			leftint = Str2Int(l);
		}

		if(rightType.compare("ID\n") == 0){
			r = div->right->GetID();

			if(setInt.count(l) && setInt.count(r))
					return setInt[l] / setInt[r];
		}
		else if(rightType.compare("Slash\n") == 0){
			return leftint/ Multiply(div->right);
		}
		else if(rightType.compare("Integer\n") == 0){
			r = div->right->GetInt();
			rightint = Str2Int(r);
			if(leftint == 0 || rightint == 0)
				return 0;
			else if(leftint)
				return leftint / rightint;
		}

		return 0;
	}
	void SetMap(PTree *tree){
		string l, r, a, b;

		l = tree->left->GetType();

		if(tree->left->left)
			a = tree->left->left->GetType();
		if(tree->left->right)
			b = tree->left->right->GetType();

		if(l.compare("Plus\n") == 0){
			if(a.compare("Integer\n") == 0 && b.compare("String\n") == 0 || a.compare("String\n") == 0 && b.compare("Integer\n") == 0){
				cout << "Missing primary\n";
			}
		}
		else if(l.compare("Star\n") == 0){
			if(a.compare("String\n") == 0 && b.compare("String\n") == 0 ){
				cout << "Expected an expression\n";
			}
		}
		else if(l.compare("Integer\n") == 0){
			int a = Str2Int(tree->left->GetInt());
			setInt[tree->GetId()] = a;
		}
		else if(l.compare("String\n") == 0){
			setStr[tree->GetId()] = tree->left->GetStr();
		}
		else if(l.compare("Minus\n") == 0){
			string a = DifferenceSTR(tree->left);
			l = tree->left->left->GetType();

			if(l.compare("ID\n") == 0){
				setStr[tree->GetId()] = a;
				setStr[tree->GetId()];
			}
		}
		else if(l.compare("Slash\n") == 0){
			string id = tree->GetId();
			string thestring = DivideSTR(tree->left);
			setStr[id] = thestring;
		}
	}
	virtual void SetId(string a) {}
	virtual void SetValue(string a) {}
	virtual void SetValue(int b) {}
	virtual string GetId() {}
	virtual string GetInt(){}
	virtual string GetStr(){}
	virtual string GetID(){}
	virtual void GetValue() {}
	virtual string GetType() {
	}
};
class PTreeStmtList : public PTree{
	string type;
public:
	PTreeStmtList() : PTree() {}
	PTreeStmtList(PTree *s1, PTree *s2) : PTree(s1, s2) {}
	string GetType(){
		string sl = "Statement List\n";
		return sl;
	}
};

class PTreeStmt : public PTree{
public:
	PTreeStmt() : PTree(){}
	PTreeStmt(PTree *expr) : PTree(expr){}
	string GetType(){
		string sl = "Statement\n";
		return sl;
	}
};
class PTreeExpr : public PTree{
public:
	PTreeExpr() : PTree(){}
	PTreeExpr(PTree *t1, PTree *t2) : PTree(t1, t2) {}
	string GetType(){
		string sl = "Expression\n";
		return sl;
	}
};
class PTreeTerm : public PTree{
public:
	PTreeTerm() : PTree(){}
	PTreeTerm(PTree *t1, PTree *t2) : PTree(t1, t2) {}
	string GetType(){
		string sl = "Term\n";
		return sl;
	}
};
class PTreePrimary : public PTree{
public:

	string qstring;
	int num;

	PTreePrimary() : PTree(){}
	void SetValue(string a){
		qstring = a;
	}
	void SetValue(int b){
		num = b;
	}
	void GetValue(){
		if(qstring.empty())
			cout << num;
		else
			cout << qstring;
	}
	string GetType(){
		string sl = "Primary\n";
		return sl;
	}
};
class PTreeSet : public PTree{
public:
	string id;
	PTreeSet() : PTree(){}
	void SetId(string a){
		id = a;
	}
	string GetId(){
		return id;
	}
	string GetType(){
		string sl = "Set\n";
		return sl;
	}
};
class PTreePrint : public PTree{
public:
	string id;
	PTreePrint() : PTree(){}
	string GetType(){
		string sl = "Print\n";
		return sl;
	}
};
class PTreeInt : public PTree{
public:
	string i;
	PTreeInt() : PTree(){}
	PTreeInt(string a){
		i = a;
	}
	string GetInt(){
		return i;
	}
	string GetType(){
		string sl = "Integer\n";
		return sl;
	}
};
class PTreeStr : public PTree{
public:
	string i;
	PTreeStr() : PTree(){}
	PTreeStr(string a){
		i = a;
	}
	string GetStr(){
		return i;
	}
	string GetType(){
		string sl = "String\n";
		return sl;
	}
};
class PTreeID : public PTree{
public:
	string i;
	PTreeID() : PTree(){}
	PTreeID(string a){
		i = a;
	}
	string GetID(){
		return i;
	}
	string GetType(){
		string sl = "ID\n";
		return sl;
	}
};
class PTreeStar : public PTree{
public:
	PTreeStar() : PTree(){}
	PTreeStar(PTree *s1, PTree *s2) : PTree(s1, s2) {}
	string GetType(){
		string sl = "Star\n";
		return sl;
	}
};
class PTreeSlash : public PTree{
public:
	PTreeSlash() : PTree(){}
	PTreeSlash(PTree *s1, PTree *s2) : PTree(s1, s2) {}
	string GetType(){
		string sl = "Slash\n";
		return sl;
	}
};
class PTreeMinus : public PTree{
public:
	PTreeMinus() : PTree(){}
	PTreeMinus(PTree *s1, PTree *s2) : PTree(s1, s2) {}
	string GetType(){
		string sl = "Minus\n";
		return sl;
	}
};
class PTreePlus : public PTree{
public:
	PTreePlus() : PTree(){}
	PTreePlus(PTree *s1, PTree *s2) : PTree(s1, s2) {}
	string GetType(){
		string sl = "Plus\n";
		return sl;
	}
};

PTree *Program(istream *br, Token &myToken, string &lexeme);
PTree *StmtList(istream *br, Token &myToken, string &lexeme);
PTree *Stmt(istream *br, Token &myToken, string &lexeme);
PTree *Expr(istream *br, Token &myToken, string &lexeme);
PTree *Term(istream *br, Token &myToken, string &lexeme);
PTree *Primary(istream *br, Token &myToken, string &lexeme);
Token id(ID), integer(INT), quotedString(STRING), plusSign(PLUS), minusSign(MINUS), star(STAR), slash(SLASH), setting(SET), print(PRINT), sc(SC), done(DONE);
PTree *programTree = new PTree;
PTree *program = new PTree;
int counter = 0;
int status = 3;
vector<string> setIds;
vector<string> printIds;
vector<string> idErrors;

//Program ::= StmtList
PTree *Program(istream *br, Token &myToken, string &lexeme){
	PTree *stmtlist = new PTree;
	stmtlist = StmtList(br, myToken, lexeme);

	return stmtlist;
}

//StmtList ::= Stmt | Stmt StmtList
PTree *StmtList(istream *br, Token &myToken, string &lexeme){
	PTree *stmt = new PTree;
	stmt = 0;

	stmt = Stmt(br, myToken, lexeme);

	if(stmt){
		counter++;
		return new PTreeStmtList(stmt, StmtList(br, myToken, lexeme));
	}

	return 0;
}

//Stmt ::= PRINT Expr SC | SET ID Expr SC
PTree *Stmt(istream *br, Token &myToken, string &lexeme){
	PTree *set = new PTreeSet;
	PTree *testset = new PTreeSet;
	PTree *printnode = new PTreePrint;
	PTree *testprint = new PTreePrint;
	testprint = 0;
	testset = 0;

	if(myToken == print && myToken){
		status = 1;
		lexeme.clear();
		myToken = getToken(br, lexeme);//gets new token
		if(myToken == id){
			printnode->SetData(myToken);
		}
		if(myToken != 9){
			testprint = Expr(br, myToken, lexeme);
			if(testprint == 0){
				printnode = 0;
			}
			else{
				printnode->Insert(testprint);
			}

			if(printnode){}
			else{
				cout << "Expected an expression\n";
				return 0;
			}
		}
		if(myToken == sc){

			lexeme.clear();
			myToken = getToken(br, lexeme);//gets new token
			return printnode;
		}
		else
			cout << "Missing semicolon\n";
	}
	else if(myToken == setting){
		status = 0;
		lexeme.clear();
		myToken = getToken(br, lexeme);//gets new token
		if(myToken != id){
			cout << "Missing ID\n";
			return 0;
		}
		else if(myToken == id){
			setIds.push_back(lexeme);
			set->SetId(lexeme);
			lexeme.clear();//clears lexeme
			myToken = getToken(br, lexeme);//gets new token
			testset = Expr(br, myToken, lexeme);//method call to check for Expr

		}
		if(testset == 0){
			set = 0;
		}
		else{
			set->Insert(testset);
		}

		if(set){}
		else{
			cout << "Expected an expression\n";
			return 0;
		}

		if(myToken == sc){
			lexeme.clear();
			myToken = getToken(br, lexeme);//gets new token
			return set;
		}
		else
			cout << "Missing semicolon\n";
	}
	else if(myToken != 10){
		cout << "not a valid statement\n";
	}

	return 0;
}

//Expr ::= Expr PLUS Term | Expr MINUS Term | Term
PTree *Expr(istream *br, Token &myToken, string &lexeme){
	PTree *term = new PTree;
	term = 0;

	term = Term(br, myToken, lexeme);//calls for term

	if(term){
		if(myToken == minusSign){
			myToken = getToken(br, lexeme);//gets new token
			return new PTreeMinus(term, Expr(br, myToken, lexeme));
		}
		else if(myToken == plusSign){
			myToken = getToken(br, lexeme);//gets new token
			return new PTreePlus(term, Expr(br, myToken, lexeme));
		}
		else
			return term;
	}
	else
		return 0;

	return 0;
}

//Term ::= Term STAR Primary | Term SLASH Primary | Primary
PTree *Term(istream *br, Token &myToken, string &lexeme){
	PTree *primary = new PTree;
	primary = 0;

	primary = Primary(br, myToken, lexeme);

	if(primary){
		if(myToken == slash){
			myToken = getToken(br, lexeme);//gets new token
			return new PTreeSlash(primary, Term(br, myToken, lexeme));
		}
		else if(myToken == star){
			myToken = getToken(br, lexeme);//gets new token
			return new PTreeStar(primary, Term(br, myToken, lexeme));
		}
		else
			return primary;
	}
	else{
		cout << "Missing primary\n";
		return 0;
	}
	return 0;
}

//Primary ::= ID | INT | STRING
PTree *Primary(istream *br, Token &myToken, string &lexeme){
	PTreePrimary *primary = new PTreePrimary;

	if(myToken == id){
		//cout << lexeme << endl;
		PTreeID *idnode = new PTreeID(lexeme);
		if(status == 1){
			printIds.push_back(lexeme);
		}
		lexeme.clear();
		myToken = getToken(br, lexeme);//gets new token
		return idnode;
	}
	else if(myToken == quotedString){
		PTreeStr *strnode = new PTreeStr(lexeme);
		lexeme.clear();
		myToken = getToken(br, lexeme);//gets new token
		return strnode;
	}
	else if(myToken == integer){
		PTreeInt *intnode = new PTreeInt(lexeme);
		lexeme.clear();
		myToken = getToken(br, lexeme);//gets new token
		return intnode;
	}
	else{}

	return 0;
}

int main(int argc, char*argv[]) {

	istream *br;
	ifstream infile;
	 // check args and open the file
	if( argc == 1 )
		br = &cin;
	else if( argc != 2 ){
		return 1; // print an error msg
	}
	else{
		infile.open(argv[1]);
		if( infile.is_open() )
			br = &infile;
		else{
			cout << argv[1] << " can't be opened\n" << endl;
			return 1;
		}
	}
	string lexeme;
	Token myToken;

	myToken = getToken(br, lexeme);

	if(myToken != done)
		program->Insert(Program(br, myToken, lexeme));

	for(int i = 0; i < printIds.size(); i++){
		int error = 0;
		for(int j = 0; j < setIds.size(); j++){
			if(printIds.at(i) == setIds.at(j)){
				error++;
			}
		}
		if(error == 0)
			idErrors.push_back(printIds.at(i));
	}

	if(!printIds.empty()){
		for(int i = 0; i < idErrors.size(); i++){
			cout << "Identifier " << idErrors.at(i) << " is used without being set\n";
		}
	}

	int test;

	test = program->traverse();

	return 0;
}
