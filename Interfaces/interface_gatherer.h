#pragma once

#include <string>

/**
* @brief ����� - ��������� ��� ������ - �������� ������ ������.
*/
class IGatherer
{
public:
	virtual ~IGatherer() = default;

	/**
	* ���������� ����� ������ ������.
	* @param hid ����� ���������.
	* @param data ������ ������.
	* @param bulk_size ����������� �� ���-�� ������ ������.
	*/
	virtual void add_one_piece(const std::uint32_t&, const std::string&, const std::size_t&) = 0;

	/**
	* ��������� �� ����� ��� ������������ ������.
	*/
	virtual void flush_pieces() = 0;
};