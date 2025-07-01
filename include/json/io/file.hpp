//
// Created by cory on 6/25/25.
//

#ifndef JSON_FILE_HPP
#define JSON_FILE_HPP


namespace json
{
	class file
	{
	public:
		explicit file(const char* dir);

		~file();

		ssize_t buffer(char* buf, size_t size) const;

		/**
		 * @return The state of this file's file descriptor, whether opened or closed.
		 */
		[[nodiscard]] bool is_open() const;

		void open_file();

		void close_file();

	private:
		const char* dir;
		int fd;
	};
}


#endif //JSON_FILE_HPP
