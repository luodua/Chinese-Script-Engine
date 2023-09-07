#include "Node.h"
#include <cmath>

Node::Node()
{
    t = {};
    left = {};
    right = {};
}

Node::Node(const Token& token)
{
    t = token;
    left = {};
    right = {};
}

Node::Node(const Token& token, std::shared_ptr<Node> l, std::shared_ptr<Node> r)
{
    t = token;
    left = l;
    right = r;
}

const Token& Node::GetToken()
{
    return t;
}

std::shared_ptr<Node> Node::GetLeft()
{
    return left;
}

std::shared_ptr<Node> Node::GetRight()
{
    return right;
}

void Node::SetLeft(std::shared_ptr<Node> l)
{
    left = l;
}

void Node::SetRight(std::shared_ptr<Node> r)
{
    right = r;
}



std::string Node::ToString()
{
    if (!left || !right)
    {
    	if(t.code==$TEMP){
    		return std::to_string(t.codevalue);
		}
    	if (t.type==NUM){return std::to_string(t.value.integer);}
        return t.value.string;
	}
	if(t.code==$TEMP){
    		return std::to_string(t.codevalue);
		}
    return '(' + left->ToString() + ' ' + t.value.string + ' ' + right->ToString() + ')';
}


