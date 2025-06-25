//
// Created by cory on 6/25/25.
//

#include <fcntl.h>
#include <unistd.h>
#include <cstdio>
#include "io/file.hpp"

namespace json
{
	file::file(const char* dir) : dir(dir), fd(-1) {}

	file::~file()
	{
		close_file();
	}

	bool file::is_open() const
	{
		return fd != -1;
	}

	void file::open_file()
	{
		close_file();

		fd = open(dir, 0, O_RDONLY);

		if (!is_open())
		{
			// Uh oh, the file didn't open
			perror("file::open_file(): open()");
		}
	}

	void file::close_file()
	{
		if (is_open())
		{
			int res = close(fd);

			if (res == -1)
			{
				perror("file::close_file(): close()");
			} else
			{
				fd = -1;
			}
		}
	}

	ssize_t file::buffer(char* buf, size_t size) const
	{
		return read(fd, buf, size);
	}
}