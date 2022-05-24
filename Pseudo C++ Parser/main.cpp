//=====================================================================================================================
//
//Author information
//  Author name: Andy Garcia
//  Author email: andygarciapc@gmail.com
//
//Program information
//  Program name: Final Program - Predictive Parser
//  Date program began: 2022-Apr-26
//  Date of last update: 2019-Apr-26
//
//This file
//   File name: main.cpp
//   Language: C++
//
//===== Begin code area ================================================================================================
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <iomanip>
#include <string>
#include <string.h>
#include <vector>
#include <unordered_map>


class ParseTable {
public:
    ParseTable(int nCount, int lCount)
    {
        nodeCount = 0;
        letterCount = 0;
        NODE_MAX = nCount;
        LETTER_MAX = lCount;
        table = (char***)malloc(sizeof(char**) * nCount);
        for (int i = 0; i < nCount; ++i)
        {
            table[i] = (char**)malloc(sizeof(char*) * lCount);
        }
    }

    void setFail(char* fill)
    {
        std::cout << "Fail: " << "\"" << fill << "\"" << std::endl;
        failChar = fill;
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
            //std::cout << std::setw(32);
            for (int j = 0; j < LETTER_MAX; ++j)
            {
                std::cout /*<< std::setw(4)*/ << "|" << table[i][j] << "|";
            }
            std::cout << std::endl;
        }
    }

    void addNode(std::string newNode)
    {
        nodes.insert({ newNode,nodeCount });
        ++nodeCount;
    }

    void addNodes(std::string newNode)
    {
        std::string temp = "";
        for (auto& it : newNode)
        {
            if (it == '|')
            {
                nodes.insert({ temp, nodeCount });
                ++nodeCount;
                temp = "";
            }
            else
            {
                temp = temp + it;
            }
        }
        nodes.insert({ temp, nodeCount });
        ++nodeCount;
    }

    void addLetter(std::string newLetter)
    {
        letters.insert({ newLetter,letterCount });
        ++letterCount;
    }

    void addLetters(std::string newLetter)
    {
        std::string temp = "";
        for (auto& it : newLetter)
        {
            if (it == '|')
            {
                letters.insert({ temp,letterCount });
                ++letterCount;
                temp = "";
            }
            else
            {
                temp = temp + it;
            }
        }
        letters.insert({ temp,letterCount });
        ++letterCount;
    }

    void printNodes()
    {
        std::cout << "Nodes: ";
        for (auto& it : nodes)
        {
            std::cout << it.first << "/" << it.second << "|";
        }
        std::cout << std::endl;
    }

    void printLetters()
    {
        std::cout << "Letters: ";
        for (auto& it : letters)
        {
            std::cout << it.first << "/" << it.second << "|";
        }
        std::cout << std::endl;
    }

    void set(std::string node, std::string letter, char* value)
    {
        int nodeIndex = nodes.at(node);
        int letterIndex = letters.at(letter);
        table[nodeIndex][letterIndex] = value;
        std::cout << "[" << node << "][" << letter << "]:[" << nodeIndex << "][" << letterIndex << "]=" << table[nodeIndex][letterIndex] << std::endl;
    }

    void setM(std::string node, std::string letter, char* value)
    {
        std::string temp = "";
        for (auto& it : letter)
        {
            if (it == '|')
            {
                set(node, temp, value);
                temp = "";
            }
            else
            {
                temp = temp + it;
            }
        }
        set(node, temp, value);
    }

    char* at(std::string node, std::string letter)
    {
        char* temp;
        int letterIndex, nodeIndex;
        std::cout << "Goto: [" << node << "][" << letter << "]";
        try
        {
            letterIndex = letters.at(letter);
            nodeIndex = nodes.at(node);
        }
        catch (std::out_of_range)
        {
            temp = (char*)failChar.c_str();
            return temp;
        }
        temp = table[nodeIndex][letterIndex];
        std::cout << "=" << temp << std::endl;
        return temp;
    }

    std::string failable()
    {
        return failChar;
    }

