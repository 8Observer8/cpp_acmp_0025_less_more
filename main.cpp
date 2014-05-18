#include <stdexcept>
#include <string>
#include <vector>
#include <sstream>

//#define TESTING
#ifndef TESTING

#include <iostream>
#include <fstream>

class FileError : public std::runtime_error {
public:

    FileError( const std::string &fileNameIn ) : std::runtime_error( "" ), m_fileName( fileNameIn ) {
    }

    virtual const char* what( ) const throw () {
        return m_msg.c_str( );
    }

    std::string getFileName( ) const {
        return m_fileName;
    }

    virtual ~FileError( ) throw () {

    }

protected:
    std::string m_fileName;
    std::string m_msg;
};

class FileOpenError : public FileError {
public:

    FileOpenError( const std::string &fileNameIn ) : FileError( fileNameIn ) {
        m_msg = "Unable to open " + fileNameIn;
    }
};

class FileReadError : public FileError {
public:

    FileReadError( const std::string &fileNameIn ) : FileError( fileNameIn ) {
        std::ostringstream ostr;
        ostr << "Error reading " << fileNameIn;
        m_msg = ostr.str( );
    }
};

class FileWriteError : public FileError {
public:

    FileWriteError( const std::string &fileNameIn ) : FileError( fileNameIn ) {
        m_msg = "Error writing " + fileNameIn;
    }
};

#endif // TESTING

class LogicError : public std::logic_error {
public:

    LogicError( ) : std::logic_error( "" ) {

    }

    virtual const char *what( ) const throw () {
        return m_message.c_str( );
    }

    virtual ~LogicError( ) throw () {

    }

protected:
    std::string m_message;
};

class OutOfRange : public LogicError {
public:

    OutOfRange( int argument, int beginOfRange, int endOfRange ) {
        std::string str_argument, str_beginOfRange, str_endOfRange;

        str_argument = intToString( argument );
        str_beginOfRange = intToString( beginOfRange );
        str_endOfRange = intToString( endOfRange );

        m_message = "Argument " + str_argument + " doesn't hit in the range [" +
                str_beginOfRange + ", " + str_endOfRange + "]";
    }

private:

    std::string intToString( int number ) {
        std::stringstream stream;
        stream << number;
        return stream.str( );
    }
};

#ifndef TESTING
void readData(const std::string &fileName, int &firstNumber, int &secondNumber)
throw( FileOpenError, FileReadError );

void writeResult(const std::string &fileName, int result)
throw( FileOpenError, FileWriteError );

int less_or_more(int first, int second)
throw( OutOfRange );

int main(int argc, char** argv) {

    // Input data
    int firstNumber;
    int secondNumber;

    // Name of the input file
    std::string fileNameIn = "input.txt";

    // Read data from the file
    try {
        readData(fileNameIn, firstNumber, secondNumber);
    } catch( const FileError &e ) {
        std::cerr << e.what( ) << std::endl;
        return 1;
    }

    // Execute target function
    int result = less_or_more(firstNumber, secondNumber);

    // Write data to the file
    std::string fileNameOut = "output.txt";
    try {
        writeResult(fileNameOut, result);
    } catch( const FileError &e ) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}

void readData(const std::string &fileName, int &firstNumber, int &secondNumber)
throw (FileOpenError, FileReadError) {
    // Open the input file for reading
    std::ifstream file;
    file.open( fileName.c_str( ) );
    if ( file.fail( ) ) {
        throw FileOpenError( fileName );
    }

    // Read data from the input file
    if ( !( file >> firstNumber >> secondNumber ) ) {
        throw FileReadError( fileName );
    }
}

void writeResult(const std::string &fileName, int result)
throw( FileOpenError, FileWriteError ) {
    // Open the output file for writing
    std::ofstream file;
    file.open( fileName.c_str() );
    if ( file.fail( ) ) {
        throw FileOpenError( fileName );
    }

    // Write data to the output file
    if (result > 0) {
        if ( !( file << ">" << std::endl )) {
            throw FileWriteError( fileName );
        }
    } else if (result < 0) {
        if ( !( file << "<" << std::endl )) {
            throw FileWriteError( fileName );
        }
    } else {
        if ( !( file << "=" << std::endl )) {
            throw FileWriteError( fileName );
        }
    }
}
#endif // TESTING

/**
 * Check what number is more
 *
 * @param firstNumber Первое число
 * @param secondNumber Второе число
 * @return 1 - if the first number is more than second
 *        -1 - if the second number is more than first
 *         0 - if the numbers are equal
 */
int less_or_more(int firstNumber, int secondNumber)
throw( OutOfRange ) {

    int beginOfRange = -2000000000;
    int endOfRange = 2000000000;

    if ( ( firstNumber < beginOfRange ) || ( endOfRange < firstNumber ) ) {
        throw( OutOfRange( firstNumber, beginOfRange, endOfRange ) );
    }

    if ( ( secondNumber < beginOfRange ) || ( endOfRange < secondNumber ) ) {
        throw( OutOfRange( secondNumber, beginOfRange, endOfRange ) );
    }

    int result = 0;

    if (firstNumber > secondNumber) {
        result = 1;
    } else if (firstNumber < secondNumber) {
        result = -1;
    } else {
        result = 0;
    }

    return result;
}
