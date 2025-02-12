#pragma once

#include <exception>
#include <string>

class ScaldException : public std::exception
{ 
public:
	ScaldException(int line, const char* file) noexcept;

	virtual const char* what() const noexcept override;
	virtual const char* GetType() const noexcept;
	int GetLine() const noexcept;
	const std::string& GetFile() const noexcept;
	std::string GetOriginString() const noexcept;

private:
	int line;
	std::string file;

protected:
	mutable std::string whatBuffer;
};