/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft.hpp                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 12:03:03 by shtanemu          #+#    #+#             */
/*   Updated: 2023/11/07 12:21:45 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>

namespace ft {
	bool strcmpCaseIns(const std::string &s1, const std::string &s2);
	int errorInInit(std::string const &errormessage);
	void putSystemError(char const *msg);
}
