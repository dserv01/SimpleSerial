#include "Arduino.h"
#ifndef SIMPLESERIAL.H
#define SIMPLESERIAL.H

class SimpleSerial {
	private:
		bool tokenEnded=false;
		const char ESCAPE; //The Symbol used to escape a character (esp the characters equal the delimiter)
		const char DELIMITER; //The symbol used to end a token
	public:
		SimpleSerial(char del=':', char esc='!'):DELIMITER(del), ESCAPE(esc){ }
		void begin(int baudrate=9600){ Serial.begin(baudrate); }

		bool available(){ return Serial.available(); }
		
		/** Jumps to the next token if the present token is finished. If the present token isn't finished
		 *  it does nothing. 
		 *  @return true if there is a next token available, false if not
		 */
		bool nextToken();
	
		/** Reads the next character of the token or '\0'=0 if the end of the token is reached.
		 *  If escaped characters will be automatically converted. You can use '\0' in the token but 
		 *  then you couldn't identify the end of the token, so better don't. Use nextToken() to
		 *  jump to the next Token if you finished this.
		 *  @return The next character
		 */
		char read();

		/** Checks if all characters of the token are read. Use nextToken to jump to the next Token.
		 *  @return True if there are no more characters in the token.
		 */
		bool end_of_token();
		
		/** Returns the index of the keyword matching the token or -1 if no keyword matches.
		 *  Does only support 1to1 matchings, the keyword has to be equal to the token. RegEx is actually
		 *  not supported but maybe in the future.
		 *  @param[in] keywords 	 array with all keywords
		 *  @param[in] keyword_count count of the keywords
		 *  @return The index of the matching keyword or -1
		 */
		int getKeyword(char *keywords[], uint8_t keyword_count);

		/** Reads the token as unsigned integer or returns false if the token is not an unsigned integer.
		 * char* "435"-> uint 435
		 *  @param[out] ret 	The token as UInt
		 *  @return true if successfull
		 */
		bool getUInt(uint16_t *ret);

		/** Reads the token as integer or returns false if the token is not an integer. 
		 *  The sign can be '-','+' or missing (+). 
		 *  char* "-45" -> int -45
		 *  @param[out] ret 	The token as Int
		 *  @return true if successfull
		 */
		bool getInt(int *ret);

		/** Returns the token as String(Char-array). Does only write the output of read() into the
		 *  buffer until the end of the token is reached or the buffer is full.
		 *  @param[out] buffer 	The Token
		 *  @param[in] buffer_size 	The size of the buffer. Please notice that a token ends with '\0'.
		 *  @return used buffer size
		 */
		uint8_t getString(char* buffer, uint8_t buffer_size);
};

#endif
