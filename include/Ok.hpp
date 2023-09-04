/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ok.hpp                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 15:02:38 by shtanemu          #+#    #+#             */
/*   Updated: 2023/09/04 16:22:17 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Result.hpp"

template <typename T>
class Ok {
	public:
		Ok(T const &_t) : t(_t) {}
		Ok(Ok const &sourceOk) { *this = sourceOk; }
		Ok & operator = (Ok const &sourceOk) {
			if (this != &sourceOk) {
				t = sourceOk.t;
			}
			return *this;
		}
		~Ok() {}
		template <typename V, typename E>
		operator Result<V, E>() const { return Result<V, E>(t); }

	private:
		Ok() {}

		T t;
};
