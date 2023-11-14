/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JsonValue.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <hboissel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 14:31:28 by hboissel          #+#    #+#             */
/*   Updated: 2023/11/14 06:54:13 by hboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "JsonValue.hpp"

JsonValue::JsonValue() : type_(STRING) {}

std::map<std::string, JsonValue> JsonValue::getObject() const {
	if (type_ == OBJECT) {
		return this->object_;
	} else {
		// Handle error or return an empty map
		std::cerr << "Error: Not an object." << std::endl;
        std::map<std::string, JsonValue> tmp;
		return tmp;
	}
}

std::vector<JsonValue> JsonValue::getArray() const {
	if (type_ == ARRAY) {
		return array_;
	} else {
		// Handle error or return an empty vector
		std::cerr << "Error: Not an array." << std::endl;
		std::vector<JsonValue> tmp;
		return tmp;
	}
}

std::string JsonValue::getString() const {
	if (type_ == STRING) {
		return value_;
	} else {
		// Handle error or return an empty string
		std::cerr << "Error: Not a string." << std::endl;
		return "";
	}
}

void JsonValue::setObject(const std::map<std::string, JsonValue>& object) {
	type_ = OBJECT;
	object_ = object;
}

void JsonValue::setArray(const std::vector<JsonValue>& array) {
	type_ = ARRAY;
	array_ = array;
}

void JsonValue::setString(const std::string& value) {
	type_ = STRING;
	value_ = value;
}

void JsonValue::printJsonValue(int indentation) {
	printJsonValueHelper(*this, indentation);
}

void JsonValue::printJsonValueHelper(const JsonValue& value, int indentation) {
	switch (value.type_) {
		case STRING:
			std::cout << "\"" << value.value_ << "\"";
			break;
		case OBJECT:
			printJsonObject(value.object_, indentation);
			break;
		case ARRAY:
			printJsonArray(value.array_, indentation);
			break;
	}
}

void JsonValue::printJsonObject(const std::map<std::string, JsonValue>& object, int indentation) {
	std::cout << "{" << std::endl;

	for (std::map<std::string, JsonValue>::const_iterator it = object.begin(); it != object.end(); ++it) 
	{
		printIndentation(indentation + 1);
        std::cout << "\"" << it->first << "\": ";
        printJsonValueHelper(it->second, indentation + 1);
        std::cout << "," << std::endl;
	}

	printIndentation(indentation);
	std::cout << "}";
}

void JsonValue::printJsonArray(const std::vector<JsonValue>& array, int indentation) {
	std::cout << "[" << std::endl;

	for (std::vector<JsonValue>::const_iterator it = array.begin(); it != array.end(); ++it) {
        printIndentation(indentation + 1);
        printJsonValueHelper(*it, indentation + 1);
        std::cout << "," << std::endl;
    }

	printIndentation(indentation);
	std::cout << "]";
}

void JsonValue::printIndentation(int indentation) {
	for (int i = 0; i < indentation; ++i) {
		std::cout << "  ";  // Use two spaces for each level of indentation
	}
}

bool JsonValue::isObject(void) const {
	if (this->type_ == OBJECT)
		return (true);
	return (false);
}

bool JsonValue::isString(void) const {
	if (this->type_ == STRING)
		return (true);
	return (false);
}

bool JsonValue::isArray(void) const {
	if (this->type_ == ARRAY)
		return (true);
	return (false);
}
