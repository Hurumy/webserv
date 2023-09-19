*** Settings ***
Documentation     A test suite for responses of webserv.
Library           RequestsLibrary
Default Tags      positive

*** Test Cases ***
Send simple HTTP GET request
    ${response}=    GET  http://localhost:8080
    Status Should Be  200
    Should Be Equal As Strings   Hello world!\r\n   ${response.text}
