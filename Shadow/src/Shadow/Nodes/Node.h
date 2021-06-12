#pragma once

#include "Shadow/Core.h"
#include <string>

enum NodeType
{
	OPERATION
};
struct inputPint
{

};
class Node
{
private:
	std::string name;
	std::string display_name;
	NodeType category;

};