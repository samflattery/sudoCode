#include "reader.hpp"

Reader::Reader(std::string filename) {
	m_closed = false;
	m_filename = new char [filename.length() + 1];
	std::strcpy(m_filename, filename.c_str());
}

Reader::~Reader() {
	if (!m_closed) {
		close_file();
	}
	delete m_filename;
}

const char *Reader::read_file(size_t &length) {
	m_fd = open(m_filename, O_RDONLY);
	if (m_fd == -1) {
		return NULL;
	}

	struct stat sb;
	if (fstat(m_fd, &sb) == -1) {
		return NULL;
	}

	m_length = sb.st_size;
	char *addr = static_cast<char*>(mmap(NULL, m_length, PROT_READ,
				MAP_PRIVATE, m_fd, 0u));
	if (addr == MAP_FAILED) {
		return NULL;
	}

	m_file_addr = addr;
	length = m_length;
	return addr;
}

bool Reader::close_file() {
	m_closed = true;
	if (munmap(m_file_addr, m_length) == -1) {
		return false;
	}
   	if (close(m_fd) == -1) {
		return false;
	}
	return true;
}
