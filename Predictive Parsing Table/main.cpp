//=====================================================================================================================
//
//Author information
//  Author name: Andy Garcia
//  Author email: andygarciapc@gmail.com
//
//Program information
//  Program name: Predictive Parsing Table
//  Date program began: 2022-Mar-07
//  Date of last update: 2019-Mar-07
//
//This file
//   File name: main.cpp
//   Language: C++
//
//===== Begin code area ================================================================================================
#include <iostream>
#include <string>
#include <string.h>
#include <vector>
#include <unordered_map>

class ParseTable {
public:
	ParseTable(int nCount, int lCount)
	{
		NODE_MAX = nCount;
		LETTER_MAX = lCount;
		table = (char***)malloc(sizeof(char**) * nCount);
		for (int i = 0; i < nCount; ++i)
		{
			table[i] = (char**)malloc(sizeof(char*) * lCount);
		}
	}

	void fillTable(char* fill)
	{
		for (int i = 0; i < NODE_MAX; ++i)
		{
			for (int j = 0; j < LETTER_MAX; ++j)
			{
				table[i][j] = fill;
			}
		}
	}

	void print()
	{
		for (int i = 0; i < NODE_MAX; ++i)
		{
			for (int j = 0; j < LETTER_MAX; ++j)
			{
				std::cout << table[i][j] << "|";
			}
			std::cout << std::endl;
		}
	}

	void addNode(char newNode)
	{
		nodes.insert({ newNode,nodeCount });
		++nodeCount;
	}

	void addNode(char* newNodes)
	{
	    firstNode = newNodes[0];
		for (int i = 0; i < strlen(newNodes); ++i)
		{
			nodes.insert({ newNodes[i], nodeCount });
			++nodeCount;
		}
	}

	void addLetter(char newLetter)
	{
		letters.insert({ newLetter,letterCount });
		++letterCount;
	}

	void addLetter(char* newLetters)
	{
		for (int i = 0; i < strlen(newLetters); ++i)
		{
			letters.insert({ newLetters[i], letterCount });
			++letterCount;
		}
	}
	
	void printNodes()
	{
	    for(auto& it : nodes)
	    {
	        std::cout << it.first << "/" << it.second << "|";
	    }
	    std::cout << std::endl;
	}
	
	void printLetters()
	{
	    for(auto& it : letters)
	    {
	        std::cout << it.first << "/" << it.second << "|";
	    }
	    std::cout << std::endl;
	}

	void set(char node, char letter, char* value)
	{
		int nodeIndex = nodes.at(node);
		int letterIndex = letters.at(letter);
		table[nodeIndex][letterIndex] = value;
		//std::cout << table[nodeIndex][letterIndex] << std::endl;
	}
	
	char* at(char node, char letter)
	{
	    int nodeIndex = nodes.at(node);
	    int letterIndex = letters.at(letter);
	    return table[nodeIndex][letterIndex];
	}
	
	char startNode()
	{
	    return firstNode;
	}

private:
	char*** table;
	std::unordered_map<char, int> nodes;
	std::unordered_map<char, int> letters;
	int nodeCount = 0, letterCount = 0;
	char firstNode;
	int NODE_MAX;
	int LETTER_MAX;
};

void PrintStack(const std::vector<char> vector)
    {
    	std::cout << "Stack: ";
    	for (auto& it : vector)
    	{
    		std::cout << it;
    	}
    	std::cout << std::endl;
    }