private:
    char*** table;
    std::unordered_map<std::string, int> nodes;
    std::unordered_map<std::string, int> letters;
    int nodeCount, letterCount;
    std::string failChar;
    int NODE_MAX;
    int LETTER_MAX;
};

void printStack(const std::vector<std::string> vector)
{
    std::cout << "Stack: ";
    for (auto& it : vector)
    {
        std::cout << it << " ";
    }
    std::cout << std::endl;
}

bool Parse(ParseTable table, std::vector<std::string>& parseThis)
{
    std::vector<std::string> parseStack;
    int iterations = 1, readIndex = 0, nodeIndex = 0, letterIndex = 0, subIndex = 0;
    std::string read, currentNode, currentRead, subRead;

    parseThis.push_back("$");
    printStack(parseThis);
    parseStack.push_back("$");
    parseStack.push_back("S");
    read = parseThis[readIndex];
    ++readIndex;
    std::cout << "Push: $\nPush: S\nRead: " + read << std::endl;
    printStack(parseStack);
    bool isReserved = false, onWord = false;
    while (readIndex < parseThis.size())
    {
        int a = 0, b = 0;
        std::vector<std::string> toPush;
        std::string tok;
        std::string temp1;

        currentNode = parseStack.back();
        parseStack.pop_back();
        std::cout << iterations << ".Pop: " << currentNode << std::endl;

        if (currentNode == read && currentNode == "$")
        {
            std::cout << "Match found!\n";
            printStack(parseStack);
            return true;
        }

        if (currentNode == read)
        {
            std::cout << "Match found!\n";
            printStack(parseStack);
            read = parseThis[readIndex];
            ++readIndex;
            currentNode = parseStack.back();
            parseStack.pop_back();
            std::cout << "Read: " << read << std::endl;
        }
        if (read == "program" || read == "var" || read == "begin" || read == "end." || read == "integer" || read == "write" || read == ";" || read == "\"value=\"" || read == "(" || read == ",")
        {
            std::cout << read << " is a keyword." << std::endl;
            isReserved = true;
        }
        else
        {
            isReserved = false;
            if (!onWord)
            {
                onWord = true;
                std::cout << "Reading: " << read << std::endl;
            }
        }
        if (!isReserved)
        {
            if (currentNode == read && currentNode == "$")
            {
                std::cout << "Match found!\n";
                printStack(parseStack);
                return true;
            }

            std::string subChar;
            subChar = "";
            subChar = subChar + read[subIndex];
            if (currentNode == subChar && subIndex == read.size() - 1)
            {
                std::cout << read << " found!\n";
                printStack(parseStack);
                subIndex = 0;
                currentNode = parseStack.back();
                parseStack.pop_back();
                read = parseThis[readIndex];
                ++readIndex;
                std::cout << "Read: " << read << std::endl;
                onWord = false;
                goto stopIteration;
            }
            if (currentNode == subChar)
            {
                std::cout << "Match found!\n";
                printStack(parseStack);
                currentRead = read[subIndex];
                ++subIndex;
                currentNode = parseStack.back();
                parseStack.pop_back();
                std::cout << "Read: " << read[subIndex] << std::endl;
            }
            currentRead = read[subIndex];
        }
        else
        {
            if (currentNode == read)
            {
                std::cout << "Match found!\n";
                printStack(parseStack);
                read = parseThis[readIndex];
                ++readIndex;
                //currentNode = parseStack.back();
                //parseStack.pop_back();
                std::cout << /*"Pop: " << currentNode <<*/ "\nRead: " << read << std::endl;
                goto stopIteration;
            }

            currentRead = read;
        }

        char* temp;
        temp = table.at(currentNode, currentRead);
        temp1 = temp;
        if (temp == table.failable())
        {
            return false;
        }
        if (temp == "lambda")
        {
            goto stopIteration;
        }

        //PUSH TO PARSE STACK
        tok = "";
        if (temp1.find(" ", 0) != std::string::npos)
        {
            for (int i = 0; i < temp1.length(); ++i)
            {
                if (temp1[i] == ' ')
                {
                    tok = temp1.substr(a, i - a);
                    a = i + 1;
                    toPush.push_back(tok);
                }
                b = i;
            }
            tok = temp1.substr(a, b);
            toPush.push_back(tok);
            for (int i = toPush.size() - 1; i >= 0; --i)
            {
                std::cout << "Push: " << toPush[i] << std::endl;
                parseStack.push_back(toPush[i]);
            }
        }
        else
        {
            std::cout << "Push: " << temp1 << std::endl;
            parseStack.push_back(temp1);
        }
        //END PUSH TO PARSE

    stopIteration:
        ++iterations;
        printStack(parseStack);
    }

    return false;
}

