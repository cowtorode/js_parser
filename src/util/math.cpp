//
// Created by cory on 6/30/25.
//

bool is_relevant(char c)
{
	switch (c)
	{
		case '\t': // Horizontal Tab (0x09)
		case '\n': // Line Feed (0x0A)
		case '\v': // Vertical Tab (0x0B)
		case '\f': // Form Feed (0x0C)
		case '\r': // Carriage Return (0x0D)
		case ' ':  // Space (0x20)
			return false;
		default:
			return true;
	}
}