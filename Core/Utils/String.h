#pragma once

namespace Utils
{
	std::string Format(std::string format, ...)
	{
		va_list args;
		va_start(args, format);
		char buffer[1024];
		vsnprintf(buffer, 1024, format.c_str(), args);
		va_end(args);
		return std::string(buffer);
	}

	std::string Join(const std::vector<std::string>& values, const std::string& separator)
	{
		std::string result;
		for (size_t i = 0; i < values.size(); i++)
		{
			result += values[i];
			if (i < values.size() - 1)
			{
				result += separator;
			}
		}
		return result;
	}
}