void InitTable(ParseTable* table)
{
    table->addLetters("a|b|c|d|w|f|0|1|2|3|4|5|6|7|8|9|+|-|*|/|=|;|:|,|(|)|$|program|begin|end.|write|integer|\"value=\"");
    table->addNodes("S|A|B|C|D|Z|E|F|Y|G|I|K|L|M|N|O|P|Q|R|X|T");
    table->setFail((char*)"[]");
    table->set("S", "program", (char*)"program A ; var C begin F end.");
    table->setM("A", "a|b|c|d|w|f", (char*)"T B");
    table->setM("B", "a|b|c|d|w|f", (char*)"T B");
    table->setM("B", "0|1|2|3|4|5|6|7|8|9", (char*)"X B");
    table->setM("B", "+|-|*|/|=|;|:|,|)", (char*)"lambda");
    table->setM("C", "a|b|c|d|w|f", (char*)"D : E ;");
    table->setM("D", "a|b|c|d|w|f", (char*)"A Z");
    table->set("Z", ":", (char*)"lambda");
    table->set("Z", ",", (char*)", D");
    table->set("E", "integer", (char*)"integer");
    table->setM("F", "a|b|c|d|w|f|write", (char*)"G Y");
    table->setM("Y", "a|b|c|d|w|f|write", (char*)"F");
    table->set("Y", "end.", (char*)"lambda");
    table->setM("G", "a|b|c|d|w|f", (char*)"A = K ;");
    table->set("G", "write", (char*)"write ( I A ) ;");
    table->setM("I", "a|b|c|d|w|f", (char*)"lambda");
    table->set("I", "\"value=\"", (char*)"\"value=\" ,");
    table->setM("K", "a|b|c|d|w|f|0|1|2|3|4|5|6|7|8|9|+|-|(", (char*)"M L");
    table->set("L", "+", (char*)"+ M L");
    table->set("L", "-", (char*)"- M L");
    table->setM("L", ";|)", (char*)"lambda");
    table->setM("M", "a|b|c|d|w|f|0|1|2|3|4|5|6|7|8|9|+|-|(", (char*)"O N");
    table->setM("N", "+|-|;|)", (char*)"lambda");
    table->set("N", "*", (char*)"* O N");
    table->set("N", "/", (char*)"/ O N");
    table->setM("O", "a|b|c|d|w|f", (char*)"A");
    table->setM("O", "0|1|2|3|4|5|6|7|8|9|+|-", (char*)"P");
    table->set("O", "(", (char*)"( K )");
    table->setM("P", "0|1|2|3|4|5|6|7|8|9|+|-", (char*)"R X Q");
    table->setM("Q", "0|1|2|3|4|5|6|7|8|9", (char*)"X Q");
    table->setM("Q", "+|-|*|/|;", (char*)"lambda");
    table->setM("R", "0|1|2|3|4|5|6|7|8|9", (char*)"lambda");
    table->set("R", "+", (char*)"+");
    table->set("R", "-", (char*)"-");
    table->set("X", "0", (char*)"0");
    table->set("X", "1", (char*)"1");
    table->set("X", "2", (char*)"2");
    table->set("X", "3", (char*)"3");
    table->set("X", "4", (char*)"4");
    table->set("X", "5", (char*)"5");
    table->set("X", "6", (char*)"6");
    table->set("X", "7", (char*)"7");
    table->set("X", "8", (char*)"8");
    table->set("X", "9", (char*)"9");
    table->set("T", "a", (char*)"a");
    table->set("T", "b", (char*)"b");
    table->set("T", "c", (char*)"c");
    table->set("T", "d", (char*)"d");
    table->set("T", "w", (char*)"w");
    table->set("T", "f", (char*)"f");
}

