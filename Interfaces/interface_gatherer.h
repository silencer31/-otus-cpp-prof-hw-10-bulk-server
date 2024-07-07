#pragma once

#include <string>

/**
* @brief Класс - интерфейс для класса - сборщика порций данных.
*/
class IGatherer
{
public:
	virtual ~IGatherer() = default;

	/**
	* Добавление одной порции данных.
	* @param hid номер контекста.
	* @param data порция данных.
	* @param bulk_size ограничение на кол-во порций данных.
	*/
	virtual void add_one_piece(const std::uint32_t&, const std::string&, const std::size_t&) = 0;

	/**
	* Отправить на вывод все накопившиеся порции.
	*/
	virtual void flush_pieces() = 0;
};