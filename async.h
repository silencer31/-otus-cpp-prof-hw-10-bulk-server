#pragma once

#include "context_collector.h"

namespace async
{
	/*
	* @brief создает контекст для реализации многопоточной работы
	* @param number количество команд в блоке
	* @return контекст для функций receive и disconnect
	*/
	handle_t connect(std::size_t number);

	/*
	* @brief Обработка данных указанным контекстом.
	* @param handle контекст для функций receive и disconnect
	* @param data указатель на буфер с тем, что ввели
	* @param number количество символов в буфере
	*/
	void receive(handle_t handle, const char* data, std::size_t number);

	/*
	* @brief Прекращение обработки данных.
	* @param handle контекст для функций receive и disconnect
	*/
	void disconnect(handle_t handle);

}