std::string trim(const std::string s)
{
    std::string line = s;
    char curr;
    int x;
    for (int i = 0; i < line.length(); ++i)
    {
        switch (line[i])
        {
        case ' ':
            while (line[i + 1] == ' ')
            {
                line.erase(i + 1, 1);
            }
            break;
        case ';':
            if (line[i - 1] != ' ')
            {
                line.insert(i, " ");
            }
            ++i;
            while (line.back() != ';')
            {
                line.pop_back();
            }
            break;
        case ':':
            curr = ':';
            goto addSpace;
        case ',':
            curr = ',';
            goto addSpace;
        case '*':
            curr = '*';
            goto addSpace;
        case '/':
            curr = '/';
            goto addSpace;
        case '+':
            curr = '+';
            goto addSpace;
        case '-':
            curr = '-';
            goto addSpace;
        case '(':
            curr = '(';
            goto addSpace;
        case ')':
            curr = ')';
            goto addSpace;
        case '=':
            curr = '=';
        addSpace:
            //Add space before and after key chars
            if (line[i - 1] != ' ')
            {
                line.insert(i, " ");
            }

            x = line.find(curr, i);
            if (x != std::string::npos)
            {
                if (line[x + 1] != ' ')
                {
                    line.insert(x + 1, " ");
                }
            }
            break;
        default:
            break;
        }
    }

    if (line[0] == ' ')
    {
        line.erase(0, 1);
    }
    x = line.find("\"value = \"", 0);
    if (x != std::string::npos)
    {
        line.replace(x, 12, "\"value=\" ,");
    }
    return line;
}

std::string CopyFile(std::string filename)
{
    std::string newfile = filename;
    newfile.erase(filename.size() - 5, 5);
    newfile = newfile + "2.txt";
    std::cout << "Creating file: " + newfile << std::endl;

    std::string currLine;
    std::fstream rf;
    std::ofstream wf(newfile);

    rf.open(filename);
    if (wf.fail())
    {
        std::cout << "Fail to create/open text file" + newfile << std::endl;
        exit(-1);
    }
    if (rf.fail())
    {
        std::cout << "Fail to open text file " + filename << std::endl;
        exit(-1);
    }

    while (rf.good() && wf.good())
    {
        getline(rf, currLine);

        //START COMMENT REMOVAL
        size_t commentStart;
        size_t commentEnd;
        std::string comment = "";
        commentStart = currLine.find("/*", 0);
        if (commentStart != std::string::npos)
        {
            commentEnd = currLine.find("*/", commentStart);
            if (commentEnd != std::string::npos)
            {
                currLine.erase(commentStart, commentEnd);
            }
            else
            {
                currLine.erase(0, currLine.length());
            }
        }
        commentEnd = currLine.find("*/");
        if (commentEnd != std::string::npos)
        {
            currLine.erase(0, currLine.length());
        }
        //END COMMENT REMOVAL

        //START SPACE REMOVAL
        currLine = trim(currLine);
        //END SPACE REMOVAL

        if (currLine.size() > 1)
        {
            wf << currLine << std::endl;
        }
    }
    rf.close();
    wf.close();

    return newfile;
}

