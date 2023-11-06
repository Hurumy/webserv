/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIResponseCreator.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 22:54:44 by shtanemu          #+#    #+#             */
/*   Updated: 2023/11/06 17:28:42 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGIResponseCreator.hpp"

#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <string>
#include <ctime>

#include "Result.hpp"
#include "puterror.hpp"

CGIResponseCreator::CGIResponseCreator(Request &_request, Response &_response,
									   const std::string &_cgiPath)
	: request(_request),
	  response(_response),
	  phase(CGIResponseCreator::CGISTARTUP),
	  pid(0),
	  monitoredfd(0),
	  revents(0),
	  cgiIntput(request.getBody()),
	  cgiPath(_cgiPath),
	  portNum(0),
	  cntExecTime(0) {
	std::memset(inpfd, 0, sizeof(inpfd));
	std::memset(outpfd, 0, sizeof(outpfd));
}

CGIResponseCreator::tag const &CGIResponseCreator::getPhase() const {
	return phase;
}

CGIResponseCreator::type const &CGIResponseCreator::getResponseType() const {
	return responseType;
}

void CGIResponseCreator::setPhase(CGIResponseCreator::tag const &_phase) {
	phase = _phase;
}

pid_t CGIResponseCreator::getPid() const { return pid; }

short CGIResponseCreator::getRevents() const { return revents; }

void CGIResponseCreator::setRevents(short const _revents) {
	revents = _revents;
}

void CGIResponseCreator::setPortNum(int const _portNum) { portNum = _portNum; }

int CGIResponseCreator::getPortNum() const { return portNum; }

void CGIResponseCreator::setHostName(std::string const &_hostName) {
	hostName = _hostName;
}
std::string const &CGIResponseCreator::getHostName() const { return hostName; }

bool CGIResponseCreator::_setAuthType() {
	Result<std::string, bool> result(request.getHeader("Authorization"));
	std::string authScheme;

	if (result.isError() == true) {
		return false;
	}
	std::istringstream iss(result.getOk());
	iss >> authScheme;
	metaVariables.setMetaVar(MetaVariables::AUTH_TYPE, authScheme);
	return true;
}

bool CGIResponseCreator::_setContentLength() {
	std::stringstream ss;

	ss << request.getBody().size();
	metaVariables.setMetaVar(MetaVariables::CONTENT_LENGTH, ss.str());
	return true;
}

bool CGIResponseCreator::_setContentType() {
	Result<std::string, bool> result(request.getHeader("Content-Type"));

	if (result.isError() == true) {
		return false;
	}
	metaVariables.setMetaVar(MetaVariables::CONTENT_TYPE, result.getOk());
	return true;
}

bool CGIResponseCreator::_setGateWayInterface() {
	metaVariables.setMetaVar(MetaVariables::GATEWAY_INTERFACE, WS_CGI_VERSION);
	return true;
}

bool CGIResponseCreator::_setPathInfo() {
	std::string filename;
	std::string postFilename;
	std::size_t posCut;

	filename = cgiPath.substr(cgiPath.rfind("/"));
	postFilename = request.getUrl().substr(request.getUrl().find(filename) +
										   filename.size());
	posCut = std::min(postFilename.find("?"), postFilename.find("#"));
	metaVariables.setMetaVar(MetaVariables::PATH_INFO,
							 postFilename.substr(0, posCut));
	return true;
}

bool CGIResponseCreator::_setPathTranslated() {
	std::string pathInfo;

	pathInfo = metaVariables.getMetaVar(MetaVariables::PATH_INFO);
	metaVariables.setMetaVar(MetaVariables::PATH_TRANSLATED,
							 cgiPath + pathInfo);
	return true;
}

bool CGIResponseCreator::_setQuerySring() {
	std::string filename;
	std::string postFilename;
	std::size_t posQueryString;
	std::size_t posHash;

	filename = cgiPath.substr(cgiPath.rfind("/"));
	postFilename = request.getUrl().substr(request.getUrl().find(filename) +
										   filename.size());
	posQueryString = postFilename.find("?");
	if (posQueryString == std::string::npos) return false;
	posHash = postFilename.find("#");
	if (posHash == std::string::npos || posQueryString > posHash) {
		metaVariables.setMetaVar(MetaVariables::QUERY_STRING,
								 postFilename.substr(posQueryString + 1));
	} else {
		metaVariables.setMetaVar(
			MetaVariables::QUERY_STRING,
			postFilename.substr(posQueryString + 1, posHash));
	}
	return true;
}

bool CGIResponseCreator::_setRemoteAddr() {
	metaVariables.setMetaVar(MetaVariables::REMOTE_ADDR,
							 request.getRemoteAddr());
	return true;
}

bool CGIResponseCreator::_setRequestMethod() {
	metaVariables.setMetaVar(MetaVariables::REQUEST_METHOD,
							 request.getMethod());
	return true;
}

bool CGIResponseCreator::_setScriptName() {
	std::string filename;
	std::string scriptName;

	if (cgiPath.rfind("/") != std::string::npos) {
		filename = cgiPath.substr(cgiPath.rfind("/"));
	} else {
		filename = cgiPath;
	}
	if (request.getUrl().find(filename) == std::string::npos) {
		return false;
	}
	scriptName = request.getUrl().substr(
		0, request.getUrl().find(filename) + filename.size());
	metaVariables.setMetaVar(MetaVariables::SCRIPT_NAME, scriptName);
	return true;
}

bool CGIResponseCreator::_setServerName() {
	metaVariables.setMetaVar(MetaVariables::SERVER_NAME, hostName);
	return true;
}

bool CGIResponseCreator::_setServerPort() {
	std::ostringstream os;

	os << portNum;
	metaVariables.setMetaVar(MetaVariables::SERVER_PORT, os.str());
	return true;
}

bool CGIResponseCreator::_setServerProtocol() {
	metaVariables.setMetaVar(MetaVariables::SERVER_PROTOCOL, WS_HTTP_VERSION);
	return true;
}

// server hostname 取れるように
// server port 取れるように
// SERVER_NAME, SERVER_PORT
bool CGIResponseCreator::setEnvVars() {
	// For develope
	_setAuthType();
	_setContentLength();
	_setContentType();
	_setGateWayInterface();
	_setPathInfo();
	_setPathTranslated();
	_setQuerySring();
	_setRemoteAddr();
	_setRequestMethod();
	_setScriptName();
	_setServerName();
	_setServerPort();
	_setServerProtocol();
	return true;
}

char **CGIResponseCreator::_createEnvp() {
	extern char **environ;
	char **envp;
	char **head;

	std::vector<std::vector<char> > vstrings;
	for (std::size_t i_environ = 0; environ[i_environ] != NULL; ++i_environ) {
		std::vector<char> vcharacters(
			environ[i_environ],
			environ[i_environ] + std::strlen(environ[i_environ]));
		vcharacters.push_back('\0');
		vstrings.push_back(vcharacters);
	}
	std::map<std::string, std::string> const &metaVariablesMap =
		metaVariables.getMetaVariables();
	for (std::map<std::string, std::string>::const_iterator iter =
			 metaVariablesMap.begin();
		 iter != metaVariablesMap.end(); ++iter) {
		std::string metaVar(iter->first + iter->second);
		std::vector<char> vcharacters(metaVar.c_str(),
									  metaVar.c_str() + metaVar.size());
		vcharacters.push_back('\0');
		vstrings.push_back(vcharacters);
	}
	std::map<std::string, std::string> const &optionalMetaVariablesMap =
		metaVariables.getOptionalMetaVariables();
	for (std::map<std::string, std::string>::const_iterator iter =
			 optionalMetaVariablesMap.begin();
		 iter != optionalMetaVariablesMap.end(); ++iter) {
		std::string metaVar(iter->first + iter->second);
		std::vector<char> vcharacters(metaVar.c_str(),
									  metaVar.c_str() + metaVar.size());
		vcharacters.push_back('\0');
		vstrings.push_back(vcharacters);
	}
	envp = new (std::nothrow) char *[vstrings.size() + 1];
	if (envp == NULL) {
		return NULL;
	}
	head = envp;
	for (std::vector<std::vector<char> >::iterator iter = vstrings.begin();
		 iter != vstrings.end(); ++iter) {
		*envp = new (std::nothrow) char[iter->size()];
		if (*envp == NULL) {
			char **iteratedEnvp = head;
			char **deletedEnvp = iteratedEnvp;
			while (*iteratedEnvp != *envp) {
				deletedEnvp = iteratedEnvp;
				iteratedEnvp++;
				delete[] * deletedEnvp;
			}
			delete[] head;
			return NULL;
		}
		std::strncpy(*envp, iter->data(), iter->size());
		envp++;
	}
	*envp = NULL;
	return head;
}

char **CGIResponseCreator::_createArgv() {
	char **argv;
	char **head;
	std::size_t posDir(0);
	std::string cgiScriptFileName;

	argv = new (std::nothrow) char *[3];
	if (argv == NULL) {
		return NULL;
	}
	head = argv;
	*argv = new (std::nothrow) char[runtimePath.size() + 1];
	if (*argv == NULL) {
		delete[] head;
		return NULL;
	}
	std::strncpy(*argv, runtimePath.c_str(), runtimePath.size() + 1);
	argv++;
	*argv = new (std::nothrow) char[cgiPath.size() + 1];
	if (*argv == NULL) {
		delete[] head[0];
		delete[] head;
		return NULL;
	}
	posDir = cgiPath.rfind('/');
	if (posDir == std::string::npos) {
		std::strncpy(*argv, cgiPath.c_str(), cgiPath.size() + 1);
	} else {
		cgiScriptFileName = "." + cgiPath.substr(posDir);
		std::strncpy(*argv, cgiScriptFileName.c_str(), cgiScriptFileName.size() + 1);
	}
	// *argv[cgiPath.size() + 1] = '\0';
	argv++;
	*argv = NULL;
	return head;
}

bool CGIResponseCreator::_setRuntime() {
	std::size_t posExtension;
	std::string extension;
	std::string runtimeName;
	char *path;

	posExtension = cgiPath.rfind(".");
	if (posExtension != std::string::npos) {
		extension = cgiPath.substr(posExtension);
		if (extension.compare(".py") == 0) {
			runtimeName = "python";
		} else if (extension.compare(".pl") == 0) {
			runtimeName = "perl";
		} else if (extension.compare(".php") == 0) {
			runtimeName = "php";
		} else {
			runtimeName = "sh";
		}
	} else {
		runtimeName = "sh";
	}
	path = std::getenv("PATH");
	if (path == NULL) {
		runtimePath = "./" + runtimeName;
	} else {
		std::string candidatePath;
		std::istringstream iss(path);

		while (iss.eof() == false) {
			std::getline(iss, candidatePath, ':');
			candidatePath.append("/" + runtimeName);
			if (access(candidatePath.c_str(), X_OK) == 0) {
				runtimePath = candidatePath;
				return true;
			}
		}
		runtimePath = "./" + runtimeName;
	}

	return true;
}

bool CGIResponseCreator::_chDirectory() {
	std::string cgiDirPath;
	std::size_t posDir(0);

	posDir = cgiPath.rfind('/');
	if (posDir == std::string::npos) { return true; }
	cgiDirPath = cgiPath.substr(0, posDir + 1);
	if (chdir(cgiDirPath.c_str()) == -1) {
		putSytemError("chdir");
		return false;
	}
	return true;
}

bool CGIResponseCreator::execCGIScript() {
	char **envp;
	char **argv;

	if (cntExecTime > 10) { return false; }
	if (pipe(inpfd) == -1) {
		// error handling
		putSytemError("pipe");
		return false;
	}
	if (fcntl(inpfd[0], F_SETFL, O_NONBLOCK, FD_CLOEXEC) == -1) {
		// error handling
	}
	if (fcntl(inpfd[1], F_SETFL, O_NONBLOCK, FD_CLOEXEC) == -1) {
		// error handling
	}
	if (pipe(outpfd) == -1) {
		// error handling
		putSytemError("pipe");
		close(inpfd[0]);
		close(inpfd[1]);
		return false;
	}
	if (fcntl(outpfd[0], F_SETFL, O_NONBLOCK, FD_CLOEXEC) == -1) {
		// error handling
	}
	if (fcntl(outpfd[1], F_SETFL, O_NONBLOCK, FD_CLOEXEC) == -1) {
		// error handling
	}
	pid = fork();
	if (pid == -1) {
		putSytemError("fork");
		deinit();
		// error handling
		return false;
	}
	if (pid == 0) {
		// for developement
		if (dup2(inpfd[0], 0) == -1) {
			putSytemError("dup2");
			deinit();
			std::exit(EXIT_FAILURE);
		}
		if (dup2(outpfd[1], 1) == -1) {
			putSytemError("dup2");
			deinit();
			std::exit(EXIT_FAILURE);
		}
		close(inpfd[0]);
		close(inpfd[1]);
		close(outpfd[0]);
		close(outpfd[1]);
		if (_chDirectory() == false) { std::exit(EXIT_FAILURE); }
		setEnvVars();
		envp = _createEnvp();
		_setRuntime();
		argv = _createArgv();
		execve(runtimePath.c_str(), argv, envp);
		putSytemError("execve");
		// delete envp
		std::exit(EXIT_FAILURE);
	}
	startTime = std::time(NULL);
	cntExecTime++;
	return true;
}

int CGIResponseCreator::getMonitoredfd() const { return monitoredfd; }

void CGIResponseCreator::setMonitoredfd(CGIResponseCreator::tag const &_phase) {
	switch (_phase) {
		case CGIResponseCreator::CGIWRITE:
			monitoredfd = inpfd[1];
			break;
		case CGIResponseCreator::CGIRECV:
			monitoredfd = outpfd[0];
			break;
		default:
			break;
	}
}

bool CGIResponseCreator::writeMessageBody() {
	ssize_t lenWrite(0);

	if ((revents & POLLOUT) != POLLOUT) {
		return false;
	}
	if (cgiIntput.empty() == false) {
		lenWrite = write(inpfd[1], cgiIntput.c_str(), cgiIntput.size());
		switch (lenWrite) {
			case -1: break ;
			default : { cgiIntput.erase(0, lenWrite); } break ;
		}
	}
	if (cgiIntput.empty() == false) { return false; }
	close(inpfd[1]);
	inpfd[1] = 0;
	return true;
}

bool CGIResponseCreator::recvCGIOutput() {
	ssize_t readLen;
	char buf[BUFFER_SIZE + 1] = {0};

	if ((revents & POLLIN) != POLLIN) {
		return false;
	}
	readLen = read(outpfd[0], buf, BUFFER_SIZE);
	if (readLen == -1) {
		// errorhandling
		putSytemError("read");
		return false;
	}
	if (readLen == 0) {
		phase = CGIResponseCreator::CGIFIN;
		return true;
	}
	// for develope
	cgiOutput.append(buf, readLen);
	return true;
}

pid_t CGIResponseCreator::waitChildProc() {
	pid_t rwait;

	rwait = waitpid(pid, &wstatus, WNOHANG);
	switch (rwait) {
		case -1: {
			phase = CGIResponseCreator::CGIFIN;
			putSytemError("waitpid");
		} break;
		case 0: {
			// nothing to do
			if (15 > std::difftime(std::time(NULL), startTime)) { break; }
			if (kill(pid, SIGTERM) == -1) { putSytemError("kill"); }
		} break;
		default: {
			if (phase != CGIResponseCreator::CGIFIN) {
				phase = CGIResponseCreator::CGILASTRECV;
				setMonitoredfd(CGIResponseCreator::CGIRECV);
			}
		} break;
	}
	return rwait;
}

bool CGIResponseCreator::setCGIOutput(std::vector<Config> const &configs) {
	std::istringstream issline(cgiOutput);
	std::string line;
	std::size_t bodySize(0);

	std::getline(issline, line);
	if (line.empty() == false) {
		std::istringstream issheader(line);
		std::string key;
		
		std::getline(issheader, key, ':');
		if (key.compare("Content-Type") == 0) {
			std::string value;
			
			issheader >> value;
			response.addHeader("Content-Type", value);
			while (issline.eof() == false) {
				std::getline(issline, line);
				if (line.empty() == true) {
					std::string body;

					while (issline.eof() == false) {
						std::getline(issline, body);
						if (issline.eof() == true) { response.addBody(body); }
						else { response.addBody(body + "\r\n"); }
					}
				} else {
					issheader.clear();
					issheader.str(line);
					std::getline(issheader, key, ':');
					std::getline(issheader, value);
					response.addHeader(key, value);
				}
			}
			response.setStatus(200);
			response.setStatusMessage("OK");
			bodySize = response.getBody().size();
			if (bodySize != 0) {
				std::stringstream ss;
				ss << bodySize;
				response.addHeader("Content-Length", ss.str());
			}
			responseType = CGIResponseCreator::DOC;
			return true;
		} else if (key.compare("Location") == 0) {
			std::string location;

			std::getline(issheader, location);
			if (location.at(0) == '/') {
				std::string extra;

				request.setUrl(location);
				std::getline(issheader, extra);
				if (extra.empty() == false) {
					responseType = CGIResponseCreator::OTHER;
					return false;
				}
				responseType = CGIResponseCreator::LOCALREDIR;
				return true;
			}
			if (location.find(':') != std::string::npos) {
				std::string value;

				response.setHeader("Location", location);
				while (issline.eof() == false) {
					std::getline(issline, line);
					if (line.empty() == true) {
						std::string body;

						while (issline.eof() == false) {
							std::getline(issline, body);
							if (issline.eof() == true) { response.addBody(body); }
							else { response.addBody(body + "\r\n"); }
						}
					} else {
						issheader.clear();
						issheader.str(line);
						std::getline(issheader, key, ':');
						std::getline(issheader, value);
						response.addHeader(key, value);
					}
				}
				Result<std::string, bool> result = response.getHeader("Status");
				if (result.isOK() == true) {
					unsigned int statusCode;
					std::string reasonPhrase;

					std::stringstream ssStatus(result.getOk());
					ssStatus >> statusCode;
					ssStatus >> reasonPhrase;
					response.setStatus(statusCode);
					std::clog << result.getOk() << std::endl;
					std::clog << statusCode << std::endl;
					std::clog << reasonPhrase << std::endl;
					response.setStatusMessage(reasonPhrase);
				} else {
					response.setStatus(302);
					response.setStatusMessage("Found");
				}
				bodySize = response.getBody().size();
				if (bodySize != 0) {
					std::stringstream ss;
					ss << bodySize;
					response.addHeader("Content-Length", ss.str());
				}
				responseType = CGIResponseCreator::CLIENTREDIR;
				return true;
			}
			else {
				RequestHandler requestHandler(configs, request);
				requestHandler.searchMatchHost();
				response.setStatus(500);
				response.setStatusMessage("Internal Server Error");
				requestHandler.setCgiResponse(response);
				response = requestHandler.getResponse();
				responseType = CGIResponseCreator::OTHER;
				return false;
			}
		} else {
			RequestHandler requestHandler(configs, request);
			requestHandler.searchMatchHost();
			response.setStatus(500);
			response.setStatusMessage("Internal Server Error");
			requestHandler.setCgiResponse(response);
			response = requestHandler.getResponse();
			responseType = CGIResponseCreator::OTHER;
			return false;
		}
	}
	RequestHandler requestHandler(configs, request);
	requestHandler.searchMatchHost();
	response.setStatus(500);
	response.setStatusMessage("Internal Server Error");
	requestHandler.setCgiResponse(response);
	response = requestHandler.getResponse();
	responseType = CGIResponseCreator::OTHER;
	return false;
}

bool CGIResponseCreator::deinit() {
	if (inpfd[0] != 0) {
		if (close(inpfd[0]) == -1) {
			putSytemError("close");
			// error handling
		}
		inpfd[0] = 0;
	}
	if (inpfd[1] != 0) {
		if (close(inpfd[1]) == -1) {
			// error handling
			putSytemError("close");
		}
		inpfd[1] = 0;
	}
	if (outpfd[0] != 0) {
		if (close(outpfd[0]) == -1) {
			// error handling
			putSytemError("close");
		}
		outpfd[0] = 0;
	}
	if (outpfd[1] != 0) {
		if (close(outpfd[1]) == -1) {
			// error handling
			putSytemError("close");
		}
		outpfd[1] = 0;
	}
	runtimePath.clear();
	cgiIntput.clear();
	cgiOutput.clear();
	cgiPath.clear();
	hostName.clear();
	metaVariables.init();
	startTime = 0;
	return true;
}

void CGIResponseCreator::setCGIPath(const std::string &_cgiPath) {
	cgiPath = _cgiPath;
}
