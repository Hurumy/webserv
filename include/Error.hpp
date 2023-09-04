/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Error.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 15:03:22 by shtanemu          #+#    #+#             */
/*   Updated: 2023/09/04 16:27:59 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Result.hpp"
#include <iostream>

template <typename E>
class Error {
	public:
		Error(E const &_e) : e(_e) { std::clog << "this is error" << std::endl; }
		Error(Error const &sourceError) { *this = sourceError; }
		Error & operator = (Error const &sourceError) {
			if (this != &sourceError) {
				e = sourceError.e;
			}
			return *this;
		}
		~Error() {}
		template <typename T, typename V>
		operator Result<T, V>() const { return Result<T, V>(e); }

	private:
		Error() {}

		E e;
};