void FiletoVector(std::string filename, std::vector<std::string>& parseThis)
{
    std::fstream rf;
    std::string currLine;
    rf.open(filename);
    int linesParsed = 0;

    if (rf.fail())
    {
        std::cout << "Fail to open text file " + filename << std::endl;
        exit(-1);
    }
    else
    {
        std::cout << "Opened " + filename << std::endl;
    }

    while (rf.good())
    {
        std::string token = "";

        getline(rf, currLine);

        int a = 0;

        for (int i = 0; i < currLine.length(); ++i)
        {
            bool isSpace = (currLine[i] == ' ');
            if (isSpace)
            {
                token = currLine.substr(a, i - a);
                a = i + 1;
                parseThis.push_back(token);
            }
            if (currLine.find(";", 0) != std::string::npos && i == currLine.length() - 1)
            {
                parseThis.push_back(";");
            }
        }
        if (currLine.find(' ') == std::string::npos)
        {
            parseThis.push_back(currLine);
        }

        ++linesParsed;
    }
    parseThis.pop_back();
}

void writeProgram(std::string filename)
{
    std::string file = filename;
    file.erase(filename.size() - 4, 4);
    file = file + ".cpp";
    std::cout << "Creating file: " + file << std::endl;
    std::ofstream wf(file);
    std::fstream rf;

    rf.open(filename);

    if (wf.fail() || rf.fail())
    {
        std::cout << "Failed to create/open text file.\n";
        exit(-1);
    }

    std::string currLine;

    wf << "#include <iostream>" << std::endl;
    wf << "using namespace std;" << std::endl;
    while (getline(rf, currLine))
    {
        bool pushLine = true;

        if (currLine.find("program", 0) != std::string::npos)
        {
            pushLine = false;
            wf << "int main()" << std::endl;
            wf << "{" << std::endl;
        }

        if (currLine.find("var", 0) != std::string::npos || currLine.find("begin", 0) != std::string::npos || currLine.find("end.", 0) != std::string::npos)
        {
            pushLine = false;
        }

        if (currLine.find(": integer", 0) != std::string::npos)
        {
            wf << "int ";
            currLine.erase(currLine.find(": integer"), 10);
        }

        if (currLine.find("write", 0) != std::string::npos)
        {
            bool startWrite = false;
            pushLine = false;
            wf << "cout <<";
            for (int i = 0; i < currLine.length(); ++i)
            {
                if (currLine[i] == '(' || currLine[i] == ')')
                {
                    startWrite = !startWrite;
                    continue;
                }
                if (currLine[i] == ',')
                {
                    wf << " <<";
                    continue;
                }
                if (startWrite)
                {
                    wf << currLine[i];
                }
            }
            wf << "<< endl ;" << std::endl;
        }

        if (pushLine)
        {
            wf << currLine << std::endl;
        }
    }
    wf << "}";

}

int main()
{
    ParseTable table(21, 33);
    InitTable(&table);
    table.printNodes();
    table.printLetters();
    table.print();
    std::cout << "---End Table Initialization---" << std::endl << std::endl;

    std::vector<std::string> parseThis;
    std::string filename;
    char repeat = 'y';

    while (repeat != 'n')
    {
        std::cout << "Please input text file to parse." << std::endl;
        getline(std::cin, filename);
        filename = CopyFile(filename);
        FiletoVector(filename, parseThis);
        printStack(parseThis);

        if (Parse(table, parseThis))
        {
            std::cout << "\nString is valid." << std::endl;
            writeProgram(filename);
        }
        else
        {
            std::cout << "\nString is not valid." << std::endl;
        }
        std::cout << "Would you like to try another string for this language set?(y/n):";
        std::cin >> repeat;
        std::cin.ignore();
    }
    repeat = 'y';


    return 0;
}
