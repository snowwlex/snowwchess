#ifndef _TEST_H
#define	_TEST_H

#include <stdexcept>
#include <iostream>
#include <vector>
#include <string>

#define TEST( logic_expr ) \
    ++myQtyTests; \
    try { \
        if (logic_expr == false) { \
            addFailure(Failure(#logic_expr, __FILE__, __LINE__) );  \
        } else { \
            ++myQtySuccess; \
        } \
    } catch (std::exception& exc) { \
        std::string STR =#logic_expr; \
        STR += ", catch exception: '"; \
        STR += exc.what(); \
        STR += "'"; \
        addFailure(Failure(STR.c_str(), __FILE__, __LINE__)); \
    }


#define CHECK_THROWS( expr ) \
    ++myQtyTests; \
    try { \
      expr ; \
      std::string STR =#expr; \
      STR += ", expected to throw exception, but didn't"; \
      addFailure(Failure(STR.c_str(), __FILE__, __LINE__) );  \
    } catch (... ) { \
        ++myQtySuccess; \
    } \


class Failure {
public:

    Failure(const char* expr, const char* file, int line) :
    myExpr(expr), myFile(file), myLine(line) {
    }
private:

    std::string myExpr;
    std::string myFile;
    int myLine;

    //friend:

    friend std::ostream & operator<<(std::ostream& stream, const Failure& failure) {
        std::cout << failure.myFile << " at line " << failure.myLine << ": " << failure.myExpr;
	return stream;
    }
};

class Test {
public:
    Test();
    virtual ~Test();
public:
    virtual void run();
    virtual void report();
protected:
    void addFailure(const Failure& failire);
    void clear();
protected:
    typedef std::vector<Failure> FAILURES;
    FAILURES myFailures;
    int myQtyTests;
    int myQtySuccess;

};

#endif	/* _TEST_H */

