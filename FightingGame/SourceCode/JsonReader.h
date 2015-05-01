#pragma once

#include "reader.h"
#include "document.h"
#include "writer.h"
#include "stringbuffer.h"
#include <fstream>
#include <string>

using namespace rapidjson;

namespace{
	
	Document parse_jsonFile(std::string path)
	{
		std::fstream f(path);
		Document data;
		if (f.is_open())
		{
			std::string content((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
			data.Parse(content.c_str());
			f.close();
		}
		else
		{
			printf("Error: Can't open file %s\n",path.c_str());
		}

		return data;
	}

	void write_json_to_File(std::string path, Document& document)
	{
		std::fstream f;

		f.open(path, std::ofstream::out | std::ofstream::trunc);

		StringBuffer buffer;
		Writer<StringBuffer> writer(buffer);
		document.Accept(writer);

		if (f.is_open()){
			f << buffer.GetString();
			f.close();
		}
		else
		{
			printf("Error: Can't open f %s\n", path.c_str());
		}

	}
}
