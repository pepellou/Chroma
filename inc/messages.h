#ifndef MESSAGES_H
#define MESSAGES_H

class Messages {

	public:
		static bool enable;

		static void error(const char *message);
		static void info(const char *message);
		static void info(
			const char *message, 
			int value
		);
		static void info(
			const char *message1, 
			int value1,
			const char *message2, 
			int value2,
			const char *message3, 
			int value3,
			const char *message4, 
			int value4,
			const char *message5
		);
		static void info(
			const char *message1, 
			float value1,
			const char *message2, 
			float value2,
			const char *message3, 
			float value3,
			const char *message4, 
			float value4,
			const char *message5, 
			float value5,
			const char *message6, 
			float value6,
			const char *message7, 
			float value7,
			const char *message8, 
			float value8,
			const char *message9,
			float value9,
			const char *message10
		);

	private:
		static void forcePut(const char *text);
		static void put(const char *text);
		static void put(int value);
		static void put(float value);
		static void flushLine();

};

#endif
