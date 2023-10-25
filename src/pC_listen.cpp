/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pC_listen.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 13:55:31 by komatsud          #+#    #+#             */
/*   Updated: 2023/10/25 10:38:17 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfParser.hpp"

static int rL_searchipv6(Address &add, std::string oneline)
{
	size_t	start;
	size_t	end;
	size_t	tmp;
	size_t	tmp2;
	size_t	portsign;
	std::string	trimedstring;

	//IPv6の時
	// []を削除して : があったらポート設定もアドレス設定もある
	// []の外に何もなかったらアドレス設定のみ(デフォルトポートは80番)
	// [::]: という形だったらポート設定のみ

	start = oneline.find('[');
	end = oneline.find(']');

	//[]がなかった時(ipv6の設定ではなかった時)
	if (start == std::string::npos && end == std::string::npos)
	{
		return (0);
	}

	//[]はあるが、片方しかない時、順序がおかしいとき
	else if (start == std::string::npos || end == std::string::npos || start >= end)
	{
		errorInInit("too many [] signs detected!! (ﾉω<､)\n");
	}

	// []がそれぞれ一つずつしかないことを確認する
	tmp = oneline.find('[', start + 1);
	tmp2 = oneline.find(']', end + 1);
	if (tmp != std::string::npos || tmp2 != std::string::npos)
	{
		errorInInit("too many [] signs detected!! (ﾉω<､)\n");
	}

	// []の中身を切り出す(つまりIPアドレスの設定を読み出す)
	// []の中身が::だけであれば、それは設定がないのと同じなので何もしない
	// []の中身が正しいIpv6アドレスの形式になっているかどうかをきちんとチェックするようにしたい
	trimedstring = oneline.substr(start + 1, end - start - 1);

	// []の中に::だけでない文字列が入っていた時(後でちゃんと形式チェックしたい)
	if (trimedstring != "::")
	{
		add.setIpVers(IPV6);
		add.setIpAddress(trimedstring);
	}
	// [::]だった場合、IPに未指定アドレスをセット
	else
	{
		add.setIpVers(IPV6);
		add.setIpAddress("0:0:0:0:0:0:0:0");
	}


	// []の後ろに:があるかどうかを探す(ポート設定の有無を確認)
	portsign = oneline.find(':', end);

	// :がなかった場合->ポートの設定がないので、ポートを初期値にセット
	if (portsign == std::string::npos)
	{
		add.setIpVers(IPV6);
		add.setPort(80);
	}
	// :が正しい位置([]の直後)にあった場合、ポートの設定を切り出す
	else if (portsign == end + 1)
	{
		std::stringstream ss;
		int				 portnumber;

		ss << oneline.substr(portsign + 1);
		ss >> portnumber;

		//std::cout << "port: " << portnumber << std::endl;

		add.setIpVers(IPV6);
		add.setPort(portnumber);
	}
	// :があったが、[]の直後ではなかった場合、それは変な位置なのでエラー。
	else
	{
		errorInInit("syntax error in listen directive (ﾉω<､)\n");
	}

	return (1);
}

int rL_searchipv4(Address &add, std::string oneline)
{
	size_t	colon;
	size_t	comma;
	std::stringstream	ss;
	std::stringstream	sb;
	std::string		address_str;
	std::string		tmp;
	int				portnum;


	//IPv4の時
	// : があったらポート設定もアドレス設定もある
	// . があったらアドレス設定のみ (デフォルトポートは80番)
	// . がなかったらポートの設定のみ

	// :を探す
	colon = oneline.find(':');
	// std::cout << "oneline: " << oneline << std::endl;

	// :があった時(ポートの設定もアドレスの設定もある場合)
	if (colon != std::string::npos)
	{
		ss << oneline;
		std::getline(ss, address_str, ':');
		std::getline(ss, tmp, ':');
		// std::cout << "addstr: " << address_str << std::endl;
		// std::cout << "tmp: " << tmp << std::endl;
		sb << tmp;
		sb >> portnum;
		// std::cout << "portnum: " << portnum << std::endl;

		add.setIpVers(IPV4);
		add.setIpAddress(address_str);
		add.setPort(portnum);
		return (1);
	}
	// :がなかった時(ポート設定のみの時かアドレス設定のみの時)
	else
	{
		comma = oneline.find('.');
		// .があった時(アドレス設定のみの時) 後でちゃんとvalidateしたい
		if (comma != std::string::npos)
		{
			add.setIpVers(IPV4);
			add.setIpAddress(oneline);

			// アドレスの指定のみでポートの設定がなかった場合、デフォルトでポートを80番に設定する
			add.setPort(80);
			return (1);
		}
		// .がなかった時(ポート設定のみの時) 後でValidate!
		else
		{
			ss << oneline;
			ss >> portnum;
			add.setIpVers(IPV4);
			add.setPort(portnum);

			//ポートの設定しかなかった場合、IPアドレスを0.0.0.0に設定する
			add.setIpAddress("0.0.0.0");
			return (1);
		}
	}
	return (0);	
}


int readListen(Config &conf, std::string oneline)
{
	std::vector<std::string> lines;
	std::string				content;
	Address add;
	unsigned long long status;

	// std::cout << oneline << std::endl;
	lines = lineSpliter(oneline, " ");
	lines.erase(std::remove(lines.begin(), lines.end(), ""), lines.end());
	content = lines.at(1);

	// std::cout << content << std::endl;

	//IPv6の時
	// []を削除して : があったらポート設定もアドレス設定もある
	// []の外に何もなかったらアドレス設定のみ(デフォルトポートは80番)
	// [::]: という形だったらポート設定のみ
	// return: この中でIPv6アドレスだとわかって、セットした場合は1。IPv6でなかった場合は0。エラーの場合は即終了
	status = rL_searchipv6(add, content);


	//IPv4の時
	// : があったらポート設定もアドレス設定もある
	// . があったらアドレス設定のみ (デフォルトポートは80番)
	// . がなかったらポートの設定のみ
	if (status != 1)
		status = rL_searchipv4(add, content);

	//default_serverの設定があった場合を含める
	// if (lines.size() == 3)
	// {
	// 	if (lines.at(3) == "default_server")
	// 	{
			
	// 	}
	// 	else
	// 		errorInInit("Unknown directive found....°(ಗдಗ。)°.");
	// }


	// std::cout << "ipaddress: " << add.getIpAddress() << std::endl;
	// std::cout << "port: " << add.getPort() << std::endl;

	conf.addAddresses(add);

	return (0);
}

// listenが一つもなかった場合にデフォルト設定を書き込む
// Nginx"スーパーユーザー権限で実行されている場合は80番ポート
// そうでない場合は8080番ポートが利用される"と書いてあるんですが、全然対応できてないです。。
int	thereisnoListen(Config &conf)
{
	Address add;

	add.setPort(80);
	add.setIpVers(IPV4);
	add.setIpAddress("0.0.0.0");
	conf.addAddresses(add);
	return (0);
}


