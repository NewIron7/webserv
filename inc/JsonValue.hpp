/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JsonValue.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <hboissel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 14:31:23 by hboissel          #+#    #+#             */
/*   Updated: 2023/11/25 09:00:55 by hboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef JSONVALUE_HPP
# define JSONVALUE_HPP

# include <iostream>
# include <map>
# include <vector>

class JsonValue {
	public:
		enum Type { STRING, OBJECT, ARRAY };

		JsonValue();

		std::map<std::string, JsonValue> getObject() const;
		std::vector<JsonValue> getArray() const;
		std::string getString() const;
		void setObject(const std::map<std::string, JsonValue>& object);
		void setArray(const std::vector<JsonValue>& array);
		void setString(const std::string& value);
        void printJsonValue(int indentation = 0);
		bool isObject(void) const;
		bool isString(void) const;
		bool isArray(void) const;

	private:
		Type type_;
		std::string value_;
		std::map<std::string, JsonValue> object_;
		std::vector<JsonValue> array_;

        static void printJsonValueHelper(const JsonValue& value,
            int indentation);
        static void printJsonObject(const std::map<std::string,
				JsonValue>& object, int indentation);
        static void printJsonArray(const std::vector<JsonValue>& array,
            int indentation);
        static void printIndentation(int indentation);
};

#endif