void Parse(ParseTable table,std::string string){
    std::vector<char> parseStack;
    char readChar, currentNode;
    int iterations = 0, readIndex = 0, nodeIndex = 0, letterIndex = 0;
    
    parseStack.push_back('$');
    parseStack.push_back(table.startNode());
    readChar = string[readIndex];
    ++readIndex;
    //std::cout << "Read: " << readChar << std::endl;
    //PrintStack(parseStack);
    
    while(parseStack.size() > 0)
    {
        currentNode = parseStack.back();
        parseStack.pop_back();
        //std::cout << iterations << ".Pop: " << currentNode << std::endl;
        if(currentNode == readChar && currentNode == '$')
        {
            std::cout << "Match found!\n";
            PrintStack(parseStack);
            goto finished;
        }
        if(currentNode == readChar)
        {
            std::cout << "Match found!\n";
            PrintStack(parseStack);
            currentNode = parseStack.back();
            parseStack.pop_back();
            readChar = string[readIndex];
            ++readIndex;
            //std::cout << "Read: " << readChar << std::endl;
        }
        
        //std::cout << "Go to " << "[" << currentNode << "]" << "[" << readChar << "]" << std::endl;
        char* temp = table.at(currentNode, readChar);
        
        if(temp == "0")
        {
            //std::cout << "[" << currentNode << "]" << "[" << readChar << "]" << " = " << "lambda" << std::endl;
            goto endwhile;
        }
        if(temp == "-")
        {
            goto fail;
        }
        for(int i = strlen(temp) - 1; i >= 0; --i)
        {
            parseStack.push_back(temp[i]);
        }
        
        endwhile:
		//PrintStack(parseStack);
		++iterations;
    }
    finished:
    std::cout << "String is valid." << std::endl;
    goto end;
    fail:
    std::cout << "String is not valid." << std::endl;
    
    end:
    return;
}

void InitializeTable1(ParseTable* table)
{
	table->addLetter((char*)"i+-*/()$");
	table->addNode((char*)"EQTRF");
	table->fillTable((char*)"-");
	table->set('E', 'i', (char*)"TQ");
	table->set('E', '(', (char*)"TQ");
	table->set('Q', '+', (char*)"+TQ");
	table->set('Q', '-', (char*)"-TQ");
	table->set('Q', ')', (char*)"0");
	table->set('Q', '$', (char*)"0");
	table->set('T', 'i', (char*)"FR");
	table->set('T', '(', (char*)"FR");
	table->set('R', '+', (char*)"0");
	table->set('R', '-', (char*)"0");
	table->set('R', '*', (char*)"*FR");
	table->set('R', '/', (char*)"/FR");
	table->set('R', ')', (char*)"0");
	table->set('R', '$', (char*)"0");
	table->set('F', 'i', (char*)"i");
	table->set('F', '(', (char*)"(E)");
}

void InitializeTable2(ParseTable* table)
{
    table->addLetter((char*)"a+-*/=()$");
    table->addNode((char*)"SWEQTRF");
    table->fillTable((char*)"-");
    table->set('S', 'a', (char*)"aW");
    table->set('W', '=', (char*)"=E");
    table->set('E', 'a', (char*)"TQ");
    table->set('E', '(', (char*)"TQ");
    table->set('Q', '+', (char*)"+TQ");
    table->set('Q', '-', (char*)"-TQ");
    table->set('Q', ')', (char*)"0");
    table->set('Q', '$', (char*)"0");
    table->set('T', 'a', (char*)"FR");
    table->set('T', '(', (char*)"FR");
    table->set('R', '+', (char*)"0");
    table->set('R', '-', (char*)"0");
    table->set('R', '*', (char*)"*FR");
    table->set('R', '/', (char*)"/FR");
    table->set('R', ')', (char*)"0");
    table->set('R', '$', (char*)"0");
    table->set('F', 'a', (char*)"a");
    table->set('F', '(', (char*)"(E)");
}

int main()
{
    
	ParseTable table1(5, 8);
	InitializeTable1(&table1);
	table1.print();

	std::string parseThis;
	char repeat = 'y';
	
	while(repeat != 'n')
	{
	std::cout << "Please input string or text file to parse." << std::endl;
	getline(std::cin, parseThis);
	Parse(table1, parseThis);
	std::cout << "Would you like to try another string for this language set?(y/n):";
    std::cin >> repeat;
    std::cin.ignore();
	}
	repeat = 'y';
	
	ParseTable table2(7,9);
	InitializeTable2(&table2);
	table2.print();
	while(repeat != 'n')
	{
	    std::cout << "Please input string or text file to parse." << std::endl;
    	getline(std::cin, parseThis);
    	Parse(table2, parseThis);
    	std::cout << "Would you like to try another string?(y/n):";
        std::cin >> repeat;
        std::cin.ignore();
	}
	

	return 0;
}