#include <string>
#include <cstring>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

class Reader {
public:
	Reader(std::string filename);
	~Reader();
	const char *read_file(size_t &length);
	bool close_file();
private:
	char* m_filename;
	int m_fd;
	size_t m_length;
	char *m_file_addr;
	bool m_closed;
};
