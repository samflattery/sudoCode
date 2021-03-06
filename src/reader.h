/*
 * reader.h
 * --------
 * defines the Reader class that reads a given file and returns a char *
 */

#include <string>
#include <cstring>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#ifndef __READER_H__
#define __READER_H__

class Reader {
public:
	Reader(std::string filename) : m_filename(filename.c_str()) {}
	~Reader();
	const char *read_file(size_t &length);
	bool close_file();
private:
	const char* m_filename;
	int m_fd;
	size_t m_length;
	char *m_file_addr;
	bool m_closed;
};

#endif /* __READER_H__ */
