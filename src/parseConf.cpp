/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseConf.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 11:37:10 by komatsud          #+#    #+#             */
/*   Updated: 2023/10/19 18:00:11 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfParser.hpp"

Result<std::vector<Config>, bool> parseConf(std::string filepath) {
	std::vector<std::string> list;
	std::vector<Config> confs;
	Config tmp;

	//まずポートごとにきりわける
	Result<std::vector<std::string>, bool> res = cutConfByDirective(filepath);
	if (res.isOK() == true)
		list = res.getOk();
	else
		return Error<bool>(false);

	//各ポートの設定ごとに、一つずつConfファイルを作り、Push_backしていく
	for (size_t i = 0; i < list.size(); i++) {
		// std::cout << RED << list.at(i) << RESET << std::endl;
		Result<Config, bool> eachconf = parsePortVecs(list.at(i));
		if (eachconf.isOK() == true) {
			tmp = eachconf.getOk();
			confs.push_back(tmp);
		}
	}

	// return
	// std::cout << "returned:" << confs.size() << std::endl;
	return Ok<std::vector<Config> >(confs);
}

// int main()
// {
// 	parseConf("../conf_files/test.conf");
// 	return 0;
// }
