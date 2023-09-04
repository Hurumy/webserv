/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Result.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 14:55:57 by shtanemu          #+#    #+#             */
/*   Updated: 2023/09/04 15:44:38 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

template <typename T, typename E>
class Result {
	public:
		Result(T const &_ok) : t(OK), ok(_ok) {}
		Result(E const &_error) : t(ERROR), error(_error) {}
		Result(Result const &sourceResult) { *this = sourceResult; }
		Result & operator = (Result const &sourceResult) {
			if (this != &sourceResult) {
				t = sourceResult.t;
				switch (t) {
					case OK:
						ok = sourceResult.ok;
						break;
					case ERROR:
						error = sourceResult.error;
						break;
				}
			}
			return *this;
		}
		~Result() {
			switch (t) {
				case OK:
					ok.~T();
					break;
				case ERROR:
					error.~E();
					break;
			}
		}

		bool isOK() const { return t == OK; }
		bool isError() const { return t == ERROR; }
		T const &getOk() const { return ok; }
		E const &getError() const { return error; }

	private:
		Result();
		
		enum tag {
			OK,
			ERROR,
		};
		tag t;
		T ok;
		E error;
};
