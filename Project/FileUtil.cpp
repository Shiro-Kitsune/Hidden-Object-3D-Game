#include "FileUtil.h"
#include <fstream>

std::string FileUtil::loadFile(const char * path)
{
	std::string fileContent;
	std::ifstream FileStream(path, std::ios::in);

	if (FileStream.is_open())
	{
		std::string line = "";
		while (getline(FileStream, line))
		{
			fileContent += "\n" + line;
		}
		FileStream.close();
	}
	else
	{
		printf("Impossible to open %s. Are you in the right directory?\n", path);
	}

	return fileContent;
}
