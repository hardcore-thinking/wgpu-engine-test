#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>

class Logger {
public:
	Logger() = default;
	~Logger() = default;

	void Info(std::string message) const;
	void Warn(std::string message) const;
	void Error(std::string message) const;
};

#endif // !LOGGER_HPP